// TI File $Revision: /main/2 $
// Checkin $Date: February 24, 2009   15:54:34 $
//###########################################################################
//
// FILE:   DSP2803x_Spi.c
//
// TITLE:  DSP2803x SPI Initialization & Support Functions.
//
//###########################################################################
// $TI Release: 2803x C/C++ Header Files V1.21 $
// $Release Date: December 1, 2009 $
//###########################################################################

#include "DSP2803x_Device.h"     // DSP2803x Headerfile Include File
#include "DSP2803x_Examples.h"   // DSP2803x Examples Include File
#include "imu_main.h"

void InitSpiARegs(void);
void InitSpiBRegs(void);

//creates a packet for sending to the fcu
void make_fcu_packet(volatile struct FCU_PACKET * fcu_pkt)
{
	int i = 0;
	//fill the fcu_pkt->data array.
	switch(fcu_pkt->type){
		case RAW_SENSOR_DATA:
			for(i=0;i<8;i++){
				fcu_pkt->data[i+1] = sensors.sensor[i];	
			}
			break;
		case EULER_ANGLES:
			break;
		case STATUS:
			break;
		default:
			break;	
	}
	fcu_pkt->data[0] = (fcu_pkt->data[0] & 0xFF00) | parity_byte(fcu_pkt->data + 1, fcu_pkt->length - 1); //dont calculate on fcu_pkt->data[0].
}

void init_fcu_packet(volatile struct FCU_PACKET * fcu_pkt, enum PACKET_TYPE type)
{
	//allocate memory for new packet type
	switch(type){
		case RAW_SENSOR_DATA:
			fcu_pkt->length = sizeof(union SENSOR_DATA) + 1; //add 1 for the start byte.
			fcu_pkt->data = (Uint16 *)malloc(fcu_pkt->length); 
			fcu_pkt->data[0] = type << 8; //start byte is upper 8bits crc is lower 8
			break;
		case EULER_ANGLES:
			break;	
		case STATUS:
			break;
		default:
			break;	
	}
	fcu_pkt->data[0] = 0xAA00;
	fcu_pkt->type = type;	
	make_fcu_packet(fcu_pkt);
	fcu_pkt->data[0] ^= 0x000F; //mess up parity_byte, so fcu knows data isn't valid yet.
}



//---------------------------------------------------------------------------
// InitSPI:
//---------------------------------------------------------------------------
// This function initializes the SPI(s) to a known state.
//
void InitSpi(void)
{
	InitSpiGpio(); //set gpio for spia and spib
	InitSpiARegs();
	InitSpiBRegs();
}

void InitSpiARegs(void)
{
	
	//setup SPI_A  (IMU_SENSE SPI, master)
	SpiaRegs.SPICCR.bit.SPISWRESET = 0; //changing configuration
	SpiaRegs.SPICCR.bit.CLKPOLARITY = 1; //sclk high when idle, input read on rising
	SpiaRegs.SPICCR.all |= SPIA_CHAR_LNGTH_MSK; //set data length
	
	SpiaRegs.SPICTL.bit.MASTER_SLAVE = 1; //master
	SpiaRegs.SPICTL.bit.TALK = 0; //the ADC is read only, so i dont need to talk
	SpiaRegs.SPICTL.bit.SPIINTENA = 1; //enbable interrupts
	
	SpiaRegs.SPIBRR = 0x10; //BRR of 16 is ~1.765MHz.
	
	SpiaRegs.SPIPRI.bit.SOFT = 1;
	SpiaRegs.SPIPRI.bit.FREE = 0;
	
	
	SpiaRegs.SPIFFTX.bit.SPIFFENA = 1; //use FIFO for this spi
	//enable RX FIFO interrupt when 4 words are in FIFO.
	SpiaRegs.SPIFFRX.bit.RXFFIENA = 1;
	SpiaRegs.SPIFFRX.bit.RXFFIL = 4;
	
	SpiaRegs.SPIFFTX.bit.SPIRST = 0; //reset FIFO RX and TX
	SpiaRegs.SPIFFTX.bit.SPIRST = 1;
	
	//reset spi 
	SpiaRegs.SPICCR.bit.SPISWRESET = 1; //everything is configured, begin
}
void InitSpiBRegs(void)
{
	//setup SPI_B  (FCU SPI, slave)
	SpibRegs.SPICCR.bit.SPISWRESET = 0;//changing configuration...
	SpibRegs.SPICCR.bit.CLKPOLARITY = 0; //sclk low when idle.
	SpibRegs.SPICCR.all |= SPIB_CHAR_LNGTH_MSK; //set data length
	
	SpibRegs.SPICTL.bit.MASTER_SLAVE = 0; //slave
	SpibRegs.SPICTL.bit.TALK = 1; //enable transmit to FCU
	SpibRegs.SPICTL.bit.SPIINTENA = 1; //enable interrupts
	
	//allow some control when running with emulator, will stop transmissions when halted
	SpibRegs.SPIPRI.bit.SOFT = 1;
	SpibRegs.SPIPRI.bit.FREE = 0;
	
	//reset spi 
	SpibRegs.SPICCR.bit.SPISWRESET = 1; //everything is configured, begin
}
//---------------------------------------------------------------------------
// Example: InitSpiGpio:
//---------------------------------------------------------------------------
// This function initializes GPIO pins to function as SPI pins
//
// Each GPIO pin can be configured as a GPIO pin or up to 3 different
// peripheral functional pins. By default all pins come up as GPIO
// inputs after reset.
//
// Caution:
// For each SPI peripheral
// Only one GPIO pin should be enabled for SPISOMO operation.
// Only one GPIO pin should be enabled for SPISOMI operation.
// Only one GPIO pin should be enabled for SPICLK  operation.
// Only one GPIO pin should be enabled for SPISTE  operation.
// Comment out other unwanted lines.

