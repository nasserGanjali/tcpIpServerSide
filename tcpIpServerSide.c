#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>

void readData(int new_fd);
void writeData(int new_fd);
/* the port users will be connecting to */
int MYPORT = 3440;
/* how many pending connections queue will hold */
#define BACKLOG 10

int main()
{
	/* listen on sock_fd, new connection on new_fd */
	int sockfd, new_fd;
	/* my address information, address where I run this program */
	struct sockaddr_in my_addr;
	/* remote address information */
	struct sockaddr_in their_addr;
	int sin_size;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1)
	{
		perror("socket() error lol!");
		exit(1);
	}
	//else
	//      printf("socket() is OK...\n");

	/* host byte order */
	my_addr.sin_family = AF_INET;
	/* short, network byte order */
	my_addr.sin_port = htons(MYPORT);
	/* auto-fill with my IP */
	my_addr.sin_addr.s_addr = INADDR_ANY;

	/* zero the rest of the struct */
	memset(&(my_addr.sin_zero), 0, 8);

	if(bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1)
	{
		perror("bind() error lol!");
		exit(1);
	}
	//else
	//      printf("bind() is OK...\n");

	if(listen(sockfd, BACKLOG) == -1)
	{
		perror("listen() error lol!");
		exit(1);
	}
	else
		printf("waiting for client ...\n");

	/* ...other codes to read the received data... */

	while(1)
	{
		sin_size = sizeof(struct sockaddr_in);
		new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);

		if(new_fd == -1)
		{
			perror("accept error !");
			continue;
		}
		else
			printf("%s is connected \n",inet_ntoa(their_addr.sin_addr));

		/*.....other codes.......*/
		int pid = fork();
		if(pid)
		{
			readData(new_fd);
			printf("killed\n");
			kill(pid, SIGKILL);
		}
		else
		{
			writeData(new_fd);
		}
		printf("%s is disconnected !!!\n",inet_ntoa(their_addr.sin_addr));
	//	printf("%d\n",close(new_fd));
	}
	close(sockfd);
	return 0;
}

void writeData(int new_fd)
{
	while(1)
	{
	//	sleep(1);
	//	printf("1\n");
//		printf(">%d<\n",new_fd);
		char *input;
		scanf("%s",input);
		write(new_fd,input,strlen(input));
	}
}

void readData(int new_fd)
{
	while(1)
	{
		char buf[256], *b;
		int msgLen = recv(new_fd,buf,256,0);
		if(msgLen > 0)
		{
			int i ;
			for(i = 0; i< msgLen; i++)
				printf("%c",buf[i]);
			fflush(stdout);
		}
		if(!msgLen)
			break;
	}
	//exit(0);
}
