#include <Arduino.h>

#define DATA    4
#define CLOCK   2
#define LATCH   3
#define REGISTERS   3
//#define BITS        0,0,0,0,0,0,0,0,\
                    0,0,0,0,0,0,0,0,\
                    0,0,0,0,0,1,0,1

/*void registerWrite(bool arr[])
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

void registerWrite(byte value)
{
    int p(REGISTERS*8);
    //bool bits[REGISTERS*8];

    digitalWrite(LATCH,0);

    for(; p >= 0; p--)
    {
        digitalWrite(CLOCK,0);
        if(value >= pow(2,p))
        {
            value = value - pow(2,p);
            digitalWrite(DATA,1); //bits[p] = 1;
        }
        else digitalWrite(DATA,0); //bits[p] = 0;

        digitalWrite(CLOCK,1);
        digitalWrite(CLOCK,0);
    }
    digitalWrite(LATCH,1);

}

void setup()
{
    Serial.begin(9600);

    pinMode(DATA,OUTPUT);
    pinMode(CLOCK,OUTPUT);
    pinMode(LATCH,OUTPUT);


}



void loop()
{
    registerWrite(0x38);
    delay(1000);
    registerWrite(0x06);
    delay(1000);
    registerWrite(0x6D);
    delay(1000);
    registerWrite(0x77);
    delay(1000);
}

