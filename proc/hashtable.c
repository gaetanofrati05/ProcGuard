#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <hashtable.h>
#define SIZE_BUCKETS 4096

unsigned int hash_function(ssize_t snap_pid){
	return snap_pid % SIZE_BUCKETS;
}

HashTable *createHashTable(){
	HashTable *table= (HashTable*)malloc(sizeof(HashTable));
	  if(table==NULL){
		  perror("malloc/table");
		  exit(1);
	  }
	table->buckets= malloc(sizeof(Entry*)*SIZE_BUCKETS);
	   if(table->buckets==NULL){
		   perror("malloc/buckets");
		   exit(1);
	   }
	for(int i=0; i<SIZE_BUCKETS;i++){
		table->buckets[i]=NULL;
	}
	return table;
}

void insertsnap(HashTable*table, ssize_t snap_pid){
	unsigned index= hash_function(snap_pid);
	Entry *current=table->buckets[index];
	while(current){
		if(current->snap_pid==snap_pid){
			current->snap_pid=snap_pid;
			return;
		}
		current=current->next;
	}
	Entry *new_node= malloc(sizeof(Entry));
	  if(new_node==NULL){
		  perror("malloc/node");
		  return;
	  }
	  new_node->snap_pid=snap_pid;
	  new_node->next= table->buckets[index];
	  table->buckets[index]=new_node;
}

int is_suspicious(HashTable*table,ssize_t newsnap_pid){
	unsigned index= hash_function(newsnap_pid);
	Entry *current= table->buckets[index];
	  while(current){
		  if(current->snap_pid==newsnap_pid){
			return 0;
		  }
		  current=current->next;
	  }
	  return 1; //processo morto o sospetto
}
int pid_exist(HashTable*table, ssize_t newsnap_pid){
	int trovato=0;
	unsigned index= hash_function(newsnap_pid);
	Entry*current= table->buckets[index];
	while(current){
		if(current->snap_pid==snap_pid){
			trovato=1;
			break;
		}
		current=current->next;
	}
	return trovato;
}
void removePid(HashTable*table, ssize_t snap_pid){
	Entry*prev=NULL;
	unsigned index= hash_function(snap_pid);
	Entry*current= table->buckets[index];
	while(current){
		if(current->snap_pid= snap_pid){
			if(prev==NULL){
				bucket[index]=current->next;
			}else{
				prev->next=current->next;
				free(current);
				return;
			}
		}
		prev=current;
		current=current->next;
	}
}
void free_hash_table(HashTable*table){
	for(int i=0; i<SIZE_BUCKETS; i++){
		Entry*current= table->buckets[i];
		while(current){
			Entry*temp=current;
			current=current->next;
			free(temp);
		}
	}
	free(table->buckets);
    free(table);	
}
