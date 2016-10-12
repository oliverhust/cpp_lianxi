/***********************************************************************
  Copyright (c) 2001, Nishan Systems, Inc.
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are
  met:

  - Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.

  - Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

  - Neither the name of the Nishan Systems, Inc. nor the names of its
    contributors may be used to endorse or promote products derived from
    this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY, NON-INFRINGEMENT AND FITNESS FOR A
  PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL NISHAN SYSTEMS, INC.
  OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
  OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

***********************************************************************/
#if 1
#include "iSNStypes.h"
#include "iSNSmsg.h"
#include "iSNShash.h"
#include "iSNSNdb.h"
#include "iSNSdebug.h"

static uint32_t hash(datum *pKey);
static uint32_t entry_match_key (SNSHashEntry *pEntry, datum *pKey);
static void* snsHashAlloc(uint32_t size);
static void snsHashFree(void *p);
static SNSHashEntry* createHashEntry(datum *pKey, datum *pContent);

SNSHashEntry **HashKeyTable[HASH_TABLE_NUM_ENTRIES];
/* SNSHashEntry *HashTable[HASH_TABLE_NUM_ENTRIES]; */
MSG_Q_ID SNSHashEntryQ = NULL;
uint8_t *HashBufferPool = NULL;

int   ndb_hash_debug = 0;
datum gNullDatum;

ndb_error ndb_hash_errno;

int ndb_hash_open()
{
    int x;
    /* memset(HashTable, 0, sizeof(HashTable)); */
    if (!SNSHashEntryQ) {
        SNSHashEntryQ = (void *)1;
        HashBufferPool = (uint8_t*)calloc(HASH_NUM_BUFFERS, HASH_BUFFER_SIZE);
        if (!HashBufferPool || !SNSHashEntryQ)
            return NULL;
    }
    gNullDatum.dsize = 0;
    gNullDatum.dptr = NULL;

    /* Initialize all hash tables */
    for (x = 0; x < END_SNS_DEVI_ATTR; x++)
    {
      HashKeyTable[x] = calloc(HASH_TABLE_NUM_ENTRIES, sizeof(SNSHashEntry *));
    }

    return NDB_SUCCESS;
}

void ndb_hash_close ()
{
    SNSHashEntryQ = NULL;
    free(HashBufferPool);
    HashBufferPool = NULL;
    return;
}

int ndb_hash_store (datum key, datum content)
{
    SNSHashEntry *pEntry, *pPrev = NULL;
    uint32_t hash_val, i, hash_key_val;
    SNSHashEntry **table_ptr;

    hash_key_val = ((SOIP_DB_Entry *)(content.dptr))->data_type;
    if (ndb_hash_debug & 0x02) {
        printf("ndb_store(): data_type = %d, hashes = %d",
           (int)hash_key_val, (int)hash_key_val);
    }

    table_ptr = HashKeyTable[ hash_key_val ];
    hash_val = hash(&key);

    if (ndb_hash_debug & 0x02) {
        printf("ndb_store(): entry stored at %d. Key size: %d\n", (int)hash_val, (int)key.dsize);
        for (i = 0; i < (unsigned)key.dsize; i ++) {
            if (i % 16 == 0)
                printf("\n");
            printf("%02x ", (unsigned)key.dptr[i]);
        }
        printf("\n");
        if (ndb_hash_debug & 0x08) {
            for (i = 0; i < MIN((unsigned)content.dsize, 64); i ++) {
                if (i % 16 == 0)
                    printf("\n");
                printf("%02x ", (unsigned)content.dptr[i]);
            }
        }
        printf("\n");
    }

    if (!table_ptr[hash_val]) {
        table_ptr[hash_val] = createHashEntry(&key, &content);
        return SUCCESS;
    }
    for (pEntry = table_ptr[hash_val]; pEntry; pEntry = pEntry->next) {
        if (entry_match_key(pEntry, &key)) {
            pEntry->data_len = MIN((unsigned)content.dsize, HASH_MAX_DATA_SIZE);
            /* Note: the entry's size might change,
               might need to re-alloc in the future */
            memcpy(pEntry->data, content.dptr, pEntry->data_len);
            return SUCCESS;
        }
        pPrev = pEntry;
    }
    pPrev->next = createHashEntry(&key, &content);
    pPrev->next->prev = pPrev;
    return SUCCESS;
}

