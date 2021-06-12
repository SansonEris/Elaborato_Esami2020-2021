
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
#include <string.h>
#include <math.h>

#define _XTAL_FREQ 8000000      

#include "stepMotor.h"
#include "UART.h"

/**************************VARIABILI GESTIONE MOTORI***************************/                                                                            
/*numero di step massimi per ogni asse*/                                      
int MStepX = 0, MStepY = 0, MStepZ = 0;                                       
                                                                              
/*scomposizione stringa ricevuta da uart*/                                    
char stepX[7] = "000";                                                                
char stepY[7] = "000";                                                                
char stepZ[7] = "000";                                                                
                                                                              
/*Stringa ricevuta da uart*/                                                 
char coords[9], canvas[7], canvasX[3] = "000", canvasY[3] = "000";                                                               
                                                                              
/*finecorsa assi*/                                                           
char limitX0 = 0,limitX1 = 0,limitY0 = 0,limitY1 = 0,limitZ0 = 0,limitZ1 = 0; 
                                                                              
/*Step convertiti in interi*/                                                 
int stepx = 0, stepy = 0, stepz = 0, canvasx = 0, canvasy = 0;                                          
/*step da fare durante il movimento*/                                         
int stepxTodo = 0, stepyTodo = 0, stepzTodo = 0;                             
int stepXprec = 0, stepYprec = 0, stepZprec = 0;                              
                                                                              
/*inizializzazione della direzione incrementale*/                             
char defaultDirx = 0, defaultDiry = 0, defaultDirz = 0;                       

char end = 0;

/**************************FUNZIONI GESTIONE MOTORI***************************/                                                                            
int getCanvasDim(char canvas[]);
int getCoords(char coords[]);                                                      
                                                                              
int toint(char c[]);                                                                  
                                                                               
int motorXInit();                                                              
int motorYInit();                                                             
int motorZInit();                                                             
                                                                              
void constSpeed(int activeX, int activeY, int activeZ);                       
void contStepX();                                                             
void contStepY();                                                             
void contStepZ();                                                           
                    
int equivVal(int Mcanvas, int Mstep, int canvas);

void reset();                                                                         

/***********************************STRUTTURA MOTORE**************************/
struct stepMotor {
    char dir;
    char speed;
    char mod; //passare un numero da 1 - 8 per la modalitï¿½ da scegliere
    int stepsToDo; //x | y | z valori convertiti in steps (deve essere convertito in out dal server php in base al valore cartesiano)
} motorX, motorY, motorZ;


