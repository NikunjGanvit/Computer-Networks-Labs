#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "rs232.h"


int main()
{
 int 
     cport_nr=0,        /* /dev/ttyS0 (COM1 on windows) */
     bdrate=9600;   
     
     long long int n;    /* 9600 baud */


 unsigned char buf[927210];


 char mode[]={'8','N','1',0};


 int fd = open("text2.txt", O_RDWR | O_CREAT, S_IRWXU);
 

 if(RS232_OpenComport(cport_nr, bdrate, mode))
 {
   printf("Can not open comport\n");


   return(0);
 }


 while(1)
 {
   n = RS232_PollComport(cport_nr, buf, 927209);


   if(n > 0)
   {
     buf[n] = 0;   /* always put a "null" at the end of a string! */


    //  for(i=0; i < n; i++)
    //  {
    //    if(buf[i] < 32)  /* replace unreadable control-codes by dots */
    //    {
    //      buf[i] = '.';
    //    }
    //  }
    
     long long int len = write(fd, buf, n);
    
     printf("received %lli bytes \n", len);
   }


   usleep(100000);  /* sleep for 100 milliSeconds */


 }


 return(0);
}


// gcc fileReciver.c rs232.c -Wall -Wextra -o2 -o test_rx
// ./text_rx




