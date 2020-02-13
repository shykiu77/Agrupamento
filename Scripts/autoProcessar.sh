#!/bin/bash
          #1                       2                                 3                    4                               5        6            7            8            9                          
# local dos binarios, pasta que contem os arquivos de entrada, numero de partições, local de saida dos algoritmos, true cluster, dataset,  Nelementos, Natributos, local de saida das metricas

for clustering in $2*; do
    folderName=$4"$(echo $clustering | cut -d'/' -f5)"
    mkdir $folderName
    ./Scripts/autoAlgoritmos.sh $1 $clustering $3 $folderName/result
    rm $folderName/*CSPA
    rm $folderName/*tempFile*
    rm $folderName/*HGPA
    Natributos=$(<$8)
    Bin/metricasEnsemble $clustering $5 $6 $Natributos $9/"$(echo $clustering | cut -d'/' -f5)"
    
done

for result in $4"$(echo $clustering | cut -d'/' -f5)"*; do
    Nelementos=$(<$7)
    Natributos=$(<$8)
    fileName="$(echo $result | cut -d'/' -f5)"
 
    ./Scripts/autoMetricas.sh $result/ $Nelementos $5 $9/$fileName $Natributos $6
    
done

#exemplo: Scripts/autoProcessar.sh Bin/ Entradas/Segmentation/Rene/Homogeneidade/ 7 Saidas/Segmentation/Rene/Homogenidade/ Entradas/Segmentation/true_clustering Entradas/Segmentation/dataset Entradas/Segmentation/Nelementos Entradas/Segmentation/Natributos Resultados/teste