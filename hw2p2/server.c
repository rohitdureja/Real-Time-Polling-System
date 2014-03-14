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
	char *name;
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
	struct VoterList *current = (struct VoterList *)malloc(sizeof(struct VoterList));
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
	struct VoterList *current = (struct VoterList *)malloc(sizeof(struct VoterList));
	int pos = checkvoterstatus(voterid);
	if(pos != -1)
		return EXISTS;
	else
	{
		if(voterlist == NULL)
		{
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
	struct VoterList *current = (struct VoterList *)malloc(sizeof(struct VoterList));
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
void printallcandidates()
{
	struct CandidateList *current = (struct CandidateList *)malloc(sizeof(struct CandidateList));
	current = candidatelist;
	while(current != NULL)
	{
		printf("Voter name: %s Vote Count: %d\n", current->name, current->voteCount);
		current = current -> next;
	}
}



/* Function to check if candidate already exists.
Parameters: char *name - name of the candidate
Returns:	index, if exists
			-1 otherwise
*/
int checkcandidatestatus(char  *name)
{
	int pos = 0;
	struct CandidateList *current = (struct CandidateList *)malloc(sizeof(struct CandidateList));
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
	struct VoterList *currentvoter = (struct VoterList *)malloc(sizeof(struct VoterList));
	struct CandidateList *currentcandidate = (struct CandidateList *)malloc(sizeof(struct CandidateList));
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
					currentcandidate -> name = name;
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
					currentcandidate -> name = name;
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

void receivegram(int sockfd, SA *pcliaddr, socklen_t clilen)
{
	int n;
	socklen_t len;
	char mesg[BUFLEN];
	
	for(;;)
	{
		len = clilen;
		n = recvfrom(sockfd, mesg, BUFLEN, 0, pcliaddr, &len);
		printf("%s\n", mesg);
		sendto(sockfd, "rohitdureja", BUFLEN, 0, pcliaddr, len);
	}
}
void dg_echo(int sockfd, SA *pcliaddr, socklen_t clilen)
{
	int n;
	socklen_t len;
	char mesg[BUFLEN];
	
	for(;;)
	{
		len = clilen;
		n = recvfrom(sockfd, mesg, BUFLEN, 0, pcliaddr, &len);
		sendto(sockfd, mesg, n, 0, pcliaddr, len);
	}
}

int main(int argc, char *argv[])
{
	//zeroize();

	/* socket holder */
	int sockfd;
	
	/* IP address holder for server and client */
	struct sockaddr_in servaddr, cliaddr;
	char servip[20];
	
	/* check for arguments */
	if(argc!=2)
	{
		printf("usage: %s <IPaddress>", argv[0]);
		return 0;
	}
	
	/* create socket */
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	
	/* clear out memory and assign IP parameters */
	memset((char *) &servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
	servaddr.sin_port = htons(PORT);
	
	/* print initialized status of server */
	inet_ntop(AF_INET, &servaddr.sin_addr, servip, 20);
	printf("Now serving Port: %d on IP: %s\n", PORT, servip);
	
	/* bind socket */
	bind(sockfd, (SA *) &servaddr, sizeof(servaddr));
	
	receivegram(sockfd, (SA *) &cliaddr, sizeof(cliaddr));
	/* call function to echo */
	//dg_echo(sockfd, (SA *) &cliaddr, sizeof(cliaddr));
	
	return 0;
}







