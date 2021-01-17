#!/bin/bash

#encore à re-tester
#lecture de l'image se fait automatiquement ./launcher.sh name_image
echo "__________________PROGRAMME DE DETECTION DE FIBRES ET MAILLAGE__________"
echo "___________________________________________________________________________"
echo "_____________________Gr TER MMK 2 ENSEIRB-MATMECA___________________________ "
echo "____________________________________________________________________________"
echo "_________________VERSION DE TEST PAR: A.DAKRI D.S.PONTES 2020/2021____________"
echo "______________________________________________________________________________"


if [ -r "$1" ];  then
  echo "________________________CONFIGURATION DES DEPENDANCES CMAKE_________________"
  echo "_____________________________________________________________________________"
  cmake .
  echo "_____________________________________CMAKE ABOUTI_____________________________"
  echo "_______________________________________________________________________________"
  make
  echo "____________________________COMPILATION SANS ERREURS___________________________"
  echo "_______________________________________________________________________________"
  ./DisplayImage $1
  echo "______________________________EXECUTION COMPLETE_______________________________"
  FreeFem++ mesh.edp
elif [ -n "$1" ];  then
  echo "la valeur que vous avez entrer n'est pas valide"
  exit 1
else
  echo "ce fichier n'existe pas"
  exit 1
fi
echo "Vous avez choisi'limage à traiter: $1"
read name_image
exit 0
