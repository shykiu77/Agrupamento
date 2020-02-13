#!/bin/bash

#args: local dos arquivos, numero de elementos, local do cluster verdadeiro, local da saida do script, numero de atributos e arquivo do dataset.
#Ex.: ./autoMetricas.sh Saidas/Segmentation/segmentation_5/ 2310 Entradas/Segmentation/true_clustering  Resultados/segmentation_5 19 Entradas/Segmentation/dataset

for filename in $1*; do
    echo $filename >> $4 
    Bin/metricasExternas $2 $3 $filename >> $4
    Bin/metricasInternas $2 $5 $6 $filename >> $4

    echo "" >> $4
done