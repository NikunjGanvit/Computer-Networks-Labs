// using Max retry

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "rs232.h"


#define MAX_FRAME 8
#define MAX_BUFFER 4096
#define TTL 1000000
#define MAX_RETRY 5


int bdrate = 9600, cport_nr = 0;
clock_t time1;
char mode[] = {'8', 'N', '1', 0};


int calculateCheckSum(char *frame, int size)
{
   int sum = 0;


   for(int i=0; i<size; i++)
   {
       sum += frame[i];
   }


   return sum % 256;
}


int sendAck()
{
   unsigned char buf[MAX_BUFFER];
   int n;
   int time = 0;


   while(time < TTL)
   {
       n = RS232_PollComport(cport_nr, buf, MAX_BUFFER);
       if(n > 0)
       {
           buf[n] = 0;
           if(strcmp((char *)buf, "Ok") == 0)
               return 1;
       }
       usleep(100000);
       time += 100000;
   }


   // Timeout --> no ack received
   return 0;
}


void sender()
{
   char buf[MAX_BUFFER];
   char frame[MAX_FRAME + 1];


   int try = 0;


   int fd = open("text.txt", O_RDONLY);


   if(fd == -1)
   {
       printf("Can't open file or file not found\n");
       return;
   }


   int len = read(fd, buf, MAX_BUFFER);
   int frameNo = 0;
  
   while(1)
   {


       buf[strcspn(buf, "\n")] = 0;   
       // printf("Data:  %s\n", buf);


       if(len > 0)
       {
           buf[len] = 0;
           printf("Length of data: %d\n", len);
          
           for(int i=0; i<len; i += MAX_FRAME)
           {
              


               printf("i = %d\n", i);
               int frameLen = (i + MAX_FRAME <= len) ? MAX_FRAME : len - i;


               printf("Frame length: %d\n", frameLen);
               strncpy(frame, buf + i, frameLen);


               if(frameLen < MAX_FRAME)
               {
                   memset(frame + frameLen, 0, MAX_FRAME - frameLen);
                  
                   // strncat(frame + frameLen, 0, MAX_FRAME - frameLen);
               }


               int chkSum = calculateCheckSum(frame, MAX_FRAME);


               frame[MAX_FRAME] = chkSum;
              
               frameNo++;


               if(frameNo == 5)
               {
                   frame[MAX_FRAME - 3] = frame[MAX_FRAME - 3] + 1;
                   frameNo = 0;
               }


               // start timer and send buffer
               time1 = clock();


               RS232_SendBuf(cport_nr, (unsigned char *) frame, MAX_FRAME + 1);


               printf("Sent frame with checksum: %d\n", chkSum);
              
               for(int i=0;i<MAX_FRAME; i++)
               {
                   printf("%c", frame[i]);
               }
               printf("\n");
               // printf(" -- checksum: %c\n", chkSum);


               // // Acknowledgment using clock() system call 
               int isTimeOut = 0;
               while(1)
               {
                   unsigned char buf[MAX_BUFFER];
                   int n = RS232_PollComport(cport_nr, buf, MAX_BUFFER);
                   if(n > 0)
                   {
                       if(strcmp((char *)buf, "Ok") == 0)
                           break;
                   }
                   time_t time2 = clock() - time1;
                   float diff = time2 / CLOCKS_PER_SEC;
                   if(diff > 1.0)
                   {
                       isTimeOut = 1;
                       break;
                   }
               }
               if(!isTimeOut)
               {
                   printf("Acknowledgment received...\n");
                   printf("--------------------\n");
               }
               else
               {
                   printf("Acknowledgment not received...\n");
                   try++;
                   printf("Retransmiting %d time...\n", try);
                   printf("--------------------\n");
                   if(try >= MAX_RETRY)
                   {
                       return;
                   }
                   i -= MAX_FRAME;
               }


               // Acknowledgment using counter
               // printf("Waiting for ack........\n");
               // if(sendAck() == 1)
               // {
               //     printf("Acknowledgment received...\n");
               //     printf("-----------------------------\n");
               // }
               // else
               // {
               //     printf("Acknowledgment not received...\n");
               //     i -= MAX_FRAME;
               // }
              
               memset(frame, 0, MAX_FRAME + 1);
           }
           return;


       }
   }
   close(fd);
}


int main()
{
   if(RS232_OpenComport(cport_nr, bdrate, mode))
   {
       printf("Error in opening Com port\n");
       return 0;
   }
   sender();
}


// gcc File_stop_and_wait_send.c rs232.c -Wall -Wextra -o2 -o test_tx
