#!/bin/bash


> "results_$1.txt"
cd images
for file in *.pgm
do 
   if [ $# -eq 1 ]
   then
      echo $file
      cd ..
      sum=0.0
      
      for i in {1..10}
      do
         res=`./app.exe $1 images/$file 2> /dev/null` 

         #echo $res >> results.txt
         if [ $i -eq 1 ]
         then 
            sum=$(echo $res | bc) 
          else 
            sum=$(echo "$sum + $res" | bc)
         fi
        
      done
      sum=$(echo "$sum * 0.1" | bc)
      #echo "--" >> results.txt
      echo "0$sum" >> "results_$1.txt"
      #echo "--------------------" >> results.txt
      cp output.pgm "images/filtered/$file"
      cd images

   fi
done
