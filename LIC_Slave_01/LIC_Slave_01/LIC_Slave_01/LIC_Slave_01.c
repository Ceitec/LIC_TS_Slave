/*
 * BronkHorst1_01.c
 *
 * Created: 2.2.2015 17:27:46
 *  Author: Lukas
 */ 
/*
_______
|.|.| - Slave 2
|.|.| - Slave 1
|.|.| - Master
*/
#include <avr/io.h>
#include "inc/AllInit.h"	
#include "inc/RS232.h"
#include "util/delay.h"
#include <avr/interrupt.h>
#include "inc/BH_Com.h"
#include "inc/Buffer.h"
#include <stdlib.h>
#include <util/twi.h>


#define I2C_Adress_Slave 0x02

//prototypes
void handleI2C();
void Null_I2C_Buffer_Rec();
void Null_I2C_Buffer_Tra();
void I2C_Init(void);
void Naplneni_I2C_ENHANCED(void);
unsigned char Prevod_ASCII(unsigned char Vstup);
ISR(USART_RXC_vect);
ISR(TWI_vect);


//Deklarace bufferu pro odesílání
#define TX_BUFFER_MAX_LENGTH    16                    // TxBuffer max length 
char TxBuffer[TX_BUFFER_MAX_LENGTH];
unsigned char TxBuffrPtr;
unsigned char TxBuffrRadek;

//Sequenèní counter
unsigned char Seq_Counter=0x00;

//Deklarace bufferu pro pøíjem
#define RX_BUFFER_MAX_LENGTH   16                    // RxBuffer max length 
char RxBuffer[RX_BUFFER_MAX_LENGTH] = {0x00, 0x00, 0x00};
char promena[RX_BUFFER_MAX_LENGTH];
volatile unsigned char RxBuffrRxPtr=0;
volatile unsigned char Rxj;
char RxBuffrOK;
char xxx;
unsigned char Odpocet_Delky=0x00, Delka_Zpravy=0x00;



#define SET(x,y) (x|=(1<<y))
#define CLR(x,y) (x&=(~(1<<y)))
#define CHK(x,y) (x&(1<<y))
#define TOG(x,y) (x^=(1<<y))

//I2C global variables
#define BUFLEN_RECV 16
uint8_t r_index =0;
uint8_t I2C_RecBuffer[BUFLEN_RECV]; //buffer to store received bytes

#define BUFLEN_TRAN 16
uint8_t t_index=0;
//test bytes to transmit
uint8_t I2C_TranBuffer[BUFLEN_TRAN] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

//variable to indicate if something went horribly wrong
uint8_t reset=0;

int i=0;
int rezim = 0;

//Preruseni prijmu sériove linky a ukládání do bufferu
ISR(USART_RXC_vect)
{
	//Ceka a ulozi do pameti prijata data
	while (!(UCSRA & (1 << RXC)));
	RxBuffer[RxBuffrRxPtr] = UDR;
	RxBuffrRxPtr++;
}

ISR(TWI_vect)
{
	//PORTB ^= (1 << PB1);
	handleI2C();
	
	//Vystup z preruseni
	TWCR |= (1 << TWIE);
}


