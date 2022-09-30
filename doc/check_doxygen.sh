#!/bin/sh

if [  -f gigatraj_doxygen.log ] ; then
   if [ -s  gigatraj_doxygen.log ] ; then
      echo "Here are Doxygen's messages:"
      cat gigatraj_doxygen.log
      false   
   else
      echo "Doxygen seems to have run without any errors."
      true
   fi
else
   echo "We are not running doxyfgen here. that is OK."
   true
fi

