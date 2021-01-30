#!/bin/zsh

rm results_$1.txt
touch results_$1.txt

for i; do :; done
file=$2

echo $file

sum=0.0

for i in {1..10}
do
	if [ $# -eq 3 ]
	then
		res=`./app.exe  $2 $1 $3 output.pgm 2> /dev/null` 
	else
		res=`./app.exe $1 $file output.pgm 2> /dev/null` 
	fi
	# res=`./app.exe $1 $2 $3 $4 output.pgm 2> /dev/null` 
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
cp output.pgm "filtered/${file%.pgm}_$1.pgm"
cd images
