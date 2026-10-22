.data
  hello_msg: .asciiz "Hello, "
  excl_mark: .asciiz "!"
  name: .space 100

.text
main:
  li $v0, 8    # syscall 8: đọc chuỗi từ bàn phím
  la $a0, name # $a0 = địa chỉ buffer nhận chuỗi
  li $a1, 100  # $a1 = số ký tự tối đa
  syscall

  la $t0, name                      # $t0 trỏ vào đầu buffer

find_newline_char:
  lb $t1, 0($t0)                    # nạp byte đang xét
  beqz $t1, print_msg               # gặp '\0' thì hết chuỗi
  beq $t1, 10, replace_newline_char # gặp '\n' thì thay thế
  addi $t0, $t0, 1                  # sang byte kế tiếp
  j find_newline_char

replace_newline_char:
  sb $zero, 0($t0)                  # ghi 0 đè lên ký tự xuống dòng

print_msg:
  li $v0, 4         # syscall 4: in chuỗi
  la $a0, hello_msg # in "Hello, "
  syscall

  li $v0, 4         # syscall 4: in chuỗi
  la $a0, name      # in tên vừa nhập
  syscall

  li $v0, 4         # syscall 4: in chuỗi
  la $a0, excl_mark # in dấu "!"
  syscall

  li $v0, 10
  syscall
