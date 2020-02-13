#!/bin/bash

#args: local dos binários, arquivo de entrada, numero de partições, local da saida do script.

for program in $1*; do
    var=${program#"Bin/"}
    case ${var:0:1} in
    [[:upper:]])
        cat $2 | $program $3 $4
        echo $var
        ;;
    esac

done

exit 0