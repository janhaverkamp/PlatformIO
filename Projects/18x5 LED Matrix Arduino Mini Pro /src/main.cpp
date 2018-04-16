#include <Arduino.h>
#include "ledmatrix18x5.h"
//#include <pins_Arduino.h>



void setup()
{
    Serial.begin(9600);
    initShifts();
    initDpad();
    matrixClear();
}

void menu()
{
    
}

void loop()
{
    squash();
}

