#include <Arduino.h>
using namespace std;

#define DATA    4
#define CLOCK   2
#define LATCH   3
#define REGISTERS   3
#define BITS        0,0,0,0,0,0,0,0, \
                    0,0,0,0,0,0,0,0, \
                    0,0,1,0,0,0,0,0

void registerWrite(bool bits[])
{
    digitalWrite(LATCH,0);
    for(int i(REGISTERS * 8); i>=0; i--)
    {
        digitalWrite(CLOCK,0);
        digitalWrite(DATA,bits[i]);
        digitalWrite(CLOCK,1);
    }
    digitalWrite(LATCH,1);
}

void setup()
{
    pinMode(DATA,OUTPUT);
    pinMode(CLOCK,OUTPUT);
    pinMode(LATCH,OUTPUT);
}

void loop()
{
    bool arr[] = {BITS};
    registerWrite(arr);
}

