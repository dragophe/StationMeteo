
// Librairie des fonctions permettant l'initialisation des CAN/CNA (KEIL)
// IUT de Cachan - 03/12/2014 

#include "LPC17xx.h"

// Fonction d'initialisation CNA
//------------------------------
void ADC_Init (void) /* pour entrée GBF P0.24 */
{

	LPC_SC->PCONP |= (1<<12 ); 			// reveil peripherique CAN
	
	LPC_PINCON->PINSEL1 |= (1<<16); 	//P0.24 (AD0.1) en mode ADC
	LPC_PINCON->PINSEL1 &= ~(1<<17); 	//P0.24 (AD0.1) en mode ADC
		
	LPC_PINCON->PINMODE1 |= (1<<17);	/* pas de pull up ni de pull down */
	LPC_PINCON->PINMODE1 &= ~(1<<18);
	
	LPC_ADC->ADCR |= 	( 1 <<  1) |    /* select AD0.1 pin   */
										( 1 <<  8) |       	/* ADC clock is 25MHz/    */
										( 1 << 21);        	/* enable ADC     */

}

// Fonction d'initialisation CAN
//------------------------------
void DAC_Init(void)
{

LPC_PINCON->PINSEL1 &=  ~( 3 << 20); 	/* pin P0-26 = DAC*/
LPC_PINCON->PINSEL1 |=  ( 2 << 20);
LPC_PINCON->PINMODE1 &=  ~( 3 << 20);
LPC_PINCON->PINMODE1 |=  ( 2 << 20);  	/* pas de résistance */
}

// Fonction de conversion CAN
// la valeur est renvoyée sur 12bits dans un entier,
// avec compensation de l'offset (analogique) du signal d'entrée
//--------------------------------------------------------------
int Conversion_AD()      /* lance la conversion et lit la valeur convertie */
{
	int entree, DONE;
	
	LPC_ADC->ADCR &= ~( 7 << 24);              /* stop conversion               */
	LPC_ADC->ADCR |=  ( 1 << 24);              /* start conversion              */
	
	do{
		DONE = LPC_ADC->ADGDR & 0x80000000; /* conversion prête ? */
	}
	while(DONE != 0x80000000);
	
	LPC_ADC->ADGDR &= 0x7FFFFFFF; 					/* CLEAR DONE bit N°31*/
	entree = (LPC_ADC->ADGDR >> 4)& 0x00000FFF;  	/* 12 bits décalage car le nombre converti est au milieu du mot*/
	entree = entree - 2048;
	return entree;
}

// Fonction de conversion CNA
// la valeur a envoyer est sur 12bits avec un entier
// elle est transformée en 10bits par décalage pour le CNA
// on ajoute l'offset numérique car CNA unipolaire
//------------------------------------------------
void Conversion_DA(int sortie)
{
sortie = (sortie + 2048)/4 ;   	/* 10 bits, division par 4 car ADC 12b et DAC 10b */
LPC_DAC->DACR = (sortie << 6);  /* decalage car le nombre à envoyer est au milieu du registre */
}

