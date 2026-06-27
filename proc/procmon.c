#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <string.h>
#include "hashtable.h"
#include "logger.h"
#include <sys/socket.h>
#include <sys/un.h>
#define LOG_FILE "procmon.log" //così se cambia il nome del file non devo aggiornare tutto

int main(int argc, char*argv[]){
	HashTable*table =createHashTable();
	init_log(LOG_FILE);
	ThreadArgs args;
	int fd_alert= socket(AF_UNIX, SOCK_SEQPACKET,0);
	  if(fd_alert==-1){
		  perror("socket");
		  return EXIT_FAILURE;
	  }
	  struct sockaddr_un addr;
      addr.sun_family = AF_UNIX;
      strcpy(addr.sun_path, "/tmp/procmon.sock"); //addr corrsisponde all'indirizzo dove i datagrammi sono inviati
	  ssize_t conn=connect(fd_alert,(struct sockaddr* )&addr, sizeof(addr));
	    if(conn==-1){
			perror("connection");
			return EXIT_FAILURE;
		}
	  args.fd_alert=fd_alert;
	  args.table=table;
	  pthread_t tid;
	   if(pthread_create(&tid, manage_procdir, NULL, &args)!=0){
		   perror("Errore nella creazione del thread");
		   return EXIT_FAILURE;
	   }
	   if(pthread_join(tid, NULL)!=0){
		   perror("Errore nel join del thread");
		   return EXIT_FAILURE;
	   }
	   free_hash_table(table);
}


typedef struct{
	HashTable*table;
	int fd_alert;
}ThreadArgs;

//funzione thread che legge continuamente la directory proc per vedere se ci sono processi malevoli
 void *manage_procdir(void *arg){
	 ThreadArgs *args= (ThreadArgs*)arg;
	 DIR*dir=opendir("/proc");
	  if(dir==NULL){
		  fprintf(stderr,"Errore di apertura della directory %s: %s\n", "/proc", strerror(errno));
		  return NULL;
	  }
	  struct dirent*entry;
	  char *endptr;
	  while((entry= readdir(dir))!=NULL){
		  ssize_t pid= strtol(entry->d_name,&endptr, 10); //converte la stringa in un numero decimale 
		  if(endptr==entry->d_name){
			  printf("Nessuna cifra trovata");
		  }else if(*endptr!='\0'){
			  continue;
		  }else{
			  insertsnap(args->table,pid);
		  }
	  }
	closedir(dir);
	  while(1){
	  sleep(5);
	  DIR*dir=opendir("/proc");
	  if(dir==NULL){
		  fprintf(stderr,"Errore di apertura della directory %s: %s\n", "/proc", strerror(errno));
		  return NULL;
	  }
	  while((entry= readdir(dir))!=NULL){
		  ssize_t new_pid= strtol(entry->d_name,&endptr, 10); //converte stringhe in cifre numeriche 
		  if(endptr==entry->d_name){
			  printf("Nessuna cifra trovata\n");
		  }else if(*endptr!='\0'){
		   continue;
		  }else{
			  int suspicious=is_suspicious(args->table, new_pid);
			  if(suspicious){
				  //gestione del processo malevolo
				  append_log_entry(LOG_FILE, new_pid,1 );
				  send_alert(args->fd_alert, new_pid, ANOMALY_PROCESS_DIED); //chiamata di send_alert che invia il segnale di invio
				  removePid(args->table, new_pid);
			  }else if(!pid_exist(args->table, new_pid)){
				  insertsnap(args->table, new_pid);
				 continue;
			  }
			   else{
			   continue;
			 }
		  }
	  }
	  closedir(dir);
	}
}