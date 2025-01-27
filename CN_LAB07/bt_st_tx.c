#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "rs232.h"


#define MAX_FRAME 8
#define MAX_BUFFER 4096


int main()
{
   int cport_nr = 0, bdrate = 9600;
   char mode[] = {'8', 'N', '1', 0};


   if (RS232_OpenComport(cport_nr, bdrate, mode))
   {
       printf("Can not open comport\n");
       return (0);
   }


   char buf[MAX_BUFFER];
   char frame[MAX_BUFFER];


   int fd = open("text.txt", O_RDONLY);


   if (fd == -1)
   {
       printf("File can not open or file not found!!!");
       return (0);
   }


   int len = read(fd, buf, 4095);


   for(int i=0; i<len; i+=MAX_FRAME)
   {
       int frameLen = (i + MAX_FRAME <= len) ? MAX_FRAME : len - i;


       frame[0] = 'A';
       int idx = 1;
       int j = 0;
       while(frameLen != 0)
       {
           if(buf[i+j] == 'A')
           {
               frame[idx] = 'E';
               idx++;
               frame[idx] = buf[i+j];
               idx++;
           }
           else if(buf[i+j] == 'E')
           {
               frame[idx] = 'E';
               idx++;
               frame[idx] = buf[i+j];
               idx++;
           }
           else
           {
               frame[idx] = buf[i+j];
               idx++;
           }
           j++;
           frameLen--;
       }
       frame[idx] = 'A';
       idx++;
       RS232_SendBuf(cport_nr, (unsigned char *)frame, idx);
       printf("Sent frame: %s with length: %d\n", frame, idx);
       memset(frame, 0, idx);
   }
}


// gcc bt_st_tx.c rs232.c -Wall -Wextra -o2 -o test_tx

// ./test_tx