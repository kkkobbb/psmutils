# 割込みハンドラ関係

	.text
	.code 32

	###### SWI ######

	# set interrupt handler
	# r0: 0=swi2 1=IRQ 2=FIQ 3=?
	# r1: handler
	.globl	sys_handler
sys_handler:
	swi	1
	mov	pc, lr

	# save
	# r0: sector number
	# r1: source
	.globl	sys_save
sys_save:
	swi	3
	mov	pc, lr

	# set system clock
	# r0: clock rate
	.globl	sys_clock
sys_clock:
	swi	4
	mov	pc, lr

	# set exec argument
	# r0: 1
	# r1: 0(system program)
	# r2: argument(r0)
	.globl	sys_execset
sys_execset:
	swi	8
	mov	pc, lr

	# exec application
	.globl	sys_exec
sys_exec:
	swi	9
	# not reached

	# get time
	.globl	sys_gettime
sys_gettime:
	swi	14
	mov	pc, lr

	# set playstation communicate
	# r0: 1=on
	.globl	sys_pscomm
sys_pscomm:
	swi	17
	mov	pc, lr

	# get system status
	.globl	sys_status
sys_status:
	swi	19
	ldr	r0, [r0]
	mov	pc, lr

	# get application number
	.globl	sys_appnum
sys_appnum:
	swi	22
	mov	pc, lr

	###### interrupt handler ######

	 # IRQ
	.globl	IntIRQ
IntIRQ:
	stmdb	sp!,{r0-r3,lr}
	ldr	r0, offset_irq
	mov	lr,pc
	bx	r0
	#mov pc,r0
	ldmia	sp!,{r0-r3,pc}
offset_irq:
	.word	irq_handler

	# FIQ
	.globl	IntFIQ
IntFIQ:
	stmdb	sp!,{r0-r3,lr}
	ldr	r8, offset_fiq
	mov	lr,pc
	bx	r8
	ldmia	sp!,{r0-r3,pc}
offset_fiq:
	.word	fiq_handler

	.end
