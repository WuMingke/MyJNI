#!/bin/bash

#查找当前目录下，所有的.sh文件，进行打包操作
# `` 源数据
#for i in `find . -name "*.sh"` ; do
#    tar -czf all.zip $i
#done
#有问题，每次循环都覆盖

tar -czf all.zip $(find . -name "*.sh")



# 读取文件内容并打印
# 文件最后一个符号必须以 回车 结束，才能正常显示
file_path="`pwd`/file.txt"

# 检查文件是否存在
if [ -e "$file_path" ]; then
    echo "文件存在：$file_path"

    # 读取文件内容并打印
    while IFS= read -r lineVarAA; do
        echo "$lineVarAA"
    done < "$file_path"

else
    echo "文件不存在：$file_path"
fi
