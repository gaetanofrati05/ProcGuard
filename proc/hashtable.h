#ifndef HASHTABLE_H
#define HASHTABLE_H

/*Struttura dati hashtable*/
typedef struct Entry{
	ssize_t snap_pid;
	struct Entry *next;
}Entry;

typedef struct HashTable{
	Entry **buckets;
}HashTable;
/*Definizione funzioni per la struttura dati hash*/
unsigned int hash_function(ssize_t pid);
HashTable *createHashTable();
void insertsnap(HashTable *table, ssize_t snap_pid);
int is_suspicious(HashTable *table,ssize_t newsnap_pid);
int pid_exist(HashTable*table, ssize_t newsnap_pid);
void remove(HashTable*table, ssize-t snap_pid);
void free_hash_table(HashTable*table);
#endif