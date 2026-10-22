.data
  prompt_a: .asciiz "Insert a: "
  prompt_b: .asciiz "Insert b: "
  prompt_c: .asciiz "Insert c: "
  prompt_d: .asciiz "Insert d: "

  msg_f: .asciiz "F = "
  remainder_msg: .asciiz ", remainder "

.text
main:
  li $v0, 4        # syscall 4: in prompt
  la $a0, prompt_a
  syscall
  li $v0, 5        # syscall 5: đọc số nguyên
  syscall
  move $s0, $v0    # lưu a vào $s0

  li $v0, 4
  la $a0, prompt_b
  syscall
  li $v0, 5
  syscall
  move $s1, $v0

  li $v0, 4
  la $a0, prompt_c
  syscall
  li $v0, 5
  syscall
  move $s2, $v0

  li $v0, 4
  la $a0, prompt_d
  syscall
  li $v0, 5
  syscall
  move $s3, $v0

  addi $t0, $s0, 10 # $t0 = a + 10
  sub $t1, $s1, $s3 # $t1 = b - d

  sll $t2, $s0, 1   # $t2 = 2a
  sub $t2, $s2, $t2 # $t2 = c - 2a

  add $t3, $s0, $s1 # $t3 = a + b
  add $t3, $t3, $s2 # $t3 = a + b + c

  mul $t0, $t0, $t1 # tử = (a + 10)(b - d)
  mul $t0, $t0, $t2 # nhân tiếp với (c - 2a)

  div $t0, $t3      # chia tử cho mẫu
  mflo $t0          # $t0 = thương
  mfhi $t1          # $t1 = số dư

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

  li $v0, 10
  syscall
