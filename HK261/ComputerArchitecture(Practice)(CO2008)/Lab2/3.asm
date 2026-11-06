.data
  arr:    .word 0:10 # mảng 10 phần tử

  prompt1: .asciiz "Please insert a element: " # lời nhắc nhập từng phần tử

  msg1:  .asciiz "Second largest value is "       # chuỗi "Second largest value is "
  msg2:  .asciiz ", found in index "              # chuỗi ", found in index "
  msg3:  .asciiz ", "                             # dấu phân cách giữa các chỉ số
  msg4:  .asciiz "."                              # dấu chấm
  msg5:  .asciiz "No second largest value exists" # thông báo không có giá trị lớn thứ hai

.text
  la $s0, arr # $s0 = địa chỉ mảng
  li $s1, 0   # $s1 = giá trị lớn nhất
  li $s2, 0   # $s2 = giá trị lớn thứ hai
  li $s3, 0   # $s3 = 1 khi đã có giá trị lớn thứ hai
  li $t0, 0   # $t0 = chỉ số i

input_loop:
  beq $t0, 10, print_second # đủ 10 phần tử thì in kết quả

  li $v0, 4                 # syscall 4: in chuỗi
  la $a0, prompt1           # in "Please insert a element: "
  syscall

  li $v0, 5                 # syscall 5: đọc số nguyên
  syscall

  sll $t1, $t0, 2           # $t1 = 4i
  add $t1, $t1, $s0         # $t1 = base + 4i
  sw $v0, 0($t1)            # lưu vào phần tử thứ i

  beqz $t0, first_input     # phần tử đầu chưa có mốc so sánh
  bgt $v0, $s1, new_max     # lớn hơn max: max cũ thành second
  beq $v0, $s1, next        # bằng max: bỏ qua

  beqz $s3, new_second      # chưa có second: nhận luôn
  bgt $v0, $s2, new_second  # lớn hơn second: thay second
  j next

first_input:
  move $s1, $v0 # phần tử đầu làm max
  j next

new_max:
  move $s2, $s1 # second = max cũ
  move $s1, $v0 # max = phần tử mới
  li $s3, 1     # đã có second
  j next

new_second:
  move $s2, $v0 # second = phần tử mới
  li $s3, 1     # đã có second
  j next

next:
  addi $t0, $t0, 1 # i++
  j input_loop


print_second:
  beqz $s3, no_second     # chưa có second: báo không tồn tại

  li $v0, 4               # syscall 4: in chuỗi
  la $a0, msg1            # "Second largest value is "
  syscall

  li $v0, 1               # syscall 1: in số nguyên
  move $a0, $s2           # in giá trị lớn thứ hai
  syscall

  li $v0, 4               # syscall 4: in chuỗi
  la $a0, msg2            # ", found in index "
  syscall

  li $t0, 0               # $t0 = chỉ số i
  li $t1, 0               # $t1 = 1 khi đã in ít nhất một chỉ số

find_second_indexes:
  beq $t0, 10, exit       # quét hết mảng thì kết thúc

  sll $t2, $t0, 2         # $t2 = 4i
  add $t2, $t2, $s0       # $t2 = base + 4i
  lw $t3, 0($t2)          # $t3 = phần tử thứ i

  bne $t3, $s2, find_next # không bằng second: bỏ qua
  beqz $t1, print_idx     # chỉ số đầu tiên: in không dấu phẩy

  li $v0, 4               # syscall 4: in chuỗi
  la $a0, msg3            # in ", " trước các chỉ số sau
  syscall

print_idx:
  li $v0, 1               # syscall 1: in số nguyên
  move $a0, $t0           # in chỉ số
  li $t1, 1               # đánh dấu đã in ít nhất một chỉ số
  syscall

find_next:
  addi $t0, $t0, 1        # i++
  j find_second_indexes   # tiếp tục quét

no_second:
  li $v0, 4    # syscall 4: in chuỗi
  la $a0, msg5 # in "No second largest value exists"
  syscall
  j exit

exit:
  li $v0, 11  # syscall 11: in ký tự
  li $a0, '.' # in dấu chấm kết thúc
  syscall

  li $v0, 10  # syscall 10: kết thúc chương trình
  syscall