void main(void) {
    PORTA = 0x00;
    PORTB = 0x00;	
    PORTC = 0x00;
    PORTD = 0x00;
    PORTE = 0x00;

    TRISA = 0b00000000;
    TRISB = 0b01100111;
    TRISC = 0b10000000;
    TRISD = 0b00000000;
    TRISE = 0b00000000;

    ANSEL = 0x00;
    ANSELH = 0x00;
    
//inizializzazione uart per comunicazione con ESP32    
    UART_Init();   
    
    motorX.dir = CCW;
    motorX.speed = 1;
    motorX.mod = 1;
    
    motorY.dir = CCW;
    motorY.speed = 1;
    motorY.mod = 1;
    
    motorZ.dir = CCW;
    motorZ.speed = 1;
    motorZ.mod = 1;
    
    /*calibrazioni del plotter*/
    
    /*settaggio grandezza massima*/
    
    MStepX = motorXInit();
    defaultDirx = motorX.dir;
    
    MStepY = motorYInit();
    defaultDiry = motorY.dir;
       

    MStepZ = motorZInit();
    defaultDirz = motorZ.dir;
    
    
    while(1){
    //variabile di fine disegno    
    end = 0;
    
    //ricezione dim canvas
    getCanvasDim(canvas);
    canvasx = toint(canvasX);
    UART_TxInt(canvasx);
    canvasy = toint(canvasY);
    UART_TxInt(canvasy);
    
    do{     
        UART_RxString(coords);
        UART_TxString("eco: ");
        UART_TxString(coords);
        
        //se ok in coords ricevo dati
        if(!strcmp(coords,"ok"))
            getCoords(coords);
        
        //controllo se la stirnga ricevuta é quella di fine disegno
        if(!strcmp(coords,"end"))
            end = 1;
          
       if(end != 1){
            //step da eseguire per ogni asse con equivalenza        
           stepx = equivVal(canvasX, MStepX,toint(stepX));
           UART_GotoNewLine(); 
           UART_TxString("ecoX: ");
           UART_TxInt(stepx);
           
           stepy = equivVal(canvasY, MStepY,toint(stepY));
           UART_GotoNewLine(); 
           UART_TxString("ecoY: ");
           UART_TxInt(stepy);
           
        //l'asse z in questo caso non necessita di equivalenze perché ha solo due posizioni equivalenti ai finecorsa
           stepz = toint(stepZ);
           UART_GotoNewLine(); 
           UART_TxString("ecoZ: ");
           UART_TxInt(stepz);

        
           //conteggio degli step da fare
           contStepX();
           UART_GotoNewLine(); 
           UART_TxString("todo X: ");
           UART_TxInt(stepxTodo);
           contStepY();
           UART_GotoNewLine(); 
           UART_TxString("todo Y: ");
           UART_TxInt(stepyTodo);
           
           //contStepZ(); //il conteggio degli step dell'asse z viene effettuato durante l'esecuzione degli step
        
        
           int largest = maxStep(stepxTodo, stepyTodo, stepzTodo);
           int activeX = 0, activeY = 0, activeZ = 0;
        
            //ottenimento del valore massimo di step e controllo per ogni motore
            for(int i = 0; i<= largest ; i++){
                activeX = activeMotor(i, stepxTodo);
                activeY = activeMotor(i, stepyTodo);

                constSpeed(activeX, activeY, activeZ);

                if(activeX == 1)
                    stepMotorX(motorX.dir, motorX.speed, motorX.mod);

                if(activeY == 1)
                    stepMotorY(motorY.dir, motorY.speed, motorY.mod);     
            } 
        
            //l'asse z si muove fino all'ipotetica posizione di disegno e successivamente si risolleva
            for(int i = 0; i<= stepz; i++){
                motorZ.dir = defaultDirz;
                stepMotorZ(motorZ.dir, motorZ.speed, motorZ.mod);
            }
            for(int i = 0; i<= stepz; i++){
                if(motorZ.dir == CCW) motorZ.dir = CW;
                else motorZ.dir = CCW;
                stepMotorZ(motorZ.dir, motorZ.speed, motorZ.mod);
            }
        
        //non é possibile sapere se nel disegno dovró disegnare una linea o solo dei punti
        //per semplicitá qundi l'algoritmo (ora) é in grado di eseguire un disegno "a puntini"
            
        stepXprec = stepx;
        stepYprec = stepy;
        //stepZprec = stepz;
       
       }
       

    }while(end!=1);
    //reset();
PORTBbits.RB3 = 0;
    }
 
}
int getCanvasDim(char canvas[]){
    //           x | y
    //canvas = '300540'
    UART_TxString("ready");
    UART_RxString(canvas);
    
    for(int j = 0; j<3; j++){
        canvasX[j] = canvas[j];
   		canvasY[j] = canvas[3+j];
	}
    
    //eco ricezione e suddivisione
    UART_TxString("X: ");
    UART_TxString(canvasX);
    UART_TxString("Y: ");
    UART_TxString(canvasY);
}
           

int getCoords(char coords[]){   
    //ottengo una stirnga xxxyyyzzz
    
    __delay_ms(1000);
    UART_RxString(coords);
    UART_TxString("eco ricezione: ");
    UART_TxString(coords);
    
    //suddivisione della strnga
    for(int i = 0; i<3; i++){
        stepX[i] = coords[i];    
        stepY[i] = coords[3+i];
        stepZ[i] = coords[6+i];
    }
}

int equivVal(int Mcanvas, int Mstep, int canvas){
        //equivalenza canvas step
/*    
    MXcanvas : MXstep = 1canvas : Xstep

    Xstep = MXstep * 1 / MXcanvas
*/  
    return (Mstep * canvas) / Mcanvas;
}

