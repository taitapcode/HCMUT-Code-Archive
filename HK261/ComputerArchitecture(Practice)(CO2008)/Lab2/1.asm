.data
  array: .word 0:5

.text
  la $s0, array    # địa chỉ đầu mảng
  li $t0, 0        # chỉ số index

process_loop:
  li $v0, 5                # syscall 5: đọc số nguyên
  syscall

  andi $t1, $v0, 3         # $t1 = n % 4
  sub $v0, $v0, $t1        # n -= n % 4: làm tròn xuống
  beq $t1, 3, round_up     # dư 3 thì bội số gần nhất nằm phía trên
  j next

round_up:
  addi $v0, $v0, 4         # n += 4: nhảy lên bội số kế tiếp

next:
  sll $t1, $t0, 2          # $t1 = 4i
  add $t1, $t1, $s0        # $t1 = base + 4i
  sw $v0, 0($t1)           # lưu vào phần tử thứ i
  addi $t0, $t0, 1         # i++
  bne $t0, 5, process_loop # đủ 5 phần tử thì dừng

  li $t0, 0         # i = 0: duyệt lại từ đầu mảng

print:
  li $v0, 1         # syscall 1: in số nguyên
  sll $a0, $t0, 2   # $a0 = 4i
  add $a0, $s0, $a0 # $a0 = base + 4i
  lw $a0, 0($a0)    # $a0 = phần tử thứ i
  syscall

  li $v0, 11        # syscall 11: in ký tự
  li $a0, ' '       # in dấu cách
  syscall

  addi $t0, $t0, 1  # i++
  bne $t0, 5, print # in đủ 5 phần tử thì dừng

exit:
  li $v0, 10 # syscall 10: kết thúc chương trình
  syscall
