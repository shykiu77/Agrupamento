#include "ensemble.h"

#define INVALIDARG 0
#define INVALIDARGNUM 1

#define HELP 0
#define CSPA 1
#define AGGREGATION 2
#define HGPA 3
#define MCLA 4
#define STRCMP(a,b) strcmp(a,b) == 0

void PrintMoreInfo(){
    printf("The CSPA and HGPA algorithms require the number of clusters to be stated after the file path\n");
}   

void PrintArgs(){
    printf("-a, --Aggregation\tUses the Aggregation algorithm\n");
    printf("-c, --CSPA\t\tUses the CSPA algorithm\n");
    printf("-h, --HGPA\t\tUses the HGPA algorithm\n");
    printf("-m, --MCLA\t\tUses the MCLA algorithm\n");
    printf("\n");

    PrintMoreInfo();
}

void ShowHelp(){
    printf("Usage: ensemble [OPTIONS] [FILE] [OPTIONAL]\n");
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
    if(STRCMP(arg,"--help"))
        return HELP;
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
            if(argc < 4){
                Error(INVALIDARGNUM,NULL);
                return;
            }
            strcat(program,"CSPA ");
            strcat(program,argv[3]);
            break;
        case HGPA:
            if(argc < 4){
                Error(INVALIDARGNUM,NULL);
                return;
            }
            strcat(program,"HGPA ");
            strcat(program,argv[3]);
            break;
        case AGGREGATION:
            strcat(program,"Aggregation");
            break;
        case MCLA:
            if(argc < 4){
                Error(INVALIDARGNUM,NULL);
                return;
            }
            strcat(program,"MCLA ");
            strcat(program,argv[3]);
            break;
    }
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
    if(argc < 3){
        ShowHelp();
        return 1;
    }
    int algorithm;
    algorithm = ReadAlgorithm(argv[1]);
    if(algorithm == -1){
        return Error(INVALIDARG,argv[1]);
    }
    Run(algorithm,argc,argv);
    return 0;
}