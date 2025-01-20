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
#define MAX_BUFFER_SIZE 4096

int ind = 0;


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

// create Frame  
void createFrame(const char * str, char * frame,char * data)
{
  for(int i= ind,j= 0;i < DATA_SIZE;i++,j++)
  {
    if(str[i] == '\n')
    {
        memset(str + i, '\0', MAX_BUFFER_SIZE-i);
    }
    data[j] = str[i];
  }
  


  ind = ind + DATA_SIZE;
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

  // creating the Frame from data

//   createData(str,data);
//   memcpy(frame,data,DATA_SIZE); // append the data to main frame
//   int frameLength = strlen(frame);
//   frame[DATA_SIZE] = calculate_checksum(data,DATA_SIZE);
//   frame[FRAME_SIZE - 1] = '\0';


  
    
  // gcc swRecive.c rs232.c -Wall -Wextra -o2 -o test_rx

  

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



// gcc swTransfer.c rs232.c -Wall -Wextra -o2 -o test_tx