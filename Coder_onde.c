#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TempsDeSimulation 5.0
#define MaxTab 250
#define Pi 3.14

//----------------------------------------------------------------------------------------------------------

float calculerValOndeEnUnPoint(int* ampli1, int* ampli2, int* freq1, int* freq2, float* temps, int* dephas);
int CalculeTauxEchantillon(int* freq1, int* freq2);

void SaisieDonnees(int* ampli1, int* ampli2, int* freq1, int* freq2, float* temps, int* dephas);

void GenererSignalOriginal(int* ampli1, int* ampli2, int* freq1, int* freq2, int* dephas);
void Echantillonnage(int* TauxEchantillonage, int* ampli1, int* ampli2, int* freq1, int* freq2, float* temps, int* dephas);
void Quantification();

void AffichageFonctionOrigine();
void AffichageEchantillonage();
void AffichageComplet();
void AffichageavantApres();

void ConversionDataBinaire();
void ConversionBinaireData();


//----------------------------------------------------------------------------------------------------------


void SaisieDonnees(int* ampli1, int* ampli2, int* freq1, int* freq2, float* temps, int* dephas){

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

    return (*ampli1) * cos(2 * Pi * (*freq1) * (*temps)) + (*ampli2) * cos((2 * Pi * (*freq2) * (*temps)) + (*dephas));


}


int CalculeTauxEchantillon(int* freq1, int* freq2){

    return 2 * Pi * (fmax(*(freq1), *(freq2)));

}


void GenererSignalOriginal(int* ampli1, int* ampli2, int* freq1, int* freq2, int* dephas){

    FILE *original = fopen("data_original.txt", "w");

    if(original == NULL){

        printf("Erreur lors de l'ouverture du fichier original\n");
        exit(-1);

    }

    float t = 0.0;
    float step = 0.001; 

    // Calcul de chaque points avec l'interval de 0.001s pour la précision
    while(t <= TempsDeSimulation){

        float val = (*ampli1) * cos(2 * Pi * (*freq1) * t) + (*ampli2) * cos(2 * Pi * (*freq2) * t + (*dephas));
        fprintf(original, "%f %f\n", t, val);
        t += step;
        
    }

    fclose(original);
}


void Echantillonnage(int* TauxEchantillonage, int* ampli1, int* ampli2, int* freq1, int* freq2, float* temps, int* dephas){

    // Ouverture fichier txt
    FILE *data;
    data = fopen("data_echantillon.txt", "w");

    if(data == NULL){

        printf("Erreur lors de l'ouverture d'un fichier");
        exit(1);

    }

    float fbuffer = 0;
    float* buffer = &fbuffer;
    float interval = (1.0 / (*TauxEchantillonage));
    float* Pinterval = &interval;

    fprintf(data, "%f\n", *Pinterval); 

    *temps = 0;
    // calcul des points à interval fixe
    while(*temps < TempsDeSimulation){

        *buffer = calculerValOndeEnUnPoint(ampli1, ampli2, freq1, freq2, temps, dephas);
        fprintf(data, "%f %f\n", *temps, *buffer); // (temps, valeur)
        *temps += interval;

    }

    fclose(data);
}


void Quantification(){


    // Ouverture des fichiers txt
    FILE *echantillon;
    FILE *quantification;

    echantillon = fopen("data_echantillon.txt", "r");
    quantification = fopen("data_Quantification.txt", "w");

    if(echantillon == NULL || quantification == NULL){
        printf("Erreur lors de l'ouverture d'un fichier\n");
        exit(1);
    }

    // Tableau pour enregistrer les valeurs de l'échantillon et traiter
    float values[MaxTab];
    float times[MaxTab];
    int count = 0;

    float interval;
    float* Pinterval = &interval;

    float meilleur_interval = 0.0;
    float erreur_min = -1.0;


    // Lexture des datas de l'échantillon
    fscanf(echantillon, "%f", Pinterval); 

    while (fscanf(echantillon, "%f %f", &times[count], &values[count]) == 2){

        count++;
        if (count >= MaxTab){

            printf("Tableau Trop petit\n");
            break;
        }

    }

    // Recherche de l’intervalle optimal
    for (int i = 1; i <= 100; i++){

        float essai = i * 0.01;
        float somme_erreur = 0.0;

        for (int j = 0; j < count; j++){

            float q = roundf(values[j] / essai) * essai;
            float diff = values[j] - q;
            somme_erreur += diff * diff;

        }

        float rmse = sqrtf(somme_erreur / count);

        if (erreur_min < 0 || rmse < erreur_min){

            erreur_min = rmse;
            meilleur_interval = essai;

        }

    }

    // On applique la quantification optimale sur les données
    for (int i = 0; i < count; i++){

        float final = roundf(values[i] / meilleur_interval) * meilleur_interval;
        fprintf(quantification, "%f %f\n", times[i], final); 

    }

    fclose(echantillon);
    fclose(quantification);

}


