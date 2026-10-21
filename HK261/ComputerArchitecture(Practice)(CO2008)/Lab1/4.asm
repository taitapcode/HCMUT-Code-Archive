.data
  prompt: .asciiz "Please enter a positive integer less than 16: "
  output_msg: .asciiz "Its binary form is: "

.text
main:

input:
  li $v0, 4
  la $a0, prompt
  syscall

  li $v0, 5
  syscall

  sltiu $t0, $v0, 16
  beq $t0, 0, input

process:
  move $s0, $v0
  li, $t0, 3

  li $v0, 4
  la $a0, output_msg
  syscall

print_loop:
  srlv $t1, $s0, $t0 # Shift right logical variable
  andi $t1, $t1, 1
  addi $t1, $t1, '0'

  li $v0, 11
  move $a0, $t1
  syscall

  subi $t0, $t0, 1
  bgez $t0, print_loop # Greater or equal


exit:
  li $v0, 10
  syscall
