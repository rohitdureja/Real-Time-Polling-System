#include <stdio.h>
#include <stdlib.h>

struct application_header
{
	u_char function;
	u_int voterid;
	u_int packet_length;
};

int main()
{
	u_char function;
	u_char voterid;
	u_char packet_length;
	char *payload;
	int payload_length, i;
	char buf[512];
	struct application_header *appheader;
	sprintf(buf,"%c%c%c%s", 67, 34, 8, "rohit");
	printf("%s\n", buf);
	sscanf(buf, "%c%c%c%s", &function, &voterid, &packet_length, payload);
	printf("function: %d\n", function);
	printf("voter id: %d\n", voterid);
	printf("packet_length :%d\n", packet_length);
	payload_length = packet_length - 3;
	printf("payload_length :%d\n", payload_length);
	for( i = 0; i < 5 ; i++)
	{
			printf("%c", *(payload + i));
	}
}