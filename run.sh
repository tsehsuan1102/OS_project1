

files=`ls ./data`
for i in $files;
do
	#echo $i
	sudo dmesg -c
	v=`echo $i | sed s/.txt//g`
	echo $v
	make run < ./data/$v.txt  1&> ./tmp_out/$v_stdout.txt
	#make run < ./data/$v.txt  
	dmesg -t > ./out/$v_dmesg.txt
done;





