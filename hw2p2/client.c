#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SA struct sockaddr

#define BUFLEN 512
#define NPACK 10
#define PORT 9930

void sendgram(int sockfd, char * data, const SA *pservaddr, socklen_t servlen)
{
	int n;
	char reply[BUFLEN];
	sendto(sockfd, data, strlen(data), 0, pservaddr, servlen);
	
	n = recvfrom(sockfd, reply, BUFLEN, 0, NULL, NULL);
	reply[n] = '\0';
	fputs(reply, stdout);
}

void dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
	int n;
	char sendline[BUFLEN], recvline[BUFLEN + 1];
	while(fgets(sendline, BUFLEN, fp) != NULL)
	{
		sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);
		
		n = recvfrom(sockfd, recvline, BUFLEN, 0, NULL, NULL);
		recvline[n] = '\0';
		fputs(recvline, stdout);
	}
}

int main(int argc, char *argv[])
{
	int sockfd; /* socket */
	struct sockaddr_in servaddr; /* server address */
	char datagram[BUFLEN]; /* datagram sent to server */
	
	/* datagram parameters */
	u_char function;
	u_char voterid;
	u_char packet_length;
	char *candidatename;
	
	/* error checking in case of less arguments */
	if(argc < 4)
	{
		printf("usage: %s <IPaddress> <port> -options\n", argv[0]);
		printf("available options: \n");
		printf("\tzeroize()\t\t: .\\client <IPaddress> <port> -z\n");
		printf("\taddvoter(voterid)\t: .\\client <IPaddress> <port> -a -i <voterID>\n");
		printf("\tvotefor(name, voterid)\t: .\\client <IPaddress> <port> -v -n <name> -i <voterID>\n");
		printf("\tlistcandidates()\t: .\\client <IPaddress> <port> -l\n");
		printf("\tvotecount(name)\t\t: .\\client <IPaddress> <port> -c -n <name>\n");
		return 0;
	}
	
	/* clear out memory and assign IP parameters */
	memset((char *) &servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(atoi(argv[2]));
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
	
	/* create socket */
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	
	/* determine procedure call and generate datagram packet */
	if(!strcmp(argv[3],"-z"))
	{
		/* zeroize() call */
		function = 1;
		sprintf(datagram, "%c", function);
		sendgram(sockfd, datagram, (SA *) &servaddr, sizeof(servaddr));
	}
	if(!strcmp(argv[3], "-a"))
	{
		/* addvoter(voterid) call */
		function = 2;
		voterid = atoi(argv[5]);
		sprintf(datagram, "%c%c", function, voterid);
		sendgram(sockfd, datagram, (SA *) &servaddr, sizeof(servaddr));
	}
	if(!strcmp(argv[3], "-v"))
	{
		/*votefor(name, voterid) call */
		function = 3;
		candidatename = argv[5];
		voterid = atoi(argv[7]);
		packet_length = 3 + strlen(candidatename);
		sprintf(datagram, "%c%c%c%s", function, voterid, packet_length, candidatename);
		sendgram(sockfd, datagram, (SA *) &servaddr, sizeof(servaddr));
	}
	if(!strcmp(argv[3], "-l"))
	{
		/* listcandidates() call */
		function = 4;
		sprintf(datagram, "%c", function);
		sendgram(sockfd, datagram, (SA *) &servaddr, sizeof(servaddr));
	}
	if(!strcmp(argv[3], "-c"))
	{
		/* votecount(name) call */
		function = 5;
		candidatename = argv[5];
		packet_length = 2 + strlen(candidatename);
		sprintf(datagram, "%c%c%s", function, packet_length, candidatename);
		sendgram(sockfd, datagram, (SA *) &servaddr, sizeof(servaddr));
	}
	
	return 0;
}