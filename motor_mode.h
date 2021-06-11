void modM1(int m){
    
    if(m == 1){
        PORTAbits.RA4 = 0;
        PORTAbits.RA3 = 0;
        PORTAbits.RA2 = 0;
    }
       
    if(m == 2){
        PORTAbits.RA4 = 1;
        PORTAbits.RA3 = 0;
        PORTAbits.RA2 = 0;
    }
    if(m == 3){
        PORTAbits.RA4 = 0;
        PORTAbits.RA3 = 1;
        PORTAbits.RA2 = 0;
    }
    if(m == 4){
        PORTAbits.RA4 = 1;
        PORTAbits.RA3 = 1;
        PORTAbits.RA2 = 0;
    }
    if(m == 5){
        PORTAbits.RA4 = 0;
        PORTAbits.RA3 = 0;
        PORTAbits.RA2 = 1;
    }
    if(m == 6){
        PORTAbits.RA4 = 1;
        PORTAbits.RA3 = 0;
        PORTAbits.RA2 = 1;
    }
    if(m == 7){
        PORTAbits.RA4 = 0;
        PORTAbits.RA3 = 1;
        PORTAbits.RA2 = 1;
    }
    if(m == 8){
        PORTAbits.RA4 = 1;
        PORTAbits.RA3 = 1;
        PORTAbits.RA2 = 1;
    }
    
}

void modM2(int m){
    
    if(m == 1){
        PORTCbits.RC0 = 0;
        PORTEbits.RE2 = 0;
        PORTEbits.RE1 = 0;
    }
       
    if(m == 2){
        PORTCbits.RC0 = 1;
        PORTEbits.RE2 = 0;
        PORTEbits.RE1 = 0;
    }
    if(m == 3){
        PORTCbits.RC0 = 0;
        PORTEbits.RE2 = 1;
        PORTEbits.RE1 = 0;
    }
    if(m == 4){
        PORTCbits.RC0 = 1;
        PORTEbits.RE2 = 1;
        PORTEbits.RE1 = 0;
    }
    if(m == 5){
        PORTCbits.RC0 = 0;
        PORTEbits.RE2 = 0;
        PORTEbits.RE1 = 1;
    }
    if(m == 6){
        PORTCbits.RC0 = 1;
        PORTEbits.RE2 = 0;
        PORTEbits.RE1 = 1;
    }
    if(m == 7){
        PORTCbits.RC0 = 0;
        PORTEbits.RE2 = 1;
        PORTEbits.RE1 = 1;
    }
    if(m == 8){
        PORTCbits.RC0 = 1;
        PORTEbits.RE2 = 1;
        PORTEbits.RE1 = 1;
    }
    
}



void modM3(int m){
    
    if(m == 1){
        PORTDbits.RD1 = 0;
        PORTDbits.RD0 = 0;
        PORTCbits.RC3 = 0;
    }
    
    if(m == 2){
        PORTDbits.RD1 = 1;
        PORTDbits.RD0 = 0;
        PORTCbits.RC3 = 0;
    }
    if(m == 3){
        PORTDbits.RD1 = 0;
        PORTDbits.RD0 = 1;
        PORTCbits.RC3 = 0;
    }
    if(m == 4){
        PORTDbits.RD1 = 1;
        PORTDbits.RD0 = 1;
        PORTCbits.RC3 = 0;
    }
    if(m == 5){
        PORTDbits.RD1 = 0;
        PORTDbits.RD0 = 0;
        PORTCbits.RC3 = 1;
    }
    if(m == 6){
        PORTDbits.RD1 = 1;
        PORTDbits.RD0 = 0;
        PORTCbits.RC3 = 1;
    }
    if(m == 7){
        PORTDbits.RD1 = 0;
        PORTDbits.RD0 = 1;
        PORTCbits.RC3 = 1;
    }
    if(m == 8){
        PORTDbits.RD1 = 1;
        PORTDbits.RD0 = 1;
        PORTCbits.RC3 = 1;
    }
    
}