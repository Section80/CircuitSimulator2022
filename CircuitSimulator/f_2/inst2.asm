.text
.globl main
main:
	addi $t0, $0, 16
	sw   $t0, 32($t0)
	lw    $t1, 48($0)    # t1 = 16
            add  $t2, $t1, $t1


