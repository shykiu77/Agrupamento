#!/bin/bash


paste "$@" | grep "sillouet" | sed 's/sillouet: //g' | sed  's/\t/,/g'

echo ' '


