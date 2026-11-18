.data
  prompt_a: .asciiz "Please insert a: "
  prompt_b: .asciiz "Please insert b: "
  prompt_c: .asciiz "Please insert c: "

  msg_x1:   .asciiz "x1 = "
  msg_x2:   .asciiz " and x2 = "
  msg_one:  .asciiz "There is one solution, x = "
  msg_none: .asciiz "There is no real solution"
  msg_inf:  .asciiz "There are infinite real solutions"

  const0f:   .float 0.0
  const2f:   .float 2.0
  const4f:   .float 4.0

.text
  # Input a
  li $v0, 4
  la $a0, prompt_a
  syscall

  li $v0, 6
  syscall
  mov.s $f20, $f0

  # Input b
  li $v0, 4
  la $a0, prompt_b
  syscall

  li $v0, 6
  syscall
  mov.s $f21, $f0

  # Input c
  li $v0, 4
  la $a0, prompt_c
  syscall

  li $v0, 6
  syscall
  mov.s $f22, $f0

  l.s $f0, const0f

  # Kiểm tra a == 0 thì pt thành dạng bx + c = 0
  c.eq.s $f20, $f0
  bc1t linear_case

  # Tính delta = b^2 - 4ac
  mul.s $f1, $f21, $f21  # b^2
  l.s $f2, const4f
  mul.s $f3, $f20, $f22  # ac
  mul.s $f3, $f3, $f2    # 4ac
  sub.s $f3, $f1, $f3    # delta = b^2 - 4ac

  c.lt.s $f3, $f0  # delta < 0
  bc1t no_solution

  c.eq.s $f3, $f0  # delta == 0
  bc1t one_solution

  # delta > 0
  sqrt.s $f4, $f3  # f4 = sqrt(delta)
  neg.s $f5, $f21  # f5 = -b
  l.s $f6, const2f # f6 = 2.0
  mul.s $f6, $f6, $f20  # f6 = 2a

  sub.s $f7, $f5, $f4  # f7 = -b - sqrt(delta)
  div.s $f8, $f7, $f6  # f8 = x1 = (-b - sqrt(delta)) / (2a)

  add.s $f9, $f5, $f4  # f9 = -b + sqrt(delta)
  div.s $f10, $f9, $f6 # f10 = x2 = (-b + sqrt(delta)) / (2a)

  # In x1
  li $v0, 4
  la $a0, msg_x1
  syscall

  li $v0, 2
  mov.s $f12, $f8
  syscall

  # In x2
  li $v0, 4
  la $a0, msg_x2
  syscall

  li $v0, 2
  mov.s $f12, $f10
  syscall

  j exit

linear_case:
  c.eq.s $f21, $f0  # b == 0
  bc1t check_c

  neg.s $f5, $f22  # f5 = -c
  div.s $f6, $f5, $f21  # f6 = x = -c / b

  li $v0, 4
  la $a0, msg_one
  syscall

  li $v0, 2
  mov.s $f12, $f6
  syscall

  j exit

one_solution:
  neg.s $f5, $f21  # f5 = -b
  l.s $f6, const2f # f6 = 2.0
  mul.s $f6, $f6, $f20  # f6 = 2a
  div.s $f7, $f5, $f6  # f7 = x = -b / (2a)

  li $v0, 4
  la $a0, msg_one
  syscall

  li $v0, 2
  mov.s $f12, $f7
  syscall

  j exit

check_c:
  c.eq.s $f22, $f0
  bc1f no_solution

  # Nếu c == 0  in vô số nghiệm
  li $v0, 4
  la $a0, msg_inf
  syscall

  j exit

no_solution:
  li $v0, 4
  la $a0, msg_none
  syscall

exit:
  li $v0, 10
  syscall