int toint(char *c){
    int i, value, r, flag;
    
    flag = 1;
    i = value = 0;
    
    for( i = 0 ; i<strlen(c) ; ++i){
        
        // if arr contain negative number
        if( i==0 && c[i]=='-' ){
            flag = -1;
            continue;
        }
        
        r = c[i] - '0';
        value = value * 10 + r;
    }
    
    value = value * flag;
    
    return value;
        
}


void constSpeed(int activeX, int activeY, int activeZ){
    //int nMotor = activeX + activeY + activeZ;
    int nMotor = activeX + activeY;
    /*if(nMotor == 3)
        motorX.speed = motorY.speed = motorZ.speed = 1;*/
    
    if(nMotor == 2)
        motorX.speed = motorY.speed = motorZ.speed = 1;
    if(nMotor == 1)
        motorX.speed = motorY.speed = motorZ.speed = 2; 
    
}


void contStepX(){
    //motore X conteggio degli step da fare 
    //modifica di una variabile globale stepxTodo
    if(stepXprec > stepx){
        stepxTodo = stepXprec - stepx;
        if(motorX.dir == CCW) motorX.dir = CW;
        else motorX.dir = CCW;
    } else if (stepXprec < stepx){
        stepxTodo = stepx - stepXprec;
        if(motorX.dir != defaultDirx){
            if(motorX.dir == CCW) motorX.dir = CW;
            else motorX.dir = CCW;
        }
    } else if (stepXprec == stepx){
        stepxTodo = stepx - stepXprec;
        }
}


void contStepZ(){
    
    //motore Z conteggio degli step da fare 
    //modifica di una variabile globale stepzTodo
    if(stepZprec > stepz){
        stepzTodo = stepZprec - stepz;
        if(motorZ.dir == CCW) motorZ.dir = CW;
        else motorZ.dir = CCW;
    } else if (stepZprec < stepz){
        stepzTodo = stepz - stepZprec;
        if(motorZ.dir != defaultDirz){
        if(motorZ.dir == CCW) motorZ.dir = CW;
        else motorZ.dir = CCW;
        }
    } else if (stepZprec == stepz){
            stepzTodo = stepz - stepZprec;
        }
}


void contStepY(){
    
    //motore Y conteggio degli step da fare 
    //modifica di una variabile globale stepyTodo
    if(stepYprec > stepy){
        stepyTodo = stepYprec - stepy;
        if(motorY.dir == CCW) motorY.dir = CW;
        else motorY.dir = CCW;
    } else if (stepYprec < stepy){
        stepyTodo = stepy - stepYprec;
        if(motorY.dir != defaultDiry){
        if(motorY.dir == CCW) motorY.dir = CW;
        else motorY.dir = CCW;
        }
    } else if (stepYprec == stepy){
            stepyTodo = stepy - stepYprec;
        }
}


int motorXInit(){
    int m = 0;
    int status = 0;
    for(int i = 0; status != 1; i++){
        motorX.dir = CCW;
        stepMotorX(motorX.dir, motorX.speed, motorX.mod);
           
        if(limitz() == 1){
            for(int j = 0; limitX1 != 1; j++){
                motorX.dir = CW;
                stepMotorX(motorX.dir, motorX.speed, motorX.mod);
                if(limitz() == 2){
                    limitX1 = 1;
                    m = j;
                } 
            }
            __delay_ms(1000);
            for(int k = 0; k<m; k++){
                motorX.dir = CCW;
                stepMotorX(motorX.dir, motorX.speed, motorX.mod);
            }
            motorX.dir = CW;
            status = 1;
        }
        
        if(limitz() == 2){
            for(int j = 0; limitX0 != 1; j++){
                motorX.dir = CW;
                stepMotorX(motorX.dir, motorX.speed, motorX.mod);
                if(limitz() == 1){
                    limitX0 = 1;
                    m = j;
                } 
            }
            motorX.dir = CCW;
            status = 1;
            __delay_ms(1000);
        }
    }
    return m;
}

