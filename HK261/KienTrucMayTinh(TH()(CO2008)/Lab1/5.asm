.data
  array: .word 1, 2, 5, 8, 12, 44, 3, 9, 0, 10
  array_size: .word 10
  comma: .asciiz ", "

.text
main:
  la $s0, array
  lw $s1, array_size
  li $t0, 1

print_reverse_loop:
  sub $t1, $s1, $t0
  sll $t1, $t1, 2
  add $t1, $s0, $t1

  li $v0, 1
  lw $a0, 0($t1)
  syscall

  beq $t0, $s1, exit

  li $v0, 4
  la $a0, comma
  syscall

  addi $t0, $t0, 1
  j print_reverse_loop

exit:
  li $v0, 10
  syscall
