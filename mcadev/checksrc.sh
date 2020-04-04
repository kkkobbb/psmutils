#!/bin/sh
# checkpatch.pl でソースコードのチェック
#
# -s
#   --strict を追加
CHECKPATCH=/usr/src/linux-source-4.4.0/scripts/checkpatch.pl


check_src() {
	src_code=$1
	strict_flag=$2

	printf "start\n"

	if $strict_flag
	then
		$CHECKPATCH -f --terse --strict $src_code
		rm -f .checkpatch*
	else
		$CHECKPATCH -f --terse $src_code
	fi

	printf "end\n"
}

USE_STRICT=false

# オプション解析
while getopts s OPT
do
	case $OPT in
		s) USE_STRICT=true ;;
		\?) exit ;;
	esac
done
shift $((OPTIND - 1))

SRCFILE=$1

# ソースコードチェック
check_src $SRCFILE $USE_STRICT
