mkdir -p theoretical-output

echo "------ TIME MEASUREMENT ------"
python3 theoretical.py < OS_PJ1_Test/TIME_MEASUREMENT.txt > theoretical-output/TIME_MEASUREMENT_stdout.txt

policies=("FIFO" "RR" "SJF" "PSJF")
# policies=("SJF" "PSJF")

for policy in ${policies[@]}; do
    for i in {1..5}; do
        tmp=$(printf "%-4s" $policy)
        echo "------ ${tmp} ${i} ------"
        python3 theoretical.py < OS_PJ1_Test/${policy}_$i.txt > theoretical-output/${policy}_${i}_stdout.txt
    done
done