int main(void)
{
	//Inicializace sériové linky
	RS232_Init(RS232_38400);
	
	I2C_Init();
	
	//Nastavení portu C jako výstupního
	//DDRB = 0xFF;
    DDRC |= (1 << DDC0);
	//Povolení pøerušení
	sei();
	
	while(1)
    {
		_delay_ms(1);
		
		if(I2C_RecBuffer[15] == 0xFF)
		{
			RxBuffer[15]=0x00;
			switch(I2C_RecBuffer[0])
			{
				case 0x01 :
					//Rezim zajistuje jakým protokolem se komunikuje s ventily
					rezim = 0;
					Null_Buffer();
					Null_I2C_Buffer_Tra();
					BH_Setpoint(I2C_RecBuffer[1], I2C_RecBuffer[2]);
					_delay_ms(5);
					I2C_TranBuffer[0] = 0x01;
					I2C_TranBuffer[1] = RxBuffer[6];
					I2C_TranBuffer[2] = RxBuffer[7];
					I2C_TranBuffer[15] = 0xFF;
					Null_I2C_Buffer_Rec();
					_delay_ms(50);
				break;
				case 0x02 :
					//Rezim zajistuje jakým protokolem se komunikuje s ventily
					rezim = 0;
					Null_Buffer();
					Null_I2C_Buffer_Tra();
					BH_Measure();
					_delay_ms(5);
					I2C_TranBuffer[0] = 0x02;
					I2C_TranBuffer[1] = RxBuffer[8];
					I2C_TranBuffer[2] = RxBuffer[9];
					I2C_TranBuffer[15] = 0xFF;
					Null_I2C_Buffer_Rec();
					_delay_ms(50);
					
				break;
				case 0x03 :
					//Rezim zajistuje jakým protokolem se komunikuje s ventily
					rezim = 0;
					Null_Buffer();
					Null_I2C_Buffer_Tra();
					BH_SetFullOpen();
					_delay_ms(5);
					I2C_TranBuffer[0] = 0x03;
					I2C_TranBuffer[1] = RxBuffer[6];
					I2C_TranBuffer[2] = RxBuffer[7];
					I2C_TranBuffer[15] = 0xFF;
					Null_I2C_Buffer_Rec();
					_delay_ms(50);
				break;
				case 0x04 :
					//Rezim zajistuje jakým protokolem se komunikuje s ventily
					rezim = 0;
					Null_Buffer();
					Null_I2C_Buffer_Tra();
					BH_SetFullClose();
					
					_delay_ms(5);
					I2C_TranBuffer[0] = 0x04;
					I2C_TranBuffer[1] = RxBuffer[6];
					I2C_TranBuffer[2] = RxBuffer[7];
					I2C_TranBuffer[15] = 0xFF;
					Null_I2C_Buffer_Rec();
					_delay_ms(50);
				break;
				case 0x05 :
					//Rezim zajistuje pro nastavení setpointu
					rezim = 0;
					Null_Buffer();
					Null_I2C_Buffer_Tra();
					BH_SetBUS_RS232();
					
					_delay_ms(5);
					I2C_TranBuffer[0] = 0x05;
					I2C_TranBuffer[1] = RxBuffer[6];
					I2C_TranBuffer[2] = RxBuffer[7];
					I2C_TranBuffer[15] = 0xFF;
					Null_I2C_Buffer_Rec();
					_delay_ms(50);
				break;
				case 0x06 :
					//Rezim Zajistuje request setpoint
					rezim = 0;
					Null_Buffer();
					Null_I2C_Buffer_Tra();
					BH_Setpoint_Ask();
					
					_delay_ms(5);
					I2C_TranBuffer[0] = 0x06;
					I2C_TranBuffer[1] = RxBuffer[7];
					I2C_TranBuffer[2] = RxBuffer[8];
					I2C_TranBuffer[15] = 0xFF;
					Null_I2C_Buffer_Rec();
					_delay_ms(50);
				break;
				
				case 0x07 :
					//Rezim Zajistuje request setpoint
					rezim = 0;
					Null_Buffer();
					Null_I2C_Buffer_Tra();
					BH_Setpoint_Ask();
					
					_delay_ms(5);
					I2C_TranBuffer[0] = 0x06;
					I2C_TranBuffer[1] = RxBuffer[7];
					I2C_TranBuffer[2] = RxBuffer[8];
					I2C_TranBuffer[15] = 0xFF;
					Null_I2C_Buffer_Rec();
					_delay_ms(50);
				break;
				
				default:
					
					I2C_TranBuffer[0] = 0xFF;
					I2C_TranBuffer[1] = 0xFF;
					I2C_TranBuffer[2] = 0xFF;
					I2C_TranBuffer[15] = 0xFF;
				
				break;
			}
		}
    }
}

void Null_I2C_Buffer_Rec()
{
	
	int i=0;
	for (i=0; i<16; i++)
	{
		I2C_RecBuffer[i]=0x00;
	}
	
	I2C_RecBuffer[15]=0x00;
}
void Null_I2C_Buffer_Tra()
{
	
	int i=0;
	for (i=0; i<16; i++)
	{
		I2C_TranBuffer[i]=0x00;
	}
	
	I2C_TranBuffer[15]=0x00;
}
void Naplneni_I2C_ASCII(void)
{
	int j=0;
	for (i=0; i<16; i++)
	{
		if (i == 0)
		{
			I2C_TranBuffer[i] = RxBuffer[i];
		} 
		else
		{
			I2C_TranBuffer[i] = ((RxBuffer[i + j] - 0x30) << 4) + (RxBuffer[i + j + 1] - 0x30);
			j++;
		}
	}
	I2C_TranBuffer[15] = 0xFF;
}

