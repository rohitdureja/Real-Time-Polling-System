Real-Time-Polling-System
========================

Objective

The homework was tacked in two parts. Two systems were implemented, running on
1. UDP as the underlying transport layer protocol
2. RPC for remote execution

Both the systems, are fundamentally the same with the difference being in the way of communication.

---

Data Structured used

Effectively two data structures keep track of the voter and candidate information for both the system. 
The first is VoterList which is implemented as a linked list data structure and contains:
1. the voterid
2. the vote status - indicating whether the voter has already voted or not

The second is CandidateList which is also implemented as a linked list data structure and contains:
1. the candidate name
2. the vote count for the candidate

Pushing instructions are performed on both the queues when a new voter is added or a new candidate is voted for. 

---

Function Implementation

The homework prompt asked for implementng five functions:
1. zeroize()
2. addvoter()
3. votefor()
4. listcandidates()
5. votecount()

zeroize()
	this functio cleans up existing records, cleaning up all voter and candidate data and freeing the memory used

addvoter(voterid)
	this function adds a new voter to the voter list

votefor(name, voterid)
	this function casts the vote by voter specificed in voterid for a candidate whose name has been specified as the first argument.

listcandidates()
	this function displays the list of candidates current in the polling system

votecount(name)
	this function returns the votecount for the specified candidate name passed as an argument.

---

Protocol Implementation

Two sets of client invocation for the system using UDP and RPC were developed. The UDP payload has a string type packet with fields corresponding to different aspects of the system. 

UDP

The packet layout of polling system using UDP is:

 ___________________________________________________________________________________________________  
|   Function Name   |   Voter id    |     Packet length     |              Payload data             |         
 ---------------------------------------------------------------------------------------------------

a) The function name is a 1-byte field detailing the function to execute on the server. The valid values are:
	-> "1" - zeroize()
	-> "2" - addvoter()
	-> "3" - votefor()
	-> "4" - listcandidates()
	-> "5" - votecount()
 
 As the field is 1-bytes, the system can support upto 256 functions.

 b) The voter id is a 1-byte field indicating the voter id of the voter. Due to the field length, the current system only supports upto 256 voters. This number can be easily increased by increasing the field length of oter id in the packet.

 c) The packet length is a 1-byte field indicating the packet length including the packet header and payload. As it is a 1-byte field, the maximum length of the packet can be 256 bytes. This can also be increased , if desired.

 d) The last one is multi byte field caled the payload. It is used to primarily carry the candidate name the voter has voted for. As the packet length is restricted 256 bytes, the payload can be at most 253 bytes, which is good enough for our purposes.

 RPC

 The specification for the system based on RPC was first written using IDL and then rpcgen was used to generate the client and server stubs, header files, client and server files. Te XDR representation of the system is:

 struct Vote {
	string name<>;
	int voterid;
};

struct Candidates {
	string name<>;
	struct Candidates *next;
};

program POLL_PROG {
	version POLL_VERS {
		string zeroize(void) = 1;
		string addvoter(int) = 2;
		string votefor(Vote) = 3;
		Candidates listcandidates(void) = 4;
		int votecount(string) = 5;
	} = 1;
} = 0x23451111;

a) The Vote struct is used in the votefor() function as a handler for both candidate name and voter id as rpcgen allows passing just one parameter in function arguments.

b) The Candidates struct is used to read the list of candidates sent back by the server when executing the listcandidates() function.

c) The name of the program is POLL_PROG with version POLL_VERS being verion 1. The five functions are assigned specifiers and the types of arguments sent and recieved are specified.

--- 

Packet Layout

1. the zeroize() function just sends the function name in the packet.
2. the addvoter() function sends the function name and voter id in the packet.
3. the votefor() function sends the function name, voterid and candidate name as payload in the packet.
4. the listcandidates() function just sends the function name in the packet.
5. the votecount() function sends the function name and candidate name as payload in the packet.

---

Invocation

Steps involved to run system using UDP

1. use make to compile all files. "make all"

2. in a terminal window, run the server using
	./server <address> <port>

3. In a seperate terminal window, run the client using
	./client <address> <port> -options

Instead of step 3, a script has been provided which can be run to simulate a voting scenario. To run the script, after step 2, run
	chmod u+rx script
	./script

4. stop server and clean up using make. "make clean"

Steps involved to run system using RPC

1. use make to compile all files. "make all"

2. in a terminal window, run the server using
	./poll_server

3. in a seperate terminal window, run the client using
	./poll_client <address> -options

Instead of step 3, a script has been provided which can be run to simulate a voting scenario. To run the script, after step 2, run
	chmod u+rx script
	./script

4. stop server and clean up using make. "make clean"

---

Client Invocation Options

Option for system using UDP

	zeroize():				 .\client <IPaddress> <port> -z
	addvoter(voterid):		 .\client <IPaddress> <port> -a -i <voterID>		
	votefor(name, voterid):	 .\client <IPaddress> <port> -v -n <name> -i <voterID>
	listcandidates():		 .\client <IPaddress> <port> -l
	votecount(name):		 .\client <IPaddress> <port> -c -n <name>

Options for syste using RPCs

	zeroize():				 .\client <IPaddress> -z
	addvoter(voterid):		 .\client <IPaddress> -a -i <voterID>		
	votefor(name, voterid):	 .\client <IPaddress> -v -n <name> -i <voterID>
	listcandidates():		 .\client <IPaddress> -l
	votecount(name):		 .\client <IPaddress> -c -n <name>

---
