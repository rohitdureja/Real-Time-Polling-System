#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* type definitions for zeroize() */
typedef int bool;
#define TRUE 1
#define FALSE 0

/* type definitions for addvoter() */
typedef int voterstatus;
#define OK 1
#define EXISTS 2
#define ERROR 0

/* type definitions for votefor() */
typedef int votestatus;
#define CANDIDATEEXISTS 1
#define NEW 2
#define NOTAVOTER -1
#define ALREADYVOTED -2
#define ERROR 0

#define SA struct sockaddr

#define BUFLEN 512
#define NPACK 10
#define PORT 9930

/* Structure to hold voter information */
struct VoterList {
	int voterid;
	int votestatus;
	struct VoterList *next;
}*voterlist;

/* Structure to hold candidate information */
struct CandidateList {
	char name[50];
	int voteCount;
	struct CandidateList *next;
}*candidatelist;

/* Reset voter list and candidate list. Bring system to NULL state.
Parameters:	None
Returns:	TRUE, if successful
			FALSE, otherwise
*/
bool zeroize()
{
	struct VoterList *currentvoter = voterlist;
	struct VoterList *nextvoter;
	while(currentvoter != NULL)
	{
		nextvoter = currentvoter -> next;
		free(currentvoter);
		currentvoter = nextvoter;
	}
	voterlist = NULL;
	
	struct CandidateList *currentcandidate = candidatelist;
	struct CandidateList *nextcandidate;
	while(currentcandidate != NULL)
	{
		nextcandidate = currentcandidate -> next;
		free(currentcandidate);
		currentcandidate = nextcandidate;
	}
	candidatelist = NULL;
	return TRUE;
}

/* Function to check if voter id already exists in the system.
Parameters:	voterid
Returns:	index, if found
			-1, otherwise
*/
int checkvoterstatus(int voterid)
{
	struct VoterList *current;// = (struct VoterList *)malloc(sizeof(struct VoterList));
	current = voterlist;
	int pos = 0;
	while(current != NULL)
	{
		if(current -> voterid == voterid)
		{
			return pos;
		}
		else
		{
			current = current -> next;
			pos = pos + 1;
		}
	}
	return -1;
}


/* Add voter to system and to the list of authorised voters.
Parameters:	int voterid - unique id assigned to the voting client
Returns:	OK, if successful
			EXISTS, if voterid is already in the system
			ERROR, if there is some error
*/
voterstatus addvoter(int voterid)
{
	struct VoterList *current;// = (struct VoterList *)malloc(sizeof(struct VoterList));
	int pos = checkvoterstatus(voterid);
	if(pos != -1)
		return EXISTS;
	else
	{
		if(voterlist == NULL)
		{
			current = (struct VoterList *)malloc(sizeof(struct VoterList));
			current -> voterid = voterid;
			current -> votestatus = 0;
			current -> next = NULL;
			voterlist = current;
			return OK;
		}
		else
		{
			current = voterlist;
			while(current -> next != NULL)
				current = current -> next;
			current -> next = (struct VoterList *)malloc(sizeof(struct VoterList));
			current = current -> next;
			current -> voterid = voterid;
			current -> votestatus = 0;
			current -> next = NULL;
			return OK;
		}
	}
}



/* Prints all registered voters 
Parameters:	None
Returns:	None
*/
void printallvoters()
{
	struct VoterList *current;// = (struct VoterList *)malloc(sizeof(struct VoterList));
	current = voterlist;
	while(current != NULL)
	{
		printf("Voter id: %d Voter status: %d\n", current->voterid, current->votestatus);
		current = current -> next;
	}
}



/* Prints all registered candidates 
Parameters:	None
Returns:	None
*/
void listcandidates()
{
	struct CandidateList *current;// = (struct CandidateList *)malloc(sizeof(struct CandidateList));
	current = candidatelist;
	while(current != NULL)
	{
		printf("Voter name: %s Vote Count: %d\n", current->name, current->voteCount);
		current = current -> next;
	}
}

void listallcandidates(char *reply)
{
	struct CandidateList *current;
	current = candidatelist;
	strcpy(reply, current->name);
	strcat(reply, "\n");
	current = current -> next;
	while(current!=NULL)
	{
		strcat(reply, current->name);
		strcat(reply, "\n");
		current = current -> next;
	}
}



