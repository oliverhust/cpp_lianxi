/***********************************************************************
  Copyright (c) 2007, Novell, Inc.
  All rights reserved.
  
  Redistribution and use in source and binary forms, with or without 
  modification, are permitted provided that the following conditions are 
  met:
  
  - Redistributions of source code must retain the above copyright notice, 
    this list of conditions and the following disclaimer. 
  
  - Redistributions in binary form must reproduce the above copyright 
    notice, this list of conditions and the following disclaimer in the 
    documentation and/or other materials provided with the distribution. 
  
  - Neither the name of the Novell, Inc. nor the names of its 
    contributors may be used to endorse or promote products derived from 
    this software without specific prior written permission. 
  
  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
  IMPLIED WARRANTIES OF MERCHANTABILITY, NON-INFRINGEMENT AND FITNESS FOR A 
  PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL Novell, INC. 
  OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
  OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 
***********************************************************************/

/*
 * This file contains definitions related to read and writing of database objects
 *
 */

#ifndef _SNSobjects_h_
#define _SNSobjects_h_

int
read_FCNodeObject (char *p_id, SOIP_Fc_Node ** p_node, SOIP_DB_Entry *p_entry);

int
read_FCPortObject (char *p_id, SOIP_Ifcp ** p_port, SOIP_DB_Entry *p_entry);

int
read_ISCSIObject (char *p_id, SOIP_Iscsi ** p_node, SOIP_DB_Entry *p_entry);

int
read_EntityObject (char *p_entity_id, SOIP_Entity ** p_entity, SOIP_DB_Entry *p_entry);

int
read_PortalObject (SOIP_Portal_Key *p_portal_key, SOIP_Portal ** p_portal, SOIP_DB_Entry *p_entry);

int
read_PortalGroupObject (SOIP_Portal_Group_Key *p_portal_key, SOIP_Portal_Group ** p_portal, SOIP_DB_Entry *p_entry);

int
read_DDObject (int id, SOIP_Dd ** p_dd, SOIP_DB_Entry * p_entry);

int
read_DDSObject (int id, SOIP_Dds ** p_dds, SOIP_DB_Entry * p_entry);

int
read_EntityidxObject (int id, void **p_idx, SOIP_DB_Entry *p_entry);

int
read_ISCSIidxObject (int id, void **p_idx, SOIP_DB_Entry *p_entry);

int
read_PortalidxObject (int id, void **p_idx, SOIP_DB_Entry *p_entry);



int
write_FCNodeObject (char *p_id, SOIP_DB_Entry p_entry);

int
write_FCPortObject (char *p_id, SOIP_DB_Entry p_entry);

int
write_ISCSIObject (char *p_id, SOIP_DB_Entry p_entry);

int
write_EntityObject (char *p_entity_id, SOIP_DB_Entry p_entry);

int
write_PortalObject (SOIP_Portal_Key *p_portal_key, SOIP_DB_Entry p_entry);

int
write_PortalGroupObject (SOIP_Portal_Group_Key *p_portal_key, SOIP_DB_Entry p_entry);

int
write_DDObject (int id, SOIP_DB_Entry p_entry);

int
write_DDSObject (int id, SOIP_DB_Entry p_entry);

int
write_ISCSIidxObject( int id, SOIP_DB_Entry p_entry);

int
write_EntityidxObject( int id, SOIP_DB_Entry p_entry);

int
write_PortalidxObject( int id, SOIP_DB_Entry p_entry);


int
delete_FCNodeObject (char * p_id);

int
delete_FCPortObject (char * p_id);

int
delete_ISCSIObject (char * p_id);

int
delete_EntityObject (char * p_entity_id);

int
delete_PortalObject (SOIP_Portal_Key * p_portal_key);

int
delete_PortalGroupObject (SOIP_Portal_Group_Key * p_portal_key);

int
delete_DDObject (int id);

int
delete_DDSObject (int id);

int
delete_EntityidxObject (int id);

int
delete_ISCSIidxObject (int id);

int
delete_PortalidxObject (int id);


#endif
