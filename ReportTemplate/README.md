# Hướng dẫn sử dụng Template Báo cáo Bách Khoa (HCMUT LaTeX Template)

Thư mục này là **bản mẫu chuẩn (Reference Template)** cho các bài báo cáo môn học, bài tập lớn (BTL), đồ án và nghiên cứu khoa học tại Trường Đại học Bách Khoa - ĐHQG TP.HCM.

---

## 1. Cấu trúc thư mục

```
Báo_cáo_mẫu/
├── main.tex                    # File điều phối trung tâm
├── hcmut.sty                   # Gói cấu hình giao diện, font, header/footer, môi trường toán/code
├── README.md                   # Hướng dẫn này
├── Images/                     # Logo và hình ảnh dùng trong bài
│   └── bachkhoa-logo.png
└── Sections/                   # Từng phần nội dung của báo cáo
    ├── 00_title.tex            # Trang bìa chuẩn mẫu
    ├── 01_introduction.tex     # Giới thiệu & tổng quan
    ├── 02_structure_and_text.tex # Định dạng văn bản, danh sách, mục lục
    ├── 03_math_and_theorems.tex  # Công thức toán học, định lý, bổ đề
    ├── 04_tables_and_figures.tex # Bảng biểu (booktabs), hình ghép, TikZ
    ├── 05_algorithms_and_code.tex# Mã giả (algorithm2e), code listings
    └── 06_conclusion_and_references.tex # Kết luận & tài liệu tham khảo
```

---

## 2. Cách dùng cho một bài báo cáo mới

1. **Cấu hình thông tin chung trong [main.tex](file:///home/tai/Downloads/Báo_cáo_mẫu/main.tex)**:
   - Sửa `\headercoursename{...}`: Nhập tên môn học và mã môn (hiển thị góc trên bên phải header).
   - Sửa `\footerreporttitle{...}`: Nhập tên báo cáo / đề tài (hiển thị góc dưới bên trái footer).

2. **Cập nhật trang bìa trong [Sections/00_title.tex](file:///home/tai/Downloads/Báo_cáo_mẫu/Sections/00_title.tex)**:
   - Tên môn học, mã môn học.
   - Tiêu đề đề tài bài tập lớn.
   - Thông tin Giảng viên hướng dẫn, Lớp, Danh sách sinh viên thực hiện và MSSV.
   - Ngày tháng (mặc định tự động điền tháng và năm hiện tại bằng `\the\month` và `\the\year`).

3. **Viết nội dung**:
   - Tùy chỉnh hoặc thêm bớt các file trong thư mục `Sections/`.
   - Khai báo các file mới vào `main.tex` qua lệnh `\input{Sections/...}`.

---

## 3. Cách biên dịch ra PDF

### Dùng Tectonic (khuyên dùng, không cần cài đặt cồng kềnh qua Nix)
```bash
nix shell nixpkgs#tectonic -c tectonic main.tex
```

### Hoặc dùng công cụ LaTeX thông thường (TeX Live / MacTeX / MikTeX)
```bash
pdflatex main.tex
# hoặc
xelatex main.tex
```
File kết quả sẽ là `main.pdf`.