datum ndb_hash_fetch (uint32_t key_type, datum key)
{
    SNSHashEntry *pEntry;
    SNSHashEntry **table_ptr;
    uint32_t hash_val;
    uint32_t hash_key_val;
    datum ret;
    unsigned i;

    hash_key_val = key_type;
    if (ndb_hash_debug & 0x02) {
        printf("ndb_fetch(): data_type = %d, hashes = %d",
           (int)key_type, (int)hash_key_val);
    }

    table_ptr = HashKeyTable[ hash_key_val ];

    hash_val = hash(&key);

    if (ndb_hash_debug & 0x04) {
        printf("ndb_fetch(): fetch hash %d.  Key size: %d\n", (int)hash_val, (int)key.dsize);
        for (i = 0; i < (unsigned)key.dsize; i ++) {
            if (i % 16 == 0)
                printf("\n");
            printf("%02x ", (unsigned)key.dptr[i]);
        }
        printf("\n");
    }

    for (pEntry = table_ptr[hash_val]; pEntry; pEntry = pEntry->next) {
        if (entry_match_key(pEntry, &key)) {
            ret.dsize = pEntry->data_len;
            ret.dptr = malloc(ret.dsize);
            memcpy(ret.dptr, pEntry->data, ret.dsize);
            return ret;
        }
    }

    return gNullDatum;
}

datum ndb_hash_fetch_sns (uint32_t key_type, datum key, char* msg)
{
    SNSHashEntry *pEntry;
    SNSHashEntry **table_ptr;
    uint32_t hash_val;
    uint32_t hash_key_val;
    datum ret;
    unsigned i;

    hash_key_val = key_type;
    if (ndb_hash_debug & 0x02) {
        printf("ndb_fetch(): data_type = %d, hashes = %d",
           (int)key_type, (int)hash_key_val);
    }
    table_ptr = HashKeyTable[ hash_key_val ];

    hash_val = hash(&key);

    if (ndb_hash_debug & 0x04) {
        printf("ndb_fetch(): fetch hash %d.  Key size: %d\n", (int)hash_val, (int)key.dsize);
        for (i = 0; i < (unsigned)key.dsize; i ++) {
            if (i % 16 == 0)
                printf("\n");
            printf("%02x ", (unsigned)key.dptr[i]);
        }
        printf("\n");
    }

    for (pEntry = table_ptr[hash_val]; pEntry; pEntry = pEntry->next) {
        if (entry_match_key(pEntry, &key)) {
            ret.dsize = pEntry->data_len;
            ret.dptr = msg;
            memcpy(ret.dptr, pEntry->data, ret.dsize);
            return ret;
        }
    }

    return gNullDatum;
}

int ndb_hash_delete (uint32_t key_type, datum key)
{
    SNSHashEntry *pEntry;
    SNSHashEntry **table_ptr;
    uint32_t hash_val;
    uint32_t hash_key_val;

    hash_key_val = key_type;
    if (ndb_hash_debug & 0x02) {
        printf("ndb_fetch(): data_type = %d, hashes = %d",
           (int)key_type, (int)hash_key_val);
    }
    table_ptr = HashKeyTable[ hash_key_val ];

    hash_val = hash(&key);
    for (pEntry = table_ptr[hash_val]; pEntry; pEntry = pEntry->next) {
        if (entry_match_key(pEntry, &key)) {
            if (pEntry->prev)
                pEntry->prev->next = pEntry->next;
            else
                table_ptr[hash_val] = pEntry->next;
            if (pEntry->next)
                pEntry->next->prev = pEntry->prev;
            snsHashFree(pEntry);
            return SUCCESS;
        }
    }

    return ERROR;
}

datum ndb_hash_firstkey (uint32_t key_type)
{
    uint32_t i, hash_key_val;
    datum ret;
    SNSHashEntry **table_ptr;

    hash_key_val = key_type;
    table_ptr = HashKeyTable[ hash_key_val ];
    for (i = 0; i < HASH_TABLE_NUM_ENTRIES; i ++) {
        if (table_ptr[i]) {
            ret.dsize = table_ptr[i]->key_len;
            ret.dptr = malloc(ret.dsize);
            memcpy(ret.dptr, table_ptr[i]->key, ret.dsize);
            return ret;
        }
    }

    return gNullDatum;
}

datum ndb_hash_nextkey (uint32_t key_type, datum key)
{
    SNSHashEntry *pEntry;
    SNSHashEntry **table_ptr;
    uint32_t hash_val, i, hash_key_val;
    datum ret;

    hash_key_val = key_type;
    table_ptr = HashKeyTable[ hash_key_val ];
    hash_val = hash(&key);
    for (pEntry = table_ptr[hash_val]; pEntry; pEntry = pEntry->next) {
        if (entry_match_key(pEntry, &key)) {
            if (pEntry->next) {
                ret.dsize = pEntry->next->key_len;
                ret.dptr = malloc(ret.dsize);
                memcpy(ret.dptr, pEntry->next->key, ret.dsize);
                return ret;
            }
            for (i = hash_val + 1; i < HASH_TABLE_NUM_ENTRIES; i ++) {
                if (table_ptr[i]) {
                    ret.dsize = table_ptr[i]->key_len;
                    ret.dptr = malloc(ret.dsize);
                    memcpy(ret.dptr, table_ptr[i]->key, ret.dsize);
                    return ret;
                }
            }
            break;
        }
    }

    return gNullDatum;
}

