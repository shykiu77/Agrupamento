#!/bin/bash

#args: local dos arquivos, numero de elementos, local do cluster verdadeiro, local da saida do script.
#Ex.: ./autoMetricasExternas.sh Saidas/Wine/ 178 Entradas/Wine/true_clusters_wine.txt Resultados/wineTeste

for filename in $1*; do
    echo $filename >> $4 
    Bin/metricas $2 $3 $filename >> $4
    echo "" >> $4
done
