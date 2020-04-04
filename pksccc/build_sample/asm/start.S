# entry point

	.text
	.code 32

	# データ保存領域
	.globl	SaveDataBuf
SaveDataBuf:
	.space	128

	# gccを使用した場合のエントリーポイント
	# mainを呼び出す処理を行う
	.globl	_start
_start:
	mov r0, $0
	b	main
	# not reached

	.end
