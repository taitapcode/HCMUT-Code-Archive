.data
  file_in:    .asciiz "raw_input.txt"
  file_out:   .asciiz "formatted_result.txt"
  header:     .asciiz "-----Student personal information-----\n"
  msg_open:   .asciiz "Error opening file.\n"
  label_name: .asciiz "Name: "
  label_id:   .asciiz "ID: "
  label_addr: .asciiz "Address: "
  label_age:  .asciiz "Age: "
  label_rel:  .asciiz "Religion: "
  label_tab:  .word label_name, label_id, label_addr, label_age, label_rel

.text
  li $v0, 13           # syscall 13: mở file
  la $a0, file_in      # đường dẫn file cần đọc
  li $a1, 0            # cờ 0: mở để đọc
  li $a2, 0            # mode bị bỏ qua
  syscall
  bltz $v0, open_error # báo lỗi nếu không mở được file đầu vào
  move $s0, $v0        # $s0 = file descriptor

  li $v0, 9            # syscall 9: cấp phát heap
  li $a0, 1000         # xin 1000 byte
  syscall
  move $s1, $v0        # $s1 = vùng đệm chứa nội dung file

  li $v0, 14           # syscall 14: đọc file
  move $a0, $s0        # đọc từ file vừa mở
  move $a1, $s1        # ghi vào vùng đệm heap
  li $a2, 1000         # đọc tối đa 1000 byte
  syscall
  move $s2, $v0        # $s2 = số byte thực đọc

  li $v0, 16         # syscall 16: đóng file
  move $a0, $s0      # descriptor cần đóng
  syscall

  li $v0, 9          # syscall 9: cấp phát heap cho chuỗi kết quả
  li $a0, 2000       # xin 2000 byte
  syscall
  move $s3, $v0      # $s3 = địa chỉ đầu chuỗi kết quả
  move $s4, $v0      # $s4 = con trỏ ghi, chạy theo từng ký tự

  la $a0, header     # chuỗi cần nối phải nằm trong $a0
  jal append_str     # nối dòng tiêu đề
  la $a0, label_name # chuỗi nhãn của trường đầu tiên
  jal append_str     # nối "Name: "

  move $t1, $s1      # $t1 = con trỏ đọc trong vùng đệm
  add $t3, $s1, $s2  # $t3 = địa chỉ kết thúc dữ liệu đọc được
  li $t0, 0          # $t0 = chỉ số trường hiện tại

copy_loop:                 # chép từng ký tự cho tới hết dòng
  beq $t1, $t3, copy_done  # đọc hết dữ liệu thì dừng

  lb $t2, 0($t1)           # $t2 = ký tự hiện tại

  beq $t2, '\n', copy_done # gặp xuống dòng thì dừng
  beqz $t2, copy_done      # gặp ký tự kết thúc chuỗi thì dừng
  beq $t2, ',', next_field # gặp dấu phẩy: sang trường kế tiếp

  move $a0, $t2            # tham số của append_char là ký tự
  jal append_char          # chép ký tự sang chuỗi kết quả

  addi $t1, $t1, 1         # tiến con trỏ đọc
  j copy_loop

next_field:         # hết một trường: xuống dòng và in nhãn kế tiếp
  addi $t0, $t0, 1  # sang trường kế tiếp
  li $a0, '\n'      # ký tự xuống dòng
  jal append_char   # xuống dòng trước nhãn mới
  la $t4, label_tab # $t4 = địa chỉ bảng nhãn
  sll $t5, $t0, 2   # $t5 = 4 * chỉ số trường
  add $t5, $t4, $t5 # $t5 = &label_tab[i]
  lw $a0, 0($t5)    # $a0 = nhãn của trường thứ i
  jal append_str    # nối nhãn vào chuỗi kết quả
  addi $t1, $t1, 1  # bỏ qua dấu phẩy
  j copy_loop

copy_done:         # đã chép xong cả dòng
  li $a0, 10       # mã ASCII của ký tự xuống dòng
  jal append_char  # kết thúc dòng cuối
  sb $zero, 0($s4) # đặt ký tự kết thúc chuỗi

  li $v0, 4        # syscall 4: in chuỗi
  move $a0, $s3    # in toàn bộ chuỗi kết quả
  syscall

  li $v0, 13           # syscall 13: mở file kết quả
  la $a0, file_out     # đường dẫn "formatted_result.txt"
  li $a1, 1            # cờ 1: mở để ghi
  li $a2, 0            # mode bị bỏ qua
  syscall
  bltz $v0, open_error # mở thất bại thì báo lỗi
  move $s0, $v0        # $s0 = descriptor của file kết quả

  li $v0, 15           # syscall 15: ghi file
  move $a0, $s0        # ghi vào file kết quả
  move $a1, $s3        # từ đầu chuỗi kết quả
  sub $a2, $s4, $s3    # số byte = con trỏ ghi - địa chỉ đầu
  syscall

  li $v0, 16           # syscall 16: đóng file kết quả
  move $a0, $s0        # descriptor của file kết quả
  syscall

  j exit

open_error:        # không mở được file
  li $v0, 4        # syscall 4: in chuỗi
  la $a0, msg_open # in "Error opening file."
  syscall
  j exit

exit:
  li $v0, 10           # syscall 10: kết thúc chương trình
  syscall

append_str:                       # nối cả chuỗi vào đệm kết quả
  lb $t6, 0($a0)                  # đọc ký tự từ chuỗi nguồn
  beq $t6, $zero, append_str_done # gặp ký tự kết thúc thì thôi
  sb $t6, 0($s4)                  # ghi sang chuỗi kết quả
  addi $a0, $a0, 1                # tiến con trỏ nguồn
  addi $s4, $s4, 1                # tiến con trỏ đích
  j append_str
append_str_done:                  # hết chuỗi nguồn
  jr $ra                          # trở về nơi gọi hàm

append_char:                      # nối một ký tự trong $a0
  sb $a0, 0($s4)                  # ghi ký tự vào vị trí con trỏ đích
  addi $s4, $s4, 1                # tiến con trỏ đích
  jr $ra                          # trở về nơi gọi hàm
