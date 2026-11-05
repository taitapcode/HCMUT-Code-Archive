.data
  array: .word 0:5

.text
  la $s0, array
  li $t0, 0

process_loop:
  li $v0, 5
  syscall

  andi $t1, $v0, 3 # r = n & 3 <-> r = n % 4
  sub $v0, $v0, $t1 # n -= r
  beq $t1, 3, round_up
  j next

round_up:
  addi $v0, $v0, 4 # n += 4

next:
  sll $t1, $t0, 2
  add $t1, $t1, $s0
  sw $v0, 0($t1)
  addi $t0, $t0, 1
  bne $t0, 5, process_loop

  li $t0, 0

print:
  li $v0, 1
  sll $a0, $t0, 2
  add $a0, $s0, $a0
  lw $a0, 0($a0)
  syscall

  li $v0, 11
  li $a0, ' '
  syscall

  addi $t0, $t0, 1
  bne $t0, 5, print

exit:
  li $v0, 10
  syscall
