
void coordinateMotori(const char *fname, int X[], int Y[], int Z[], int &totalElem){
	int  i=0;    
    char Xc[100], Yc[100], Zc[100];
    char line[100]; 
    
    FILE *file;  
    file = fopen(fname, "r");  
    
    while(fgets(line, sizeof line, file)!=NULL) {
         
        sscanf(line, "%s %s %s", &Xc, &Yc, &Zc);;

        X[i] = atoi(Xc);
        Y[i] = atoi(Yc);
        Z[i] = atoi(Zc);

        i++;
    }

    totalElem = i;
   
    fclose(file); 
}
