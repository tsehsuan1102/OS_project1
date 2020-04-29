

files=`ls data`
for i in $files;
do
	echo $i
	sudo dmesg -c
	v=`echo $i | sed s/.txt//g`
	echo $v
	sudo ./main < ./data/$v.txt  2&> ./out/$v.out
	dmesg -t > ./out/$v.dout
done;





