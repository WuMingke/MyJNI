#!bin/bash

str1="aaa"
str2="ccc"

if [ $str1 == $str2 ]; then # 这里 = 和 == 是一样的
    echo "相同"
    else
      echo "不同"
fi





#重定向
cat 0 < file.txt
# 含义：
# 0 代表屏幕，
# 屏幕 < 内容
# cat 屏幕内容     （文件content）

echo 33 > file2.txt
# 含义：
# 将 33 输入到 file2.txt