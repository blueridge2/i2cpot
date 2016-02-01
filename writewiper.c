#include <errno.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>				//Needed for I2C port
#include <fcntl.h>				//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
#include <linux/i2c-dev.h>		//Needed for I2C port
#include "i2cpot.h"
//
//there are 4 pots 0, 1, 2 ,3
//0 and 1 are on pot DUALPOT1
//2 and 3 are on DUALPOT2
//each i2c contains 2 pots, one at internal address 1 and the other internal address 3
//
//so pot 0 and 2 use internal address 1
//   pot 1 and 3 use internal address 3
//
int main(int argc, char * argv[])
{
	int file_i2c;
	int length;
   int potnumber = 0;
   int i2cAddress = 0;
   int potPosition;
   char argv1;
   char *str,*endptr;
	unsigned char buffer[60] = {0};
   char i2cFilename[256]={0};

   
   if (argc !=4 )
   {
      printf( "writewiper takes 1 argument,i2cbus,  pit number, pot the pot postion\n");
      printf( "The bus number for Raspberry Pi is 1, and for the other modules use i2c detect\n");
      return -1;

   }
   errno = 0;
   argv1=*argv[1];

   if (  !((*argv[1] >=0x30 ) &&   (*argv[1] <=0x32)))
   {
      printf("bad i2c bus number\n");
      return 1;
   }

   sprintf(i2cFilename,"/dev/i2c-%s",argv[1]);
   printf("i2cfilenam->%s\n",i2cFilename);
   //
   // find the pot number
   //
   potnumber = (int)strtol(argv[2], &endptr, 10);
   if (endptr == argv[1])
   {
      printf("Bad pot number, must be 1,2 3 or 4\n");
      return -1;
   }
   if ( ((potnumber ==LONG_MAX) || (potnumber == LONG_MIN)) && errno == ERANGE)
   {
      printf("Bad pot number, must be 1,2 3 or 4\n");
      return -1;
   }
   else if ( ! (potnumber >=0) && (potnumber <= 3))
   {
      printf("Bad pot number, must be 1,2 3 or 4\n");
      return -1;
   }
         
   //
   //convert the pot postion
   //
   errno=0;
   potPosition = (int)strtol(argv[3], (char **)NULL, 10);
   if (endptr == argv[2])
   {
      printf("Bad pot position, must be >= 0 and <=255\n");
      return -1;
   }

   if ( ((potPosition ==LONG_MAX) || (potPosition == LONG_MIN)) && errno == ERANGE)
   {
      printf("Bad pot position, must be >= 0 and <=255\n");
      return -1;
   }
   else if ( ! ((potPosition >=0) && (potPosition <= 255)))
   {
      printf("Bad pot position, must be >= 0 and <=255\n");
      return -1;
   }
	printf("pot number=%d, pot position=%d\n",potnumber, potPosition);

      
	//----- OPEN THE I2C BUS -----
	if ((file_i2c = open(i2cFilename, O_RDWR)) < 0)
	{
		printf("Failed to open the i2c bus");
		return -1;
	}
	if ( (potnumber == 0) || (potnumber == 1) )
   {
	   i2cAddress =  DUALPOT1 ;          //<<<<<The I2C address of the slave
   }
   else if ( (potnumber == 2) || (potnumber==3) )
   {
	   i2cAddress =  DUALPOT2 ;          //<<<<<The I2C address of the slave
   }
   else
   {
      printf("bad pot address passed, must be 0, 1 , 2 or 3\n");
      return -1;

   }
   //
   //set the i2c slave address
   //
	if (ioctl(file_i2c, I2C_SLAVE, i2cAddress) < 0)
	{
		printf("Failed to acquire bus access and/or talk to slave.\n");
		return;
	}
	
#if 0	
	//----- READ BYTES -----
	length = 4;			//<<< Number of bytes to read from the i2c 
	if (read(file_i2c, buffer, length) != length)	
	{
		printf("Failed to read from the i2c bus.\n");
	}
	else
	{
		printf("Data read: %s\n", buffer);
	}
#endif	
	//select the correct internal address of the i2c device to which to write.
   if (potnumber == 0 || potnumber == 2)
   {
      buffer[0] = 0x01;
   }
   else
   {
      buffer[0] = 0x03;
   }
	buffer[1] = potPosition;
	length = 2;			//<<< Number of bytes to write
	if (write(file_i2c, buffer, length) != length)	
	{
		printf("Failed to write pot postion to the i2c bus.\n");
      return -1;
	}
   printf("write sucessfull\n");
 return 0;

}


