.data
  msg1: .asciiz "A factorial for your number can not be found."
  msg2: .asciiz "Number is too large for 32-bit integer."

.text
input:
  li $v0, 5             # syscall 5: đọc số nguyên
  syscall

  move $a0, $v0         # $a0 = n: tham số cho factorial
  bltz $a0, error       # n < 0: không có giai thừa
  bgt $a0, 12, overflow # n > 12: kết quả vượt 32 bit

  jal factorial
  move $t0, $v0 # $t0 = n!

  li $v0, 1     # syscall 1: in số nguyên
  move $a0, $t0 # in n!
  syscall

  j exit

factorial:
  beqz $a0, base_case # 0! = 1

  addi $sp, $sp, -4   # mở slot cho $ra
  sw $ra, 0($sp)      # lưu địa chỉ trả về
  addi $sp, $sp, -4   # mở slot cho n
  sw $a0, 0($sp)      # lưu n

  addi $a0, $a0, -1   # tham số mới: n - 1
  jal factorial       # $v0 = (n-1)!

  lw $t0, 0($sp)      # lấy lại n
  addi $sp, $sp, 4    # đóng slot của n
  lw $ra, 0($sp)      # khôi phục địa chỉ trả về
  addi $sp, $sp, 4    # đóng slot của $ra

  mul $v0, $v0, $t0   # n! = n * (n-1)!

  jr $ra

base_case:
  li $v0, 1           # 0! = 1
  jr $ra

error:
  li $v0, 4    # syscall 4: in chuỗi
  la $a0, msg1 # "A factorial for your number can not be found."
  syscall
  j exit

overflow:
  li $v0, 4    # syscall 4: in chuỗi
  la $a0, msg2 # "Number is too large for 32-bit integer."
  syscall
  j exit

exit:
  li $v0, 10 # syscall 10: kết thúc chương trình
  syscall
