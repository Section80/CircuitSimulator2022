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

# ALU Hazard
	add $t1, $0, $t0    # hazard. t1: 1
	add $t2, $t1, $t0   # hazard. t2: 2
	add $t3, $t1, $t2	# hazard. t3: 3

	sw $t2, 0($0)	# [0]: 2

# Load Use
	lw $t0, 0($0)	# t0: 2
	add $t0, $t0, $t0,  # t0: 4

# lw sw
	lw $t3, 0($0)	# t3: 2
	sw $t3, 4($0)	# [4]: 2

# store used1
	add $t0, $t0, $t0   # t0: 8
	sw $t0, 0($t0)       # [8]: 8

# JUMP
	j BRANCH1
	add $t0, $0, $0	# this should not be happen
	nop
	nop

# Branch Hazard(alu)
BRANCH1: 
	add $t0, $t0, $t0	# t0: 16
	beq $t0, $t1, BRANCH2	# branch fail 
	add $t1, $0, $t0	# t1: 16
	beq $t0, $t1, BRANCH3	# branch success
BRANCH2: 
	add $t0, $0, $0		# this should not be happen 
BRANCH3: 
	add $t0, $t0, $t0		# t0: 32

# Branch Hazard(lw)
	lw $t0, 6($t3)		# t0 : 8
	lw $t1, 6($t3)		# t1 : 8
	beq $t0, $t1, BRANCH4
	add $t0, $0, $0		# this should not be happen 
	nop 
	nop
	nop
	nop

BRANCH4:
	add $t0, $t0, $t0		# t0: 16