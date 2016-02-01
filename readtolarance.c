//
// this program reads the tolerance registers on the i2c pots
// Pots 0 and 1 have the same tolerance registers.
// pots 2 and 3 have the same tolerance registers.
// see the AD5251 spec for a description of the regesters
//
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
//read_i2cbytes
//

int readI2cbytes(int i2cFileHandle, int length,unsigned char *buffer)
{
   int rc;
   if (  (rc=read(i2cFileHandle, buffer, length)) != length)	
   {
	   printf("Failed to read from the i2c bus.\n");
      return rc;
	}
   return 0;
	
}
//
//write the pot instruction address register with the bit 7=0 (cmd/reg) and (bit 5) ee/rdac =1
//
int writePotInstructionAddressRegister(int i2cFileHandle, int address)
{

   unsigned int cmdregister=0x20 | 0x10|address;
   unsigned char buffer[256];
   int length ;

   //printf ("address=0x%x\n",cmdregister);
    
   buffer[0] = cmdregister;
	length = 1;			//<<< Number of bytes to write

	if (write(i2cFileHandle, buffer, length) != length)	
	{
      //i2c transaction failed
		printf("Failed to write to the i2c bus.\n");
      return -1;
	}
   return 0;

}
//
//main program
//
int main(int argc, char * argv[])
{
	int i2cFileHandle;
   unsigned char  buffer[60] = {0};
   int rc;
   int address;
   char *endptr;
   int potnumber;
   char i2cFilename[256] = {0};
   unsigned char i2cPotAddress;



   if (argc !=3 )
   {
      printf( "readtolerance takes two arguments ,i2cbus,  pot number, \n");
      printf( "The bus number for Raspberry Pi is 1, and for the other modules use i2c detect\n");
      return -1;

   }
   errno = 0;

   if (  !((*argv[1] >=0x30 ) &&   (*argv[1] <=0x32)))
   {
      printf("bad i2c bus number\n");
      return 1;
   }

   sprintf(i2cFilename,"/dev/i2c-%s",argv[1]);
   printf("i2cfilenam->%s\n",i2cFilename);
   
   potnumber = (int)strtol(argv[2], &endptr, 10);
   if (endptr == argv[1])
   {
      printf("Bad pot number, must be 0,1 2 or 3\n");
      return -1;
   }
   if ( ((potnumber ==LONG_MAX) || (potnumber == LONG_MIN)) && errno == ERANGE)
   {
      printf("Bad pot number, must be 1,2 3 or 4\n");
      return -1;
   }
   if ( (potnumber==0) || (potnumber==1) )
   {
      i2cPotAddress = DUALPOT1;
   }
   else
   {
      i2cPotAddress = DUALPOT2;
   }
           
	//
   //open the i2c buss
   //
	
	if ((i2cFileHandle = open(i2cFilename, O_RDWR)) < 0)
	{
		//ERROR HANDLING: you can check errno to see what went wrong
		printf("Failed to open the i2c bus");
		return i2cFileHandle;
	}
   //
   //set the i2c slave address in the driver
   //
	if ( (rc=ioctl(i2cFileHandle, I2C_SLAVE,i2cPotAddress )) < 0)
	{
		printf("Failed to acquire bus access and/or talk to slave.\n");
		return rc;
	}
    for (address=0;address<=0xf;address++)
    {
         writePotInstructionAddressRegister(i2cFileHandle, address);
         readI2cbytes( i2cFileHandle, 1,buffer);
         printf("addr=%2x, buffer=%x\n",address|0x10,buffer[0]);



    }
 return 0;

}


