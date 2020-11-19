#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<pthread.h>

#define BUF_SIZE 1024
#define MAX_CLIENT 100
#define PORTNUM 9999


void send_msg(char* msg,int len);

int client_cnt=0;
int client_sockets[MAX_CLIENT];
pthread_mutex_t mutx;



void *handle_cli(void *arg){
	int c_sock=*((int *)arg);
	int str_len=0,i;
	char msg[BUF_SIZE];

	while((str_len=read(c_sock,msg,sizeof(msg)))!=0)
		send_msg(msg,str_len);
	pthread_mutex_lock(&mutx);
	for(i=0;i<client_cnt;i++){
		if(c_sock==client_sockets[i]){
			while(i++<client_cnt-1)
				client_sockets[i]=client_sockets[i+1];
			break;
		}
	}
	client_cnt--;
	pthread_mutex_unlock(&mutx);
	close(c_sock);
	return NULL;
}

void send_msg(char* msg,int len){
	int i;
	pthread_mutex_lock(&mutx);
	for(i=0;i<client_cnt;i++)
		write(client_sockets[i],msg,len);
	pthread_mutex_unlock(&mutx);
}

int main(){
	int sd,cd,ns,clientlen;
	struct sockaddr_in sin,cli;
	pthread_t t_id;

	clientlen=sizeof(cli);
	pthread_mutex_init(&mutx,NULL);
	sd=socket(AF_INET,SOCK_STREAM,0);

	memset((char *)&sin,'\0',sizeof(sin));
	sin.sin_family=AF_INET;
	sin.sin_port=htons(PORTNUM);
	sin.sin_addr.s_addr=inet_addr("127.0.0.1");

	if(bind(sd,(struct sockaddr *)&sin,sizeof(sin))){
		perror("bind");
		exit(1);
	}
	if(listen(sd,10)){
		perror("listen");
		exit(1);
	}

	while(1){
		if((ns=accept(sd,(struct sockaddr *)&cli,&clientlen))==-1){
			perror("accept");
			exit(1);
		}
		
		pthread_mutex_lock(&mutx);
		client_sockets[client_cnt++]=ns;
		pthread_mutex_unlock(&mutx);

		pthread_create(&t_id,NULL,handle_cli,(void*)&ns);
			
	
	}
	return 0;


}