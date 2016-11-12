#
# The contents of this file are subject to the Nishan Systems, Inc. Public 
# License Version 1.0 (the "License"); you may not use this file except in 
# compliance with the License included with the distribution. You may 
# obtain a copy of the License at www.NishanSystems.com.
#
# Software distributed under the License is distributed on an "AS IS" 
# basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See 
# the License for the specific language governing rights and limitations 
# under the License.
#
# The Original Software is Release 1.0 of the iSNS Example Client and 
# Server. Portions created by Nishan Systems are Copyright 2001 Nishan 
# Systems, Inc. All Rights Reserved. 
#
# This Original Software is subject to a standard specification issued 
# by the IETF.  The Specification is titled iSNS Internet Storage Name 
# Service.
#
# The Initial Contributor of the Original Software is Nishan Systems, Inc. 
# Revision Contributor(s) are listed below:
#
#            Submitted Revision Contributor Name and Date
#
#

SHELL = /bin/sh

DESTDIR ?=
INSTALL = install
etcdir = /etc
exec_prefix = 
initddir = $(etcdir)/init.d
sbindir = $(exec_prefix)/usr/sbin

PROGRAMS = isnssd

CC = gcc -g
SRCDIR = ./src/
INCDIR = ./include/
INCDIR += -I/usr/include/

OBJECTS = 
OBJECTS += iSNSMain.o
OBJECTS += iSNScomm.o
OBJECTS += iSNSparse.o
#OBJECTS += iSNStbl.o
OBJECTS += iSNSInit.o
OBJECTS += iSNSUtil.o
OBJECTS += iSNSdb.o
OBJECTS += iSNSscn.o
OBJECTS += iSNSesi.o
OBJECTS += iSNSresponse.o
OBJECTS += iSNSquery.o
OBJECTS += iSNSreg.o
OBJECTS += iSNSdereg.o
OBJECTS += iSNSipc.o
OBJECTS += iSNSqueue.o
OBJECTS += iSNStrcv.o
OBJECTS += iSNSLinux.o
OBJECTS += iSNSbuffer.o
OBJECTS += iSNSfsm.o
OBJECTS += iSNSrcv.o 
OBJECTS += iSNSList.o
OBJECTS += iSNSconfig.o
OBJECTS += iSNSobjects.o
OBJECTS += iSNSNdb.o
OBJECTS += iSNSNdbLdap.o
OBJECTS += iSNShash.o
OBJECTS += iSNSdbMem.o
OBJECTS += iSNSMemData.o
OBJECTS += iSNSdbLdap.o
OBJECTS += iSNSLdapLib.o

#OPTFLAGS ?= -g -O2
OPTFLAGS ?= -g
WARNFLAGS ?= -Wall
CFLAGS += $(OPTFLAGS) $(WARNFLAGS)
CFLAGS += -DSNS_LINUX
CFLAGS += -DSNS_DEBUG
LIBS = -lpthread -lldap

PROGRAMS = isnsd

all : $(PROGRAMS)	

isnsd: $(OBJECTS)
	$(CC) $^ -o $@ $(LIBS)
	rm $(OBJECTS)
	
iSNSMain.o : $(SRCDIR)iSNSMain.c 
	dos2unix $(SRCDIR)*.c
	$(CC) -I$(INCDIR) $(CFLAGS) -c $< -o $@

iSNScomm.o : $(SRCDIR)iSNScomm.c 
	$(CC) -I$(INCDIR) $(CFLAGS) -c $< -o $@

iSNSparse.o : $(SRCDIR)iSNSparse.c 
	$(CC) -I$(INCDIR) $(CFLAGS) -c $< -o $@

iSNStbl.o : $(SRCDIR)iSNStbl.c
	$(CC) -I$(INCDIR) $(CFLAGS) -c $< -o $@

iSNSInit.o : $(SRCDIR)iSNSInit.c 
	$(CC) -I$(INCDIR) $(CFLAGS) -c $< -o $@

iSNSUtil.o : $(SRCDIR)iSNSUtil.c 
	$(CC) -I$(INCDIR) $(CFLAGS) -c $< -o $@

iSNSdb.o : $(SRCDIR)iSNSdb.c 
	$(CC) -I$(INCDIR) $(CFLAGS) -c $< -o $@

iSNSresponse.o : $(SRCDIR)iSNSresponse.c 
	$(CC) -I$(INCDIR) $(CFLAGS) -c $< -o $@

iSNSscn.o : $(SRCDIR)iSNSscn.c 
	$(CC) -I$(INCDIR) $(CFLAGS) -c $< -o $@

iSNSesi.o : $(SRCDIR)iSNSesi.c 
	$(CC) -I$(INCDIR) $(CFLAGS) -c $< -o $@

