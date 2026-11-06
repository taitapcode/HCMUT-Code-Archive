.data
  prompt_a: .asciiz "a = "
  prompt_b: .asciiz "b = "
  err_msg: .asciiz "Input must be a positive integer. Please try again.\n"
  gcd_output: .asciiz "GCD = "
  lcm_output: .asciiz "\nLCM = "

.text

read_a:
  li $v0, 4         # syscall 4: in chuỗi
  la $a0, prompt_a  # in "a = "
  syscall

  li $v0, 5         # syscall 5: đọc số nguyên
  syscall
  bgtz $v0, valid_a # a > 0 thì hợp lệ

  li $v0, 4         # syscall 4: in chuỗi
  la $a0, err_msg   # thông báo nhập sai
  syscall

  j read_a          # hỏi lại từ đầu

valid_a:
  move $s0, $v0     # $s0 = a

read_b:
  li $v0, 4         # syscall 4: in chuỗi
  la $a0, prompt_b  # in "b = "
  syscall

  li $v0, 5         # syscall 5: đọc số nguyên
  syscall
  bgtz $v0, valid_b # b > 0 thì hợp lệ

  li $v0, 4         # syscall 4: in chuỗi
  la $a0, err_msg   # thông báo nhập sai
  syscall

  j read_b          # hỏi lại từ đầu

valid_b:
  move $s1, $v0     # $s1 = b

gcd:
  move $t0, $s0          # $t0 = a
  move $t1, $s1          # $t1 = b

gcd_loop:
  beqz $t1, print_output # b == 0: $t0 là GCD

  div $t0, $t1           # chia a cho b
  mfhi $t2               # $t2 = a % b

  move $t0, $t1          # a = b
  move $t1, $t2          # b = a % b

  j gcd_loop             # lặp lại với cặp mới

print_output:
  mul $t1, $s0, $s1  # $t1 = a * b
  div $t1, $t0       # chia cho GCD
  mflo $t1           # $t1 = LCM

  li $v0, 4          # syscall 4: in chuỗi
  la $a0, gcd_output # in "GCD = "
  syscall

  li $v0, 1          # syscall 1: in số nguyên
  move $a0, $t0      # in GCD
  syscall

  li $v0, 4          # syscall 4: in chuỗi
  la $a0, lcm_output # in "\nLCM = "
  syscall

  li $v0, 1          # syscall 1: in số nguyên
  move $a0, $t1      # in LCM
  syscall


exit:
  li $v0, 10 # syscall 10: kết thúc chương trình
  syscall
