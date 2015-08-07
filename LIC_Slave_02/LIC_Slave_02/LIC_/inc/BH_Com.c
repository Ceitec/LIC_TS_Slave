/*

Program: Bronkhorst Communication
Knihovna: BH_Com.c
Vlastník: Lukáš Novák

*/
#include "BH_Com.h"
#include "RS232.h"
#include <avr/interrupt.h>
#include "util/delay.h"

#define RozhodPtr 256

unsigned char Sequencni_Cislo=0x00;


void BH_Measure(void)
{
	static char Vystup[12];
	
	Vystup[0] = 0x10;						//DLE
	Vystup[1] = 0x02;						//STX
	Vystup[2] = ++Sequencni_Cislo;		//Sekvenèní èíslo
	Vystup[2] = 0x01;
	Vystup[3] = 0x03;						//Adresa
	Vystup[4] = 0x05;						//Delka
	//Zacatek dat
	Vystup[5] = 0x04;						//Command_Status
	Vystup[6] = 0x01;						//Process
	Vystup[7] = 0x21;						//FBn1
	Vystup[8] = 0x01;						//Nastaveni1
	Vystup[9] = 0x20;						//Nastaveni2
	//Konec dat
	Vystup[10] = 0x10;						//DLE
	Vystup[11] = 0x03;						//ETX
	
	RS232_Transmit_String_Buffer(Vystup, 12);
}

void BH_Setpoint(unsigned char prvni, unsigned char druhy)
{	
	//Bejvavalo toto void BH_Setpoint(unsigned int Procenta)
	//Vystup[8] = (char)(Procenta / 256);		//Nastaveni1
	//Vystup[9] = (char)(Procenta % 256);		//Nastaveni2
	
	static unsigned char Vystup[12];
	
	Vystup[0] = 0x10;						//DLE
	Vystup[1] = 0x02;						//STX
	Vystup[2] = ++Sequencni_Cislo;			//Sekvenèní èíslo	
	Vystup[2] = 0x01;
	Vystup[3] = 0x03;						//Adresa
	Vystup[4] = 0x05;						//Delka
	//Zacatek dat
	Vystup[5] = 0x02;						//Command_Status
	Vystup[6] = 0x01;						//Process
	Vystup[7] = 0x21;						//FBn1
	Vystup[8] = (char)(prvni);		//Nastaveni1
	Vystup[9] = (char)(druhy);		//Nastaveni2
	//Konec dat
	Vystup[10] = 0x10;						//DLE
	Vystup[11] = 0x03;						//ETX
	
	RS232_Transmit_String_Buffer(Vystup, 12);
}

void BH_SetFullOpen(void)
{
	static unsigned char Vystup[11];
	
	Vystup[0] = 0x10;						//DLE
	Vystup[1] = 0x02;						//STX
	Vystup[2] = ++Sequencni_Cislo;		//Sekvenèní èíslo
	Vystup[2] = 0x01;
	Vystup[3] = 0x03;						//Adresa
	Vystup[4] = 0x04;						//Delka
	//Zacatek dat
	//Control mode 04
	//Char 00
	Vystup[5] = 0x01;						//Command_Status - Zapis
	Vystup[6] = 0x01;						//Process
	Vystup[7] = 0x04;						//FBn1
	Vystup[8] = 0x07;
	//Konec dat
	Vystup[9] = 0x10;						//DLE
	Vystup[10] = 0x03;						//ETX
	
	RS232_Transmit_String_Buffer(Vystup, 11);
}

void BH_SetFullClose(void)
{
	static unsigned char Vystup[11];
	
	Vystup[0] = 0x10;						//DLE
	Vystup[1] = 0x02;						//STX
	Vystup[2] = ++Sequencni_Cislo;		//Sekvenèní èíslo
	Vystup[2] = 0x01;
	Vystup[3] = 0x03;						//Adresa
	Vystup[4] = 0x04;						//Delka
	//Zacatek dat
	//Control mode 04
	//Char 00
	Vystup[5] = 0x01;						//Command_Status - Zapis
	Vystup[6] = 0x01;						//Process
	Vystup[7] = 0x04;						//FBn1
	Vystup[8] = 0x0C;
	//Konec dat
	Vystup[9] = 0x10;						//DLE
	Vystup[10] = 0x03;						//ETX
	
	RS232_Transmit_String_Buffer(Vystup, 11);	
}

//Prvni inicializace dojde
/*
00 0D 0A 0D 0A 2A 2A 2A 2A 2A 20 42 48 54 20 4D 42 43 33 20 45 4C 20 53 
45 4C 45 43 54 20 56 31 2E 32 31 64 20 4D 61 79 20 38 20 32 30 31 33 20 
30 39 3A 32 34 3A 34 32 20 2A 2A 2A 2A 2A 0D 0A    
*/
