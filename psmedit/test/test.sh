#!/bin/sh

CMD=../psmedit

is_success()
{
	if [ "$1" = "" ]; then
		printf "SUCCESS\n"
	else
		printf "FAILURE\n"
		printf "$1"
		printf "\n"
	fi
}


# 初期化
RESULT_FILE=__test_option-F.mc
$CMD -f -S1 -o $RESULT_FILE
RESULT=$(diff 0init.mc $RESULT_FILE 2>&1)
is_success $RESULT
rm -f $RESULT_FILE

# mcbファイル書き込み 1ブロック
RESULT_FILE=__test_option-W_1block.mc
$CMD -f -U 1block.mcb -o $RESULT_FILE
RESULT=$(diff 1block.mc $RESULT_FILE 2>&1)
is_success $RESULT
rm -f $RESULT_FILE

# mcbファイル書き込み 3ブロック
RESULT_FILE=__test_option-W_3block.mc
$CMD -f -U 3block.mcb -o $RESULT_FILE
RESULT=$(diff 3block.mc $RESULT_FILE 2>&1)
is_success $RESULT
rm -f $RESULT_FILE

# mcbファイル取り出し 1ブロック
RESULT_FILE=__test_option-E_1block.mcb
$CMD -D -b 1 1block.mc -o $RESULT_FILE
RESULT=$(diff 1block.mcb $RESULT_FILE 2>&1)
is_success $RESULT
rm -f $RESULT_FILE

# mcbファイル取り出し 3ブロック
RESULT_FILE=__test_option-E_3block.mcb
$CMD -D -b 1 3block.mc -o $RESULT_FILE
RESULT=$(diff 3block.mcb $RESULT_FILE 2>&1)
is_success $RESULT
rm -f $RESULT_FILE
