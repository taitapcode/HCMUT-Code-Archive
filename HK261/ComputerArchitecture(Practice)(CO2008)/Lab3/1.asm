.data
  prompt_a: .asciiz "Please insert a: "
  prompt_b: .asciiz "Please insert b: "
  prompt_c: .asciiz "Please insert c: "

  msg_x1:   .asciiz "x1 = "
  msg_x2:   .asciiz " and x2 = "
  msg_one:  .asciiz "There is one solution, x = "
  msg_none: .asciiz "There is no real solution"
  msg_inf:  .asciiz "There are infinite real solutions"

  const0f:  .float 0.0
  const2f:  .float 2.0
  const4f:  .float 4.0

.text
  li $v0, 4        # syscall 4: in chuỗi
  la $a0, prompt_a # in "Please insert a: "
  syscall

  li $v0, 6        # syscall 6: đọc số thực
  syscall
  mov.s $f20, $f0  # $f20 = a (lưu lại trước khi $f0 bị ghi đè)

  li $v0, 4
  la $a0, prompt_b
  syscall

  li $v0, 6
  syscall
  mov.s $f21, $f0

  li $v0, 4
  la $a0, prompt_c
  syscall

  li $v0, 6
  syscall
  mov.s $f22, $f0

  l.s $f0, const0f # $f0 = 0.0 làm mốc so sánh

  c.eq.s $f20, $f0 # so sánh a với 0.0
  bc1t linear_case # a == 0 thì rẽ sang nhánh bậc nhất

  mul.s $f1, $f21, $f21 # $f1 = b^2
  l.s $f2, const4f      # $f2 = 4.0
  mul.s $f3, $f20, $f22 # $f3 = a * c
  mul.s $f3, $f3, $f2   # $f3 = 4ac
  sub.s $f3, $f1, $f3   # $f3 = delta

  c.lt.s $f3, $f0       # delta < 0
  bc1t no_solution      # rẽ nhánh vô nghiệm

  c.eq.s $f3, $f0       # delta == 0
  bc1t one_solution     # rẽ nhánh nghiệm kép

  sqrt.s $f4, $f3      # $f4 = sqrt(delta)
  neg.s $f5, $f21      # $f5 = -b
  l.s $f6, const2f     # $f6 = 2.0
  mul.s $f6, $f6, $f20 # $f6 = 2a

  sub.s $f7, $f5, $f4  # $f7 = -b - sqrt(delta)
  div.s $f8, $f7, $f6  # $f8 = x1

  add.s $f9, $f5, $f4  # $f9 = -b + sqrt(delta)
  div.s $f10, $f9, $f6 # $f10 = x2

  li $v0, 4        # syscall 4: in chuỗi
  la $a0, msg_x1   # in "x1 = "
  syscall

  li $v0, 2        # syscall 2: in số thực
  mov.s $f12, $f8  # $f12 = x1
  syscall

  li $v0, 4        # syscall 4: in chuỗi
  la $a0, msg_x2   # in " and x2 = "
  syscall

  li $v0, 2        # syscall 2: in số thực
  mov.s $f12, $f10 # $f12 = x2
  syscall

  j exit           # kết thúc chương trình sau khi in 2 nghiệm

linear_case:           # nhánh a == 0: bx + c = 0
  c.eq.s $f21, $f0     # so sánh b với 0.0
  bc1t check_c         # b == 0 thì xét tiếp c

  neg.s $f5, $f22      # $f5 = -c
  div.s $f6, $f5, $f21 # $f6 = -c / b

  li $v0, 4            # syscall 4: in chuỗi
  la $a0, msg_one      # in "There is one solution, x = "
  syscall

  li $v0, 2            # syscall 2: in số thực
  mov.s $f12, $f6      # $f12 = -c / b
  syscall

  j exit

one_solution:          # delta == 0: nghiệm kép -b / 2a
  neg.s $f5, $f21      # $f5 = -b
  l.s $f6, const2f     # $f6 = 2.0
  mul.s $f6, $f6, $f20 # $f6 = 2a
  div.s $f7, $f5, $f6  # $f7 = -b / 2a

  li $v0, 4            # syscall 4: in chuỗi
  la $a0, msg_one      # in "There is one solution, x = "
  syscall

  li $v0, 2            # syscall 2: in số thực
  mov.s $f12, $f7      # $f12 = -b / 2a
  syscall

  j exit

check_c:           # nhánh a == 0 và b == 0: xét phương trình c = 0
  c.eq.s $f22, $f0 # so sánh c với 0.0
  bc1f no_solution # c != 0 thì vô nghiệm

  li $v0, 4        # syscall 4: in chuỗi
  la $a0, msg_inf  # in "There are infinite real solutions"
  syscall

  j exit

no_solution:       # delta < 0 hoặc (a == 0, b == 0, c != 0)
  li $v0, 4        # syscall 4: in chuỗi
  la $a0, msg_none # in "There is no real solution"
  syscall

exit:              # điểm kết thúc chung
  li $v0, 10       # syscall 10: kết thúc chương trình
  syscall
