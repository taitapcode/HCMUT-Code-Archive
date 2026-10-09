.data
  prompt_pre: .asciiz "Insert "
  prompt_post: .asciiz ": "

  msg_f: .asciiz "F = "
  remainder_msg: .asciiz ", remainder "

.text
main:
  # Input
  li $a0, 'a'
  jal insert
  move $s0, $v0

  li $a0, 'b'
  jal insert
  move $s1, $v0

  li $a0, 'c'
  jal insert
  move $s2, $v0

  li $a0, 'd'
  jal insert
  move $s3, $v0

  # (a + 10)
  addi $t0, $s0, 10

  # (b - d)
  sub $t1, $s1, $s3

  # (c - 2a)
  sll $t2, $s0, 1
  sub $t2, $s2, $t2

  # (a + b + c)
  add $t3, $s0, $s1
  add $t3, $t3, $s2

  # (t0 * t1 * t2) / t3
  mul $t0, $t0, $t1
  mul $t0, $t0, $t2

  div $t0, $t3
  mflo $t0
  mfhi $t1

  # Output
  li $v0, 4
  la $a0, msg_f
  syscall

  li $v0, 1
  move $a0, $t0
  syscall

  li $v0, 4
  la $a0, remainder_msg
  syscall

  li $v0, 1
  move $a0, $t1
  syscall

  # Exit
  li $v0, 10
  syscall


insert:
  move $t0, $a0

  li $v0, 4
  la $a0, prompt_pre
  syscall

  li $v0, 11
  move $a0, $t0
  syscall

  li $v0, 4
  la $a0, prompt_post
  syscall

  li $v0, 5
  syscall

  jr $ra
