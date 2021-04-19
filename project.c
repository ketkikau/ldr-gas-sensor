#include<P18F4550.h>
void lcdcmd(unsigned char value);
void lcddata(unsigned char value);
void msdelay(unsigned int itime);

#define ldata PORTD
#define rs PORTEbits.RE0
#define rw PORTEbits.RE1
#define en PORTEbits.RE2
#define buzzer PORTCbits.RC1
#define led PORTCbits.RC0
#define GAS PORTBbits.RB6
                                                                  
void main(void)
{
unsigned int i, d;
unsigned char temp[3];
unsigned char l;
unsigned int h,t;
TRISD=0;
PORTD=0;
TRISE=0x00;
PORTE=0;
TRISCbits.RC0 = 0; // led
TRISCbits.RC1 = 0;  // buzzer
TRISBbits.RB6 = 1;  // mq-2
ADCON0 = 0X01;     //CHANNEL AN0 as input of ADC and ADC ON
ADCON1 = 0X0E;     // ONLY AN0 pin as analog remaining pins as Digital Input Output
ADCON2=0b10001010;  //Result Right justified, A/D Acquisition time 2TAD,Clock frequency=fosc/32
msdelay(15);
lcdcmd(0x38);     // 16x2 LCD in 8 bit mode
msdelay(15);
lcdcmd(0x0E);      //Display On, Cursor blinking
msdelay(15);
lcdcmd(0x01);       //CLear Display Screen
msdelay(15);
lcdcmd(0x06);       //Increment cursor shift right
msdelay(15);
while(1)
{
if((GAS == 1))
{
buzzer = 1;
}
else
{
buzzer = 0;
}
lcdcmd(0x81);
msdelay(20);
ADCON0bits.GO = 1;             // Issue SOC command
while(!ADCON0bits.DONE ==1);
l = ADRESL;
h = ADRESH;
h = h << 8;
t = h | l;
if (t > 300 && t <800)
{
led=0;
}
else
{
led=1;
}
temp[0]= (ADRESH & 0x0f);
temp[1]= (ADRESL & 0xf0)>>4;
temp[2]= (ADRESL & 0x0f);
for(d=0; d<3; d++)
{
if (temp[d] < 10)
temp[d] = temp[d]+0x30;     //Convert the digit in ASCII Value
else
temp[d] = temp[d]+0x37;     //Convert the digit in ASCII Value
lcddata(temp[d]);        // send ASCII value for display
msdelay(15);
}
msdelay(10);
}
}

void lcdcmd (unsigned char value)
{
ldata=value;    //Command value to PORTD which is connected to Data lines of LCD
rs=0;          // FOR Command register selection of LCD
rw=0;         // WRITE mode
en=1;            // generate high to low pulse on ENABLE PIN
msdelay(1);
en=0;
}
void lcddata (unsigned char value)
{
ldata=value;      //Data value to PORTD which is connected to Data lines of LCD
rs=1;            // FOR Data register selection of LCD
rw=0;
en=1;            // generate high to low pulse on ENABLE PIN
msdelay(1);
en=0;
}
void msdelay (unsigned int itime)
{
int i,j;
for(i=0;i<itime;i++)
for(j=0;j<135;j++);
}