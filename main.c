#include "16F877.H"
#include "stdlib.h"
#include "stddef.h"
#include "ctype.h"
#include "string.h"

#use standard_io(B)
#use delay (clock=10000000)
#fuses XT
#fuses  PUT, HS, NOWDT, NOLVP, NOBROWNOUT 
#use i2c(Master, sda=PIN_C4, scl=PIN_C3, force_hw)
#use rs232(BAUD=19200, xmit=PIN_C6, rcv=PIN_C7, bits=8, parity=N)
#byte PORTA=0x05
#byte PORTB=0x06
#byte PORTC=0x07
#byte PORTD=0x08
#byte PORTE=0x09
#byte TRISA=0x85
#byte TRISB=0x86
#byte TRISC=0x87
#byte TRISD=0x88
#byte TRISE=0x89
#bit LED=PORTD.0

#define    RGB_LED_MATRIX_UPDATE_SCREEN                   (0x00)

#define    RGB_LED_MATRIX_SET_POINTER_X                   (0x03)
#define    RGB_LED_MATRIX_SET_POINTER_Y                   (0x04)
#define    RGB_LED_MATRIX_SEND_DATA                      (0x05)
#define    RGB_LED_MATRIX_SET_WINDOW_BOUND_START_X         (0x06)           // NOTICE: AFTER SET THE WINDOWS BOUND, THE POINTERX AND POINTERY
#define    RGB_LED_MATRIX_SET_WINDOW_BOUND_START_Y         (0x07)           //         WILL POINT TO WINDOWS BOUND START X AND START Y AUTOMATIC
#define    RGB_LED_MATRIX_SET_WINDOW_BOUND_END_X           (0x08)           //
#define    RGB_LED_MATRIX_SET_WINDOW_BOUND_END_Y           (0x09)          //
#define    RGB_LED_MATRIX_COPY_2_BXGDRAM                  (0x0A)
#define    RGB_LED_MATRIX_DISPLAY_BXGDRAM                (0x0B)
#define    RGB_LED_MATRIX_FILL_GDRAM                     (0x0C)         // FILL THE GDRAM WITH DATA(USER DEFINED) TXCMD(#define    RGB_LED_MATRIX_FILLGDRAM, (0x01)

#define    RGB_LED_MATRIX_SET_FOREGROUND_COLOR            (0x11)
#define    RGB_LED_MATRIX_SET_BACKGROUND_COLOR            (0x12)
#define    RGB_LED_MATRIX_DISPLAY_CHAR_0508               (0x13)
#define    RGB_LED_MATRIX_DISPLAY_CHAR_0816               (0x14)

#define    RGB_LED_MATRIX_DISPLAY_CHAR_0508_NBG            (0x15)             // NBG STAND FOR NO BACKGROUND
#define    RGB_LED_MATRIX_DISPLAY_CHAR_0816_NBG            (0x16)              // IF YOU WANT TO CLEAR THESE CHARACTER IN THE MATRIX PLEASE USE THE COMMAND #define    RGB_LED_MATRIX_DISPLAYBXGDRAM(  ) ABOVE
#define    RGB_LED_MATRIX_DISPLAY_PROPELLER_HAT           (0x17)
#define    RGB_LED_MATRIX_DISPLAY_PROPELLER_HAT_NBG        (0x18)
#define    RGB_LED_MATRIX_DISPLAY_PARALLAX_LOGO           (0x19)
#define    RGB_LED_MATRIX_DISPLAY_PARALLAX_LOGO_NBG        (0x1A)
#define    RGB_LED_MATRIX_DISPLAY_CUSTOMER_BMP1           (0x1B)
#define    RGB_LED_MATRIX_DISPLAY_CUSTOMER_BMP2           (0x1C)
#define    RGB_LED_MATRIX_DRAW_DOT                       (0x1D)

#define    RGB_LED_MATRIX_SET_LINE_END_X                   (0x1E)
#define    RGB_LED_MATRIX_SET_LINE_END_Y                   (0x1F)
#define    RGB_LED_MATRIX_DRAW_LINE                      (0x20)

#define    RGB_LED_MATRIX_SET_RADIUS                     (0x21)
#define    RGB_LED_MATRIX_DRAW_CIRCLE                    (0x22)

#define    RGB_LED_MATRIX_SAVE_PICTURE_EEPROM           (0x23)
#define    RGB_LED_MATRIX_LOAD_PICTURE_EEPROM           (0x24)


#define    RGB_LED_MATRIX_SCREEN_COLOR_INVERSE            (0x30)
#define    RGB_LED_MATRIX_SCREEN_OVERLAY                 (0x31)
#define    RGB_LED_MATRIX_SCREEN_MINUSLAY                (0x32)

