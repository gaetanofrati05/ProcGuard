#ifndef LOGGER_H
#define LOGGER_H
#define ANOMALY_PROCESS_DIED 1
#define ANOMALY_SUSPICIOUS_NEW 2
typedef struct LogEntry{
	uint32_t pid;
	uint32_t tipoAnomalia;
	uint64_t timesnap;
	uint8_t prev_hash[32];
	uint8_t curr_hash[32];
}LogEntry;
void compute_entry_hash(LogEntry*entry);
void init_log(const char*filename);
int load_last_hash(const char*filename,uint8_t out_hash[32]);
void append_log_entry(const char *filename, uint32_t pid, uint32_t tipoAnomalia);
extern pthread_mutex_t log_mutex;
#endif