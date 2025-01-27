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


   int fd = open("text2.txt", O_RDWR | O_CREAT, S_IRWXU);


   unsigned char buf[MAX_BUFFER];
   char mes[MAX_BUFFER];
   int n;
   int idx = 0;
   while (1)
   {
       n = RS232_PollComport(cport_nr, buf, MAX_BUFFER - 1);


       if (n > 0)
       {
           for (int i = 0; i < n ; i++)
           {

                if(buf[i] == 'A' && (i == 0 || i == n-1))
               {
                   continue;
               }
               else if(buf[i] == 'A' && buf[i+1] == 'A')
               {
                   i++;
                   continue;
               }
               else if (buf[i] == 'E' && (buf[i + 1] == 'A' || buf[i + 1] == 'E'))
               {
                   mes[idx] = buf[i + 1];
                   i++;
               }
               else
               {
                   mes[idx] = buf[i];
               }

               idx++;
           }


           printf("Received frame (Before deframing): %s\n", buf);
           printf("Received frame (After deframing): %s\n", mes);


           write(fd, mes, idx);


           idx = 0;
       }
    //    usleep(1000000);
   }
}


// gcc bt_st_rx.c rs232.c -Wall -Wextra -o2 -o test_rx

// ./test_rx