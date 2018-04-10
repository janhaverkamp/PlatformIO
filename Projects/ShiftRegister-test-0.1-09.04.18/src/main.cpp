#include <Arduino.h>

//SHIFT REGISTER
#define DATA    4
#define CLOCK   2
#define LATCH   3
#define REGISTERS   3

//EEPROM 
#define IO1  5
#define IO8 12
#define WE  A0
#define OE  A1
#define EEPROMSIZE 8


/*void registerWrite(bool arr[]) // OLD registerWrite with array input of booleans
{
    digitalWrite(LATCH,0);
    for(int k(REGISTERS * 8); k>=0; k--)
    {
        digitalWrite(CLOCK,0);
        digitalWrite(DATA,arr[k]);
        digitalWrite(CLOCK,1);
    }
    digitalWrite(LATCH,1);
}
*/

void registerWrite(int address)
{
    int p(REGISTERS*8);         //highest address

    digitalWrite(LATCH,0);

    for(; p >= 0; p--)
    {
        digitalWrite(CLOCK,0);
        if(address >= pow(2,p))
        {
            address = address - pow(2,p);
            digitalWrite(DATA,1);   //bits[p] = 1;
        }
        else digitalWrite(DATA,0);  //bits[p] = 0;

        digitalWrite(CLOCK,1);
        digitalWrite(CLOCK,0);
    }
    digitalWrite(LATCH,1);

}

void eepromPrint()
{
    digitalWrite(WE,0x01);
    digitalWrite(OE,0x00);

    /*int readarray[EEPROMSIZE];
    for(int i(0); i <= EEPROMSIZE; i++) readarray[i] = 0;

    for(int i(0); i<= EEPROMSIZE; i++)
    {
        registerWrite(i);

        for(int j = IO1, p(0); j <= IO8; j++, p++)         //loop for readout of each pin
            if(digitalRead(j) == 1) readarray[i] += (pow(2,p));
        
    }

    for(int i(0); i < EEPROMSIZE; i++)
    {
        Serial.print(readarray[i], HEX);
        Serial.print(" ");
    }
    */

    for(int i(0); i<= EEPROMSIZE; i++)
    {
        int byte(0);

        registerWrite(i);

        for(int j = IO1, p(0); j <= IO8; j++, p++)
        {
            if(digitalRead(j) == 1) byte = byte + pow(2,p);
            else byte += 0;

            Serial.print(byte);
        }
        //Serial.print(byte,HEX);
        //Serial.print(" ");
    }

}

void setup()
{
    Serial.begin(9600);

    pinMode(DATA,OUTPUT);
    pinMode(CLOCK,OUTPUT);
    pinMode(LATCH,OUTPUT);

    pinMode(WE,OUTPUT);
    pinMode(OE,OUTPUT);

    for(int pin = IO1; pin <= IO8; pin++) pinMode(pin,INPUT);       //loop to set datapins

    eepromPrint();
    //registerWrite(0);

}



void loop()
{
}
