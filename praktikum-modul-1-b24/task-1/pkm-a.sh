#!/bin/bash

awk -F '\t' '
{split($5, a,  " ")}
{nw += length(a)} 
{if(nw>20) 
  {
    gsub("_"," ",$2); 
    print $2;
  }
}{nw=0}' ~/praktikum-modul-1-b24/resources/DataPKM.tsv