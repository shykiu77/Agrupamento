#include "ensemble.h"


//args: n elementos, n atributos
int main(int argc,char **argv){
    if(argc!=3){
        printf("args: n elementos, n atributos\n");
        return 0;
    }
    int Nelementos = atoi(argv[1]),Natributos = atoi(argv[2]);
    double **dataset = MALLOC(double*,Nelementos);
    for(int i =0;i<Nelementos;i++)
        dataset[i] = MALLOC(double,Natributos);
    
    for(int i=0;i<Nelementos;i++){
        for(int j=0;j<Natributos;j++){
            scanf("%lf",&dataset[i][j]);
        }
    }
    double **normalizedDataset = MALLOC(double*,Nelementos);
    for(int i =0;i<Nelementos;i++)
        normalizedDataset[i] = MALLOC(double,Natributos);

    for(int i=0; i<Natributos;i++){
        double maximum = dataset[0][i];
        double minimum = dataset[0][i];
        for (int j=0;j<Nelementos;j++){
            if (dataset[j][i] > maximum)
                maximum = dataset[j][i];
            if(dataset[j][i] < minimum)
                minimum = dataset[j][i];
        }
        for (int j=0;j<Nelementos;j++){
            if(minimum != maximum)
                normalizedDataset[j][i] = ((dataset[j][i] - minimum)/(maximum - minimum));
            else
                normalizedDataset[j][i] = 0;
        }

            
            
    }
    for(int i=0;i<Nelementos;i++){
        for(int j=0;j<Natributos;j++)
            printf("%lf ",normalizedDataset[i][j]);
        printf("\n");
    }
    return 0;
}