#pragma once
class Data{
public:
	double *a;
	double nbAtt;
	int cluster;
	int cCluster;
	int *groupInfo;
public:
	Data(void);
	double euclidianDistance(double a1[]);
	double euclidianDistance3(double a1,double a2);
	double euclidianDistance2(double a1[], double a2[], int d);
	~Data(void);
};
