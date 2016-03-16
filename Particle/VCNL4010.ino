// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// VCNL4010
// This code is designed to work with the VCNL4010_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/content/Light?sku=VCNL4010_I2CS#tabs-0-product_tabset-2

#include <application.h>
#include <spark_wiring_i2c.h>

// VCNL4010 I2C address is 0x13(19)
#define Addr 0x13

float luminance = 0, proximity = 0;
void setup()
{
    // Set variable
    Particle.variable("i2cdevice", "VCNL4010");
    Particle.variable("luminance", luminance);
    Particle.variable("proximity", proximity);
    
    // Initialise I2C communication as MASTER 
    Wire.begin();
    // Initialise Serial Communication, set baud rate = 9600
    Serial.begin(9600);
    
    // Start I2C Transmission
    Wire.beginTransmission(Addr);
    // Select command register
    Wire.write(0x80);
    // Enable ALS and proximity measurement, LP oscillator
    Wire.write(0xFF);
    // Stop I2C Transmission
    Wire.endTransmission();
    
    // Start I2C Transmission
    Wire.beginTransmission(Addr);
    // Select proximity rate register
    Wire.write(0x82);
    // Set 1.95 proximity measurements/sec
    Wire.write(0x00);
    // Stop I2C Transmission
    Wire.endTransmission();

    // Start I2C Transmission
    Wire.beginTransmission(Addr);
    // Select ambient light rate register
    Wire.write(0x84);
    // Continuos conversion mode, Ambient light rate 2 samples/s
    Wire.write(0x9D);
    // Stop I2C Transmission
    Wire.endTransmission();
    delay(300);
}

void loop()
{
    unsigned int data[4];
    for(int i = 0; i < 4; i++)
    { 
        // Start I2C Transmission
        Wire.beginTransmission(Addr);
        // Select data register
        Wire.write((133+i));
        // Stop I2C Transmission
        Wire.endTransmission();
        
        // Request 4 byte of data from the device
        Wire.requestFrom(Addr, 1);
        
        // Read 4 bytes of data
        // luminance msb, luminance lsb, proximity msb, proximity lsb
        if(Wire.available() == 1)
        {
            data[i] = Wire.read();
        }
        delay(300);
    }
    
    // Convert the data
    float luminance = ((data[0] * 256) + data[1]);
    float proximity = ((data[2] * 256) + data[3]);
    
    // Output data to dashboard
    Particle.publish("Ambient Light luminance :", String(luminance));
    Particle.publish("Proximity of the device :", String(proximity));
    delay(1000);
}