void InitSpiGpio()
{
   InitSpiaGpio();
   InitSpibGpio();
}

void InitSpiaGpio()
{

   EALLOW;

/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled by the user.
// This will enable the pullups for the specified pins.
    GpioCtrlRegs.GPAPUD.bit.GPIO16 = 0;   // Enable pull-up on GPIO16 (SPISIMOA)
    GpioCtrlRegs.GPAPUD.bit.GPIO17 = 0;   // Enable pull-up on GPIO17 (SPISOMIA)
    GpioCtrlRegs.GPAPUD.bit.GPIO18 = 0;   // Enable pull-up on GPIO18 (SPICLKA)
    GpioCtrlRegs.GPAPUD.bit.GPIO19 = 0;   // Enable pull-up on GPIO19 (SPISTEA)

/* Set qualification for selected pins to asynch only */
// This will select asynch (no qualification) for the selected pins.
    GpioCtrlRegs.GPAQSEL2.bit.GPIO16 = 3; // Asynch input GPIO16 (SPISIMOA)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO17 = 3; // Asynch input GPIO17 (SPISOMIA)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO18 = 3; // Asynch input GPIO18 (SPICLKA)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO19 = 3; // Asynch input GPIO19 (SPISTEA)

/* Configure SPI-A pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be SPI functional pins.
    GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 1; // Configure GPIO16 as SPISIMOA
    GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 1; // Configure GPIO17 as SPISOMIA
    GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 1; // Configure GPIO18 as SPICLKA
    GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 1; // Configure GPIO19 as SPISTEA

    EDIS;
}

void InitSpibGpio()
{
   EALLOW;

/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled disabled by the user.
// This will enable the pullups for the specified pins.
    GpioCtrlRegs.GPAPUD.bit.GPIO13 = 0;     // Enable pull-up on GPIO13 (SPISOMIB)
    GpioCtrlRegs.GPAPUD.bit.GPIO14 = 0;     // Enable pull-up on GPIO14 (SPICLKB)
    GpioCtrlRegs.GPAPUD.bit.GPIO15 = 0;     // Enable pull-up on GPIO15 (SPISTEB)
	GpioCtrlRegs.GPAPUD.bit.GPIO24 = 0;     // Enable pull-up on GPIO24 (SPISIMOB)

/* Set qualification for selected pins to asynch only */
// This will select asynch (no qualification) for the selected pins.
    GpioCtrlRegs.GPAQSEL1.bit.GPIO13 = 3;   // Asynch input GPIO13 (SPISOMIB)
    GpioCtrlRegs.GPAQSEL1.bit.GPIO14 = 3;   // Asynch input GPIO14 (SPICLKB)
    GpioCtrlRegs.GPAQSEL1.bit.GPIO15 = 3;   // Asynch input GPIO15 (SPISTEB)
	GpioCtrlRegs.GPAQSEL2.bit.GPIO24 = 3;   // Asynch input GPIO24 (SPISIMOB)
	
/* Configure SPI-B pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be SPI functional pins.
    GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 3;    // Configure GPIO13 as SPISOMIB
    GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 3;    // Configure GPIO14 as SPICLKB
    GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 3;    // Configure GPIO15 as SPISTEB
	GpioCtrlRegs.GPAMUX2.bit.GPIO24 = 3;    // Configure GPIO24 as SPISIMOB
    
    EDIS;
}



//===========================================================================
// End of file.
//===========================================================================
