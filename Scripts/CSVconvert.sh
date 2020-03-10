#!/bin/bash


paste "$@" | grep "Distancias_ao_centro" | sed 's/Distancias_ao_centro: //g' | sed  's/\t/,/g'

echo ' '


