#!/bin/bash

awk -F '\t' '
{count[$2]++} 
{if(count[$2]>1)
  {print $6}}' ~/praktikum-modul-1-b24/resources/DataPKM.tsv