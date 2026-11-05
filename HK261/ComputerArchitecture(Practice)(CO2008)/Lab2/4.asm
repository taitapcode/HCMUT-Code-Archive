.data
  prompt: .asciiz "Enter a number: "
  msg1: .asciiz "The factorial of your number is: "
  msg2: .asciiz "A factorial for your number can not be found."
  msg3: .asciiz "Number is too large for 32-bit integer."

.text
input:
  li $v0, 4
  la $a0, prompt
  syscall

  li $v0, 5
  syscall

  move $a0, $v0
  bltz $a0, error
  bgt $a0, 12, overflow

  jal factorial
  move $t0, $v0

  li $v0, 4
  la $a0, msg1
  syscall

  li $v0, 1
  move $a0, $t0
  syscall

  j exit

factorial:
  beqz $a0, base_case

  addi $sp, $sp, -4
  sw $ra, 0($sp)
  addi $sp, $sp, -4
  sw $a0, 0($sp)

  addi $a0, $a0, -1
  jal factorial

  lw $t0, 0($sp)
  addi $sp, $sp, 4
  lw $ra, 0($sp)
  addi $sp, $sp, 4

  mul $v0, $v0, $t0

  jr $ra

base_case:
  li $v0, 1
  jr $ra

error:
  li $v0, 4
  la $a0, msg1
  syscall
  j exit

overflow:
  li $v0, 4
  la $a0, msg3
  syscall
  j exit

exit:
  li $v0, 10
  syscall
