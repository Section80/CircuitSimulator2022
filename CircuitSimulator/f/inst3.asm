.data 0x10000000
.byte 1
.byte 2
.double 1.123456789215654

.text
.globl main
main:
	addi $t0, $t0, 1
	beq $t0, $t0, main