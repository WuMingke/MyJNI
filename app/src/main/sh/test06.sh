#!bin/bash

function test01() {
    echo "调用了 test01 函数"
}

# 调用函数
test01


function test02() {
    str1="aaa"
    str2="bbb"
    echo $str1
    echo $str2
}

test02


# 函数传参 (内置传参)
function test03() {
    echo "参数是：$1"
}

test03 BBB


# 脚本传参 test07.sh
