#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<pthread.h>
#include<ncurses.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#define BUF_SIZE 100



void* send_msg(void* arg);
void* recv_msg(void* arg);


char ID[20];
char msg[BUF_SIZE];
char text[200];



int main(){
	int sock;
	struct sockaddr_in serv_addr;
	pthread_t snd_thread,rcv_thread;
	void* thread_return;
	int len;
	
//	initscr();
	

	printf("ID입력:");
	fgets(ID,sizeof(ID)-1,stdin);
	ID[strlen(ID)-1]='\0';
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
	char *tmp;
	FILE *file;
	int file_len,i,size;
	char buf[20];
	while(1){
		fgets(msg,BUF_SIZE,stdin);
		strcpy(text,ID);
		strcat(text,":");
		strcat(text,msg);	
			
		write(sock,text,strlen(text));//name:msg


		if(!strncmp(msg,"/send",5)){//메세지가 send명령이면
			tmp=strchr(msg,' ');
			tmp++;
			strcpy(buf,tmp);
			i=strlen(buf);
			buf[i-1]='\0';
			file=fopen(buf,"rb");
		
			fseek(file,0,SEEK_END);
			file_len=ftell(file);
			fseek(file,0,SEEK_SET);

			write(sock,&file_len,sizeof(file_len));//파일 크기 전송
			while(1){
				size=fread(buf,sizeof(char),sizeof(buf),file);
				write(sock,buf,size);	
				if(feof(file))
					break;
			}


			fclose(file);
			
		}
	
	
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
		msg[str_len]='\0';
		fputs(msg,stdout);
	
	}
	return NULL;

}
