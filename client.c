#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<pthread.h>
#include<ncurses.h>
#define BUF_SIZE 100



void* send_msg(void* arg);
void* recv_msg(void* arg);


char name[20];
char msg[BUF_SIZE];




int main(){
	int sock;
	struct sockaddr_in serv_addr;
	pthread_t snd_thread,rcv_thread;
	void* thread_return;


	sock=socket(AF_INET,SOCK_STREAM,0);
		
	memset(&serv_addr,0,sizeof(serv_addr));	
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	serv_addr.sin_port=htons(9999);

	if(connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1){
		perror("conncet");
		exit(1);
	}

	pthread_create(&snd_thread,NULL,send_msg,(void*)&sock);
	pthread_create(&rcv_thread,NULL,recv_msg,(void*)&sock);
	pthread_join(snd_thread,&thread_return);
	pthread_join(rcv_thread,&thread_return);
	close(sock);
	return 0;
}


void* send_msg(void* arg){
	int sock=*((int*)arg);	
	
	while(1){
		fgets(msg,BUF_SIZE,stdin);

		write(sock,msg,strlen(msg));
	
	
	}	
	return NULL;

}

void* recv_msg(void* arg){

	int sock=*((int*)arg);
	char msg[BUF_SIZE];
	int str_len;
	while(1){
	
		str_len=read(sock,msg,BUF_SIZE-1);
		if(str_len==-1)
			return (void*)-1;
		msg[str_len]=0;
		fputs(msg,stdout);
	
	}
	return NULL;

}
