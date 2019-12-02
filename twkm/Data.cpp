#include "Data.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

Data::Data(){
	
}

Data::~Data(){
}

double Data::euclidianDistance(double a1[]){

	double dist = 0.0;
	
	for(int i=0; i<nbAtt; i++){
		//printf("\n%f - %f", a[i],a1[i]);
		dist += pow(a[i] - a1[i], 2);
	}

	dist = sqrt(dist);
	
	return dist;
}

double Data::euclidianDistance3(double a1,double a2) {

	double dist = 0.0;

	dist = pow(a1 - a2, 2);
	
	//printf("\nDist: %f", dist);
	//dist = sqrt(dist);

	return dist;
}

double Data::euclidianDistance2(double a1[], double a2[], int d){

	double dist = 0.0;
	
	for(int i=0; i<d; i++)
		dist += pow(a2[i] - a1[i], 2);

	dist = sqrt(dist);
	return dist;
}