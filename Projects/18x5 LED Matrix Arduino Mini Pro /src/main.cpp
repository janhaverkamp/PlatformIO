//LEDMATRIX 18x5 MAIN.CPP 
//JAN HAVERKAMP

//Header files
#include <Arduino.h>
#include "ledmatrix18x5.h"

//Setup function
void setup()
{
    Serial.begin(9600);     //Serialport for debugging
    initShifts();           //Initialisation of the shiftregisters
    initDpad();             //  "                   D-Pad
    matrixClear();          //Clear screen once
}

//Loop "main" / "menu" loop
void loop()
{
    menu();     //Call of menu
}