#define    RGB_LED_MATRIX_RESET                         (0xA0)
#define    RGB_LED_MATRIX_SET_BAUDRATE                   (0xA1)
#define    RGB_LED_MATRIX_SET_MODE                       (0xA2)
#define    RGB_LED_MATRIX_GET_SCREEN_SIZE                 (0xA7)
#define    RGB_LED_MATRIX_CHANGE_SCREEN_SIZE              (0xA8)
#define    RGB_LED_MATRIX_GET_DEVICEID                   (0xA9)
#define    RGB_LED_MATRIX_CHANGE_DEVICEID                (0xAA)
#define    RGB_LED_MATRIX_GET_SYSTEM_CONFIGURATION        (0xAB)


byte  TPA81_ID = 0xD0; // Standardadresse I2C f?r TPA81
byte  i, servo, count; 
int16 t[10];

int32 colors[] = {
    0x00CC3300, //darkblue 1
    0x00990099, //dviolet 2
    0x006600CC, //violet 3
    0x000000CC, //red 4
    0x00005CE6, //orange 5
    0x0000FFFF, //yellow 6 
    0x00B3FFFF, //lyellow 7
    0x00FFFFFF //white
};

int colorchoose;


struct header_st
{
	byte str[4];
	byte cmd;
	union data_value_union
	{
		int32 bit_32;
		byte bit_8[4];
	} data_value;
} txPacket;

long colordata = 0;

void tx_data_packet(void)
{


	putc(txPacket.str[0]);
    putc(txPacket.str[1]);
    putc(txPacket.str[2]);
    putc(txPacket.str[3]);
	putc(txPacket.cmd);
	putc(txPacket.data_value.bit_8[3]);
	putc(txPacket.data_value.bit_8[2]);
	putc(txPacket.data_value.bit_8[1]);
	putc(txPacket.data_value.bit_8[0]);
}

void setup(void)
{
	txPacket.str[0] = 'P';
	txPacket.str[1] = 'M';
	txPacket.str[2] = 'T';
	txPacket.str[3] = 'X';
	txPacket.cmd = 0;
	txPacket.data_value.bit_32 = 0;	
    tx_data_packet();
	delay_ms(1000);
}

void set_servo_tpa81 ( byte slaveID, byte servo_pos ) { 
    i2c_start(); 
    i2c_write(slaveID); 
    i2c_write(0x00); 
    if(servo_pos>30)
        servo_pos = 30;
    i2c_write(servo_pos+9);              // 0.. 31 
    i2c_stop (); 
    delay_ms(40);
 } 


void read_tpa81( byte slaveID ) {

   for ( i=0; i<10; i++) {

      i2c_start();
      i2c_write(slaveID);
      i2c_write(i);            // Leseregister
      i2c_start();
      i2c_write(slaveID+1);
      t[i] =  i2c_read(0);
      t[i] += i2c_read(0);
      t[i] += i2c_read(0);
      t[i] += i2c_read(0);
      t[i] += i2c_read(0);
      t[i] += i2c_read(0);
      t[i] = t[i]/6;
      i2c_stop ();
      delay_ms(1);
   }
} 

void main(void)
{
    delay_ms(2000);
    TRISD=0b00000000;
    setup();
    
    set_servo_tpa81 (0xD0, 0);
    
    while(1)
    {
        servo=0;
        
        while(1)
        {
            read_tpa81(0xD0);
            
            for(count=0; count<8; count++)
            {
                txPacket.cmd = RGB_LED_MATRIX_SET_POINTER_X;                    
                txPacket.data_value.bit_32 = 0x00000000;
                txPacket.data_value.bit_8[0] = servo;
                tx_data_packet();

                txPacket.cmd = RGB_LED_MATRIX_SET_POINTER_Y;
                txPacket.data_value.bit_32 = 0x00000000;
                txPacket.data_value.bit_8[0] = count;
                tx_data_packet();

                txPacket.cmd = RGB_LED_MATRIX_DRAW_DOT;

                txPacket.data_value.bit_32 = 0x00000000;
                
                if((t[1])>(t[9-count]))                                         //Ambient Temperatur als 0 definieren
                    colorchoose = 0;
                else
                    colorchoose = (t[9-count])-t[1];

                //colorchoose = colorchoose/2;

                if(colorchoose > 7) 
                    colorchoose = 7;

                txPacket.data_value.bit_32 = colors[colorchoose];  

                tx_data_packet();
                
                delay_ms(2);
            }
            
            servo++;
            if (servo > 7)
            {
                servo = 0;
                set_servo_tpa81 (0xD0, servo);
                delay_ms(190);
                txPacket.cmd = RGB_LED_MATRIX_UPDATE_SCREEN;
                txPacket.data_value.bit_32 = 0x00000000;
                tx_data_packet();
            }
            else
            set_servo_tpa81 (0xD0, servo);
        }
    }
}
