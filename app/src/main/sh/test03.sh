#!bin/bash

a=0

for (( i = 0; i <= 100; i++ )); do
#    a=`expr $i + $a`
     ((a += i)) # 或者这样写，（()）中间的括号做计算
done

echo "累加结果："$a


#  find . -name "*.sh" 查找当前目录下的文件
#  tar czf all.zip *.sh 打包压缩文件
