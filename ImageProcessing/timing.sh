#!/bin/zsh

sum=0.0
file=$2
file2=$3
if [ $# -lt 2 ]
then
	file="images/big.pgm"
	file2="images/big.pgm"
fi
rm -rf "results/$1.txt"
echo Performing $1 on $file
for parallel in {0..1}
do
	# decide which version of program to use
	if [ $parallel -eq 0 ]
	then
		app="./app_serial.exe"
	else 
		echo "----------------------" >> "results/$1.txt"
		app="./app.exe"
	fi
	
	# run the image filtering 10 times and append the results to the result file
	for i in {1..10}
	do
		if [ $# -lt 3 ]
		then
			res=`$app $1 $file output.pgm 2> /dev/null`
		elif [ $# -eq 3 ]
		then
			res=`$app  $file $1 $file2 output.pgm 2> /dev/null` 
		elif  [ $# -eq 4 ]
		then
			res=`$app $1 $2 $3 $4 output.pgm 2> /dev/null` 
		fi
		
		echo "$res" >> "results/$1.txt"
		
		if [ $i -eq 1 ]
		then 
			sum=$(echo $res | bc) 
		else 
			sum=$(echo "$sum + $res" | bc)
		fi
	done
	
	echo Done $parallel
	sum=$(echo "$sum * 0.1" | bc)
	echo "-------" >> "results/$1.txt"
	echo "0$sum" >> "results/$1.txt"

	cp output.pgm "filtered/$1_$parallel.pgm"

done
echo All done!