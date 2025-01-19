#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include "rs232.h"


#define FRAME_SIZE 10
#define DATA_SIZE 8
#define MODULO 256


// Calculate the Check Sum
unsigned char calculate_checksum(const char * frame,int length)
{
  int checksum;
  for(int i = 0; i < length; i++)
  {
	checksum += frame[i];
  }

  return checksum % MODULO;
}


int main()
{

  char frame[FRAME_SIZE];
  char data[DATA_SIZE];


  int i=0,
  	cport_nr=0,   	 
  	bdrate=9600;  	 

  char mode[]={'8','N','1',0},
   	str[4096];


  if(RS232_OpenComport(cport_nr, bdrate, mode))
  {
	printf("Can not open comport\n");

	return(0);
  }

  // Enter the data to send
  printf("\nEnter the message : ");
  scanf("%[^\n]s",str);

  // printf("%s",str);

  // creating the Fram from data
    

  

//   while(1)
//   {

// 	scanf("%[^\n]s",str);

// 	while(str[i] != NULL)
// 	{
//     	RS232_SendByte(cport_nr,str[i]);
//     	i++;
// 	}

// #ifdef _WIN32
// 	Sleep(1000);
// #else
// 	usleep(1000000);  /* sleep for 1 Second */
//   }

  return(0);
}