static uint32_t hash(datum *pKey)
{
    uint32_t ret_val = 0;
    uint32_t i;
    uint8_t *key = (uint8_t*)pKey->dptr;
    uint8_t *end_key = key + pKey->dsize;

    if (pKey->dsize > 1) {
        while (key != end_key) {
            i = (uint32_t)(*key);
            ret_val ^= i;
            /*ret_val <<= 1;*/
            ret_val = (ret_val << 1) + (ret_val % 2);
            key ++;
        }
    }
    else {
        ret_val = ((uint32_t)(*key) << 8) + (uint32_t)(*key);
    }

    return (ret_val % HASH_TABLE_NUM_ENTRIES);
}

static uint32_t entry_match_key (SNSHashEntry *pEntry, datum *pKey)
{
    return (pEntry->key_len == pKey->dsize && !memcmp(pEntry->key, pKey->dptr, pKey->dsize));
}

static void* snsHashAlloc(uint32_t size)
{
    void *ret;
   ret=malloc(size);
    return ret;
}

static void snsHashFree(void *p)
{
    free(p);
}

static SNSHashEntry* createHashEntry(datum *pKey, datum *pContent)
{
    SNSHashEntry *pEntry;
    uint32_t data_len;

    data_len = MIN(HASH_MAX_DATA_SIZE, (unsigned)pContent->dsize);
    pEntry = snsHashAlloc(sizeof(SNSHashEntry) + data_len - sizeof(pEntry->data));
    memset(pEntry, 0, sizeof(SNSHashEntry) + data_len - sizeof(pEntry->data));
    /*pEntry->data = (char*)pEntry + sizeof(SNSHashEntry);*/
    pEntry->data_len = data_len;
    memcpy(pEntry->data, pContent->dptr, pEntry->data_len);
    pEntry->key_len = MIN(HASH_KEY_SIZE, pKey->dsize);
    memcpy(pEntry->key, pKey->dptr, pEntry->key_len);

    return pEntry;
}

#if 0
void snsShowHashEntry(uint32_t idx, uint32_t num_bytes)
{
    uint32_t     i;
    int          count = 0;
    SNSHashEntry *pEntry = HashTable[idx];

    if (!pEntry) {
        printf("Empty entry\n");
        return;
    }

    while (pEntry) {
        count ++;
        printf("\n************************\n");
        printf("Current entry at: 0x%x\n", (unsigned)pEntry);
        printf("next: 0x%x, prev: 0x%x\n", (unsigned)pEntry->next, (unsigned)pEntry->prev);
        printf("key_len: %d, data_len: %d\n", (unsigned)pEntry->key_len, (unsigned)pEntry->data_len);
        printf("key:");
        for (i = 0; i < pEntry->key_len; i ++) {
            if (i % 16 == 0)
                printf("\n");
            printf("%02x ", (unsigned)pEntry->key[i]);
        }
        num_bytes = num_bytes?num_bytes:pEntry->data_len;
        num_bytes = MIN(pEntry->data_len, num_bytes);
        printf("\ndata: %d bytes shown", (unsigned)num_bytes);
        for (i = 0; i < num_bytes; i ++) {
            if (i % 16 == 0)
                printf("\n");
            printf("%02x ", (unsigned)pEntry->data[i]);
        }
        printf("\n************************\n");
        pEntry = pEntry->next;
    }
    printf("Total: %d entries\n\n", count);
}
#endif

void snsShowHashTable( void )
{
    int x;
    int i;
    int slot_count = 0;
    int total_count = 0;
    int count_per_entry;
    SNSHashEntry **table_ptr;
    SNSHashEntry *pEntry;

    for (x = 0; x < END_SNS_DEVI_ATTR; x++)
    {
       table_ptr = HashKeyTable[ x ];
       for (i = 0; i < HASH_TABLE_NUM_ENTRIES; i ++) {
           count_per_entry = 0;
           for (pEntry = table_ptr[i]; pEntry; pEntry = pEntry->next)
               count_per_entry ++;
           if (count_per_entry) {
               if (slot_count % 8 == 0 )
                   printf("\n");
               printf("%d", i);
               if (count_per_entry > 1)
                   printf("(%d)", count_per_entry);
               printf("  ");
               total_count += count_per_entry;
               slot_count ++;
           }
       }
    }
    printf("\nTotal: %d entries in %d slots\n\n", total_count, slot_count);
}

#endif
