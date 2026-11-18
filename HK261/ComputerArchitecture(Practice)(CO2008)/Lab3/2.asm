.data
  prompt_u: .asciiz "Please insert u: "
  prompt_v: .asciiz "Please insert v: "
  prompt_a: .asciiz "Please insert a: "
  prompt_b: .asciiz "Please insert b: "
  prompt_c: .asciiz "Please insert c: "
  prompt_d: .asciiz "Please insert d: "
  prompt_e: .asciiz "Please insert e: "
  res_msg:  .asciiz "Result: "
  msg_div0: .asciiz "Cannot divide by zero"

  const0f:  .float 0.0
  const7f:  .float 7.0
  const6f:  .float 6.0
  const2f:  .float 2.0

.text
  li $v0, 4
  la $a0, prompt_u
  syscall
  li $v0, 6
  syscall
  mov.s $f20, $f0

  li $v0, 4
  la $a0, prompt_v
  syscall
  li $v0, 6
  syscall
  mov.s $f21, $f0

  li $v0, 4
  la $a0, prompt_a
  syscall
  li $v0, 6
  syscall
  mov.s $f22, $f0

  li $v0, 4
  la $a0, prompt_b
  syscall
  li $v0, 6
  syscall
  mov.s $f23, $f0

  li $v0, 4
  la $a0, prompt_c
  syscall
  li $v0, 6
  syscall
  mov.s $f24, $f0

  li $v0, 4
  la $a0, prompt_d
  syscall
  li $v0, 6
  syscall
  mov.s $f25, $f0

  li $v0, 4
  la $a0, prompt_e
  syscall
  li $v0, 6
  syscall
  mov.s $f26, $f0

  mul.s $f4, $f25, $f25 # f4 = d^2
  mul.s $f4, $f4, $f4   # f4 = d^4

  mul.s $f5, $f26, $f26 # f5 = e^2
  mul.s $f5, $f5, $f26  # f5 = e^3

  add.s $f30, $f4, $f5  # f30 = d^4 + e^3
  l.s $f6, const0f
  c.eq.s $f30, $f6      # d^4 + e^3 == 0 ?
  bc1t div_zero

  mov.s $f12, $f20
  jal F
  mov.s $f27, $f0       # $f27 = F(u)

  mov.s $f12, $f21
  jal F
  mov.s $f28, $f0       # $f28 = F(v)

  sub.s $f29, $f27, $f28 # f29 = F(u) - F(v)

  div.s $f12, $f29, $f30

  li $v0, 4
  la $a0, res_msg
  syscall

  li $v0, 2
  syscall

  li $v0, 10
  syscall

div_zero:
  li $v0, 4
  la $a0, msg_div0
  syscall

  li $v0, 10
  syscall

F:
  mul.s $f4, $f12, $f12 # $f4 = x^2
  mul.s $f5, $f4, $f4   # $f5 = x^4
  mul.s $f6, $f5, $f4   # $f6 = x^6
  mul.s $f7, $f6, $f12  # $f7 = x^7

  mul.s $f8, $f22, $f7 # $f8 = a * x^7
  l.s   $f9, const7f   # $f9 = 7.0
  div.s $f8, $f8, $f9  # $f8 = (a * x^7) / 7.0

  mul.s $f10, $f23, $f6 # $f10 = b * x^6
  l.s   $f9, const6f  # $f9 = 6.0
  div.s $f10, $f10, $f9 # $f10 = (b * x^6) / 6.0

  mul.s $f11, $f24, $f4 # $f11 = c * x^4
  l.s   $f9, const2f # $f9 = 2.0
  div.s $f11, $f11, $f9 # $f11 = (c * x^4) / 2.0

  # (a * x^7) / 7.0 + (b * x^6) / 6.0 + (c * x^4) / 2.0
  add.s $f0, $f8, $f10
  add.s $f0, $f0, $f11

  jr $ra
