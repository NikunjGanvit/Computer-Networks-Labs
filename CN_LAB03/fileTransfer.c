#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "rs232.h"


int main()
{
 int i=0,
     cport_nr=0,        /* /dev/ttyS0 (COM1 on windows) */
     bdrate=9600;       /* 9600 baud */


 char mode[]={'8','N','1',0};
 char buf[5708];


 int fd = open("doc.txt", O_RDONLY);


 if(fd == -1)
 {
   printf("File not found!!!\n");
   return(0);
 }


 int len = read(fd, buf, 5708);


 if(RS232_OpenComport(cport_nr, bdrate, mode))
 {
   printf("Can not open comport\n");


   return(0);
 }


 while(buf[i] != 0 && i < len)
 {
   RS232_SendByte(cport_nr, buf[i]);
   i++;
 }


 printf("sent file: text.txt\n");


 usleep(1000000);  /* sleep for 1 Second */


 return(0);
}


// gcc fileTransfer.c rs232.c -Wall -Wextra -o2 -o test_tx
// ./test_tx
