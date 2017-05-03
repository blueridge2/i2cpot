//
//Ralph Blach
//found on the web as an i2c read write example
//no copyright attached.
//this program writes to the git remote MCP23017
// i2c io expander. it turns the gpio a into to outputs and wrtes to them.
// copyright 2016 all rights reserverd
//
#include <stdio.h>
#include <unistd.h>				//Needed for I2C port
#include <fcntl.h>				//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
#include <linux/i2c-dev.h>		//Needed for I2C port

int main(int argc, char * argv[])
{
	int file_i2c;
	int length;
	unsigned char buffer[60] = {0};

	
	//----- OPEN THE I2C BUS -----
	char *filename = (char*)"/dev/i2c-1";
	if ((file_i2c = open(filename, O_RDWR)) < 0)
	{
		//ERROR HANDLING: you can check errno to see what went wrong
		printf("Failed to open the i2c bus");
		return;
	}
	
	int addr = 0x20;          //<<<<<The I2C address of the slave
	if (ioctl(file_i2c, I2C_SLAVE, addr) < 0)
	{
		printf("Failed to acquire bus access and/or talk to slave.\n");
		//ERROR HANDLING; you can check errno to see what went wrong
		return;
	}
	
	
	
	//----- write the io direction register -----
	buffer[0] = 0x00;  //write 0's the the iodirection of the first bank of registers.
	buffer[1] = 0x00;  //making them an output.
	
	length = 2;			//<<< Number of bytes to write
    //write() returns the number of bytes actually written,
    //if it doesn't match then an error occurred (e.g. no response from the device)

	if (write(file_i2c, buffer, length) != length)			{
		/* ERROR HANDLING: i2c transaction failed */
		printf("Failed to write to the i2c bus.\n");
	}
    
	length = 1;			//<<< Number of bytes to write
	if (write(file_i2c, buffer, length) != length)			{
		/* ERROR HANDLING: i2c transaction failed */
		printf("Failed to write to the i2c bus.\n");
	}


 	//----- read the io direction register from the i2cgpio -----
	length = 1;			//<<< Number of bytes to read
	if (read(file_i2c, buffer, length) != length)		
    //read() returns the number of bytes actually read, 
    //if it doesn't match then an error occurred (e.g. no response from the device)
	{
		//ERROR HANDLING: i2c transaction failed
		printf("Failed to read from the i2c bus.\n");
	}
	else
	{
		printf("IO direction register= %X\n", buffer[0]);
	}

    //now loop through and write 
    //
    while (1)
    {
        buffer[0] = 0x12;  //set the address of the of the first bank of gpio registers
        if (buffer[1])
        {
	        buffer[1] = 0x00;  //if the were 1 before, set them to zeros
        }
        else
        {
	        buffer[1] = 0xff;  //making them an output.
        }
       
	    length = 2;			//<<< Number of bytes to write
        if (write(file_i2c, buffer, length) != length)			
        {
		    /* ERROR HANDLING: i2c transaction failed */
		    printf("Failed to write to the i2c bus.\n");
	    }
        printf("sleeping\n");
        sleep(2);
    }


}


