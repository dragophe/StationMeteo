#include <stdio.h>
#include "Driver_I2C.h"                 // ::CMSIS Driver:I2C
#include "Board_GLCD.h"                 // ::Board Support:Graphic LCD
#include "GLCD_Config.h"                // Keil.MCB1700::Board Support:Graphic LCD



#define SLAVE_I2C_ADDR       0x1D			// Adresse esclave sur 7 bits

extern GLCD_FONT GLCD_Font_16x24;

extern ARM_DRIVER_I2C Driver_I2C0;

uint8_t DeviceAddr;

void Init_I2C(void){
	Driver_I2C0.Initialize(NULL);
	Driver_I2C0.PowerControl(ARM_POWER_FULL);
	Driver_I2C0.Control(	ARM_I2C_BUS_SPEED,				// 2nd argument = débit
							ARM_I2C_BUS_SPEED_STANDARD  );	// 100 kHz
	Driver_I2C0.Control(	ARM_I2C_BUS_CLEAR,
							0 );
}

void write1byte(unsigned char composant, unsigned char registre, unsigned char valeur)
{

	uint8_t tab[10];
	
	tab[0] = registre;
	tab[1] = valeur;
	
	Driver_I2C0.MasterTransmit (composant, tab, 1, false);		// false = avec stop
	while (Driver_I2C0.GetStatus().busy == 1);	// attente fin transmission

}

unsigned char read1byte(unsigned char composant, unsigned char registre)
{

	uint8_t tab[10], maValeur;
	
	tab[0] = registre;
	
	Driver_I2C0.MasterTransmit (composant, tab, 1, true);		// false = avec stop
	while (Driver_I2C0.GetStatus().busy == 1);	// attente fin transmission
	
	Driver_I2C0.MasterReceive (composant, &maValeur, 1, false);		// false = avec stop
	while (Driver_I2C0.GetStatus().busy == 1);	// attente fin transmission
	
	return maValeur;
	
}

int main (void){
	
	uint8_t X1, X2,i=0;
	uint8_t Y1, Y2;
	char maChaine[10];
	
	Init_I2C();
	GLCD_Initialize();
	
	// Adresse module esclave sur 7 bits
	DeviceAddr = SLAVE_I2C_ADDR;
	
	GLCD_ClearScreen();
	GLCD_SetFont(&GLCD_Font_16x24);
	
	write1byte(DeviceAddr, 0x24, 0x70);	//configuration I2C
	write1byte(DeviceAddr, 0x25, 0x20);	//configuration magnétomètre
			write1byte(DeviceAddr, 0x26, 0x00);	//début de la lecture
	
	while (1)
	{
		i++;
		

		
		X1 = read1byte(DeviceAddr, 0x08);
		X2 = read1byte(DeviceAddr, 0x09);
		
		Y1 = read1byte(DeviceAddr, 0x0a);
		Y2 = read1byte(DeviceAddr, 0x0b);
		
		sprintf(maChaine,"mag X = %06d", (short)(X2 << 8 | X1));
		GLCD_DrawString(1, 1, (char*)maChaine);
		
		sprintf(maChaine,"mag Y = %06d", (short)(Y2 << 8 | Y1));
		GLCD_DrawString(1, 25, (char*)maChaine);
		
		sprintf(maChaine,"tour = %06d", i);
		GLCD_DrawString(1, 50, (char*)maChaine);
	}
	
	return 0;
}
