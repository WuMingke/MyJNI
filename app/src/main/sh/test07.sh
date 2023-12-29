#!bin/bash


# 执行 sh test07.sh 999

function test() {
    echo "脚本传参数：$1"
}

test $1  #实际是脚本配合内置传参