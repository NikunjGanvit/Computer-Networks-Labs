#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "rs232.h"
int cport_nr = 0;  /* /dev/ttyS0 (COM1 on Windows) */
int bdrate = 9600; /* 9600 baud */
char mode[] = {'8', 'N', '1', 0};




void *read_serial() {
  unsigned char buffer[4096];
  int n;
  while (1) {
      memset(buffer, 0, sizeof(buffer));
      n = RS232_PollComport(cport_nr, buffer, sizeof(buffer));
      if (n > 0) {
          printf("\nReceived: %s\n", buffer);
          fflush(stdout);
      }
  }
  return NULL;
}
int main(){

  pthread_t recv_t;
  if(RS232_OpenComport(cport_nr , bdrate, mode)){
      printf("Cannot open comport\n");
      return 1;
  }
  if(pthread_create(&recv_t,NULL,read_serial,NULL) != 0){
      printf("Error creating receive thread\n");
      RS232_CloseComport(cport_nr);
      return 1;
  }
 char send_buf[4096];
  while(1){
      if (fgets(send_buf, sizeof(send_buf), stdin) != NULL)
      {
          // send_buf[strcspn(send_buf, "\n")] = 0;
          RS232_SendBuf(cport_nr, (unsigned char *)send_buf, strlen(send_buf));
          printf("Sent: %s\n", send_buf);
      }
  }
  // int byte = read(0,buffer,sizeof(buffer));
  RS232_CloseComport(cport_nr);
  pthread_join(recv_t, NULL);
  return 0;
}
