#include "ensemble.h"



double recall(unsigned int *apriori,unsigned int *clustering,unsigned int Nelements,unsigned int i,unsigned int j){
    unsigned int nij=0;
	unsigned int ni=0;

	for(unsigned int k=0; k<Nelements; k++)
		if(apriori[k] == i && clustering[k] == j)
			nij++;
	
	for(unsigned int k=0; k<Nelements; k++)
			if(apriori[k]==i)
				ni++;

	if(ni == 0 )
		return 0;

	return (double) nij/ni;
}

double precision(unsigned int *apriori,unsigned int *clustering,unsigned int Nelements,unsigned int i,unsigned int j){
    unsigned int nij=0;
	unsigned int nj=0;

	for(unsigned int k=0; k<Nelements; k++)
		if(apriori[k] == i && clustering[k] == j)
			nij++;
	
	for(unsigned int k=0; k<Nelements; k++)
			if(clustering[k]==j)
				nj++;

	if(nj == 0 )
		return 0;	

	return (double) nij/nj;
}
double fmeasure(unsigned int *apriori,unsigned int *clustering,unsigned int Nelements,unsigned int i,unsigned int j){
    double p=precision(apriori, clustering, Nelements, i,j);
	double r=recall(apriori, clustering, Nelements, i ,j);
	
	if(p==0 || r==0)
		return 0;

	return (double) 2*(p*r)/(p+r);
}

double fmeasuret(unsigned int *apriori, unsigned int *clustering, unsigned int Nelements, unsigned int m, unsigned int k){

	double f=0;
	
	unsigned int *count= (unsigned int *) malloc(sizeof(int)*k+1);
	double *value = (double *) malloc (sizeof(double)*k+1);

	for(unsigned int i=1; i<=m; i++){
		for(unsigned int j=1; j<=k; j++)
			value[j] = (double) fmeasure(apriori, clustering, Nelements, i, j);
		

		double max = -1;

		for(unsigned int j=1; j<=k; j++)
			max = MAX(max,value[j]);
			

		for(unsigned int j=1; j<=k; j++)
			count[j]=0;
	
		for(unsigned int j=0; j<Nelements; j++){
			count[apriori[j]]++;
		}
		
		f+= (double) count[i]*max;		
	}
        free(value);
        free(count);

	return (double) f/Nelements;
}

double cr_index(unsigned int *apriori,unsigned int *clustering,unsigned int Nelements){
    double a = 0.0, b = 0.0, c = 0.0, d = 0.0, p, cr;
	for(unsigned int i = 0; i < Nelements-1; i++)
        for(unsigned int j = (i+1); j < Nelements; j++){
			if ((clustering[i] == clustering[j]) && (apriori[i] == apriori[j]))
				a += 1.0;
			if ((clustering[i] != clustering[j]) && (apriori[i] == apriori[j]))
				b += 1.0;
			if ((clustering[i] == clustering[j]) && (apriori[i] != apriori[j]))
				c += 1.0;
			if ((clustering[i] != clustering[j]) && (apriori[i] != apriori[j]))
				d += 1.0;
        }
    p = a + b + c + d;
    cr = ((a + d) - ((a + b) * (a + c) + (d + b) * (d + c)) * (1 / p)) / (p - ((a + b) * (a + c) + (d + b) * (d + c)) * (1 / p));
    return cr;
}


double jaccard_index(unsigned int *apriori, unsigned int *clustering, unsigned int Nelements){
	double a = 0.0, b = 0.0, c = 0.0, d = 0.0, p, j;
	 for(unsigned int i = 0; i < Nelements-1; i++)
        for(unsigned int j = (i+1); j < Nelements; j++){
			if((clustering[i] == clustering[j]) && (apriori[i] == apriori[j]))
                a += 1.0;
            if((clustering[i] != clustering[j]) && (apriori[i] == apriori[j]))
                b += 1.0;
            if((clustering[i] == clustering[j]) && (apriori[i]!= apriori[j]))
                c += 1.0;
            if((clustering[i] != clustering[j]) && (apriori[i]!= apriori[j]))
                d += 1.0;
        }	
    p = a + b + c;
    j = a /p;
    return j;
}

double purity_index(unsigned int *apriori, unsigned int *clustering, unsigned int Nelements,unsigned int m,unsigned int k) {
	unsigned int nij = 0;
	double purity = 0;
	double max=0;


	for(unsigned int i = 1; i <= m; i++){
        max=0;
		for(unsigned int j = 1; j <= k; j++){
            nij=0;
			for(unsigned int z = 0; z < Nelements; z++)
				if (apriori[z] == i && clustering[z] == j)
					nij++;
			if(nij>max)
				max = nij;
        }
		purity+=max;
	}

	purity = (double) purity / Nelements;

	return purity;
}

unsigned int *normalize_clusters(unsigned int *cluster,unsigned int Nelements){
	unsigned int *taken = CALLOC(unsigned int,NCLUSTERSMAX);
	unsigned int *newCluster = MALLOC(unsigned int,Nelements);

	unsigned int Nclusters = 0;

	for(unsigned int i=0;i<Nelements;i++){
		if( taken[cluster[i]] == 0 ){
			newCluster[i] = taken[cluster[i]] = ++Nclusters;
		}
		else{
			newCluster[i] = taken[cluster[i]];
		}
	}

	return newCluster;
}

//NOTA: Tanto os agrupamentos verdadeiros quanto os resultantes de um algoritmo devem começar de 0.
int main(int argc, char **argv){
    FILE *file1,*file2;
    file1 = fopen(argv[2],"r");
    file2 = fopen(argv[3],"r");
    if(!(file1 && file2)){
        printf("Não foi possível abrir um dos arquivos\n");
        return 1;
    }
    unsigned int Nelements = atoi(argv[1]);
    unsigned int *apriori = MALLOC(unsigned int,Nelements);
	unsigned int *clustering = MALLOC(unsigned int,Nelements);
    for(unsigned int i=0;i<Nelements;i++){
        fscanf(file1,"%d",&apriori[i]);
        fscanf(file2,"%d",&clustering[i]);
    }
    fclose(file1);
    fclose(file2);

	apriori = normalize_clusters(apriori,Nelements);
	clustering = normalize_clusters(clustering,Nelements);

	

	
    unsigned int NclustersApriori=0,NclustersClustering=0;
    for(unsigned int i=0;i<Nelements;i++){
        NclustersApriori = MAX(NclustersApriori,apriori[i]);
        NclustersClustering = MAX(NclustersClustering,clustering[i]);
    }

    
	double jaccard = jaccard_index(apriori,clustering,Nelements);
	double fmeasure_index = fmeasuret(apriori,clustering,Nelements,NclustersApriori,NclustersClustering);
	double purity = purity_index(apriori,clustering,Nelements,NclustersApriori,NclustersClustering);
	double cr = cr_index(apriori,clustering,Nelements);
	printf("jaccard: %lf\n",jaccard);
	printf("fmeasure: %lf\n",fmeasure_index);
	printf("purity: %lf\n",purity);
	printf("cr_index: %lf\n",cr);
    return 0;
}