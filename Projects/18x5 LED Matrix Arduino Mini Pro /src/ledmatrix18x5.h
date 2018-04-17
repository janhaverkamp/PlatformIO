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
int dpadinput;

static bool arr[5][18] ={
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

bool png[5][18] ={
    {0,0,0,1,1,0,0,1,0,0,0,1,0,0,1,1,1,1},
    {0,0,1,0,0,1,0,1,1,0,0,1,0,1,0,0,0,0},
    {0,0,1,1,1,0,0,1,0,1,0,1,0,1,1,1,1,0},
    {0,0,1,0,0,0,0,1,0,0,1,1,0,1,0,0,0,1},
    {0,0,1,0,0,0,0,1,0,0,0,1,0,0,1,1,1,0}
};

bool tst[5][18] ={
    {0,0,1,1,1,0,1,1,1,0,0,1,1,1,0,1,1,1},
    {0,0,0,1,0,0,1,0,0,0,1,0,0,0,0,0,1,0},
    {0,0,0,1,0,0,1,1,0,0,0,1,1,0,0,0,1,0},
    {0,0,0,1,0,0,1,0,0,0,0,0,0,1,0,0,1,0},
    {0,0,0,1,0,0,1,1,1,0,1,1,1,0,0,0,1,0}
};

void initShifts()//SHIFT REGISTER
{
    pinMode(DATA,OUTPUT);
    pinMode(CLOCK,OUTPUT);
    pinMode(LATCH,OUTPUT);
    pinMode(A3,OUTPUT);     //Output for two last columns
    pinMode(A2,OUTPUT);
    for(int i(4);i<=9; i++) pinMode(i,OUTPUT);      //Output of Rows
}

void pciSetup(byte pin)
{
    *digitalPinToPCMSK(pin) |= bit (digitalPinToPCMSKbit(pin));
    PCIFR |= bit(digitalPinToPCICRbit(pin));
    PCICR |= bit(digitalPinToPCICRbit(pin)); 

}

void initDpad()		//Init DPAD
{
	pinMode(A1,INPUT);      //DPAD analog in
    pinMode(A0,INPUT);      //DPAD analog in
    pinMode(A5,INPUT);      //DPAD analog in
    pinMode(A4,INPUT);      //DPAD analog in

    digitalWrite(A0, HIGH); //Internal pullups
    digitalWrite(A1, HIGH);
    digitalWrite(A4, HIGH);
    digitalWrite(A5, HIGH);

    pciSetup(A0);
    pciSetup(A1);
    pciSetup(A4);
    pciSetup(A5);
}

ISR (PCINT1_vect)
{
    if(digitalRead(A0) == 1) dpadinput = 1;
    if(digitalRead(A1) == 1) dpadinput = 4;
    if(digitalRead(A4) == 1) dpadinput = 2;
    if(digitalRead(A5) == 1) dpadinput = 3;

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

void matrixArray(bool array[5][18],int t)
{
    for(int z(0);z<=t;z++)      //Hold on time
    {
        for(int y(0); y <= 4; y++)      //Y row loop 
        {
            for(int i(0); i <= 4; i++) digitalWrite(yarray[i],0x01);    //Turn off all y pins (HIGH)
            for(int x(0); x<= 17; x++)                                  //X Column loop
            {
                if(array[y][x] == true)     //only execute if pixel is on
                {
                    switch(x)               //decide btw. pin 1, 2 o. shiftregister
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
    for(int x(1), y(1); ; )
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

void squash()         //Pong-game knockoff
{
    int x(4),y(2),pb(2),cb(17),vx(0),vy(0),pp(2),cp(2),speed(20);       //Game variables

    while(speed > 0)    //Loop until speed is 1:0
    {
        while(pb < cb)  //Loop until playerbar reaches cpubar
        {
            if      (dpadinput == 1 && pp > 0) pp -= 1;                 //Position of playerbar
            else if (dpadinput == 3 && pp < 4) pp += 1;

            arr[pp][pb] = 1; arr[pp-1][pb] = 0; arr[pp+1][pb] = 0;      //Write player cursor pos in array

            if(dpadinput == 2 && vx == 0 && vy == 0)                    //Start directions for ball
            {
                if(pp == 2) vy=(random(10,30) > 20) ? -1 : 1;
                else if(pp > 2) vy = -1;
                else if(pp < 2) vy = 1;
                y = pp; x = pb;
                vx = 1;
            }
            dpadinput = 0;                                              //Clear input

            

            if(vx == 1 && x <= cb)      {x++; arr[y][x] = 1; arr[y][x-1] = 0;}      //Boundries
            else if(vx == -1 && x >= pb) {x--; arr[y][x] = 1; arr[y][x+1] = 0;}     //and write pos into array
            if(vy == 1 && y <= 4)       {y++; arr[y][x] = 1; arr[y-1][x] = 0;}
            else if(vy == -1 && y >= 0) {y--; arr[y][x] = 1; arr[y+1][x] = 0;}

            if(y == 0 || y == 4)                vy = -vy;               //Change of direction on touch of a 
            if(x == cb || (x == pb && pp == y)) vx = -vx;               //boundry
            else if(x == pb && pp != y && !(vx == 0 && vx == 0))        //When ball wasn't catched
            {
                vx     =  0; vy     =  0;
                arr[pp][pb] = 0;
                pb++;
            }
            matrixArray(arr,speed);                                     //Write array into function
        }
        for(int b(0); b <= 4; b++)                                      //clear after gameover
        {
            for(int n(0); n <= 17; n++)
            {
                arr[b][n] = 0;
            }
        }
        speed -= 5;
    }
}

void menu()         //Mainmenu loop
{
    int men(0), mlimit(2);                                  //Limit of menuitems
    while(true)                                             //Endless loop
    {
        if(dpadinput == 1 && men < mlimit) men++;           //Change of menuentries
        else if(dpadinput == 3 && men > 0) men--;
        dpadinput = 0;

        switch(men)                                         //switch for menuitems
        {
            case 0:
                    matrixArray(hme,10);
                    break;

            case 1:                                         //PONG
                    matrixArray(png,10);
                    if(digitalRead(A4) == 1) squash();      //Call of the function squash
                    break;
            case 2:                                         //TestMatrix
                    matrixArray(tst,10);
                    if(digitalRead(A4) == 1) matrixTest();  //Call a matrix test program to test all led's and buttons
                    break;
            default:                                        //Default case, if no item is selected
                    matrixClear();
                    break;
        }
    }
}

#endif
