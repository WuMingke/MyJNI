#!bin/bash
echo "success !!!"

# 定义变量
A=10
echo $A

#B=$?
#检测上一个命令是否成功
echo $?

#显示路径
echo pwd:$PWD

#接收参数  sh test01.sh AAA BBB
echo 当前Shell脚本的名称是:$0
echo 参数:$1
echo "参数："$2

echo "本次执行状态："
#A =
if [ $? -eq 0 ]; then
    echo "success"
else
    echo "failed"
fi

echo "外界传递的所有参数：$*"
echo "外界传递参数的个数：$#"