void ConversionDataBinaire(){

    // Comme d'habitude, ouverture des fichiers
    FILE *quantifiaction = fopen("data_Quantification.txt", "r");
    FILE *data_bianire = fopen("data_binaire.txt", "w");

    if (quantifiaction == NULL || data_bianire == NULL){

        printf("Erreur lors de l'ouverture des fichiers pour la conversion binaire\n");
        exit(1);
    }

    

}


void ConversionBinaireData(){
    
}


void AffichageFonctionOrigine(){

    FILE* gnuplot = popen("gnuplot -persistent", "w");

    if (gnuplot){

        fprintf(gnuplot, "set title 'Signal : original'\n");
        fprintf(gnuplot, "set xlabel 'Temps (s)'\n");
        fprintf(gnuplot, "set ylabel 'Amplitude'\n");
        fprintf(gnuplot, "set grid\n");
        fprintf(gnuplot, "plot 'data_original.txt' using 1:2 with lines title 'Original' lt rgb 'blue', \\\n");
        pclose(gnuplot);

    } 
    else{

        printf("Erreur lors de l'ouverture de Gnuplot\n");
    }

}


void AffichageEchantillonage(){

    FILE* gnuplot = popen("gnuplot -persistent", "w");

    if (gnuplot){

        fprintf(gnuplot, "set title 'Signal : original, échantillonné'\n");
        fprintf(gnuplot, "set xlabel 'Temps (s)'\n");
        fprintf(gnuplot, "set ylabel 'Amplitude'\n");
        fprintf(gnuplot, "set grid\n");
        fprintf(gnuplot, "plot 'data_original.txt' using 1:2 with lines title 'Original' lt rgb 'blue', \\\n");
        fprintf(gnuplot, "     'data_echantillon.txt' using 1:2 with linespoints title 'Échantillonné' lt rgb 'green', \\\n");
        pclose(gnuplot);

    } 
    else{

        printf("Erreur lors de l'ouverture de Gnuplot\n");
    }

}


void AffichageComplet(){

    FILE* gnuplot = popen("gnuplot -persistent", "w");

    if (gnuplot){

        fprintf(gnuplot, "set title 'Signal : original, échantillonné, quantifié'\n");
        fprintf(gnuplot, "set xlabel 'Temps (s)'\n");
        fprintf(gnuplot, "set ylabel 'Amplitude'\n");
        fprintf(gnuplot, "set grid\n");
        fprintf(gnuplot, "plot 'data_original.txt' using 1:2 with lines title 'Original' lt rgb 'blue', \\\n");
        fprintf(gnuplot, "     'data_echantillon.txt' using 1:2 with linespoints title 'Échantillonné' lt rgb 'green', \\\n");
        fprintf(gnuplot, "     'data_Quantification.txt' using 1:2 with linespoints title 'Quantifié' lt rgb 'red'\n");
        pclose(gnuplot);

    } 
    else{

        printf("Erreur lors de l'ouverture de Gnuplot\n");
    }

}


void AffichageavantApres(){

    FILE* gnuplot = popen("gnuplot -persistent", "w");

    if (gnuplot){

        fprintf(gnuplot, "set title 'Signal : original, échantillonné, quantifié'\n");
        fprintf(gnuplot, "set xlabel 'Temps (s)'\n");
        fprintf(gnuplot, "set ylabel 'Amplitude'\n");
        fprintf(gnuplot, "set grid\n");
        fprintf(gnuplot, "plot 'data_original.txt' using 1:2 with lines title 'Original' lt rgb 'blue', \\\n");
        fprintf(gnuplot, "     'data_Quantification.txt' using 1:2 with linespoints title 'Quantifié' lt rgb 'red'\n");
        pclose(gnuplot);

    } 
    else{

        printf("Erreur lors de l'ouverture de Gnuplot\n");
    }

}



//----------------------------------------------------------------------------------------------------------

int main(){

    int ampli1 = 2; // A
    int ampli2 = 2; // B
    int freq1 = 2; //f
    int freq2 = 1; // g
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



    TauxEchantillonage = CalculeTauxEchantillon(Pfreq1, Pfreq2);

    Echantillonnage(PTauxEchantillonage, Pampli1, Pampli2, Pfreq1, Pfreq2, Ptemps, Pdephas);

    GenererSignalOriginal(Pampli1, Pampli2, Pfreq1, Pfreq2, Pdephas);

    Quantification();

    affichageavantApres();
    //printf("%.2lf", calculerValOndeEnUnPoint(Pampli1, Pampli2, Pfreq1, Pfreq2, Ptemps, Pdephas));

    
    
    //printf("%d\n",*(PTauxEchantillonage));
    //printf("%lf", cos(0.5));
    return 0;
}