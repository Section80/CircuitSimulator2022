.text
.globl main
main:
	addi $t1, $0, 16
	sw   $t1, 16($t1)
	lw   $t0, 16($t1)
	add  $0, $0, $0
	sw   $t0, 32($t0)
	lw    $t1, 48($0)    # t1 = 16
	add  $t2, $t1, $t1