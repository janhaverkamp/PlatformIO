#include <Arduino.h>
//using namespace std;

#define DATA    4
#define CLOCK   2
#define LATCH   3
#define REGISTERS   3
#define BITS        0,0,0,0,0,0,0,0,\
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
    int p(0);
    bool bits[REGISTERS*8];

    for(int ref; ref < value; ref = pow(2,(p+2)), p++);
    Serial.println(p);
    return bits;
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
    registerWrite(10);
}

