#include "ensemble.h"

typedef struct metricas{
    double jaccard;
    double fmeasure_index;
    double purity;
    double cr;
    double sillouet;
    double dist;
}Metricas;

void assign(Metricas *metricas,char *token,int linha){
    switch(linha){
        case 1:
            metricas->jaccard = atof(token);
        case 2:
            metricas->fmeasure_index = atof(token);
        case 3:
            metricas->purity = atof(token);
        case 4:
            metricas->cr = atof(token);
        case 5:
            metricas->sillouet = atof(token);
        case 6:
            metricas->dist = atof(token);
    }
}

int main(int argc, char**argv){
    if(argc != 6){
        printf("Usage: ./metricasEnsemble entrada true_clustering dataset Natributos saida ");
        return 1;
    }
    FILE *entrada = fopen(argv[1],"r");
   

    int Nelementos;
    int Nclusterings;
    fscanf(entrada,"%d %d",&Nelementos,&Nclusterings);

    int **ensemble = MALLOC(int*,Nclusterings);
    for(int i=0;i<Nclusterings;i++)
        ensemble[i] = MALLOC(int,Nelementos);
    
    for(int i=0;i<Nclusterings;i++)
        for(int j=0;j<Nelementos;j++)
            fscanf(entrada,"%d",&ensemble[i][j]);
    fclose(entrada);
    Metricas **metricas = MALLOC(Metricas*,Nclusterings);
    for(int i=0;i<Nclusterings;i++)
        metricas[i] = MALLOC(Metricas,1);

    for(int i=0;i<Nclusterings;i++){
        char filename[40];
        mkdir("./temp",0700);
        strcpy(filename,"./temp/clust");
        char num[10];
        sprintf(num,"%d",i);
        strcat(filename,num);
        FILE *temp = fopen(filename,"w");
        for(int j=0;j<Nelementos;j++)
            fprintf(temp,"%d ",ensemble[i][j]);
        fprintf(temp,"\n");
        fclose(temp);
    }
    if(fork() == 0){
        char charNelementos[10];
        sprintf(charNelementos,"%d",Nelementos);
        execl("/home/carlos/VScode Projects/Agrupamento/autoMetricas.sh","autoMetricas","./temp/",charNelementos,argv[2],"./temp/saida",argv[4],argv[3],(char*) NULL);
        printf("%s\n",strerror(errno));
        
    }
    else{
        wait(NULL);
        FILE *resultados = fopen("./temp/saida","r");
        for(int i=0;i<Nclusterings;i++){
            char **bloco = MALLOC(char*,8);
            size_t buf_size = 100;
            for(int i=0;i<8;i++){
                bloco[i] = MALLOC(char,buf_size);
                getline(&bloco[i],&buf_size,resultados);
            }
            for(int j=0;j<8;j++){
                if(j!=0 && j != 7){
                    char *token = strtok(bloco[j],": ");
                    token = strtok(NULL,":");
                    assign(metricas[i],token,j);
                }
            }
        }
        fclose(resultados);
    }
    Metricas media;
    media.cr=0;media.dist=0;media.fmeasure_index=0;media.jaccard=0;media.purity=0;media.sillouet=0;
    for(int i=0;i<Nclusterings;i++){
        media.cr += metricas[i]->cr;
        media.dist += metricas[i]->dist;
        media.fmeasure_index += metricas[i]->fmeasure_index;
        media.jaccard += metricas[i]->jaccard;
        media.purity += metricas[i]->purity;
        media.sillouet += metricas[i]->sillouet;
    }
    media.cr /= Nclusterings;
    media.dist /= Nclusterings;
    media.fmeasure_index /= Nclusterings;
    media.jaccard /= Nclusterings;
    media.purity /= Nclusterings;
    media.sillouet /= Nclusterings;

    FILE *saida = fopen(argv[5],"w");
    fprintf(saida,"jaccard: %lf\n",media.jaccard);
    fprintf(saida,"fmeasure: %lf\n",media.fmeasure_index);
    fprintf(saida,"purity: %lf\n",media.purity);
    fprintf(saida,"cr_index: %lf\n",media.cr);
    fprintf(saida,"sillouet: %lf\n",media.sillouet);
    fprintf(saida,"distancias_aos_centros: %lf\n",media.dist);
    fclose(saida);
    system("rm -rf ./temp");
    for(int i=0;i<Nclusterings;i++){
        printf("%lf\n",metricas[i]->jaccard);
        printf("%lf\n",metricas[i]->fmeasure_index);
        printf("%lf\n",metricas[i]->purity);
        printf("%lf\n",metricas[i]->cr);
        printf("%lf\n",metricas[i]->sillouet);
        printf("%lf\n",metricas[i]->dist);
        printf("----------------\n");
    }
    return 0;
}