/* Function to check if candidate already exists.
Parameters: char *name - name of the candidate
Returns:	index, if exists
			-1 otherwise
*/
int checkcandidatestatus(char *name)
{
	int pos = 0;
	struct CandidateList *current;// = (struct CandidateList *)malloc(sizeof(struct CandidateList));
	current = candidatelist;
	while(current != NULL)
	{
		if(strcmp(current->name, name)==0)
		{
			return pos;
		}
		else
		{
			current = current -> next;
			pos = pos + 1;
		}
	}
	return -1;
}


/* Add a vote for a candidate in the list.
Parameters:	char *name - name of the candidate
			int voterid - unique id assigned to the voting client
Returns: 	EXISTS, if the candidate exists and was successfully voted for
			NEW, if the candidate didn't previously exist and was successfully voted for
			NOTAVOTER, if voterid is not on the list of voters
			ALREADYVOTED, if the voterid has already voted
			ERROR, otherwise
*/
votestatus votefor(char *name, int voterid)
{
	struct VoterList *currentvoter;// = (struct VoterList *)malloc(sizeof(struct VoterList));
	struct CandidateList *currentcandidate;// = (struct CandidateList *)malloc(sizeof(struct CandidateList));
	int pos, counter;
	pos = checkvoterstatus(voterid);
	if(pos == -1)
		return NOTAVOTER;
	else
	{
		currentvoter = voterlist;
		for(counter = 0 ; counter < pos ; counter++)
			currentvoter = currentvoter -> next;
		if(currentvoter -> votestatus != 0)
		{
			return ALREADYVOTED;
		}
		else
		{
			currentvoter -> votestatus = 1;
			pos = checkcandidatestatus(name);
			if(pos == -1)
			{
				if(candidatelist == NULL)
				{
					currentcandidate = (struct CandidateList *)malloc(sizeof(struct CandidateList));
					strcpy(currentcandidate -> name,name);
					currentcandidate -> voteCount = 1;
					currentcandidate -> next = NULL;
					candidatelist = currentcandidate;
					return NEW;
				}
				else
				{
					currentcandidate = candidatelist;
					while(currentcandidate -> next != NULL)
						currentcandidate = currentcandidate -> next;
					currentcandidate -> next = (struct CandidateList *)malloc(sizeof(struct CandidateList));
					currentcandidate = currentcandidate -> next;
					strcpy(currentcandidate -> name,name);
					currentcandidate -> voteCount = 1;
					currentcandidate -> next = NULL;
					return NEW;
				}
				
			}
			else
			{
				currentcandidate = candidatelist;
				for(counter = 0 ; counter < pos ; counter++)
					currentcandidate = currentcandidate -> next;
				currentcandidate->voteCount += 1;
				return CANDIDATEEXISTS;
			}
		}
	}
}



/* Function to return number of votes for a named candidate.
Parameters:	name - name of the candidate
Returns: 	voteCount, if candidate exists
			-1, otherwise
*/
char votecount(char *name)
{
	//int pos = 0;
	struct CandidateList *current;// = (struct CandidateList *)malloc(sizeof(struct CandidateList));
	current = candidatelist;
	while(current != NULL)
	{
		if(strcmp(current->name, name)==0)
		{
			return current->voteCount;
		}
		else
		{
			current = current -> next;
		}
	}
	return -1;
}

