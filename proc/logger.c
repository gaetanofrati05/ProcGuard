#include <stdint.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>      
#include "sha256.h"
#include "logger.h"
pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;
static size_t serialize_LogEntry(const LogEntry*entry, uint8_t *out){
	size_t offset=0;
	memcpy(out+offset,&entry->pid, sizeof(uint32_t));
	offset+=sizeof(uint32_t);
	memcpy(out+offset,&entry->tipoAnomalia, sizeof(uint32_t));
	offset+=sizeof(uint32_t);
	memcpy(out+offset,&entry->timesnap, sizeof(uint64_t));
	offset+=sizeof(uint64_t);
	memcpy(out+offset,entry->prev_hash,32);
	offset+=32;
	return offset;
}
void compute_entry_hash(LogEntry*entry){
	uint8_t buffer[48];
	size_t len= serialize_LogEntry(entry, buffer);
	compute_sha256(buffer, len, entry->curr_hash);
}

void init_log(const char*filename){
	FILE *f=fopen(filename, "rb");
	 if(f!=NULL){
		 fclose(f);
		 return;
	 }
	 f=fopen(filename, "wb");
	 LogEntry genesis;
	 memset(&genesis, 0, sizeof(LogEntry));
	 genesis.pid=0;
	 genesis.tipoAnomalia=0;
	 genesis.timesnap=(uint64_t)time(NULL);
	 compute_entry_hash(&genesis);
	 fwrite(&genesis,sizeof(LogEntry), 1,f);
	 fclose(f);
}

int load_last_hash(const char*filename,uint8_t out_hash[32]){
	FILE*f=fopen(filename, "rb");
	if(!f) return 0;
	if(fseek(f,-sizeof(LogEntry),SEEK_END)!=0){
		fclose(f);
		return 0;
	}
	LogEntry last;
	size_t r= fread(&last,sizeof(LogEntry),1,f);
	if(r!=1) return 0;
	memcpy(out_hash, last.curr_hash,32);
	return 1;
}

void append_log_entry(const char *filename, uint32_t pid, uint32_t tipoAnomalia){
	LogEntry entry;
	uint8_t last_hash[32];
	int valido= load_last_hash(filename,last_hash);
	if(valido){
		memcpy(entry.prev_hash, last_hash, 32);
		entry.pid=pid;
		entry.tipoAnomalia=tipoAnomalia;
		entry.timesnap=(uint64_t) time(NULL);
		compute_entry_hash(&entry);
		pthread_mutex_lock(&log_mutex);
		FILE*f=fopen(filename, "ab");
	    if(!f){
			pthread_mutex_unlock(&log_mutex);
			return;
		}
		fwrite(&entry, sizeof(LogEntry), 1,f);
		fclose(f);
		pthread_mutex_unlock(&log_mutex);
		
	}else{
		perror("invalid");
		return;
	}
}
