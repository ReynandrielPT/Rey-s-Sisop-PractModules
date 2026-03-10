#!/bin/bash

max=0
awk -F '\t' '
{count[$7]++} 
{if(count[$7] > max) 
  {
    max=count[$7]; 
    mval=$7;
  }
} END {print mval}' ~/praktikum-modul-1-b24/resources/DataPKM.tsv