int motorYInit(){
    int m = 0;
    int status = 0;
    for(int i = 0; status != 1; i++){
        motorY.dir = CCW;
        stepMotorY(motorY.dir, motorY.speed, motorY.mod);
           
        if(limitz() == 1){
            for(int j = 0; limitY1 != 1; j++){
                motorY.dir = CW;
                stepMotorY(motorY.dir, motorY.speed, motorY.mod);
                if(limitz() == 2){
                    limitY1 = 1;
                    m = j;
                } 
            }
            __delay_ms(1000);
            for(int k = 0; k<m; k++){
                motorY.dir = CCW;
                stepMotorY(motorY.dir, motorY.speed, motorY.mod);
            }
            motorY.dir = CW;
            status = 1;
        }
        
        if(limitz() == 2){
            for(int j = 0; limitY0 != 1; j++){
                motorY.dir = CW;
                stepMotorY(motorY.dir, motorY.speed, motorY.mod);
                if(limitz() == 1){
                    limitY0 = 1;
                    m = j;
                } 
            }
            motorY.dir = CCW;
            status = 1;
            __delay_ms(1000);
        }
    }
    return m;
}

int motorZInit(){
    int m = 0;
    int status = 0;
    for(int i = 0; status != 1; i++){
        motorZ.dir = CCW;
        
        stepMotorZ(motorZ.dir, motorZ.speed, motorZ.speed);
             
        if(limitz() == 1){
            
            for(int j = 0; limitZ1 != 1; j++){
                motorZ.dir = CW;
                stepMotorZ(motorZ.dir, motorZ.speed, motorZ.mod);
                if(limitz() == 2){
                    limitZ1 = 1;
                    m = j;
                } 
            }
            __delay_ms(1000);
            for(int k = 0; k<m; k++){
                motorZ.dir = CCW;
                stepMotorZ(motorZ.dir, motorZ.speed, motorZ.mod);
            }
            motorZ.dir = CW;
            status = 1;
            
        }
        
        if(limitz() == 2){
            for(int j = 0; limitZ0 != 1; j++){
                motorZ.dir = CW;
                stepMotorZ(motorZ.dir, motorZ.speed, motorZ.mod);
                if(limitz() == 1){
                    limitZ0 = 1;
                    m = j;
                } 
            }
            motorZ.dir = CCW;
            status = 1;
            __delay_ms(1000);
        }
    }
    return m;
}

void reset(){
//riporo all'origine i motori
    //reimposto la direzione default dei motori
    motorX.dir = defaultDirx;
    motorY.dir = defaultDiry;
    motorZ.dir = defaultDirz;
    
    //inverto la direzione dei motori per portare tutti all'origine
    if(defaultDirx == CCW) motorX.dir = CW;
    else motorX.dir = CCW;
    if(defaultDiry == CCW) motorY.dir = CW;
    else motorY.dir = CCW;
    if(defaultDirz == CCW) motorZ.dir = CW;
    else motorZ.dir = CCW;
    
    int controllo = 0;
    for(int i = 0; controllo != 1; i++){
        if(limitx()!= 1) stepMotorX(motorX.dir, motorX.speed, motorX.mod);
        if(limity()!= 1) stepMotorY(motorY.dir, motorY.speed, motorY.mod);
        if(limitz()!= 1) stepMotorZ(motorZ.dir, motorZ.speed, motorZ.mod);
        
        if(limitx() == 1 && limity() == 1 && limitz() == 1) controllo  = 1;
    }
    
//reset valori delle variabili
 MStepX = 0, MStepY = 0, MStepZ = 0;                                       
                                                                                                                                       
 limitX0 = 0,limitX1 = 0,limitY0 = 0,limitY1 = 0,limitZ0 = 0,limitZ1 = 0; 
                                                                              
 stepx = 0, stepy = 0, stepz = 0;                                          

 stepxTodo = 0, stepyTodo = 0, stepzTodo = 0;                             
 stepXprec = 0, stepYprec = 0, stepZprec = 0;                              
                                                                              
 defaultDirx = 0, defaultDiry = 0, defaultDirz = 0;                       

 end = 0;
 
}