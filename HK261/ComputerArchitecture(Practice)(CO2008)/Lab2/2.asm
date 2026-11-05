.data
  prompt_a: .asciiz "a = "
  prompt_b: .asciiz "b = "
  err_msg: .asciiz "Input must be a positive integer. Please try again.\n"
  gcd_output: .asciiz "GCD = "
  lcm_output: .asciiz "LCM = "

.text

read_a:
  li $v0, 4
  la $a0, prompt_a
  syscall

  li $v0, 5
  syscall
  bgtz $v0, valid_a

  li $v0, 4
  la $a0, err_msg
  syscall

  j read_a

valid_a:
  move $s0, $v0

read_b:
  li $v0, 4
  la $a0, prompt_b
  syscall

  li $v0, 5
  syscall
  bgtz $v0, valid_b

  li $v0, 4
  la $a0, err_msg
  syscall

  j read_b

valid_b:
  move $s1, $v0

gcd:
  move $t0, $s0
  move $t1, $s1

gcd_loop:
  beqz $t1, print_output

  div $t0, $t1
  mfhi $t2

  move $t0, $t1
  move $t1, $t2

  j gcd_loop

print_output:
  mul $t1, $s0, $s1
  div $t1, $t0
  mflo $t1

  li $v0, 4
  la $a0, gcd_output
  syscall

  li $v0, 1
  move $a0, $t0
  syscall

  li $v0, 11
  li $a0, '\n'
  syscall

  li $v0, 4
  la $a0, lcm_output
  syscall

  li $v0, 1
  move $a0, $t1
  syscall


exit:
  li $v0, 10
  syscall
