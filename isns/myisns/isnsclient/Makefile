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

CC = gcc -g
SRCDIR = ./src/
INCDIR = ./include/
INCDIR += -I../isnsserver/include/
INCDIR += -I/usr/include/

OBJECTS = getarg.o
OBJECTS += comm.o
OBJECTS += entity.o
OBJECTS += iscsi.o
OBJECTS += parse.o
OBJECTS += isns.o
OBJECTS += payloads.o
OBJECTS += ifcp.o
OBJECTS += fcnode.o
OBJECTS += custom.o
OBJECTS += portal.o
OBJECTS += util.o
OBJECTS += menu.o

PROG1 = main.o
PROG2 = main_ui.o

WARNFLAGS = -Wall
OPTFLAGS ?= -g -O2
CFLAGS += $(OPTFLAGS) $(WARNFLAGS)
CFLAGS += -DSNS_LINUX
CFLAGS += -DSNS_DEBUG
LIBS = -lpthread

DESTDIR ?=
INSTALL = install
sbindir = $(exec_prefix)/sbin

PROGRAMS = isnsc isnsadm

all : $(PROGRAMS)

isnsc: $(PROG2) $(OBJECTS) 
	$(CC) $^ -o $@ $(LIBS)

isnsadm: $(PROG1) $(OBJECTS)
	$(CC) $^ -o $@ $(LIBS)

getarg.o : $(SRCDIR)getarg.c 
	$(CC) -I$(INCDIR) $(CFLAGS) -c $< -o $@

main.o : $(SRCDIR)main.c 
	$(CC) -I$(INCDIR) $(CFLAGS) -c $< -o $@

main_ui.o : $(SRCDIR)main_ui.c 
	$(CC) -I$(INCDIR) $(CFLAGS) -c $< -o $@

comm.o : $(SRCDIR)comm.c 
	$(CC) -I$(INCDIR) $(CFLAGS) -c $< -o $@

menu.o : $(SRCDIR)menu.c 
	$(CC) -I$(INCDIR) $(CFLAGS) -c $< -o $@

entity.o : $(SRCDIR)entity.c
	$(CC) -I$(INCDIR) $(CFLAGS) -c $< -o $@

parse.o : $(SRCDIR)parse.c
	$(CC) -I$(INCDIR) $(CFLAGS) -c $< -o $@

util.o : $(SRCDIR)util.c 
	$(CC) -I$(INCDIR) $(CFLAGS) -c $< -o $@

isns.o : $(SRCDIR)isns.c 
	$(CC) -I$(INCDIR) $(CFLAGS) -c $< -o $@

payloads.o : $(SRCDIR)payloads.c 
	$(CC) -I$(INCDIR) $(CFLAGS) -c $< -o $@

ifcp.o : $(SRCDIR)ifcp.c 
	$(CC) -I$(INCDIR) $(CFLAGS) -c $< -o $@

fcnode.o : $(SRCDIR)fcnode.c
	$(CC) -I$(INCDIR) $(CFLAGS) -c $< -o $@

custom.o : $(SRCDIR)custom.c
	$(CC) -I$(INCDIR) $(CFLAGS) -c $< -o $@

portal.o : $(SRCDIR)portal.c
	$(CC) -I$(INCDIR) $(CFLAGS) -c $< -o $@

iscsi.o : $(SRCDIR)iscsi.c
	$(CC) -I$(INCDIR) $(CFLAGS) -c $< -o $@

clean :
	rm $(PROG1) $(PROG2) $(OBJECTS) isnsc isnsadm

install: install_programs

install_programs: $(PROGRAMS)
	$(INSTALL) -d $(DESTDIR)$(sbindir)
	$(INSTALL) -m 755 $^ $(DESTDIR)$(sbindir)

