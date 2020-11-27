#!/bin/bash


#encore à re-tester
#lecture de l'image se fait automatiquement ./launcher.sh name_image


if [ -f "$1"] ; then
  cmake .
  echo "cmake abouti"
  make
  echo "compilation sans erreurs"
  ./Display_image $1
  echo "execution complète"
  FreeFem+ mesh.edp  
elif [ -n "$1" ] ; then
  echo "la valeur que vous avez entrer n'est pas valide"
else
  echo "ce fichier n'existe pas"
fi
echo "Vous avez choisi'limage à traiter: $1"
read name_image


