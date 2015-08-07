/*
Program: Bronkhorst
Knihovna: BH_Com.h
Vlastn�k: Luk� Nov�k
*/



#ifndef BH_Com_H_
#define BH_Com_H_


void BH_Initreset();



void BH_Sensortype();

char Node_Address(unsigned int Vstup);

char Command_Status(unsigned int Vstup);

void BH_Measure(void);
/* Funkce pro zm��en� p�esn�ho tlaku */

void BH_Setpoint(unsigned char prvni, unsigned char druhy);
/* Funkce pro nastaven� Setpointu */

void BH_SetFullOpen(void);
/* Funkce pro kompletn� otev�en� ventilu */

void BH_SetFullClose(void);
/* Funkce pro kompletn� zav�en� ventilu */

void BH_SetBUS_RS232(void);
/* FUnkce pro nastavovani BUS/RS232 setpointu */

void BH_Setpoint_Ask(void);
/* Funkce pro vypis nastaveni setpointu*/

void BH_ControlMode_Ask(void);
/* FUnkce pro ziskani controlniho modu */

char Rozdeleni_ASCII_FIRST(unsigned char Promena);

char Rozdeleni_ASCII_SECOND(unsigned char Promena);

/*
int Node_Address_Out(char Vstup[2]);

int Command_Status_Out(char Vstup[2]);

int Value_Out(char Vstup[3]);

void Status_Message(char Vstup);
*/
#endif /* BH_Com_H_ */