void Naplneni_I2C_ENHANCED(void)
{
	I2C_TranBuffer[1] = RxBuffer[8];
	I2C_TranBuffer[2] = RxBuffer[9];
	I2C_TranBuffer[15] = RxBuffer[15];
}

unsigned char Prevod_ASCII(unsigned char Vstup)
{
	unsigned char Vystup=0x00;
	if(Vstup > 0x3A)
	{
		Vystup = (Vstup - 0x37);
	}
	else
	{
		Vystup = (Vstup - 0x30);
	}
	return Vystup;
}

void HB_LED_RXC()
{
	PORTB ^= (1 << PB0);
}

void Text_to_Buffer(char Buffer)
{
	TxBuffer[TxBuffrPtr] = Buffer;
	TxBuffrPtr++;
}

void Null_Buffer()
{
	TxBuffrPtr = 0;
	RxBuffrRxPtr = 0;
}

void I2C_Init(void)
{
	//load slave address
	TWAR = (I2C_Adress_Slave<<1); //we're using address 0x01
		
	//enable I2C hardware & enable interrupt
	TWCR = (1<<TWEN)|(1<<TWEA) | (1 << SREG) | (1 << TWIE);
}


/*
TWI registr pro urceni rychlosti
TWINT - FLag pro povoleni prerusei
TWEN - Povoli I2C
TWIE - Povoli preruseni

TWS -Status registr
TWPS - PReddelicka
TWDR - Data ktera maji byt poslana
TWAR - Adresa slave

Nejprve posle TWCR jako zacatek pro inicializacni start
Nastavei TWINT
Zkontroluje TWSR jestli zacatek byl poslan
Vlozi data do slave zapisu TWDR
a nacte kontrolni signal TWCR
ujistise ze TWINT je nastaven do 1 a TWSTA do 0

*/




//setup the I2C hardware to ACK the next transmission
//and indicate that we've handled the last one.
#define TWACK (TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWEA))
//setup the I2C hardware to NACK the next transmission
#define TWNACK (TWCR=(1<<TWINT)|(1<<TWEN))
//reset the I2C hardware (used when the bus is in a illegal state)
#define TWRESET (TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWSTO)|(1<<TWEA))

void handleI2C()
{
	//check if we need to do any software actions
	if(CHK(TWCR,TWINT)){
		switch(TW_STATUS){
			//--------------Slave receiver------------------------------------
			//SLA_W received and acked, prepare for data receiving
			case 0x60:
				TWACK;
				r_index =0;
			break;
			case 0x80:  //a byte was received, store it and
				//setup the buffer to recieve another
				I2C_RecBuffer[r_index] = TWDR;
				r_index++;
			//don't ack next data if buffer is full
				if(r_index >= BUFLEN_RECV)
				{
					TWNACK;
				}
				else TWACK;
			
			break;
			case 0x68://adressed as slave while in master mode.
			//should never happen, better reset;
				reset=1;
				case 0xA0: //Stop or rep start, reset state machine
					TWACK;
				break;
			//-------------- error recovery ----------------------------------
			case 0x88: //data received  but not acked
			//should not happen if the master is behaving as expected
			//switch to not adressed mode
				TWACK;
			break;
			//---------------Slave Transmitter--------------------------------
			case 0xA8:  //SLA R received, prep for transmission
				//and load first data
				t_index=1;
				TWDR = I2C_TranBuffer[0];
				TWACK;
			break;
			case 0xB8:  //data transmitted and acked by master, load next
				TWDR = I2C_TranBuffer[t_index];
				t_index++;
				
				//designate last byte if we're at the end of the buffer
				if(t_index >= BUFLEN_TRAN)
				{
					TWNACK;
				}
				else
				{
					TWACK;
				}
				
			break;
			case 0xC8: //last byte send and acked by master
			//last bytes should not be acked, ignore till start/stop
			//reset=1;
			case 0xC0: //last byte send and nacked by master
			//(as should be)
				TWACK;
			break;
			//--------------------- bus error---------------------------------
			//illegal start or stop received, reset the I2C hardware
			case 0x00:
				TWRESET;
			break;
		}
	}
}

