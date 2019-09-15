#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

typedef struct Elemento{
	double *atributos;
	int cluster;
	int clusterVerdadeiro;
}Elemento;

typedef struct Parametros{
	int Natributos;
	int Nelementos;
	int K;
	int iteracaoMaxima;
}Parametros;

#define MALLOC(t,n) (t*)malloc(sizeof(t)*n)
#define CALLOC(t,n) (t*)calloc(sizeof(t),n)

double Distancia(double ,double );



Elemento *SelecionarCentroidesAleatoriamente(Elemento *,Parametros *);

double DistribuirPontos(Elemento *,Elemento *,Parametros *,int *);

void RecalcularCentroides(Elemento *,Elemento *,Parametros *,int *);

double Kmeans(Elemento *,Parametros *);


