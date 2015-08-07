/*
Program: Bronkhorst
Knihovna: BH_Com.h
Vlastník: Lukáš Novák
*/



#ifndef BH_Com_H_
#define BH_Com_H_


void BH_Initreset();

void BH_Measure(void);

void BH_Setpoint(unsigned char prvni, unsigned char druhy);

void BH_Sensortype();

char Node_Address(unsigned int Vstup);

char Command_Status(unsigned int Vstup);

void BH_SetFullOpen(void);

void BH_SetFullClose(void);

/*
int Node_Address_Out(char Vstup[2]);

int Command_Status_Out(char Vstup[2]);

int Value_Out(char Vstup[3]);

void Status_Message(char Vstup);
*/
#endif /* BH_Com_H_ */