/* Function to handle request from client and respond accordingly
Parameters: sockfd - socket
			pcliaddr - client address handler
			clilen - length of pcliaddr
Returns: 	None
*/
void receivegram(int sockfd, SA *pcliaddr, socklen_t clilen)
{
	int n, i;
	socklen_t len;
	char mesg[BUFLEN];
	struct sockaddr_in * clientaddr;
	u_char function;
	
	for(;;)
	{
		len = clilen;
		n = recvfrom(sockfd, mesg, BUFLEN, 0, pcliaddr, &len);
		sscanf(mesg, "%c", &function);
		switch(function)
		{
			bool zeroizestatus;
			voterstatus addvoterstatus;
			votestatus voteforstatus;
			int votecountresult;
			u_char voterid;
			u_char packet_length;
			char candidatename[BUFLEN];
			char namedata[BUFLEN];
			char reply[BUFLEN];
			char name_length;
			char vote_count;

			case 1:	zeroizestatus = zeroize();
					if(zeroizestatus == TRUE)
						sendto(sockfd, "TRUE\n", BUFLEN, 0, pcliaddr, len);
					if(zeroizestatus == FALSE)
						sendto(sockfd, "FALSE\n", BUFLEN, 0, pcliaddr, len);
					clientaddr = (struct sockaddr_in *)pcliaddr;
					printf("Client at address %s:%d called zeroize().\n", inet_ntoa(clientaddr->sin_addr), (clientaddr->sin_port));
					break;
			
			case 2: sscanf(mesg, "%c%c", &function, &voterid);
					addvoterstatus = addvoter(voterid);
					if(addvoterstatus == OK)
						sendto(sockfd, "OK\n", BUFLEN, 0, pcliaddr, len);
					if(addvoterstatus == EXISTS)
						sendto(sockfd, "EXISTS\n", BUFLEN, 0, pcliaddr, len);
					if(addvoterstatus == ERROR)
						sendto(sockfd, "ERROR\n", BUFLEN, 0, pcliaddr, len);
					clientaddr = (struct sockaddr_in *)pcliaddr;
					printf("Client at address %s:%d called addvoter().\n", inet_ntoa(clientaddr->sin_addr), (clientaddr->sin_port));
					break;
					
			case 3:	sscanf(mesg, "%c%c%c%s", &function, &voterid, &packet_length, namedata);
					name_length = packet_length - 3;
					for(i = 0 ; i < name_length ; i++)
						candidatename[i] = *(namedata + i);
					candidatename[name_length] = '\0';
					voteforstatus = votefor(candidatename, voterid);
					if(voteforstatus == CANDIDATEEXISTS)
						sendto(sockfd, "EXISTS\n", BUFLEN, 0, pcliaddr, len);
					if(voteforstatus == NEW)
						sendto(sockfd, "NEW\n", BUFLEN, 0, pcliaddr, len);
					if(voteforstatus == NOTAVOTER)
						sendto(sockfd, "NOTAVOTER\n", BUFLEN, 0, pcliaddr, len);
					if(voteforstatus == ALREADYVOTED)
						sendto(sockfd, "ALREADYVOTED\n", BUFLEN, 0, pcliaddr, len);
					if(voteforstatus == ERROR)
						sendto(sockfd, "ERROR\n", BUFLEN, 0, pcliaddr, len);
					clientaddr = (struct sockaddr_in *)pcliaddr;
					printf("Client at address %s:%d called votefor().\n", inet_ntoa(clientaddr->sin_addr), (clientaddr->sin_port));
					break;

			case 4:	listallcandidates(reply);
					sendto(sockfd, reply, BUFLEN, 0, pcliaddr, len);
					clientaddr = (struct sockaddr_in *)pcliaddr;
					printf("Client at address %s:%d called listcandidates().\n", inet_ntoa(clientaddr->sin_addr), (clientaddr->sin_port));
					break;
			case 5:	sscanf(mesg, "%c%c%s", &function, &packet_length, namedata);
					name_length = packet_length - 2;
					for(i = 0 ; i < name_length ; i++)
						candidatename[i] = *(namedata + i);
					candidatename[name_length] = '\0';
					vote_count = votecount(candidatename);
					sprintf(reply, "%d\n", vote_count);
					sendto(sockfd, reply, BUFLEN, 0, pcliaddr, len);
					clientaddr = (struct sockaddr_in *)pcliaddr;
					printf("Client at address %s:%d called votecount.\n", inet_ntoa(clientaddr->sin_addr), (clientaddr->sin_port));
					break;
		}
	}
}


int main(int argc, char *argv[])
{
	int sockfd;
	
	/* IP address holder for server and client */
	struct sockaddr_in servaddr, cliaddr;
	char servip[20];
	
	/* check for arguments */
	if(argc!=3)
	{
		printf("usage: %s <IPaddress> <port>\n", argv[0]);
		return 0;
	}
	
	/* create socket */
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	
	/* clear out memory and assign IP parameters */
	memset((char *) &servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
	servaddr.sin_port = htons(atoi(argv[2]));
	
	/* print initialized status of server */
	inet_ntop(AF_INET, &servaddr.sin_addr, servip, 20);
	printf("Now serving Port: %d on IP: %s\n", atoi(argv[2]), servip);
	
	/* bind socket */
	bind(sockfd, (SA *) &servaddr, sizeof(servaddr));
	
	/* call function to recieve datagram */
	receivegram(sockfd, (SA *) &cliaddr, sizeof(cliaddr));
	
	return 0;
}







