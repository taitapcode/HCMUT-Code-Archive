.data
  prompt: .asciiz "Please enter a positive integer less than 16: "
  output_msg: .asciiz "Its binary form is: "

.text
main:
input:
  li $v0, 4          # syscall 4: in prompt
  la $a0, prompt
  syscall

  li $v0, 5          # syscall 5: đọc số nguyên
  syscall

  sltiu $t0, $v0, 16 # 1 nếu 0 <= n < 16, so sánh không dấu
  beq $t0, 0, input  # ngoài khoảng thì nhập lại

  move $s0, $v0        # lưu n vào $s0 trước khi in
  li $t0, 3            # bắt đầu từ bit 3, ứng với trọng số 8

  li $v0, 4            # syscall 4: in chuỗi
  la $a0, output_msg   # "Its binary form is: "
  syscall

print_loop:
  srlv $t1, $s0, $t0   # đưa bit thứ $t0 xuống vị trí 0
  andi $t1, $t1, 1     # lấy đúng bit đó
  addi $t1, $t1, '0'   # đổi sang mã ASCII

  li $v0, 11           # syscall 11: in ký tự
  move $a0, $t1
  syscall

  subi $t0, $t0, 1     # lùi sang bit thấp hơn
  bgez $t0, print_loop # còn bit thì lặp

  li $v0, 10
  syscall
