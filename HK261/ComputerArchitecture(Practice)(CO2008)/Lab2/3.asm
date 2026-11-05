.data
  arr:    .word 0:10

  prompt1: .asciiz "Please insert a element: "

  msg1:  .asciiz "Second largest value is "
  msg2:  .asciiz ", found in index "
  msg3:  .asciiz ", "
  msg4:  .asciiz "."
  msg5:  .asciiz "No second largest value exists"

.text
  la $s0, arr  # $s0: địa chỉ mảng
  li $s1, 0    # $s1: max hiện tại
  li $s2, 0    # $s2: second hiện tại
  li $s3, 0    # $s3: đã có second chưa (0/1)
  li $t0, 0    # $t0: i

input_loop:
  beq $t0, 10, print_second

  li $v0, 4
  la $a0, prompt1
  syscall

  li $v0, 5
  syscall

  sll $t1, $t0, 2
  add $t1, $t1, $s0
  sw $v0, 0($t1)

  beqz $t0, first_input
  bgt $v0, $s1, new_max
  beq $v0, $s1, next

  beqz $s3, new_second
  bgt $v0, $s2, new_second
  j next

first_input:
  move $s1, $v0
  j next

new_max:
  move $s2, $s1
  move $s1, $v0
  li $s3, 1
  j next

new_second:
  move $s2, $v0
  li $s3, 1
  j next

next:
  addi $t0, $t0, 1
  j input_loop


print_second:
  beqz $s3, no_second

  li $v0, 4
  la $a0, msg1
  syscall

  li $v0, 1
  move $a0, $s2
  syscall

  li $v0, 4
  la $a0, msg2
  syscall

  li $t0, 0
  li $t1, 0 # is printed first index yet

find_second_indexes:
  beq $t0, 10, exit

  sll $t2, $t0, 2
  add $t2, $t2, $s0
  lw $t3, 0($t2)

  bne $t3, $s2, find_next
  beqz $t1, print_idx

  li $v0, 4
  la $a0, msg3
  syscall

print_idx:
  li $v0, 1
  move $a0, $t0
  li $t1, 1
  syscall

find_next:
  addi $t0, $t0, 1
  j find_second_indexes

no_second:
  li $v0, 4
  la $a0, msg5
  syscall
  j exit

exit:
  li $v0, 11
  li $a0, '.'
  syscall

  li $v0, 10
  syscall
