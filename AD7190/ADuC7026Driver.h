/********************************************************************************
 Author : CAC (China Applications Support Team) 

 Date :   January, 2012

 File name :   ADuC7026Driver.h

 Description :	 Use the GPIO to simulate the SPI communication of AD7192

 Hardware plateform : 	ADuC7026 and AD7190/92EBZ
********************************************************************************/

#ifndef ADUC7026_DRIVER_H
#define ADUC7026_DRIVER_H

// add the header file here
//GPIO Define

#define CS			0x40	// pin CS = P4.0
#define SYNC		0x42	// pin SYNC  = P4.2
#define SCLK		0x44	// pin SCLK  = P4.4
#define	SDI			0x45	// pin SDI   = P4.5
#define	SDO			0x05	// pin SDO   = P0.5

	
int sendchar (int ch);   
unsigned char ADuC7026InputBit(unsigned char GPIONum);
void ADuC7026OutputBit(unsigned char GPIONum, unsigned char Data);	
void ADuC7026Initialization(void);
void ADuC7026SpiOperation(unsigned char* WriteBuffer, unsigned char *ReadBuffer, unsigned char NumberOfByte);
void ADuC7026DelayMs( unsigned int TimeMs);
void ADuC7026DelayUs( unsigned int TimeUs);

#endif





