#!/bin/bash

#args: local dos arquivos, numero de elementos, local do cluster verdadeiro, local da saida do script, numero de atributos e arquivo do dataset.
#Ex.: ./autoMetricas.sh Saidas/Subcorel2/Subcorel2_5/  400 Entradas/Subcorel2/true_clustering  Resultados/subcorel2_5 338 Entradas/Subcorel2/subcorel2_attributes

for filename in $1*; do
    echo $filename >> $4 
    Bin/metricasExternas $2 $3 $filename >> $4
    Bin/metricasInternas $2 $5 $6 $filename >> $4

    echo "" >> $4
done