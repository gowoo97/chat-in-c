#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<pthread.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

#define BUF_SIZE 1024
#define MAX_CLIENT 100
#define PORTNUM 9999


void send_msg(char* msg,int len,int sock);

int client_cnt=0;
int client_sockets[MAX_CLIENT];
pthread_mutex_t mutx;



void *handle_cli(void *arg){
	int c_sock=*((int *)arg);
	int str_len=0,i;
	char msg[BUF_SIZE];
	char *ptr;
	while((str_len=read(c_sock,msg,sizeof(msg)))!=0){
		msg[str_len]='\0';
		send_msg(msg,str_len,c_sock);
	}
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

void send_msg(char* msg,int len,int sock){
	int i,data_len,size;
	char *tmp;
	FILE *file;
	char buf[20];
	for(i=0;i<client_cnt;i++){
		write(client_sockets[i],msg,len);
	}
	size=0;
	pthread_mutex_lock(&mutx);
	tmp=strchr(msg,':');
	tmp++;
	i=0;


	if(!strncmp(tmp,"/send",5)){
		tmp=strchr(tmp,' ');
		tmp++;
		strcpy(buf,tmp);
		i=strlen(buf);
		buf[i]='\0';
		file=fopen(buf,"wb");
			
		read(sock,&data_len,sizeof(int));


		while(data_len>size){
			size+=read(sock,buf,sizeof(buf));
			fwrite(buf,sizeof(char),sizeof(buf),file);
		}


		fclose(file);	
	}
	if(!strncmp(tmp,"/down",5)){
		tmp=strchr(tmp,' ');
		tmp++;
		strcpy(buf,tmp);
		printf("%s",buf);
		i=strlen(buf);
		buf[i]='\0';
		file=fopen(buf,"rb");

		fseek(file,0,SEEK_END);
		data_len=ftell(file);
		fseek(file,0,SEEK_SET);
		
		


		write(sock,&data_len,sizeof(data_len));
		while(1){
			size=fread(buf,sizeof(char),sizeof(buf),file);
			write(sock,buf,size);
			if(feof(file))
				break;
		}
		fclose(file);
	}	
	

	
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
