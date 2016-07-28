// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// VCNL4010
// This code is designed to work with the VCNL4010_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/content/Light?sku=VCNL4010_I2CS#tabs-0-product_tabset-2

#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

void main() 
{
	// Create I2C bus
	int file;
	char *bus = "/dev/i2c-1";
	if ((file = open(bus, O_RDWR)) < 0) 
	{
		printf("Failed to open the bus. \n");
		exit(1);
	}
	// Get I2C device, VCNL4010 I2C address is 0x13(19)
	ioctl(file, I2C_SLAVE, 0x13);

	// Select command register(0x80)
	// Enables ALS and proximity measurement, LP oscillator(0xFF)
	char config[2] = {0};
	config[0] = 0x80;
	config[1] = 0xFF;
	write(file, config, 2);
	// Select proximity rate register(0x82)
	// 1.95 proximity measurement / s(0x00)
	config[0] = 0x82;
	config[1] = 0x00;
	write(file, config, 2);
	// Select ALS register(0x84)
	// Continuos conversion mode, ALS rate 2 samples / s(0x9D)
	config[0] = 0x84;
	config[1] = 0x9D;
	write(file, config, 2);

	// Read 4 bytes of data from register(0x85)
	// luminance msb, luminance lsb, proximity msb, proximity lsb
	char reg[1] = {0x85};
	write(file, reg, 1);
	char data[4] ={0};
	if(read(file, data, 4) != 4)
	{
		printf("Error : Input/Output error \n");
	}
	else
	{
		// Convert the data
		int luminance = (data[0] * 256 + data[1]);
		int proximity = (data[2] * 256 + data[3]);

		// Output data to screen
		printf("Ambient Light Luminance : %d lux \n", luminance);
		printf("Proximity Of The Device : %d \n", proximity);
	}
}
