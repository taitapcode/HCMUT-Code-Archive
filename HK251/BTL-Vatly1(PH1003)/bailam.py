import numpy as np
import matplotlib.pyplot as plt

# ==============================================
# HÀM TÍNH TOÁN TỪ TRƯỜNG BỞI VÒNG DÂY DÒNG ĐIỆN
# ==============================================


def calc_magnetic_field(R, I, x, y, z, N=360):
    MU_0 = 4 * np.pi * 1e-7
    D_THETA = 2 * np.pi / N
    B = np.array([0.0, 0.0, 0.0])
    P = np.array([x, y, z])

    for i in range(N):
        theta1 = i * D_THETA
        theta2 = (i + 1) * D_THETA

        # Tọa độ của hai đầu đoạn dl
        dl1 = np.array([0.0, R * np.cos(theta1), R * np.sin(theta1)])
        dl2 = np.array([0.0, R * np.cos(theta2), R * np.sin(theta2)])

        # Vector dl
        dl = dl2 - dl1

        # Vị trí trung điểm của đoạn dl
        mid = (dl1 + dl2) / 2

        # Vector r từ dl đến điểm P
        r_vec = P - mid
        r_abs = np.linalg.norm(r_vec)

        # Tích có hướng dl x r
        cross = np.cross(dl, r_vec)
        if r_abs > 0:
            dB = (MU_0 / (4 * np.pi)) * (I * cross) / (r_abs**3)
            B += dB

    return B


# ==============================================
# NHẬP THÔNG SỐ
# ==============================================


print("--- TÍNH TỪ TRƯỜNG DO VÒNG DÂY DÒNG ĐIỆN (BIOT-SAVART) ---")
try:
    R = float(input("Nhập bán kính vòng dây R (m) [Mặc định: 0.1]: ") or 0.1)
    I = float(input("Nhập cường độ dòng điện I (A) [Mặc định: 5.0]: ") or 5.0)
except ValueError:
    print("Giá trị không hợp lệ. Sử dụng giá trị mặc định R = 0.1m, I = 5.0A.")
    R, I = 0.1, 5.0

print("\n--- TÍNH TỪ TRƯỜNG TẠI ĐIỂM P(x,y,z) ---")
try:
    x = float(input("Nhập tọa độ x (m) [Mặc định: 0.0]: ") or 0.0)
    y = float(input("Nhập tọa độ y (m) [Mặc định: 0.0]: ") or 0.0)
    z = float(input("Nhập tọa độ z (m) [Mặc định: 0.0]: ") or 0.0)
except ValueError:
    print("Giá trị không hợp lệ. Tính tại gốc tọa độ (0,0,0).")
    x, y, z = 0.0, 0.0, 0.0

# ==============================================
# TÍNH TOÁN TỪ TRƯỜNG TẠI ĐIỂM P
# ==============================================


B_p = calc_magnetic_field(R, I, x, y, z)
B_p_abs = np.linalg.norm(B_p)

print(f"\nTừ trường tại điểm P({x}, {y}, {z}):")
print(f"Vector cảm ứng từ B = [{B_p[0]:.4e}, {B_p[1]:.4e}, {B_p[2]:.4e}]")
print(f"Độ lớn cảm ứng từ |B| = {B_p_abs:.6e} T")

# ==============================================
# TÍNH TOÁN TỪ TRƯỜNG TRÊN LƯỚI
# ==============================================


# Tạo lưới tọa độ 2D trên mặt phẳng XY
grid_size = 20
limit = 2 * R
x_val = np.linspace(-limit, limit, grid_size)
y_val = np.linspace(-limit, limit, grid_size)
x_grid, y_grid = np.meshgrid(x_val, y_val)

# Khởi tạo các mảng chứa thành phần vector B và |B| tại từng điểm lưới
B_grid_x = np.zeros_like(x_grid)
B_grid_y = np.zeros_like(x_grid)
B_grid_z = np.zeros_like(x_grid)
B_grid_abs = np.zeros_like(x_grid)

# Tính toán cho từng điểm trên lưới
for i in range(grid_size):
    for j in range(grid_size):
        b_vec = calc_magnetic_field(R, I, x_grid[i, j], y_grid[i, j], 0, N=100)
        B_grid_x[i, j], B_grid_y[i, j], B_grid_z[i, j] = b_vec

        # Tính độ lớn |B| tại mỗi điểm lưới để dùng cho việc tô màu
        B_grid_abs[i, j] = np.linalg.norm(b_vec)


# ==============================================
# VẼ ĐỒ THỊ
# ==============================================


fig = plt.figure(figsize=(10, 8))
ax = fig.add_subplot(111)

# Vẽ các đường sức từ
strm = ax.streamplot(
    x_grid,
    y_grid,
    B_grid_x,
    B_grid_y,
    color=B_grid_abs,
    cmap="plasma",
    density=abs(I / 5),
)

# Thêm thanh màu chú thích
cbar = fig.colorbar(strm.lines, ax=ax)
cbar.set_label("Độ lớn cảm ứng từ |B| (Tesla)")

# Vẽ vị trí dây cắt qua mặt phẳng (tại y = R và y = -R)
symbol_out = r"$\odot$"
symbol_in = r"$\otimes$"
ax.plot(
    0,
    R,
    marker=(symbol_in if I < 0 else symbol_out),
    color="red",
    markersize=10,
    linestyle="None",
    label=("Dòng điện VÀO" if I < 0 else "Dòng điện RA"),
)
ax.plot(
    0,
    -R,
    marker=(symbol_out if I < 0 else symbol_in),
    color="red",
    markersize=10,
    linestyle="None",
    label=("Dòng điện RA" if I < 0 else "Dòng điện VÀO"),
)

# Thiết lập tiêu đề và nhãn trục
ax.set_title("Đường sức từ (Mặt phẳng XY, z=0)")
ax.set_xlabel("x (m)")
ax.set_ylabel("y (m)")
ax.legend()
ax.grid(True)

plt.tight_layout()
plt.show()
