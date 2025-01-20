// Transmitter.c
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "rs232.h"


#define MAX_FRAME 8
#define MAX_BUFFER 4096
#define TTL 1000000


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


// int getAck()
// {
//     unsigned char buf[MAX_BUFFER];
//     int n;
  
  
//     n = RS232_PollComport(cport_nr, buf, MAX_BUFFER);
//     if(n > 0)
//     {
//         buf[n] = 0;
//         if(strcmp((char *)buf, "Ok") == 0)
//             return 1;
//     }
//     return 0;
// }


void sender()
{
   char buf[MAX_BUFFER];
   char frame[MAX_FRAME + 1];
  
   scanf("%[^\n]s", buf);
   while(1)
   {


       buf[strcspn(buf, "\n")] = 0;   
       printf("Data:  %s\n", buf);
       int len = strlen(buf);


       if(len > 0)
       {
           buf[len] = 0;
           printf("Length of data: %d\n", len);
           int i;
           for(i=0; i<len; i += MAX_FRAME)
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


               // start timer and send buffer
               // time1 = clock();


               RS232_SendBuf(cport_nr, (unsigned char *) frame, MAX_FRAME + 1);


               printf("Sent frame with checksum: %d\n", chkSum);
              
               for(int i=0;i<MAX_FRAME; i++)
               {
                   printf("%c", frame[i]);
               }
               printf("\n");
               // printf(" -- checksum: %c\n", chkSum);


               printf("Waiting for ack........\n");
               if(sendAck() == 1)
               {
                   printf("Acknowledgment received...\n");
                   printf("-----------------------------\n");
                   // memset(frame, 0, MAX_FRAME + 1);
                   // continue;
               }
               else
               {
                   printf("Acknowledgment not received...\n");
                   i -= MAX_FRAME;
               }
              
               // time_t time2 = clock() - time1;
               // float diff = time2 / CLOCKS_PER_SEC;


               // if(diff > 1.0)
               // {
               //     printf("Acknowledgment not received...\n");
               //     i -= MAX_FRAME;
               // }
               memset(frame, 0, MAX_FRAME + 1);
           }
           // printf("si = %d\n", i);
           return;


       }
   }
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
// gcc sw_trs_usr.c rs232.c -Wall -Wextra -o2 -o test_tx
