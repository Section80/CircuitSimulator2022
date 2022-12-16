addi $t0, $0, 0
addi $t1, $0, 1
nop
nop
nop
nop
nop
addi $t0, $0, 10
addi $t1, $0, 10
beq $t0, $t1, TARGET
add $t0, $0, $0

TARGET:
addi $t0, $0, 999