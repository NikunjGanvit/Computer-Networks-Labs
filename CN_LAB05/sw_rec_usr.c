#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "rs232.h"


#define MAX_FRAME 8
#define MAX_BUFFER 4096
#define TTL 1000000


int bdrate = 9600, cport_nr = 0;


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


void receiver()
{
   unsigned char buf[MAX_BUFFER];
   int n, idx = 0;
   char frame[MAX_FRAME + 1];


   while(1)
   {
       n = RS232_PollComport(cport_nr, buf + idx, MAX_BUFFER - idx);
       // printf("%d", n);
       if(n > 0)
       {
           idx += n;
           if(idx >= MAX_FRAME + 1)
           {
               strncpy(frame, (char *)buf, MAX_FRAME);
               int senderChkSum = buf[MAX_FRAME];
               int receiverChkSum = calculateCheckSum(frame, MAX_FRAME);
                printf("%s",frame);
               printf("Received checksum: %d, Calculated checksum: %d\n",senderChkSum, receiverChkSum);


               if(senderChkSum != receiverChkSum)
               {
                   printf("Error in data while calculating checksum.\n");
               }
               else
               {
                   printf("Received one frame : %s\n", frame);
                   RS232_SendBuf(cport_nr, (unsigned char *)"Ok", 3);
                   printf("Ack sent\n");
               }
               idx = 0;
           }


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


   receiver();


}
// gcc sw_rec_usr.c rs232.c -Wall -Wextra -o2 -o test_rx
