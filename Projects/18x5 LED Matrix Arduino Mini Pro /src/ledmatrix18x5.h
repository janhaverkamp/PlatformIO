#ifndef LEDMATRIX18X15                          //Definition of headerfile

#define DATA    10                              //Shiftregister pins
#define CLOCK   12
#define LATCH   11
#define REGISTERS   2
                                                //Macros for shiftregister-pulses
#define CLOCKPULSE digitalWrite(CLOCK,0x01);\
                   digitalWrite(CLOCK,0x00);
#define LATCHPULSE digitalWrite(LATCH,0x01);\
                   digitalWrite(LATCH,0x00);

#define ROWS    5                               //Definition of the Led-Matrix
#define COLUMNS 18

const int yarray[] = {4,6,7,8,9};               //Pinout-array for loops and etc..
int dpadinput;                                  //Input var for D-Pad interrupts

                                                //Display arrays 

static bool arr[5][18] ={                       //Clear global array
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

const bool png[5][18] ={                        //PONG bitmap array
    {0,0,0,1,1,0,0,1,0,0,0,1,0,0,1,1,1,1},
    {0,0,1,0,0,1,0,1,1,0,0,1,0,1,0,0,0,0},
    {0,0,1,1,1,0,0,1,0,1,0,1,0,1,1,1,1,0},
    {0,0,1,0,0,0,0,1,0,0,1,1,0,1,0,0,0,1},
    {0,0,1,0,0,0,0,1,0,0,0,1,0,0,1,1,1,0}
};

const bool tst[5][18] ={                        //TEST bitmap array
    {0,0,1,1,1,0,1,1,1,0,0,1,1,1,0,1,1,1},
    {0,0,0,1,0,0,1,0,0,0,1,0,0,0,0,0,1,0},
    {0,0,0,1,0,0,1,1,0,0,0,1,1,0,0,0,1,0},
    {0,0,0,1,0,0,1,0,0,0,0,0,0,1,0,0,1,0},
    {0,0,0,1,0,0,1,1,1,0,1,1,1,0,0,0,1,0}
};

const bool hme[5][18] ={                        //HOME bitmap array
    {0,0,0,1,1,1,0,0,0,1,1,1,0,0,1,1,1,0},
    {0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,0,1},
    {0,0,1,0,0,0,1,0,1,0,0,0,1,0,1,1,1,1},
    {0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,0,1},
    {0,0,0,1,1,1,0,0,0,1,1,1,0,0,1,1,1,0}
};

void initShifts()//SHIFT REGISTER               //Initialisation of shiftregisters
{
    pinMode(DATA,OUTPUT);                       //Setting the outputs
    pinMode(CLOCK,OUTPUT);
    pinMode(LATCH,OUTPUT);
    pinMode(A3,OUTPUT);                         //Output for two last columns, that aren't in the registers
    pinMode(A2,OUTPUT);
    for(int i(4);i<=9; i++) pinMode(i,OUTPUT);  //Output for the Rows
}

void pciSetup(byte pin)                         //Setup of general interrupt-funtion
{
    *digitalPinToPCMSK(pin) |= bit (digitalPinToPCMSKbit(pin));
    PCIFR |= bit(digitalPinToPCICRbit(pin));
    PCICR |= bit(digitalPinToPCICRbit(pin)); 

}

void initDpad()		                            //Initialisation of the DPAD
{
	pinMode(A1,INPUT);                          //DPAD analog in
    pinMode(A0,INPUT);
    pinMode(A5,INPUT);
    pinMode(A4,INPUT);

    digitalWrite(A0, HIGH);                     //Internal pullups
    digitalWrite(A1, HIGH);
    digitalWrite(A4, HIGH);
    digitalWrite(A5, HIGH);

    pciSetup(A0);                               //Setup interrupt-pins
    pciSetup(A1);
    pciSetup(A4);
    pciSetup(A5);
}

ISR (PCINT1_vect)                               //Interupt-routine function
{
    if(digitalRead(A0) == 1) dpadinput = 1;     //Write values into var, when interrupting
    if(digitalRead(A1) == 1) dpadinput = 4;
    if(digitalRead(A4) == 1) dpadinput = 2;
    if(digitalRead(A5) == 1) dpadinput = 3;

}

void matrixDot(int x, int y)                    //Old function that displays only one pixel, by (x)(y)
{
    for(int pin(4); pin <= 9; pin++) digitalWrite(pin,0x01);    //Turn Y pins off as default
    digitalWrite(A2,0x00);                      //Turn of two X over pins 
    digitalWrite(A3,0x00);

    if     (y==1) digitalWrite(4,0x00);         //Decide which row to turn on, by Y
    else if(y==2) digitalWrite(6,0x00);
    else if(y==3) digitalWrite(7,0x00);
    else if(y==4) digitalWrite(8,0x00);
    else if(y==5) digitalWrite(9,0x00);

    if(x == 1) digitalWrite(A3,0x01);           //Turn on A3 / A2 when not in registers
    if(x == 2) digitalWrite(A2,0x01); 
    if(x >= 3)                                  //If in area of registers. Display function
        {
        digitalWrite(LATCH,0);                  //Clock and Latch off-pulses
        digitalWrite(CLOCK,0);

        for(int a(18); a >= 0; a--)             //Loop for the shiftregisters to get data and clock pulses
            {
                if(a == -(x-18)) digitalWrite(DATA,0x01);   //Pulse DATA HIGH, when true
            digitalWrite(CLOCK,0x01);           //Clock pulse
            digitalWrite(DATA,0x00);            //Data off
            digitalWrite(CLOCK,0x00);           //Clock pulse off
            }
        digitalWrite(LATCH,1);                  //Shift out
    }




}

void matrixArray(bool array[5][18],int t)       //Momentary function, that displays by an array[][] and a hold on time (t)
{
    for(int z(0);z<=t;z++)                      //Hold on time
    {
        for(int y(0); y <= 4; y++)              //Change Y rows loop 
        {
            for(int i(0); i <= 4; i++) digitalWrite(yarray[i],0x01);    //Turn off all y pins (HIGH)
            for(int x(0); x<= 17; x++)                                  //X Column loop
            {
                if(array[y][x] == true)         //only execute if pixel is on
                {
                    switch(x)                   //decide btw. pin 1, 2 o. shiftregister
                    {
                        case 0:                 //Pin A3 column 1
                                digitalWrite(A3,0x01);
                                digitalWrite(A2,0x00);
                                digitalWrite(DATA,0x00);
                                break;
                        case 1:                 //Pin A2 column 2
                                digitalWrite(A2,0x01);
                                digitalWrite(A3,0x00);
                                digitalWrite(DATA,0x00);
                                break;
                        default:                //Registers columns 3+
                                digitalWrite(DATA,0x01);
                                digitalWrite(A3,0x00);
                                digitalWrite(A2,0x00);
                    }
                }
                else                            //If no bit, than turn all pins off
                {
                    digitalWrite(DATA,0x00);
                    digitalWrite(A2,0x00);
                    digitalWrite(A3,0x00);
                }
                CLOCKPULSE                      //Clockpulse afterevery cycle
            }
            LATCHPULSE                          //Latchpulse shift out 
            digitalWrite(yarray[y],0x00);       //Set Y-pins columns to on (LOW)
            delay(1);                           //Delay to display properly
        }
    }
}

void matrixClear()                              //Display-clear function, hardwareside, no arrays
{
    digitalWrite(LATCH,0x00);                   //Turn off Latch and data pins
    digitalWrite(DATA,0x00);

    digitalWrite(A2,0x00);                      //Also two X-pins
    digitalWrite(A3,0x00);

    for(int i(0); i <= 16; i++) {CLOCKPULSE LATCHPULSE}     //Loop to pulse trough empty signal to registers
}

void matrixTest()                               //Test function to check led's and buttons. No boundries
{
    for(int x(1), y(1); ; )                     //Endless loop with (x) and (y) definition
    {
		if(digitalRead(A0) == 1) y -= 1;        //Change position when pressin a button
		else if(digitalRead(A1) == 1) x -= 1;
		else if(digitalRead(A5) == 1) y += 1;
		else if(digitalRead(A4) == 1) x += 1;

		if(x == 19) x = 1;                      //Jump to other side, when crossing boundry
		else if( x== 0) x = 18;
		if(y == 6)  y = 1;
		else if(y == 0) y = 5;

		matrixDot(x,y);                         //Display the dot, by function matrixDot(int x,int y);
		delay(100);                             //Delay to reduce speed
		matrixClear();                          //Clear screen, by function matrixClear();
    }
}

void squash()                                   //Pong-game knockoff
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

void menu()                                     //Mainmenu loop
{
    int men(0), mlimit(2);                                  //Limit of menuitems
    while(true)                                             //Endless loop
    {
        if(dpadinput == 1 && men < mlimit) men++;           //Change of menuentries
        else if(dpadinput == 3 && men > 0) men--;
        dpadinput = 0;

        switch(men)                                         //switch for menuitems
        {
            case 0:                                         //Homescreen
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

#endif                                          //End of definition
