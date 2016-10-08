README.TXT
______________________
iSNS Release
Version 2.0
______________________
Introduction
1) Please read and accept the "LICENSE.TXT" before use

2) The client and server application can be built with
	For Linux: make
	  SLSE 10 was used during test

3) Pre-built executable for both client and server are provided for 32-bit

3a) The server executable is isnsd

3b) The client executable is isnsc

4) The tested OS platforms are:
	SLES 10 32 and 64 bit
______________________
To Build

The C source code can be built using Linux.  
The option for building the code is to use the Linux makefile, 

make 
make install
______________________
To Provide Feedback

Please provide feedback to the development mail list for the isns server

______________________
To Uninstall

1) Delete the top-level directory that the iSNS code was installed in.

_____________________
Server Options

If the server executable is run without command line options, or the "-?" 
option is on the command line, the following supported options are listed:

iSNSserver>isnss
Usage: isnsd [options]
   NOTES: IP-Addresses must be in IP dotted notation.
Options:
   [-a addr]  IP-Address for SNSP traffic.
   [-b addr]  Broadcast Address for SNSP traffic.
   [-p port]  primary port for SNSP traffic.
   [-d]       Full debug options on.
   [-f]       Foreground and not as a daemon.
   [-?]       Help.

_____________________
Client Options

If the client executable is run without command line options, or the "-?" 
option is on the command line, the following supported options are listed:

iSNSclient>isnsc
IETF iSNS Client, v1.1.
Usage: isnsc [options]
Options:
    [-h]          Listen for L2 heartbeat from an iSNS.
    [-a ip]       iSNS Address.
    [-t]          TCP mode.
    [-p port]     Port used for sending. Default is port 3205.
    [-?]          Help.

For the client to connect with the iSNS server, one of two options needs 
to be used, "-h" or "-a".  The "-h" option allows for discovery using L2 
broadcast heartbeats from the server.  The "-a" option allows for static 
configuration of the iSNS server IP-Address.

_____________________
About Discovery Domains and Discovery Domain Sets

A Discovery Domain Set (DDS) is a collection of Discovery Domains.  A 
DDS can be either Enabled or Disabled.

A Discovery Domain (DD) is a collection of storage devices in the 
storage area network which will be allowed to query for information 
about each other.  These queries will only be successful if the DD 
is a member of an Enabled DDS.

A device does not necessarily know which DD it is a member of.  Also, 
a device may belong to more than one DD.  A DD can be a member of 
more then one DDS.

The SOURCE field of every iSNS query and registration message 
identifies the storage device that sent the request.  The iSNS Server 
uses this field to scope the operation to the set of DDs that the 
origin iSNS client is a member of.  If the SOURCE field contains 
NULL, the iSNS message is assumed to have originated from an 
administrative iSNS client, so the message SHALL BE scoped to the 
entire contents of the iSNS, regardless of the configuration of 
Discovery Domains.

If, after registering storage nodes, queries are done with the source 
of the message specified, then at least one DD should have been created 
and placed into an enabled DDS for the query to return results.  Each 
device that is to be visible to each other needs to be placed into the 
same DD, and the DD needs to be placed in an enabled DDS.

For more discussion on Discovery Domains please refer to the iSNS
Specification.

_____________________
About Registering Access Control Lists (ACLs) For an iSCSI Target

ACLs map directly to an iSNS DD.  If a target has a set of iSCSI Nodes
that have access rights to it, then it can register a DD containing 
the iSCSI Node ID of the target and the set of iSCSI Node IDs of the
initiators that have access to it.  Additionally, the DD must be in an 
enabled DDS for it to be enforced by the iSNS.

An example showing a simple ACL registration is shown below.

_____________________
Getting Started

The best way to start is to run the server and client in separate console 
windows on a Linux, NT or Windows 2000 based workstation.

Then practice registering and querying several nodes without creating any 
Discovery Domains or Discovery Domain Sets.

NOTE: it is important to leave the source field of any registration or 
query messages as NULL if no Discovery Domains and Discovery Domains Sets 
are created.  Otherwise the queries will return with an error stating no 
matching nodes found.

After doing registrations without DD/DDS's, create discovery domains
and place nodes in the DD's to see how ACL's can registered and modified 
in the iSNS.
_____________________
Example Test

The interactive client generates iSNS registration and query messages 
that are sent to the server.  The server responds with the appropriate
messages to satisfy the registration and query messages.  The PDU
stream is printed to the console to facilitate testing and integration
of the iSNS with storage products.

The PDU's are documented in the iSNS Specification.  The current version
as of this specification is included in the iSNSserver directory of this 
release.

1) Open two console windows, possibly on separate workstations

2) Assuming a server with an interface having
	IP-Address 192.100.100.116
	Broadcast Address 192.100.100.255

3) Launch the server
	>isnsd -f

4) Launch the client and use TCP for communication
	>isnsc -a 192.100.100.116 -t

   Or, to demonstrate the autodetection of the server in a layer 2 network, enter

	>isnsc -h -t
