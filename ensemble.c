#include "ensemble.h"

#define INVALIDARG 0
#define INVALIDARGNUM 1

#define HELP 0
#define CSPA 1
#define AGGREGATION 2
#define HGPA 3
#define MCLA 4
#define LWEA 5
#define IVC 6
#define ALL 7
#define STRCMP(a,b) strcmp(a,b) == 0

void PrintMoreInfo(){
    
}   

void PrintArgs(){
    printf("-a, --Aggregation\tUses the Aggregation algorithm\n");
    printf("-c, --CSPA\t\tUses the CSPA algorithm\n");
    printf("-h, --HGPA\t\tUses the HGPA algorithm\n");
    printf("-m, --MCLA\t\tUses the MCLA algorithm\n");
    printf("-l, --LWEA\t\tUses the LWEA algorithm\n");
    printf("-i, --IVC\t\tUses the IVC algorithm\n");

    printf("-ALL\t\tUses all algorithms.\n");
    printf("\n");

    PrintMoreInfo();
}

void ShowHelp(){
    printf("Usage: ensemble [OPTIONS] [INPUT FILE] [NUMBER OF PARTITIONS] [OUTPUT FILE]\n");
    printf("Uses an clustering ensemble algorithm to cluster elements of the input file\n\n");
    printf("Arguments: \n");

    PrintArgs();
}

int ReadAlgorithm(char *arg){
    if(STRCMP(arg,"-a") || STRCMP(arg,"--Aggregation"))
        return AGGREGATION;
    if(STRCMP(arg,"-c") || STRCMP(arg,"--CSPA"))
        return CSPA;
    if(STRCMP(arg,"-h") || STRCMP(arg,"--HGPA"))
        return HGPA;
    if(STRCMP(arg,"-m") || STRCMP(arg,"--MCLA"))
        return MCLA;
    if(STRCMP(arg,"-l") || STRCMP(arg,"--LWEA"))
        return LWEA;
    if(STRCMP(arg,"-i") || STRCMP(arg,"--IVC"))
        return IVC;
    if(STRCMP(arg,"--help"))
        return HELP;
    if(STRCMP(arg,"-ALL"))
        return ALL;
    return -1;
}

int Error(int errorCode,char *arg){
    switch(errorCode){
        case INVALIDARG:
            printf("ensemble: invalid option %s\n",arg);
            break;
        case INVALIDARGNUM:
            printf("ensemble: insufficient number of arguments\n");
            break;
    }
    return -1;
}

void Run(int algorithm,int argc,char **argv){
    FILE *fpAlg;
    char program[30];

    strcpy(program,"Bin/");
    switch(algorithm){
        case HELP:
            ShowHelp();
            return;
            break;
        case CSPA:
            strcat(program,"CSPA ");
            break;
        case HGPA:
            strcat(program,"HGPA ");
            break;
        case AGGREGATION:
            strcat(program,"Aggregation ");
            break;
        case MCLA:
            strcat(program,"MCLA ");
            break;
        case LWEA:
            strcat(program,"LWEA ");
            break;
         case IVC:
            strcat(program,"IVC ");
            break;
    }
    strcat(program,argv[3]);
    strcat(program," ");
    strcat(program,argv[4]);
    
  
    fpAlg = popen(program,"w");
    int c;
    FILE *fpFile;
    fpFile = fopen(argv[2],"r");

    if(fpFile){
        while ((c = getc(fpFile)) != EOF)
            fputc(c,fpAlg);
        fclose(fpFile);
    }

    pclose(fpAlg);
}

int main(int argc, char **argv){
    if(argc < 4){
        ShowHelp();
        return 1;
    }
    int algorithm;
    algorithm = ReadAlgorithm(argv[1]);
    if(algorithm == -1){
        return Error(INVALIDARG,argv[1]);
    }
    if(algorithm != ALL)
        Run(algorithm,argc,argv);
    else
        for(int i=1;i<ALL;i++)
            Run(i,argc,argv);
    
    return 0;
}