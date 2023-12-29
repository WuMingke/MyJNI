#!bin/bash

for i in {1..5} ; do
    echo $i
done

for i in `seq 6 10` ; do  # `` 数据源
    echo $i
done

for (( i = 11; i < 15; i++ )); do
    echo $i
done

i=0
while [ $i -lt 10 ]; do
    ((i=i+1))
done
echo $i