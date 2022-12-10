.data 0x10000000
.byte 1
.byte 2
.word 0x04030201	# word align

.text
.globl main
main:
	addi $t0, $0, 1   # t0: 1
	nop
	nop
	nop

	sw $t0, 0($t0)
	lw $t1, 0($t1)
	j BRANCH1
	add $t0, $0, $0	# this should not be happen

BRANCH1:
	beq $t1, $t1, BRANCH2
	add $t0, $0, $0	# this should not be happen

BRANCH2:
	add $t0, $t0, $t0	# t0: 2
