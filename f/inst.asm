.text
.globl main
main:
	lw $t0, 4($t1)
	add $t1, $t0, $t2
	addi $t2, $0, 16
	sw $t2, 16($t2)
	addi $t3, $0, 8
	sub $t4, $t3, $t2
	addi $t5, $0, 16


