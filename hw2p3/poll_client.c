/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "poll.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main( int argc, char* argv[] )
{
	char *host;
	CLIENT *clnt;
	char * *result_1;
	char*  zeroize_1_arg;
	char * *result_2;
	int  addvoter_1_arg;
	char * *result_3;
	Vote  votefor_1_arg;
	Candidates  *result_4;
	char*  listcandidates_1_arg;
	int  *result_5;
	char * votecount_1_arg;
    
	/* error checking in case of less arguments */
	if(argc < 3)
	{
		printf("usage: %s <IPaddress> -options\n", argv[0]);
		printf("available options: \n");
		printf("\tzeroize()\t\t: .\\client <IPaddress> -z\n");
		printf("\taddvoter(voterid)\t: .\\client <IPaddress> -a -i <voterID>\n");
		printf("\tvotefor(name, voterid)\t: .\\client <IPaddress> -v -n <name> -i <voterID>\n");
		printf("\tlistcandidates()\t: .\\client <IPaddress> -l\n");
		printf("\tvotecount(name)\t\t: .\\client <IPaddress> -c -n <name>\n");
		return 0;
	}
    
	host = argv[1];
	clnt = clnt_create(host, POLL_PROG, POLL_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror(host);
		exit(1);
	}
    
	/* determine procedure call and generate datagram packet */
	if(!strcmp(argv[2],"-z"))
	{
		/* zeroize() call */
		result_1 = zeroize_1((void*)&zeroize_1_arg, clnt);
		if (result_1 == NULL) {
			clnt_perror(clnt, "call failed:");
		}
		else
		{
			printf("%s", *result_1);
		}
	}
	else if(!strcmp(argv[2], "-a"))
	{
		/* addvoter(voterid) call */
		addvoter_1_arg = atoi(argv[4]);
		result_2 = addvoter_1(&addvoter_1_arg, clnt);
		if (result_2 == NULL) {
			clnt_perror(clnt, "call failed:");
		}
		else
		{
			printf("%s", *result_2);
		}
	}
	else if(!strcmp(argv[2], "-v"))
	{
		/*votefor(name, voterid) call */
		votefor_1_arg.voterid = atoi(argv[6]);
		strcpy(votefor_1_arg.name, argv[4]);
		result_3 = votefor_1(&votefor_1_arg, clnt);
		if (result_3 == NULL) {
			clnt_perror(clnt, "call failed:");
		}
		else
		{
			printf("%s", *result_3);
		}
	}
	else if(!strcmp(argv[2], "-l"))
	{
		/* listcandidates() call */
		result_4 = listcandidates_1((void*)&listcandidates_1_arg, clnt);
		if (result_4 == NULL) {
			clnt_perror(clnt, "call failed:");
		}
		else
		{
			Candidates *current;
			current = result_4;
			while(current!=NULL)
			{
				printf("%s\n", current->name);
				current = current -> next;
			}
		}
	}
	else if(!strcmp(argv[2], "-c"))
	{
		/* votecount(name) call */
		votecount_1_arg = argv[4];
		result_5 = votecount_1(&votecount_1_arg, clnt);
		if (result_5 == NULL) {
			clnt_perror(clnt, "call failed:");
		}
		else
		{
			printf("%d\n", *result_5);
		}
	}
	clnt_destroy( clnt );
	return 0;
}
