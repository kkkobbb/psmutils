#!/bin/sh
#
# to_mcb.sh <binfile>
#
# binfile: データ
#
# データ名を標準入力から受け取り、
# mcb形式のデータを標準出力に出力する
ALIGN=8192
HEAD_SIZE=20
HEAD_ZERO_SIZE=34

# print_zero n
# n個の"\0"を標準出力に出力する
print_zero(){
    zero_n=$1

    i=0
    while [ $i -lt $zero_n ]
    do
        printf "\0"
        i=$((i+1))
    done
}

# align_0x2000 <file>
# ファイルを0x2000(8192)byteでアラインメントする
# 標準出力へ出力する
align_0x2000(){
    file=$1

    filesize=$(wc -c $file | awk '{print $1}')
    len_padding=$(expr $ALIGN - $filesize % $ALIGN)
    cat $file
    print_zero $len_padding
}

# 標準入力を受け取る
# 20byte未満の場合、"\0"でパディング
# 20byteより多い場合、20byteまでのみ使用
# 次の34byteは0埋め
get_mcbhead(){
    head=$(cut -b -$HEAD_SIZE -)
    len_head=$(printf "$head" | wc -c)
    len_padding=$(expr $HEAD_SIZE - $len_head)
    printf $head
    print_zero $len_padding
    print_zero $HEAD_ZERO_SIZE
}

binfile=$1

get_mcbhead
align_0x2000 $binfile
