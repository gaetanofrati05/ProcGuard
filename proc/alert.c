#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include "alert.h"
#include <sys/socket.h>
#include <sys/un.h>

void send_alert(int fd, uint32_t pid, uint32_t tipoAnomalia){
	uint8_t buffer[8]; //serializza a 8 bit send_alert e invia tutto con send alla socket creata nel main 
	memcpy(buffer, &pid, sizeof(uint32_t));
	memcpy(buffer +sizeof(uint32_t), &tipoAnomalia, sizeof(uint32_t));
	ssize_t sent=send(fd, buffer, sizeof(buffer), 0);
	   if(sent==-1){
		   perror("send");
	   }
}