iSNSquery.o : $(SRCDIR)iSNSquery.c 
	$(CC) -I$(INCDIR) $(CFLAGS) -c $< -o $@

iSNSreg.o : $(SRCDIR)iSNSreg.c 
	$(CC) -I$(INCDIR) $(CFLAGS) -c $< -o $@

iSNSdereg.o : $(SRCDIR)iSNSdereg.c 
	$(CC) -I$(INCDIR) $(CFLAGS) -c $< -o $@

iSNSipc.o : $(SRCDIR)iSNSipc.c 
	$(CC) -I$(INCDIR) $(CFLAGS) -c $< -o $@

iSNSqueue.o : $(SRCDIR)iSNSqueue.c
	$(CC) -I$(INCDIR) $(CFLAGS) -c $< -o $@

iSNStrcv.o : $(SRCDIR)iSNStrcv.c
	$(CC) -I$(INCDIR) $(CFLAGS) -c $< -o $@

iSNSLinux.o : $(SRCDIR)iSNSLinux.c 
	$(CC) -I$(INCDIR) $(CFLAGS) -c $< -o $@

iSNSbuffer.o : $(SRCDIR)iSNSbuffer.c
	$(CC) -I$(INCDIR) $(CFLAGS) -c $< -o $@

iSNSfsm.o : $(SRCDIR)iSNSfsm.c 
	$(CC) -I$(INCDIR) $(CFLAGS) -c $< -o $@

iSNSrcv.o : $(SRCDIR)iSNSrcv.c 
	$(CC) -I$(INCDIR) $(CFLAGS) -c $< -o $@

iSNSList.o : $(SRCDIR)iSNSList.c
	$(CC) -I$(INCDIR) $(CFLAGS) -c $< -o $@ 

iSNSconfig.o : $(SRCDIR)iSNSconfig.c
	$(CC) -I$(INCDIR) $(CFLAGS) -c $< -o $@ 

iSNSobjects.o : $(SRCDIR)iSNSobjects.c
	$(CC) -I$(INCDIR) $(CFLAGS) -c $< -o $@ 

iSNSNdb.o : $(SRCDIR)iSNSNdb.c
	$(CC) -I$(INCDIR) $(CFLAGS) -c $< -o $@ 

iSNSNdbLdap.o : $(SRCDIR)iSNSNdbLdap.c
	$(CC) -I$(INCDIR) $(CFLAGS) -c $< -o $@ 

iSNShash.o : $(SRCDIR)iSNShash.c
	$(CC) -I$(INCDIR) $(CFLAGS) -c $< -o $@

iSNSMemData.o : $(SRCDIR)iSNSMemData.c
	$(CC) -I$(INCDIR) $(CFLAGS) -c $< -o $@

iSNSdbMem.o : $(SRCDIR)iSNSdbMem.c
	$(CC) -I$(INCDIR) $(CFLAGS) -c $< -o $@

iSNSdbLdap.o : $(SRCDIR)iSNSdbLdap.c
	$(CC) -I$(INCDIR) $(CFLAGS) -c $< -o $@

iSNSLdapLib.o : $(SRCDIR)iSNSLdapLib.c
	$(CC) -I$(INCDIR) $(CFLAGS) -c $< -o $@

ut : 
	dos2unix $(SRCDIR)NdbTest.c
	$(CC) -std=c99 -I$(INCDIR) $(CFLAGS) $(LIBS) -o ndbTest $(SRCDIR)iSNSNdb.c $(SRCDIR)NdbTest.c $(SRCDIR)iSNSNdbLdap.c $(SRCDIR)iSNShash.c

clean :
	rm $(OBJECTS) isnsd || :
	dos2unix $(SRCDIR)*.c

install: install_programs install_initd

install_programs: $(PROGRAMS)
	$(INSTALL) -d $(DESTDIR)$(sbindir)
	$(INSTALL) -m 755 $^ $(DESTDIR)$(sbindir)

install_initd:
	if [ -f /etc/SuSE-release ]; then \
		$(MAKE) install_initd_suse ; \
	elif [-f /etc/debian_version ]; then \
		$(MAKE) install_initd_debian ; \
	elif [-f /etc/redhat_version ]; then \
		$(MAKE) install_initd_redhat ; \
        fi

install_initd_debian:
	$(INSTALL) -d $(DESTDIR)$(initddir) 
	$(INSTALL) -m 755 etc/initd.debian $(DESTDIR)$(initddir)/isns

install_initd_suse:
	$(INSTALL) -d $(DESTDIR)$(initddir) 
	$(INSTALL) -m 755 etc/initd.suse $(DESTDIR)$(initddir)/isns

install_initd_redhat:
	$(INSTALL) -d $(DESTDIR)$(initddir) 
	$(INSTALL) -m 755 etc/initd.suse $(DESTDIR)$(initddir)/isns
