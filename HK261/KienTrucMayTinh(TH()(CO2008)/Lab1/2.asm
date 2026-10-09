.data
  array: .word 0:5
  array_size: .word 5

  input_prompt: .asciiz "Please input element "
  index_prompt: .asciiz "Please enter index:"

.text
main:
  li $t0, 0
  la $s0, array
  lw $s1, array_size

input_element_loop:
  # Print prompt
  li $v0, 4
  la $a0, input_prompt
  syscall

  li $v0, 1
  move $a0, $t0
  syscall

  li $v0, 11
  li $a0, ':'
  syscall

  # Get element value
  li $v0, 5
  syscall
  move $t1, $v0

  # Store element in array
  sll $t2, $t0, 2 # Offset = index x 4 (Shift left 2)
  add $t2, $s0, $t2 # Address = base + offset
  sw $t1, 0($t2)

  # Increase the loop counter
  addi $t0, $t0, 1
  bne $t0, $s1, input_element_loop

input_index:
  li $v0, 4
  la $a0, index_prompt
  syscall

  li $v0, 5
  syscall

  move $t0, $v0
  sll $t2, $t0, 2
  add $t3, $s0, $t2

  li $v0, 1
  lw $a0, 0($t3)
  syscall

exit:
  li $v0, 10
  syscall
