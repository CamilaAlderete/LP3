	.file	"mutex.c"
	.text
	.globl	var
	.bss
	.align 4
	.type	var, @object
	.size	var, 4
var:
	.zero	4
	.comm	mutex,40,32
	.text
	.globl	routine
	.type	routine, @function
routine:
.LFB6:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	$0, -4(%rbp)
	jmp	.L2
.L3:
	leaq	mutex(%rip), %rdi
	call	pthread_mutex_lock@PLT
	movl	var(%rip), %eax
	addl	$1, %eax
	movl	%eax, var(%rip)
	leaq	mutex(%rip), %rdi
	call	pthread_mutex_unlock@PLT
	addl	$1, -4(%rbp)
.L2:
	cmpl	$9999999, -4(%rbp)
	jle	.L3
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	routine, .-routine
	.section	.rodata
.LC0:
	.string	"pthread_create: "
.LC1:
	.string	"Hilo %d creado\n"
.LC2:
	.string	"pthread_join: "
.LC3:
	.string	"Hilo %d iniciado\n"
.LC4:
	.string	"Valor de var:%d\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB7:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$80, %rsp
	movl	%edi, -68(%rbp)
	movq	%rsi, -80(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movl	$0, %esi
	leaq	mutex(%rip), %rdi
	call	pthread_mutex_init@PLT
	movl	$0, -56(%rbp)
	jmp	.L5
.L8:
	movl	-56(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	leaq	-48(%rbp), %rax
	addq	%rdx, %rax
	movl	$0, %ecx
	leaq	routine(%rip), %rdx
	movl	$0, %esi
	movq	%rax, %rdi
	call	pthread_create@PLT
	testl	%eax, %eax
	je	.L6
	leaq	.LC0(%rip), %rdi
	call	perror@PLT
	movl	$1, %eax
	jmp	.L12
.L6:
	movl	-56(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC1(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	addl	$1, -56(%rbp)
.L5:
	cmpl	$3, -56(%rbp)
	jle	.L8
	movl	$0, -52(%rbp)
	jmp	.L9
.L11:
	movl	-52(%rbp), %eax
	cltq
	movq	-48(%rbp,%rax,8), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	call	pthread_join@PLT
	testl	%eax, %eax
	je	.L10
	leaq	.LC2(%rip), %rdi
	call	perror@PLT
	movl	$2, %eax
	jmp	.L12
.L10:
	movl	-52(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC3(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	addl	$1, -52(%rbp)
.L9:
	cmpl	$3, -52(%rbp)
	jle	.L11
	leaq	mutex(%rip), %rdi
	call	pthread_mutex_destroy@PLT
	movl	var(%rip), %eax
	movl	%eax, %esi
	leaq	.LC4(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$0, %eax
.L12:
	movq	-8(%rbp), %rcx
	xorq	%fs:40, %rcx
	je	.L13
	call	__stack_chk_fail@PLT
.L13:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 8
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 8
4:
