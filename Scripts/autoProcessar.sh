#!/bin/bash
          #1                       2                                 3                    4                               5        6            7            8            9                          
# local dos binarios, pasta que contem os arquivos de entrada, numero de partições, local de saida dos algoritmos, true cluster, dataset,  Nelementos, Natributos, local de saida das metricas

#for clustering in $2*; do
 #   folderName=$4"$(echo $clustering | cut -d'/' -f5)"
  #  mkdir $folderName
   # ./Scripts/autoAlgoritmos.sh $1 $clustering $3 $folderName/result
    #rm $folderName/*CSPA
    #rm $folderName/*tempFile*
    #rm $folderName/*HGPA
#done

for result in $4"$(echo $clustering | cut -d'/' -f5)"*; do
    #echo $result/
    Nelementos=$(<$7)
    Natributos=$(<$8)
    ./Scripts/autoMetricas $result/ $Nelementos $5 $9/result $Natributos $6
    
done