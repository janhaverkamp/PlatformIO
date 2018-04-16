#include <Arduino.h>
#include "ledmatrix18x5.h"

static bool arr[5][18] ={
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0},
    {1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
    {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

void setup()
{
    Serial.begin(9600);
    initShifts();
    initDpad();
    matrixClear();
}

void loop()
{
    matrixArray(arr);
    //matrixTest();
}

