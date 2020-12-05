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
int my_sock;
pthread_mutex_t mutx;

int main(){
	int sock;
	struct sockaddr_in serv_addr;
	pthread_t snd_thread,rcv_thread;
	void* thread_return;
	int len;
	
//	initscr();
	pthread_mutex_init(&mutx,NULL);

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

	/*	if(!strncmp(msg,"/down",5)){
			pthread_mutex_lock(&mutx);
			printf("ggggggggggggggggggggggggggg\n");
			size=0;
			tmp=strchr(msg,' ');
			tmp++;
			strcpy(buf,tmp);
			printf("buf=%s\n",buf);
			i=strlen(buf);
			printf("strlen=%d",i);
			buf[i-1]='\0';	
			file=fopen(buf,"wb");
			

			read(sock,&file_len,sizeof(int));
			printf("file len=%d",file_len);
			while(file_len>size){
				size+=read(sock,buf,sizeof(buf));
				fwrite(buf,sizeof(char),sizeof(buf),file);
			}

			fclose(file);
			pthread_mutex_unlock(&mutx);

		}*/
	
	
	}	
	return NULL;

}

void* recv_msg(void* arg){

	int sock=*((int*)arg),i,file_len,size;
	char msg[BUF_SIZE];
	int str_len;
	char *tmp;
	char buf[20];
	FILE *file;
	while(1){
		
		pthread_mutex_lock(&mutx);
		size=0;
		str_len=read(sock,msg,BUF_SIZE-1);
		if(str_len==-1)
			return (void*)-1;
		msg[str_len]='\0';
		fputs(msg,stdout);

		tmp=strchr(msg,':');
		tmp++;
		strcpy(buf,tmp);

		if(!strncmp(buf,"/down",5)){
			tmp=strchr(buf,' ');
			tmp++;
			strcpy(buf,tmp);
			i=strlen(buf);
			buf[i-1]='\0';
			file=fopen(buf,"wb");

			read(sock,&file_len,sizeof(file_len));
			while(file_len>size){
				size+=read(sock,buf,sizeof(buf));
				fwrite(buf,sizeof(char),sizeof(buf),file);
			}
			fclose(file);

		}


		pthread_mutex_unlock(&mutx);
	
	}
	return NULL;

}
