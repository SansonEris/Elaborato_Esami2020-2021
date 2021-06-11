
// CONFIG1
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator: Crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)


//INCLUDE
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>

#define _XTAL_FREQ 8000000      

#include "stepMotor.h"


char veloc = 1;
char direction = CW; //CW = 0 clock wise, CCW = 1 counter clock wise;
char giri = 1;
//numero di step massimi per ogni asse
char MStepX = 0, MStepY = 0, MStepZ = 0;

void main(void) {
    PORTA = 0x00;
    PORTB = 0x00;	
    PORTC = 0x00;
    PORTD = 0x00;
    PORTE = 0x00;

    TRISA = 0b00000000;
    TRISB = 0b00000000;
    TRISC = 0b00000000;
    TRISD = 0b00000000;
    TRISE = 0b00000000;

    ANSEL = 0x00;
    ANSELH = 0x00;
    
       
    struct stepMotor {
        char dir;           //CW(0) / CCW(1)
        char speed;         //velocitá motore
        char mod;           //modalitá funzionamento (full Drive, half drive, half half drive, 8microstep, 16microstep...)
        char stepsToDo;     //numeri di giri da fare in step
    }motorX, motorY, motorZ;

/*
    MStepX = motorX_init(motorX);
    MStepY = motorY_init(motorY);
    MStepZ = motorZ_init(motorZ)

    aggiungere funzioni per calcolare il rapporto step a piano cartesiano web

*/


    veloc = 1;
    giri = 1;

       
    while (1) {
        full_drive(direction, speed, giri);
        direction = CCW;
        giri = 3;
        full_drive(direction, speed, giri);
    }

}

