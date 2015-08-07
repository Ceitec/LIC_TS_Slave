/*

Program: Bronkhorst
Knihovna: RS232.c
Vlastn�k: Luk� Nov�k

*/

#include <util/delay.h>
#include <avr/io.h>
#include "RS232.h"


//////////////////////////////////////////////////////////////////////
/*		Inicializa�n� procedura obsahuj�c� nastaven� odes�l�n�,		*/
/*		p��jem dat a povolen� p�eru�en� p��jmu po s�riov� lince.	*/
/*			D�le nastaven� R/T pomoc� 8 bit� a 1 stop bit.			*/
//////////////////////////////////////////////////////////////////////


void RS232_Init( unsigned int baud )
{
	
	UBRRH = (unsigned char)(baud>>8);							//Registr pro nastaven� vrchn�ch byt� rychlosti s�riov� linky.
	UBRRL = (unsigned char)baud;								//Registr pro nastaven� spodn�ch byt� rychlosti s�riov� linky.
	UCSRB = (1<<RXEN) | (1<<TXEN) | (1<<RXCIE);					//Povolen� v registru odes�l�n�, p��jmu a povolen� p�eru�en� p��jmu
	//UCSRB = (1<<TXEN) | (1 << RXEN);											//Povolen� v registru odes�l�n�, p��jmu a povolen� p�eru�en� p��jmu
	UCSRC = (1<<URSEL) | (0<<USBS) | (1<<UCSZ0)| (1<<UCSZ1);	//Nastaven� R/T 8 bit� data, 1 stop bitu, asynchronn� p�enos a bez parity.
}

//////////////////////////////////////////////////////////////////////////
/*				Procedura pro p��jem jednoho znaku a ukl�d�n�			*/
/*					z registru UDR do vr�cen� prom�nn�.					*/
//////////////////////////////////////////////////////////////////////////

unsigned char RS232_Receive_Char( void )
{
  unsigned char data;
  while ( !(UCSRA & (1<<RXC)) );	//�ek�n� dokud nen� dokon�en p�enos dat
  data=UDR;							//�ten� dat z registru UDR a ukl�d�n� do prom�nn�
  return data;						//Vr�cen� p�ijmut�ho znaku
}

//////////////////////////////////////////////////////////////////////////
/*  		Procedura pro prijem vice znaku po s�riov� lince.			*/
//////////////////////////////////////////////////////////////////////////

void RS232_Receive_String( void )
{
	int RxBPtr=0;
	RxBPtr = 256;
	char RxB[RxBPtr];
	while (RS232_Receive_Char() != 0x0A)
	{
		RxB[RxBPtr] = RS232_Receive_Char();
		RS232_Transmit_Char(RxB[RxBPtr]);
		RxBPtr++;
		
	}
}

//////////////////////////////////////////////////////////////////////////
/*		Procedura pro odes�l�n� jednoho znaku po s�riov� lince.			*/
//////////////////////////////////////////////////////////////////////////

void RS232_Transmit_Char( unsigned char data )
{
  while ( !( UCSRA & (1<<UDRE)) );	//�ek�n� dokud nen� pr�zdn� buffer
  UDR = data;						//Nastaven� znaku do registru
}

//////////////////////////////////////////////////////////////////////////
/*			Roz���en� procedury pro odes�l�n� jednoho znaku,			*/
/*						 na odes�l�n� pole znak�.						*/
//////////////////////////////////////////////////////////////////////////

void RS232_Transmit_String(char* StringPtr){
	
	while(*StringPtr != 0x00)			//Opakov�n� cyklu dokud se nevypr�zdn� buffer
	{
		RS232_Transmit_Char(*StringPtr);		//Vyu�it� procedury pro odesl�n� jednoho znaku
		StringPtr++;					//Posunut� o dal�� znak
	}
	
}

//////////////////////////////////////////////////////////////////////////
/*				 Upraven� procedura odes�l�n� pole znak�				*/
/*			s p�idan�m ukon�ovac�m znakem <CR> pro AT p��kazy.			*/
//////////////////////////////////////////////////////////////////////////

void RS232_Transmit_String_CR(char* StringPtr){
	
	while(*StringPtr != 0x00)			//Opakov�n� cyklu dokud se nevypr�zdn� buffer
	{
		RS232_Transmit_Char(*StringPtr);		//Vyu�it� procedury pro odes�l�n� jednoho znaku
		StringPtr++;					//Posunut� o dal�� znak
	}
	RS232_Transmit_Char(0x0D);				//Na konci odesl�n znak <CR>
}


void RS232_Transmit_String_Buffer(char* StringPtr, int TXStringPtr)
{
	
//	while(*StringPtr != 0x0A)			//Opakov�n� cyklu dokud se nevypr�zdn� buffer
	while(TXStringPtr != 0)
	{
		RS232_Transmit_Char(*StringPtr);		//Vyu�it� procedury pro odes�l�n� jednoho znaku
		StringPtr++;					//Posunut� o dal�� znak
		TXStringPtr--;
	}
}

