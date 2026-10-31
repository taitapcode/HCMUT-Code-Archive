.data
  array: .word 0:5

.text
  la $s0, array
  li $s1, 0

process_loop:
  li $v0, 5
  syscall

  andi $t0, $v0, 3 # r = n & 3 <-> n % 4
  sub $t1, $v0, $t0 # n - r
  beq $t0, 3, round_up
  j next

round_up:
  addi $t1, $t1, 4

next:
  sll $t0, $s1, 2
  add $t0, $t0, $s0
  sw $v0, 0($t0)
  addi $s1, $s1, 1
  bne $s1, 5, process_loop
