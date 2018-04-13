#include <Arduino.h>
#include "ledmatrix18x5.h"


void setup()
{
    Serial.begin(9600);
    initShifts();
    initDpad();
    matrixClear();
}



void loop()
{
    static bool arr[18][5] ={
        {1,1,1,0,1},
        {0,0,0,0,0},
        {1,1,1,1,1},
        {0,1,0,1,0},
        {1,1,1,1,1},
        {0,0,0,0,0},
        {1,1,0,1,1},
        {0,0,0,0,0},
        {1,1,1,1,1},
        {0,0,1,0,0},
        {1,1,1,1,1},
        {0,0,0,0,0},
        {1,1,1,0,1},
        {0,0,0,0,1},
        {1,1,1,1,1},
        {1,0,1,0,1},
        {1,0,1,0,1},
        {1,0,1,0,0}
    };
    matrixArray(arr);
    matrixClear();
    //matrixTest();
}

