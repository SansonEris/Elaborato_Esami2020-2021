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
#define STEPM1 PORTDbits.RD0
#define DIRM1 PORTDbits.RD1
//motore 2
#define STEPM2 PORTDbits.RD2
#define DIRM2 PORTDbits.RD3
//motore 3
#define STEPM3 PORTDbits.RD4
#define DIRM3 PORTDbits.RD5

//fine corsa
#define Xlimit0 PORTCbits.RC0
#define Xlimit1 PORTCbits.RC1
#define Ylimit0 PORTCbits.RC2
#define Ylimit1 PORTCbits.RC3
#define Zlimit0 PORTCbits.RC4
#define Zlimit1 PORTCbits.RC5

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

#define fullDrive 0x00
#define halfDrive 0x01
#define halfhalfDrive 0x02
#define micro8 0x03
#define micro16 0x04
#define micro32_1 0x05
#define micro32_2 0x06
#define micro32_3 0x07


char steps = (char)(360 / deg);

char speed;

void ms_delay(unsigned char val){
    unsigned char i;
    for (i = 0; i < val; i++)
        __delay_ms(2);
}

char conteggioGiri(char nGiri) {
    return nGiri * steps;
}



//il motore gira in modalit� full_drive per n giri
void full_drive(char direction, char speed, char giri) {
    char rotazioni = conteggioGiri(giri);
    PORTB = fullDrive;
    for (char i = 0; i <= rotazioni; i++) {
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
}

//il motore gira in modalit� half drive per n giri
void half_drive(char direction, char speed, char giri) {
    char rotazioni = conteggioGiri(giri);
    PORTB = halfDrive;
    for (char i = 0; i <= (rotazioni * 2); i++) {
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
}

//il motore gira in modalit� micro step 8 per n giri
void microstep8(char direction, char speed, char giri) {
    char rotazioni = conteggioGiri(giri);
    PORTB = micro8;
    for (char i = 0; i <= (rotazioni * 8); i++) {
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
}


//selezione modalit� e controllo motore singolo
void motore(char direction, char speed, char giri, char mode) {
    if (mode == 1) {
        PORTB = fullDrive;
        full_drive(direction, speed, giri);
    }
    if (mode == 2) {
        PORTB = halfDrive;
        half_drive(direction, speed, giri);
    }
    if(mode == 3){
        PORTB = micro8;
        microstep8(direction, speed, giri);
    }

}


/************************************************************TEST 3MOTORI CONTEMPORANEAMENTE************************************************************/

struct stepMotor {
    char dir;
    char speed;
    char mod; //passare un numero da 1 - 8 per la modalit� da scegliere
    char stepsToDo; //x | y | z valori convertiti in steps (deve essere convertito in out dal server php in base al valore cartesiano)
};


//converto un valore da 1-8 nella modalit� da assegare alle porte
char modConversionFormNumber(char a) {
    if (a != 0) a -= 1;
    char modeToNumber[8] = { fullDrive,halfDrive,halfhalfDrive,micro8,micro16,micro32_1,micro32_2,micro32_3 };
    return modeToNumber[a];
}

//converto un valore da 1-8 nel moltiplicatore di step necessari per quella modalit�
char modConversionFormNumberToMultiplier(char a) {
    if (a != 0) a -= 1;
    char val[8] = { 1,2,4,8,16,32,32,32 };
    return val[a];
}


//uno step unico, per la gestione in contemporanea di 3 motori
void stepMotor(char dir, char indStep, char indDir, char speed) { //devo passare l'indirizzo che deve essere portato a 1 per l'impulso
    if (dir == CCW) {
        indStep = 1;
        indDir = 1;
        ms_delay(speed);
        indStep = 0;
        indDir = 1;
        ms_delay(speed);
    }
    else if(dir == CW){
        indStep = 1;
        indDir = 0;
        ms_delay(speed);
        indStep = 0;
        indDir = 0;
        ms_delay(speed);
    }
}

char limitDetection(){
    if(Xlimit1||Xlimit0||Ylimit0||Ylimit1||Zlimit0||Zlimit1) return 1
}


void simultaneusStepperMotor(struct stepMotor m1, struct stepMotor m2, struct stepMotor m3) {
    //converto il valore da 1-8 in valore da assegnare alle porte di uscita
    char mod1 = modConversionFormNumber(m1.mod);
    char mod2 = modConversionFormNumber(m2.mod);
    char mod3 = modConversionFormNumber(m3.mod);

    //esempio gestione 3 modalitá di pilotaggio motore con PORTB e A
    PORTB = mod1;
    PORTB = mod2 << 3;
    PORTA = mod3;

    //moltiplicatocanvasMatrix[mouse.y][mouse.x] = 1;re per il numero di step da eseguire
    char n1 = modConversionFormNumberToMultiplier(m1.mod);
    char n2 = modConversionFormNumberToMultiplier(m2.mod);
    char n3 = modConversionFormNumberToMultiplier(m3.mod);

    //calcolo step totali con i moltiplicatori
    char step1 = m1.stepsToDo * n1;
    char step2 = m2.stepsToDo * n2;
    char step3 = m3.stepsToDo * n3;

    //verifico quanti step massimi possono essere fatti
    char largest = step1 > step2 ? (step1 > step3 ? step1 : step3) : (step2 > step3 ? step2 : step3);

    //eseguo tanti giri quanti il numero di step massimi
    for (char i = 0; i < largest && !limitDetection(); i++) {
        //controllo se il motore ha gia fatto il suo numero di step o se deve ancora farne
        if (step1 <= i) {
            stepMotor(m1.dir, STEPM1, DIRM1, m1.speed);
        }
        if (step2 <= i) {
            stepMotor(m2.dir, STEPM2, DIRM2, m2.speed);
        }
        if (step3 <= i) {
            stepMotor(m3.dir, STEPM3, DIRM3, m3.speed);
        }
    }
}


//conto il numero di step che compongono un asse
char motorX_init(struct stepMotor m){
    char conteggioStep = 0;
    char check = 0;
    PORTB = 0x00;

    while(!check){
        //continuo a fare step fin che non trovo un finecorsa
        stepMotor(CW, STEPM1, DIRM1, m.speed);
        if(Xlimit0 == 1){
            __delay_ms(100);
            //conto gli step arrivando all'altro finecorsa
            while(!Xlimit1 == 1){
                stepMotor(CCW, STEPM1, DIRM1, m.speed);
                conteggioStep++;
            }
            //porto il motore nella posizione 0
            while(!Xlimit0 == 1) stepMotor(CW, STEPM1, DIRM1, m.speed);
            check = 1;
        }
        if(Xlimit1 == 1){
            __delay_ms(100);
            //riporto il motore in posizione 0 contando gli step
            while(!Xlimit0 == 1){
                stepMotor(CCW, STEPM1, DIRM1, m.speed);
                conteggioStep++;
            }
            check = 1;
        }
    }
    return conteggioStep;
}

char motorY_init(struct stepMotor m){
    char conteggioStep = 0;
    char check = 0;
    PORTB = 0x00;

    while(!check){
        //continuo a fare step fin che non trovo un finecorsa
        stepMotor(CW, STEPM2, DIRM2, m.speed);
        if(Ylimit0 == 1){
            __delay_ms(100);
            //conto gli step arrivando all'altro finecorsa
            while(!Ylimit1 == 1){
                stepMotor(CCW, STEPM2, DIRM2, m.speed);
                conteggioStep++;
            }
            //porto il motore nella posizione 0
            while(!Ylimit0 == 1) stepMotor(CW, STEPM2, DIRM2, m.speed);
            check = 1;
        }
        if(Ylimit1 == 1){
            __delay_ms(100);
            //riporto il motore in posizione 0 contando gli step
            while(!Ylimit0 == 1){
                stepMotor(CCW, STEPM2, DIRM2, m.speed);
                conteggioStep++;
            }
            check = 1;
        }
    }
    return conteggioStep;
}

char motorZ_init(struct stepMotor m){
    char conteggioStep = 0;
    char check = 0;
    PORTA = 0x00;

    while(!check){
        //continuo a fare step fin che non trovo un finecorsa
        stepMotor(CW, STEPM3, DIRM3, m.speed);
        if(Zlimit0 == 1){
            __delay_ms(100);
            //conto gli step arrivando all'altro finecorsa
            while(!Zlimit1 == 1){
                stepMotor(CCW, STEPM3, DIRM3, m.speed);
                conteggioStep++;
            }
            //porto il motore nella posizione 0
            while(!Zlimit0 == 1) stepMotor(CW, STEPM3, DIRM3, m.speed);
            check = 1;
        }
        if(Zlimit1 == 1){
            __delay_ms(100);
            //riporto il motore in posizione 0 contando gli step
            while(!Zlimit0 == 1){
                stepMotor(CCW, STEPM3, DIRM3, m.speed);
                conteggioStep++;
            }
            check = 1;
        }
    }
    return conteggioStep;
}

