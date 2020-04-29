
files=`ls ./data`
for i in $files;do
	echo $i
	sudo dmesg --clear
	v=`echo $i | sed s/.txt//g`
	echo $v

	make run < ./data/$v.txt  > ./out/$v"_stdout.txt"
	#make run < ./data/$v.txt > ./out/ams.txt
	dmesg -t > ./out/$v"_dmesg.txt"
done;





