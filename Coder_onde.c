#include <stdio.h>
#include <stdlib.h>
#include <math.h>

float calculerValOndeEnUnPoint(int* ampli1, int* ampli2, int* freq1, int* freq2, int* temps, int* dephas);
void EntrerDonnees(int* ampli1, int* ampli2, int* freq1, int* freq2, int* temps, int* dephas);


//----------------------------------------------------------------------------------------------------------


void EntrerDonnees(int* ampli1, int* ampli2, int* freq1, int* freq2, int* temps, int* dephas){

        printf("/nEntrer Valeur Amplitude A : ");
        scanf("%d", ampli1);

        printf("/nEntrer Valeur Amplitude B : ");
        scanf("%d", ampli2);

        printf("/nEntrer Valeur frequece f : ");
        scanf("%d", freq1);
        printf("/nEntrer Valeur frequence g : ");
        scanf("%d", freq2);

        printf("/nEntrer Valeur temps t : ");
        scanf("%d", temps);

        printf("/nEntrer Valeur déphasage phi1 : ");
        scanf("%d", dephas);

}


float calculerValOndeEnUnPoint(int* ampli1, int* ampli2, int* freq1, int* freq2, int* temps, int* dephas){

    return (*ampli1) * cos(2 * 3.14 * (*temps)) + (*ampli2) * cos((2 * 3.14 * (*temps)) + (*dephas));

}

FILE* ouvrirFichierTXT(){

    FILE *f;
    char ch; 

    f = fopen("data.txt", "w");
 
    if(f==NULL){
        printf("Erreur lors de l'ouverture d'un fichier");
        exit(1);
    }
 
    return f;
}

void fermerFichierTXT(FILE* pointeurFichier){

    fclose(pointeurFichier);

}


int main(){

    int ampli1 = 0; // A
    int ampli2 = 0; // B
    int freq1 = 0; //f
    int freq2 = 0; // g
    int temps = 0; // t
    int dephas = 0; // phi

    int* Pampli1 = &ampli1; // A
    int* Pampli2 = &ampli2; // B
    int* Pfreq1 = &freq1; //f
    int* Pfreq2 = &freq2; // g
    int* Ptemps = &temps; // t
    int* Pdephas = &dephas; // phi
    
    char* dataLink =  "data.txt";

    FILE* pFichier;
    pFichier = ouvrirFichierTXT();

    //printf("%lf", cos(0.5));

    fermerFichierTXT(pFichier);
    printf("%.2lf", calculerValOndeEnUnPoint(Pampli1, Pampli2, Pfreq1, Pfreq2, Ptemps, Pdephas));

    return 0;
}