#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TempsDeSimulation 5.0

float calculerValOndeEnUnPoint(int* ampli1, int* ampli2, int* freq1, int* freq2, float* temps, int* dephas);
void EntrerDonnees(int* ampli1, int* ampli2, int* freq1, int* freq2, float* temps, int* dephas);
FILE* ouvrirFichierTXT();
void fermerFichierTXT(FILE* pointeurFichier);


//----------------------------------------------------------------------------------------------------------


void EntrerDonnees(int* ampli1, int* ampli2, int* freq1, int* freq2, float* temps, int* dephas){

        printf("/nEntrer Valeur Amplitude A : ");
        scanf("%d", ampli1);

        printf("/nEntrer Valeur Amplitude B : ");
        scanf("%d", ampli2);

        printf("/nEntrer Valeur frequece f : ");
        scanf("%d", freq1);
        printf("/nEntrer Valeur frequence g : ");
        scanf("%d", freq2);

        // printf("/nEntrer Valeur temps t : ");
        // scanf("%d", temps);

        printf("/nEntrer Valeur déphasage phi1 : ");
        scanf("%d", dephas);

}


float calculerValOndeEnUnPoint(int* ampli1, int* ampli2, int* freq1, int* freq2, float* temps, int* dephas){

    return (*ampli1) * cos(2 * 3.14 * (*temps)) + (*ampli2) * cos((2 * 3.14 * (*temps)) + (*dephas));

}

FILE* ouvrirFichierTXT(char* dataLink){

    FILE *f;
    char ch; 

    f = fopen(dataLink, "w");
 
    if(f==NULL){
        printf("Erreur lors de l'ouverture d'un fichier");
        exit(1);
    }
 
    return f;
}

void fermerFichierTXT(FILE* pointeurFichier){

    fclose(pointeurFichier);

}


void Echantillonnage(FILE* data2, int* TauxEchantillonage, int* ampli1, int* ampli2, int* freq1, int* freq2, float* temps, int* dephas){


    FILE *data;
    char ch; 

    data = fopen("data1.txt", "w");
 
    if(data==NULL){
        printf("Erreur lors de l'ouverture d'un fichier");
        exit(1);
    }

    float fbuffer = 0;
    float* buffer = &fbuffer;
    float FreqEchant = *TauxEchantillonage;
    float interval = (TempsDeSimulation) / (*TauxEchantillonage);
    float* Pinterval = &interval;
    
    //printf("taux d'echantillonage = %d\n", *TauxEchantillonage);
    //printf("Interval = %f\n", interval);
    
    fprintf(data, "%f\n", *Pinterval);

    int i = 0;
    *temps = 0;
    
    while(*temps < TempsDeSimulation){

        //printf("T = %f\n", interval);
        *buffer = calculerValOndeEnUnPoint(ampli1, ampli2, freq1, freq2, temps, dephas);
        *temps = *temps + interval;
        i++;
        //printf("%.2f\n", *buffer);
        fprintf(data, "%f\n", *buffer);

    }

}


int CalculeTauxEchantillon(int* freq1, int* freq2){

    return 2 * 3.14 * (fmax(*(freq1), *(freq2)));

}


//----------------------------------------------------------------------------------------------------------

int main(){

    int ampli1 = 2; // A
    int ampli2 = 2; // B
    int freq1 = 3; //f
    int freq2 = 7; // g
    float temps = 0; // t
    int dephas = 0; // phi
    int TauxEchantillonage = 1;

    int* Pampli1 = &ampli1; // A
    int* Pampli2 = &ampli2; // B
    int* Pfreq1 = &freq1; //f
    int* Pfreq2 = &freq2; // g
    float* Ptemps = &temps; // t
    int* Pdephas = &dephas; // phi
    int* PTauxEchantillonage = &TauxEchantillonage;
    

    char* dataLink1 =  "dataEchantillon.txt";


    FILE* pFichierEchant;
    pFichierEchant = ouvrirFichierTXT(dataLink1);

    TauxEchantillonage = CalculeTauxEchantillon(Pfreq1, Pfreq2);

    Echantillonnage(pFichierEchant, PTauxEchantillonage, Pampli1, Pampli2, Pfreq1, Pfreq2, Ptemps, Pdephas);

    fermerFichierTXT(pFichierEchant);
    //printf("%.2lf", calculerValOndeEnUnPoint(Pampli1, Pampli2, Pfreq1, Pfreq2, Ptemps, Pdephas));

    
    
    //printf("%d\n",*(PTauxEchantillonage));
    //printf("%lf", cos(0.5));
    return 0;
}