#!/bin/bash

# local dos binarios, pasta que contem os arquivos de entrada, numero de partições, local de saida

for clustering in $2*; do
    folderName=$4"$(echo $clustering | cut -d'/' -f5)"
    mkdir $folderName
    ./Scripts/autoAlgoritmos.sh $1 $clustering $3 $folderName
done