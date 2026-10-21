.data
  hello_msg: .asciiz "Hello "
  excl_mark: .asciiz "!"
  name: .space 100

.text
main:
  # Get name string
  li $v0, 8
  la $a0, name
  li $a1, 100
  syscall

  la $t0, name

find_newline_char:
  lb $t1, 0($t0) # load current character
  beqz $t1, print_msg
  beq $t1, 10, replace_newline_char
  addi $t0, $t0, 1
  j find_newline_char


replace_newline_char:
  sb $zero, 0($t0)

print_msg:
  li $v0, 4
  la $a0, hello_msg
  syscall

  li $v0, 4
  la $a0, name
  syscall

  li $v0, 4
  la $a0, excl_mark
  syscall

  li $v0, 10
  syscall
