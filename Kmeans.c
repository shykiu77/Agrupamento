#include "header.h"

double Distancia(double d1,double d2){
	return (d1-d2)*(d1-d2);
}


Elemento *SelecionarCentroidesAleatoriamente(Elemento *dataset,Parametros *param){
	Elemento *centroides = MALLOC(Elemento,param->K);
	for(int i=0;i<param->K;i++)
	 centroides[i].atributos = MALLOC(double,param->Natributos);
	int Ncentroides = 0;
	int *escolhidos=CALLOC(int,param->Nelementos);
	while(Ncentroides!=param->K){
		int elemento = rand()%param->Nelementos;
		//int elemento = Ncentroides;
		if(escolhidos[elemento]==0){
			for(int i=0;i<param->Natributos;i++)
				centroides[Ncentroides].atributos[i]=dataset[elemento].atributos[i];
			escolhidos[elemento]=1;
			Ncentroides++;
		}
	}
	return centroides;
}


double DistribuirPontos(Elemento *dataset,Elemento *centroides,Parametros *param,int *NelementosEmCentroides){
	for(int i=0;i<param->K;i++)
		NelementosEmCentroides[i]=0;

	double somaMenoresDistancias=0;

	for(int i=0;i<param->Nelementos;i++){
	 double menorDistancia=INFINITY;
	 for(int j=0;j<param->K;j++){
		 double distancia=0;
		 for(int k=0;k<param->Natributos;k++){
		   distancia += Distancia(dataset[i].atributos[k],centroides[j].atributos[k]);
		 }
		 distancia = sqrt(distancia);
		 if(distancia < menorDistancia){
		   menorDistancia = distancia;
		   dataset[i].cluster = j;
		 }
	 }
	 NelementosEmCentroides[dataset[i].cluster]++;
	 somaMenoresDistancias += menorDistancia;
	}
	return somaMenoresDistancias;
}
void RecalcularCentroides(Elemento *dataset,Elemento *centroides,Parametros *param,int *NelementosEmCentroides){
	for(int i=0;i<param->K;i++)
	 if(NelementosEmCentroides[i]!=0)
		 for(int j=0;j<param->Natributos;j++)
		   centroides[i].atributos[j]=0;

	for(int i=0;i<param->Nelementos;i++)
	 for(int j=0;j<param->Natributos;j++)
		 centroides[dataset[i].cluster].atributos[j] += dataset[i].atributos[j];
	 

	for(int i=0;i<param->K;i++)
	 for(int j=0;j<param->Natributos;j++)
		 if(NelementosEmCentroides[i]!=0)
		   centroides[i].atributos[j] /= NelementosEmCentroides[i];
 
}
double Kmeans(Elemento *dataset,Parametros *param){
	Elemento *centroides = SelecionarCentroidesAleatoriamente(dataset,param);
	int iteracao=0;
	param->iteracaoMaxima=100;
	int *NelementosEmCentroides = MALLOC(int,param->K);
	double funcaoObjetivo[param->iteracaoMaxima+1];
	funcaoObjetivo[iteracao]=DistribuirPontos(dataset,centroides,param,NelementosEmCentroides);
	do{
		iteracao++;
		RecalcularCentroides(dataset,centroides,param,NelementosEmCentroides);
		funcaoObjetivo[iteracao] = DistribuirPontos(dataset,centroides,param,NelementosEmCentroides);
	}while(iteracao < param->iteracaoMaxima && funcaoObjetivo[iteracao] < funcaoObjetivo[iteracao-1]);

	
	if(iteracao == param->iteracaoMaxima)
		return funcaoObjetivo[iteracao];
	else
		return funcaoObjetivo[iteracao-1];
}



int main(int argc, char** argv){
	if(argc!=2){
		printf("uso: ./Kmenas K\n");
		return 1;	
	}
	srand(time(NULL));
	Parametros *param = MALLOC (Parametros,1);
	scanf("%d%d",&param->Nelementos,&param->Natributos);
	Elemento *dataset = MALLOC(Elemento,param->Nelementos);

	for(int i=0;i<param->Nelementos;i++)
		dataset[i].atributos = CALLOC(double,1);

	for(int i=0;i<param->Nelementos;i++){
		scanf("%d",&dataset[i].clusterVerdadeiro);
		for(int j=0;j<param->Natributos;j++)
			scanf("%lf",&dataset[i].atributos[j]);
	}
	
	param->K = atoi(argv[1]);
	Kmeans(dataset,param);
	
	for(int i=0;i<param->Nelementos;i++){
		printf("%d ",dataset[i].clusterVerdadeiro);
	}
	return 0;
	
}
