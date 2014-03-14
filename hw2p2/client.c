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
	int sockfd;
	struct sockaddr_in servaddr;
	
	if(argc!=2)
	{
		printf("usage: %s <IPaddress>", argv[0]);
		return 0;
	}
	
	memset((char *) &servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
	
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	
	char buf[512];
	sprintf(buf,"%c%c%c%s", 67, 34, 8, "rohit");
	sendgram(sockfd, buf, (SA *) &servaddr, sizeof(servaddr));

	//dg_cli(stdin, sockfd, (SA *) &servaddr, sizeof(servaddr));
	
	return 0;
}