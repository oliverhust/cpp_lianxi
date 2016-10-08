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


/*
 * This file contains source code for managing 
 * queues of iSNS requests pending a response from
 * other SOIP service entities.
 *
 */
#include "iSNS.h"
#include "iSNStypes.h"
#include "iSNSqueue.h"

static int sns_retry_timeout = SNS_REQUEST_TIMEOUT;

/*
 * iSNS message queues. There are two queues used for
 * staging requests that require a response. One queue
 * is used for staging transaction messages like
 * attribute registeration, query etc. The other
 * queue is used for staging FSM messages like
 * DISCOVERY, CLAIM etc.
 *
 */
static struct SNS_Msg_Descp_Q  sns_q[SNS_NUM_QUEUES] = { {NULL, NULL}, 
                                                         {NULL, NULL} };


int
SNSQInit (SNS_Queue_Name name)
{

    if (name > SNS_NUM_QUEUES)
       return (ERROR);

    sns_q[name].head = sns_q[name].tail = NULL;

    return (SUCCESS);
}



void
ISNSEnqReq (SNS_Queue_Name name, ISNS_Msg_Descp *p_md)
{

    ISNS_Msg_Descp  *p_tmp;
    int             current_time;


    if (name > SNS_NUM_QUEUES)
       return;

    /*  
     * Add the message to the end of the request queue
     */
     if (sns_q[name].tail != NULL)
     {
        p_tmp             = sns_q[name].tail;
        sns_q[name].tail  = p_md;
        p_md->cb.next     = NULL;
        p_md->cb.prev     = p_tmp;
        p_tmp->cb.next    = p_md;

       /*
        * Compute the timeout based on other elements in the queue
        */
#ifdef SNS_LINUX
        current_time       = (long double)time ((time_t*) 0);
#else
        current_time       = tickGet();
#endif
        p_md->cb.resp_timeout = current_time + 
                                (sns_retry_timeout * sysClkRateGet());
     }
     else
     {
        sns_q[name].head   = p_md;
        sns_q[name].tail   = p_md; 
        p_md->cb.next      = NULL;
        p_md->cb.prev      = NULL;
#ifdef SNS_LINUX
        current_time       = (long double)time ((time_t*) 0);
#else
        current_time       = tickGet();
#endif
        p_md->cb.resp_timeout = current_time + 
                                (sns_retry_timeout * sysClkRateGet());
     }

}



ISNS_Msg_Descp *
ISNSQHead (SNS_Queue_Name name)
{
    if (name > SNS_NUM_QUEUES)
       return (NULL); 

    return(sns_q[name].head); 
}


ISNS_Msg_Descp *
ISNSDeqReq (SNS_Queue_Name name, uint16_t xid, int tid)
{

    ISNS_Msg_Descp  *p_md;
    ISNS_Msg_Descp  *p_tmp;

    if (name > SNS_NUM_QUEUES)
       return (NULL);

    if (sns_q[name].head == NULL)
       return(NULL);

    for (p_md = sns_q[name].head; p_md != NULL; p_md = p_md->cb.next)
    {
       /* 
        * Check the task id queue. If there is a task id
        * in here it means that the async function is active for this task.
        * Get the iSNS info from the tid queue. Otherwise its the sync 
        * function that is active. Get the info from the transaction queue.
        */
        if (name == ISNS_CALLBACK_QUEUE && tid != 0)
        {
            if (p_md->cb.task_id == tid)
                break;
        }
        else
        {
            if (p_md->msg.hdr.xid == xid)
               break;

            if (ntohs(p_md->msg.hdr.xid) == xid)
                break;
        }
    }

    /*
     * remove the queued request
     */
     if (p_md != NULL)
     {
        /*
         * Is this the only element in the queue
         */
         if (sns_q[name].head == sns_q[name].tail)
         {
            sns_q[name].head = NULL;
            sns_q[name].tail = NULL;

            return (p_md);
         }
         else 
         {
             p_tmp = p_md->cb.prev;
             if (p_tmp != NULL)
                 p_tmp->cb.next = p_md->cb.next; 
             else
                sns_q[name].head = p_md->cb.next;
           
             p_tmp = p_md->cb.next;
             if (p_tmp != NULL)
                p_tmp->cb.prev = p_md->cb.prev; 
             else
                sns_q[name].tail = p_md->cb.prev;
         }
     }

     return (p_md);
}

ISNS_Msg_Descp *
SNSGetReq (SNS_Queue_Name name, int xid, int tid)
{
    ISNS_Msg_Descp  *p_md;

    /* 
     * Init the local vars.
     */
    p_md = NULL;
    /* 
     * Validate the queue.
     */
    if (name > SNS_NUM_QUEUES)
       return (NULL);

    if (sns_q[name].head == NULL)
       return(NULL);
    /* 
     * Go through the list and find the entry.
     */
    for (p_md = sns_q[name].head; p_md != NULL; p_md = p_md->cb.next)
    {
       /* 
        * Check the task id queue. If there is a task id
        * in here it means that the async function is active for this task.
        * Get the iSNS info from the tid queue. Otherwise its the sync 
        * function that is active. Get the info from the transaction queue.
        */
        if (name == ISNS_CALLBACK_QUEUE && tid != 0)
        {
            if (p_md->cb.task_id == tid)
            {
                if (p_md->cb.xid_status == XID_BEGIN)
                {
                    p_md->cb.xid_status = XID_IN_PROGRESS;
                    break;
                }
                else
                    continue;
            }
        }
        else if (name == ISNS_CALLBACK_QUEUE && xid != 0)
        {
            if (p_md->msg.hdr.xid == xid)
               break;
        }
    }
    return (p_md);
}
