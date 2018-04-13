#ifndef LEDMATRIX18X15

#define DATA    10
#define CLOCK   12
#define LATCH   11
#define REGISTERS   2

#define CLOCKPULSE  digitalWrite(CLOCK,0x01); digitalWrite(CLOCK,0x00);
#define LATCHPULSE   digitalWrite(LATCH,0x01); digitalWrite(LATCH,0x00);

#define ROWS    5
#define COLUMNS 18

const int yarray[] = {4,6,7,8,9};

void initShifts()//SHIFT REGISTER
{
    pinMode(DATA,OUTPUT);
    pinMode(CLOCK,OUTPUT);
    pinMode(LATCH,OUTPUT);
    pinMode(A3,OUTPUT);     //Output for two last columns
    pinMode(A2,OUTPUT);
    for(int i(4);i<=9; i++) pinMode(i,OUTPUT);      //Output of Rows
}

void initDpad()		//Init DPAD
{
	pinMode(A1,INPUT);      //DPAD analog in
    pinMode(A0,INPUT);      //DPAD analog in
    pinMode(A5,INPUT);      //DPAD analog in
    pinMode(A4,INPUT);      //DPAD analog in
}

void matrixDot(int x, int y)
{
    for(int pin(4); pin <= 9; pin++) digitalWrite(pin,0x01);    //Turn Y pins off as default
    digitalWrite(A2,0x00);
    digitalWrite(A3,0x00);

    if     (y==1) digitalWrite(4,0x00);
    else if(y==2) digitalWrite(6,0x00);
    else if(y==3) digitalWrite(7,0x00);
    else if(y==4) digitalWrite(8,0x00);
    else if(y==5) digitalWrite(9,0x00);

    if(x == 1) digitalWrite(A3,0x01);
    if(x == 2) digitalWrite(A2,0x01); 
    if(x >= 3)
        {
        digitalWrite(LATCH,0);
        digitalWrite(CLOCK,0);

        for(int a(18); a >= 0; a--) 
            {
                if(a == -(x-18)) digitalWrite(DATA,0x01); 
            digitalWrite(CLOCK,0x01);
            digitalWrite(DATA,0x00);
            digitalWrite(CLOCK,0x00);
            }
        digitalWrite(LATCH,1);

    }




}

void matrixArray(bool array[5][18])
{
    for(int y(0); y <= 4; y++)
    {
        for(int i(0); i <= 4; i++) digitalWrite(yarray[i],0x01);
        for(int x(0); x<= 17; x++)
        {
            if(array[y][x] == true)
            {
                switch(x)
                {
                    case 0: 
                            digitalWrite(A3,0x01);
                            digitalWrite(A2,0x00);
                            digitalWrite(DATA,0x00);
                            break;
                    case 1: 
                            digitalWrite(A2,0x01);
                            digitalWrite(A3,0x00);
                            digitalWrite(DATA,0x00);
                            break;
                    default:
                            digitalWrite(DATA,0x01);
                            digitalWrite(A3,0x00);
                            digitalWrite(A2,0x00);
                }
            }
            else
            {
                digitalWrite(DATA,0x00);
                digitalWrite(A2,0x00);
                digitalWrite(A3,0x00);
            }
            CLOCKPULSE
        }
        LATCHPULSE
        digitalWrite(yarray[y],0x00);
        delay(1);
    }
}

void matrixClear()
{
    digitalWrite(LATCH,0x00);
    digitalWrite(DATA,0x00);

    digitalWrite(A2,0x00);
    digitalWrite(A3,0x00);

    //for(int pin(4); pin <= 9; pin++) digitalWrite(pin,0x01);

    for(int i(0); i <= 16; i++)
    {
        digitalWrite(CLOCK,0x01);
        digitalWrite(CLOCK,0x00);
    }
    digitalWrite(LATCH,0x01);
    digitalWrite(LATCH,0x00);
}

void matrixTest()
{
    for(int x(1), y(1); true; )
    {
		if(digitalRead(A0) == 1) y -= 1;
		else if(digitalRead(A1) == 1) x -= 1;
		else if(digitalRead(A5) == 1) y += 1;
		else if(digitalRead(A4) == 1) x += 1;
		if(x == 19) x = 1;
		else if( x== 0) x = 18;
		if(y == 6)  y = 1;
		else if(y == 0) y = 5;
		matrixDot(x,y);
		delay(100);
		matrixClear();
    }
}

#endif
