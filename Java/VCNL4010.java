// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// VCNL4010
// This code is designed to work with the VCNL4010_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/content/Light?sku=VCNL4010_I2CS#tabs-0-product_tabset-2

import com.pi4j.io.i2c.I2CBus;
import com.pi4j.io.i2c.I2CDevice;
import com.pi4j.io.i2c.I2CFactory;
import java.io.IOException;

public class VCNL4010
{
	public static void main(String args[]) throws Exception
	{
		// Create I2CBus
		I2CBus bus = I2CFactory.getInstance(I2CBus.BUS_1);
		// Get I2C device, VCNL4010 I2C address is 0x13(19)
		I2CDevice device = bus.getDevice(0x13);

		// Select command register
		// Enables ALS and proximity measurement, LP oscillator
		device.write(0x80, (byte)0xFF);
		// Select proximity rate register
		// 1.95 proximity measurement / s
		device.write(0x82, (byte)0x00);
		// Select ALS register
		// Continuos conversion mode, ALS rate 2 samples / s
		device.write(0x84, (byte)0x9D);
		Thread.sleep(800);

		// Read 4 bytes of data
		// luminance msb, luminance lsb, proximity msb, proximity lsb
		byte[] data = new byte[4];
		device.read(0x85, data, 0, 4);

		// Convert the data
		int luminance = ((data[0] & 0xFF) * 256) + (data[1] & 0xFF);
		int proximity = ((data[2] & 0xFF) * 256) + (data[3] & 0xFF);

		// Output data to screen
		System.out.printf("Ambient Light Luminance : %d lux %n", luminance);
		System.out.printf("Proximity Of The Device : %d %n", proximity);
	}
}
