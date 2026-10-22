.data
  array: .word 0:5
  array_size: .word 5
  input_prompt: .asciiz "Please input element "
  index_prompt: .asciiz "Please enter index:"

.text
main:
  li $t0, 0
  la $s0, array
  lw $s1, array_size

input_element_loop:
  li $v0, 4            # syscall 4: in chuỗi
  la $a0, input_prompt # "Please input element "
  syscall

  li $v0, 1            # syscall 1: in số nguyên
  move $a0, $t0        # in chỉ số đang duyệt
  syscall

  li $v0, 11           # syscall 11: in ký tự
  li $a0, ':'          # in dấu hai chấm
  syscall

  li $v0, 5
  syscall
  move $t1, $v0

  sll $t2, $t0, 2   # $t2 = 4i
  add $t2, $s0, $t2 # $t2 = base + 4i
  sw $t1, 0($t2)    # cất $t1 vào phần tử thứ i

  addi $t0, $t0, 1
  bne $t0, $s1, input_element_loop

  li $v0, 4
  la $a0, index_prompt
  syscall

  li $v0, 5
  syscall

  move $t0, $v0     # đưa chỉ số vào $t0
  sll $t2, $t0, 2   # dịch trái 2 bit để nhân 4
  add $t3, $s0, $t2 # tính địa chỉ phần tử

  li $v0, 1
  lw $a0, 0($t3)
  syscall

  li $v0, 10
  syscall
