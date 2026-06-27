#include "alert.h"
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#define BUFFER_SIZE 8
int main(void){
	int down_flag = 0;
    int ret;
    int connection_socket;
    int data_socket;
    int result;
    ssize_t r, w;
    struct sockaddr_un  name;
    char buffer[BUFFER_SIZE];
	
	  connection_socket = socket(AF_UNIX, SOCK_STREAM, 0);
	    if(connection_socket==-1){
			perror("socket");
			return EXIT_FAILURE;
		}
		name.sun_family= AF_UNIX;
		strncpy(name.sun_path, "/tmp/procmon.sock", sizeof(name.sun_path));
		ret= bind(connection_socket, (const struct sockaddr*)&name, sizeof(name));
		if(ret==-1){
			perror("bind");
			return EXIT_FAILURE;
		}
		ret= listen(connection_socket,5);
		if(ret==-1){
			perror("listen");
			return EXIT_FAILURE;
		}
		for(;;){
			data_socket= accept(connection_socket, NULL,NULL);
			if(data_socket==-1){
				perror("accept");
				return EXIT_FAILURE;
			}
			result=0;
			for(;;){
				r= read(data_socket, buffer, sizeof(buffer));
				if(r==-1){
					perror("read");
				    return EXIT_FAILURE;
				}
				
				uint32_t pid;
				uint32_t tipoAnomalia;
				memcpy(&pid, buffer, sizeof(uint32_t));
				memcpy(&tipoAnomalia, buffer+sizeof(uint32_t), sizeof(uint32_t));
				printf("Pid processo: %d tipo anomalia %d", pid, tipoAnomalia);
				if(r==0){
					break; //se abbiamo finito di leggere buffer usciamo 
				  }
				}
				close(data_socket);
		}
		close(connection_socket);
		unlink("/tmp/procmon.sock");
		return EXIT_SUCCESS;
}