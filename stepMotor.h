//interfacciamento con DRV8825
/*
RD0 = stepm1
RD1 = dirm1

RD2 = stepm2
RD3 = dirm2

RD4 = stepm3
RD5 = dirm3

*/

//motore 1
#define STEPM1 PORTAbits.RA1
#define DIRM1 PORTAbits.RA0
//motore 2
#define STEPM2 PORTEbits.RE0
#define DIRM2 PORTAbits.RA5
//motore 3
#define STEPM3 PORTCbits.RC2
#define DIRM3 PORTCbits.RC1

//fine corsa
#define Xlimit0 PORTBbits.RB0
#define Xlimit1 PORTBbits.RB1
#define Ylimit0 PORTBbits.RB2
#define Ylimit1 PORTBbits.RB3
#define Zlimit0 PORTBbits.RB4
#define Zlimit1 PORTBbits.RB5

#define writeVelocity 1
#define moveVelocity 5

#define deg 1.8 //caratteristica del motore
//#define steps 200 // conteggi per ogni giro con motoriduttore
#define CW 0 // rotazione in senso orario (clock wise)
#define CCW 1 // rotazione in senso anti-orario (counter clock wise)

/*
    RB1 = M0
    RB2 = M1
    RB3 = M2

    M2  M1  M0
    0   0   0   Full step (2-phase excitation) with 71% current
    0   0   1   1/2 step (1-2 phase excitation)
    0   1   0   1/4 step (W1-2 phase excitation)
    0   1   1   8 microsteps/step
    1   0   0   16 microsteps/step
    1   0   1   32 microsteps/step
    1   1   0   32 microsteps/step
    1   1   1   32 microsteps/step
*/
#include "motor_mode.h"

void debug(){
    PORTBbits.RB3 = 1;
    __delay_ms(1000);
    PORTBbits.RB3 = 0;
}

char steps = (char)(360 / deg);

char speed;

void ms_delay(unsigned char val){
    unsigned char i;
    for (i = 0; i < val; i++)
        __delay_ms(1);
}

char conteggioGiri(char nGiri) {
    return nGiri * steps;
}


//il motore gira in modalit� full_drive per n giri
void full_drive(char direction, char speed) {
    PORTAbits.RA4 = 0;
        PORTAbits.RA3 = 0;
        PORTAbits.RA2 = 0;
        if (direction == CCW) {
            DIRM1 = 1;
            STEPM1 = 1;
            ms_delay(speed);
            STEPM1 = 0;
            ms_delay(speed);
        }
        if (direction == CW) {
            DIRM1 = 0;
            STEPM1 = 1;
            ms_delay(speed);
            STEPM1 = 0;
            ms_delay(speed);
        }
    
}

//il motore gira in modalit� half drive per n giri
void half_drive(char direction, char speed) {
    PORTAbits.RA4 = 1;
        PORTAbits.RA3 = 0;
        PORTAbits.RA2 = 0;
        if (direction == CCW) {
            STEPM1 = 1;
            DIRM1 = 1;
            ms_delay(speed);
            STEPM1 = 0;
            DIRM1 = 1;
            ms_delay(speed);
        }
        if (direction == CW) {
            STEPM1 = 1;
            DIRM1 = 0;
            ms_delay(speed);
            STEPM1 = 0;
            DIRM1 = 0;
            ms_delay(speed);
        }
}

//il motore gira in modalit� micro step 8 per n giri
void microstep8(char direction, char speed) {
    PORTAbits.RA4 = 1;
        PORTAbits.RA3 = 1;
        PORTAbits.RA2 = 0;
        if (direction == CCW) {
            STEPM1 = 1;
            DIRM1 = 1;
            ms_delay(speed);
            STEPM1 = 0;
            DIRM1 = 1;
            ms_delay(speed);
        }
        if (direction == CW) {
            STEPM1 = 1;
            DIRM1 = 0;
            ms_delay(speed);
            STEPM1 = 0;
            DIRM1 = 0;
            ms_delay(speed);
        }
}


/***********************************************************************************************************************/


//converto un valore da 1-8 nel moltiplicatore di step necessari per quella modalit�
int modConversionFormNumberToMultiplier(int a) {
    if (a != 0) a -= 1;
    int val[8] = { 1,2,4,8,16,32,32,32 };
    return val[a];
}


//uno step unico, per la gestione in contemporanea di 3 motori
void stepMotorX(char dir, char speed, int mod) { //devo passare l'indirizzo che deve essere portato a 1 per l'impulso
    modM1(mod);
    
    if (dir == CCW) {
        STEPM1 = 1;
        DIRM1 = 1;
        ms_delay(speed);
        STEPM1 = 0;
        DIRM1 = 1;
        ms_delay(speed);
    }
    else if(dir == CW){
        STEPM1 = 1;
        DIRM1 = 0;
        ms_delay(speed);
        STEPM1 = 0;
        DIRM1 = 0;
        ms_delay(speed);
    }
}

//uno step unico, per la gestione in contemporanea di 3 motori
void stepMotorY(char dir, char speed, int mod) { //devo passare l'indirizzo che deve essere portato a 1 per l'impulso
    modM2(mod);
    
    if (dir == CCW) {
        STEPM2 = 1;
        DIRM2 = 1;
        ms_delay(speed);
        STEPM2 = 0;
        DIRM2= 1;
        ms_delay(speed);
    }
    else if(dir == CW){
        STEPM2 = 1;
        DIRM2 = 0;
        ms_delay(speed);
        STEPM2 = 0;
        DIRM2 = 0;
        ms_delay(speed);
    }
}
//uno step unico, per la gestione in contemporanea di 3 motori
void stepMotorZ(char dir, char speed, int mod) { //devo passare l'indirizzo che deve essere portato a 1 per l'impulso
    modM3(mod);
        
    if (dir == CCW) {
        
        STEPM3 = 1;
        DIRM3 = 1;
        ms_delay(speed);
        
        STEPM3= 0;
        DIRM3 = 1;
        ms_delay(speed);
    }
    else if(dir == CW){
        
        STEPM3 = 1;
        DIRM3 = 0;
        ms_delay(speed);
        
        STEPM3 = 0;
        DIRM3 = 0;
        ms_delay(speed);
    }
}

char limitDetection(){
    if(Xlimit1||Xlimit0||Ylimit0||Ylimit1||Zlimit0||Zlimit1) return 1;
}

int maxStep(int stepx, int stepy, int stepz){
   int l = 0;
   return l = stepx > stepy ? (stepx > stepz ? stepx : stepz) : (stepy > stepz ? stepy : stepz); 
}

//controllo se il motore deve ancora fare degli step
int activeMotor(int i, int step){
    if(i<step)
        return 1;
    else return 0;
}


int limitx(){
    if(Xlimit0 == 1)
        return 1;
    if(Xlimit1 == 1)
        return 2;
    
    else return 0;
    
}

int limity(){
    if(Ylimit0 == 1)
        return 1;
    if(Ylimit1 == 1)
        return 2;
    
    else return 0;
    
}
int limitz(){
    if(Zlimit0 == 1)
        return 1;
    if(Zlimit1 == 1)
        return 2;
    
    else return 0;
    
}