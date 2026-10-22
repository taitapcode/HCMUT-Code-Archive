.data
  array: .word 1, 2, 5, 8, 12, 44, 3, 9, 0, 10
  array_size: .word 10
  comma: .asciiz ", "

.text
main:
  la $s0, array
  lw $s1, array_size
  li $t0, 1

print_reverse_loop:
  sub $t1, $s1, $t0 # chỉ số = array_size - bộ đếm
  sll $t1, $t1, 2   # đổi chỉ số sang offset byte
  add $t1, $s0, $t1 # $t1 = địa chỉ phần tử

  li $v0, 1         # syscall 1: in số nguyên
  lw $a0, 0($t1)    # nạp giá trị phần tử
  syscall

  beq $t0, $s1, exit   # phần tử cuối thì không in dấu phẩy

  li $v0, 4            # syscall 4: in chuỗi
  la $a0, comma        # ", "
  syscall

  addi $t0, $t0, 1     # sang phần tử kế
  j print_reverse_loop

exit:
  li $v0, 10
  syscall
