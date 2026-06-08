# ĐẶC TẢ BÀI TẬP LỚN 2: VUA HẢI TẶC - ONE PIECE
## Arc Water 7 - Enies Lobby (Phần 2)
**Môn học:** Kỹ thuật Lập trình - CO1027
**Khoa Khoa học và Kỹ thuật Máy tính - Trường Đại học Bách Khoa ĐHQG-HCM** **Phiên bản:** 1.0
**Thời gian:** Tháng 05/2026

---

## 1. Chuẩn đầu ra
Sau khi hoàn thành bài tập lớn này, sinh viên ôn lại và sử dụng thành thục:
* Các thao tác đọc/ghi tập tin.
* Con trỏ và cấp phát động.
* Lập trình hướng đối tượng (OOP).
* Danh sách liên kết đơn (Singly Linked List).

---

## 2. Dẫn nhập
Trong phần trước, chúng ta đã theo chân băng Mũ Rơm trong hành trình tại Water 7, nơi những mâu thuẫn nội bộ dần được hé lộ và những quyết định quan trọng được đưa ra. Đỉnh điểm của sự kiện là khi Nico Robin bị Chính phủ Thế giới bắt giữ và đưa đến Enies Lobby - một trong ba cơ quan quyền lực lớn nhất của Chính phủ.

Tại Enies Lobby, băng Mũ Rơm buộc phải đối đầu trực diện với lực lượng đặc vụ tinh nhuệ CP9 để giải cứu đồng đội của mình. Trận chiến không chỉ đơn thuần là đối đầu giữa các cá nhân, mà còn là sự giằng co giữa thời gian, môi trường chiến trường và các yếu tố chiến thuật khác nhau. Các công trình như Cổng chính, Tòa án, Tháp công lý hay Cầu do dự đều đóng vai trò quan trọng, ảnh hưởng trực tiếp đến tiến trình của cuộc giải cứu.

Trong bài tập lớn này, sinh viên được yêu cầu mô phỏng lại trận chiến tại Enies Lobby thông qua việc xây dựng các lớp (class) và hiện thực các cơ chế tương tác giữa nhân vật, công trình và trạng thái trận chiến. Các nhân vật sẽ hành động theo lượt, thực hiện tấn công hoặc sử dụng kỹ năng đặc biệt, trong khi các công trình liên tục tác động đến diễn biến chung của trận đánh.

---

## 3. Các quy định và ràng buộc chung
* **Làm tròn số nguyên:** Trong khuôn khổ Bài tập lớn này, khi các giá trị tính toán cần làm tròn về số nguyên, sinh viên cần **làm tròn lên** (Ví dụ: dùng công thức toán nguyên hoặc hàm làm tròn lên).
* **Khoảng giá trị hợp lệ sau mỗi lần cập nhật:**
  * `hp` của nhân vật: `[0, maxHp]`
  * `hp` của công trình: `[0, maxHP]`
  * `energy`, `morale`, `alarmLevel`, `rescueProgress`, `escapeProgress`: `[0, 100]`
  * `busterCallTimer`: Không được nhỏ hơn `0` (tức là $\ge 0$).

---

## 4. Các Lớp (Class) Trong Chương Trình

### 4.1 Lớp Nhân Vật Cơ Sở (`Character`)
Mỗi nhân vật tham gia trận chiến được biểu diễn thông qua một lớp cơ sở trừu tượng `Character`. Đây là lớp cha của tất cả các nhân vật (Mũ Rơm và CP9).

#### Thuộc tính (`protected`):
* `name`: kiểu `string`, tên của nhân vật.
* `hp`: kiểu `int`, lượng máu hiện tại của nhân vật.
* `maxHp`: kiểu `int`, lượng máu tối đa của nhân vật.
* `atk`: kiểu `int`, chỉ số tấn công cơ bản.
* `def`: kiểu `int`, chỉ số phòng thủ cơ bản.
* `speed`: kiểu `int`, tốc độ hành động của nhân vật.
* `energy`: kiểu `int`, năng lượng hiện tại dùng để thi triển kỹ năng.
* `alive`: kiểu `bool`, trạng thái còn khả năng chiến đấu.

#### Phương thức (`public`):
* `Character(string name, int hp, int atk, int def, int speed, int energy);`
  * Gán giá trị tham số cho thuộc tính tương ứng. `maxHp` được gán bằng `hp`.
  * Nếu `hp > 0` thì `alive = true`, ngược lại là `false`.
* `virtual ~Character();` (Phương thức hủy ảo)
* `virtual int attack(Character* target, BattleContext& context) = 0;` (Tấn công thường lên nhân vật khác, trả về sát thương thực tế).
* `virtual int specialSkill(Character* target, BattleContext& context) = 0;` (Thi triển kỹ năng đặc biệt lên nhân vật khác, trả về sát thương thực tế).
* `virtual int attack(Building* target, BattleContext& context);` (Tấn công lên công trình. Mặc định không gây sát thương, trả về `0`).
* `virtual int specialSkill(Building* target, BattleContext& context);` (Kỹ năng lên công trình. Mặc định không gây sát thương, trả về `0`).
* `virtual void endTurn(BattleContext& context);` (Xử lý hiệu ứng cuối lượt, mặc định trống).
* `void receiveDamage(int damage);`
  * Sát thương thực tế nhận vào = `damage` - `def` của bản thân. Nếu hiệu này $\le 0$, nhân vật không nhận sát thương.
  * Nếu sau khi nhận sát thương, `hp <= 0` thì `hp = 0` và `alive = false`.
* `bool isAlive() const;`
* `string getName() const;`
* `int getHP() const;`
* `int getEnergy() const;`
* `virtual bool isStrawHat() const;` (Mặc định trả về `false`).
* `virtual bool isCP9() const;` (Mặc định trả về `false`).
* `virtual string str() const = 0;` (Phương thức thuần ảo biểu diễn thông tin).

---

### 4.2 Phe Băng Mũ Rơm (`StrawHat`)
Kế thừa từ `Character`.

#### Thuộc tính (`protected`):
* `bounty`: kiểu `long long`, tiền truy nã của nhân vật.

#### Phương thức (`public`):
* Constructor nhận đầy đủ các tham số tương tự lớp cha và khởi tạo thêm `bounty`.
* `virtual bool isStrawHat() const;` -> Trả về `true`.
* `virtual string str() const;`
  * Định dạng chuỗi: `StrawHat [name=<name>, hp=<hp>, atk=<atk>, def=<def>, speed=<speed>, energy=<energy>, bounty=<bounty>]`
  * *Lưu ý:* Không có khoảng trắng dư thừa ngoài định dạng.
* **Quy tắc hạ gục mặc định:** Khi một thành viên băng Mũ Rơm hạ gục một đặc vụ CP9, nếu mô tả nhân vật đó không nói gì thêm về `morale` thì mặc định giá trị `morale` trong `BattleContext` tăng thêm `5`.

#### Các thành viên cụ thể của băng Mũ Rơm:

##### 1. Monkey D. Luffy (`Luffy`)
* **`attack`**: Sát thương phụ thuộc vào % máu hiện tại:
  * Nếu máu hiện tại $> 50\%$ `maxHp`: sát thương bằng đúng `atk`.
  * Nếu $30\% <$ máu hiện tại $\le 50\%$ `maxHp`: sát thương tăng thêm $15\%$.
  * Nếu máu hiện tại $\le 30\%$ `maxHp`: sát thương tăng thêm $30\%$.
  * Nếu đòn đánh hạ gục mục tiêu, `morale` tăng thêm `5`.
* **`specialSkill`** (Gear Second): Chi phí `20` năng lượng.
  * Gây sát thương bằng $200\%$ chỉ số `atk`, tăng `speed` và `atk` của bản thân thêm `15` đơn vị vĩnh viễn.
  * Làm tăng `alarmLevel` thêm `10` và làm giảm máu hiện tại của Luffy một lượng bằng $8\%$ `maxHp` (lượng giảm này trừ thẳng vào `hp`, không tính `def`, nếu `hp` xuống $\le 0$ thì xử lý tử vong bình thường).
  * Điều kiện sử dụng: Năng lượng hiện tại $\ge 20$ và `hp` hiện tại $\ge 15\%$ `maxHp`.
* **`endTurn`**: Nếu máu hiện tại $\le 30\%$ `maxHp`, `morale` tăng thêm `3`. Nếu Luffy vừa hạ gục đối thủ trong lượt này, hồi lại `5` năng lượng.

##### 2. Roronoa Zoro (`Zoro`)
* **`attack`**: Sát thương bằng `atk` cộng thêm $20\%$ chỉ số `def` của bản thân. Nếu mục tiêu có lượng máu hiện tại $< 40\%$ `maxHp` của mục tiêu, sát thương được tăng thêm $15\%$.
* **`specialSkill`**: Chi phí `15` năng lượng.
  * Gây sát thương bằng $220\%$ chỉ số `atk`.
  * Nếu mục tiêu có lượng máu $< 50\%$ `maxHp` của mục tiêu, sát thương kỹ năng tăng thêm $50\%$.
  * Nếu kỹ năng hạ gục mục tiêu, Zoro hồi lại `8` năng lượng và `morale` tăng thêm `4`.
* **`endTurn`**: Nếu Zoro hạ gục được mục tiêu trong lượt, `morale` tăng thêm `6` và chỉ số `atk` của Zoro tăng thêm $5\%$.

##### 3. Vinsmoke Sanji (`Sanji`)
* **`attack`**: Sát thương bằng `atk` cộng thêm $50\%$ chỉ số `speed` hiện tại. Nếu mục tiêu có chỉ số `def` thấp hơn của Sanji, sát thương tăng thêm $10\%$ (quy tắc tăng $10\%$ này không áp dụng khi mục tiêu là công trình).
* **`specialSkill`**: Chi phí `18` năng lượng.
  * Gây sát thương bằng $210\%$ chỉ số `atk`.
  * Sau khi trúng đòn, `def` của mục tiêu giảm `8` đơn vị (bỏ qua nếu mục tiêu là công trình).
  * Nếu mục tiêu là `Jabra`, phòng thủ mục tiêu giảm `12` đơn vị thay vì `8`.
* **`endTurn`**: Nếu đối thủ bị hạ gục trong lượt, `morale` tăng thêm `8` và chỉ số `atk` của Sanji tăng thêm $10\%$.

##### 4. Nami (`Nami`)
* **`attack`**: Sát thương bằng chỉ số `atk` và bỏ qua $30\%$ phòng thủ (`def`) của mục tiêu. Nếu tấn công công trình, sát thương chỉ bằng $50\%$ chỉ số `atk`.
* **`specialSkill`**: Chi phí `20` năng lượng.
  * Gây sát thương bằng `atk + 40`. Đồng thời làm giảm tốc độ (`speed`) của mục tiêu đi `10` đơn vị.
  * Nếu mục tiêu là công trình, sát thương tăng thêm $50\%$.
  * Sau khi sử dụng, `busterCallTimer` tăng thêm `1` và `alarmLevel` giảm `5`.
* **`endTurn`**: Nếu đối thủ bị hạ gục trong lượt, Nami hồi lại `6` năng lượng.

##### 5. Tony Tony Chopper (`Chopper`)
* **`attack`**: Sát thương bằng chỉ số `atk` của Chopper.
* **`specialSkill`**: Chi phí `15` năng lượng.
  * Kỹ năng hồi máu này **chỉ có thể sử dụng lên thành viên của băng Mũ Rơm đang có lượng máu hiện tại thấp nhất** (còn sống).
  * Hồi cho mục tiêu một lượng máu bằng `35 + 50% * atk` của Chopper.
  * Nếu mục tiêu được hồi máu là `Luffy`, `morale` tăng thêm `5`.
* **`endTurn`**: Không có hiệu ứng đặc biệt cuối lượt.

##### 6. Usopp / Sogeking (`Usopp`)
* **`attack`**: Sát thương bằng chỉ số `atk`. Nếu mục tiêu có `speed < 50`, sát thương tăng thêm $20\%$. Nếu là công trình, sát thương chỉ bằng $50\%$ chỉ số `atk`.
* **`specialSkill`**: Chi phí `16` năng lượng.
  * Gây sát thương bằng $80\%$ chỉ số `atk`.
  * Làm giảm tốc độ mục tiêu `12` đơn vị (bỏ qua nếu là công trình).
  * Sau khi sử dụng, `escapeProgress` tăng thêm `8`.
* **`endTurn`**: Sau khi Sogeking tấn công (hoặc dùng chiêu hụt/trúng), `morale` toàn đội tăng thêm `10` đơn vị.

##### 7. Franky / Cutty Flam (`Franky`)
* **`attack`**: Sát thương bằng `atk + 30% * def` của bản thân. Nếu mục tiêu là nhân vật CP9, sát thương tăng thêm $10\%$.
* **`specialSkill`**: Quyết định chọn kỹ năng dựa vào năng lượng hiện có:
  * Nếu năng lượng $\ge 30$, sử dụng chiêu **Coup de Vent** tiêu tốn `30` năng lượng: Gây sát thương bằng $120\%$ chỉ số `atk` lên mục tiêu. **Đặc biệt: Nếu mục tiêu là một `Building`, công trình đó ngay lập tức bị phá hủy hoàn toàn (hp về 0).**
  * Nếu năng lượng nằm trong đoạn `[20, 29]`, sử dụng chiêu **Strong Right** tiêu tốn `20` năng lượng: Gây sát thương bằng $180\%$ chỉ số `atk` và làm giảm `speed` mục tiêu `8` đơn vị (bỏ qua giảm tốc nếu là công trình). Nếu mục tiêu là `Lucci`, sát thương tăng thêm $20\%$.
* **`endTurn`**:
  * Nếu máu hiện tại $> 70\%$ `maxHp`, `def` của Franky tăng thêm `5` đơn vị.
  * Nếu máu hiện tại $< 30\%$ `maxHp`, `atk` của Franky tăng thêm $10\%$.

---

### 4.3 Phe Lực Lượng Đặc Vụ CP9 (`CP9Agent`)
Kế thừa từ `Character`.

#### Thuộc tính (`protected`):
* `doriki`: kiểu `int`, đại diện cho lực chiến tổng thể.

#### Phương thức (`public`):
* Constructor nhận đầy đủ các tham số của lớp cha và thuộc tính `doriki`.
* `virtual bool isCP9() const;` -> Trả về `true`.
* `virtual string str() const;`
  * Định dạng chuỗi: `CP9 [name=<name>, hp=<hp>, atk=<atk>, def=<def>, speed=<speed>, energy=<energy>, doriki=<doriki>]`
* **Quy tắc hạ gục mặc định:** Khi một đặc vụ CP9 hạ gục một thành viên băng Mũ Rơm, nếu mô tả không nói gì thêm thì mặc định giá trị `morale` trong `BattleContext` giảm đi `5`.

#### Các đặc vụ cụ thể của CP9:

##### 1. Rob Lucci (`Lucci`)
* **`attack`**: Sát thương bằng `atk + doriki / 20`. Nếu mục tiêu có lượng máu hiện tại $< 50\%$ `maxHp` của mục tiêu, sát thương tăng thêm $20\%$.
* **`specialSkill`**: Chi phí `25` năng lượng.
  * Gây sát thương bằng $280\%$ chỉ số `atk`.
  * Đòn đánh bỏ qua $50\%$ chỉ số phòng thủ (`def`) của mục tiêu.
  * Nếu kỹ năng hạ gục mục tiêu, `morale` giảm thêm `10`.
* **`endTurn`**: Nếu máu hiện tại $< 40\%$ `maxHp`, chỉ số `atk` tăng thêm $5\%$.

##### 2. Kaku (`Kaku`)
* **`attack`**: Sát thương bằng chỉ số `atk`.
* **`specialSkill`**: Chi phí `20` năng lượng.
  * Thực hiện **3 đòn đánh liên tiếp** tuần tự lên cùng một mục tiêu:
    * Đòn 1: Sát thương bằng $120\%$ `atk`.
    * Đòn 2: Sát thương bằng $100\%$ `atk`.
    * Đòn 3: Sát thương bằng $80\%$ `atk`.
  * *Lưu ý:* Nếu mục tiêu bị hạ gục trước khi kết thúc 3 đòn, các đòn còn lại dừng lại ngay lập tức.
* **`endTurn`**: Không có hiệu ứng đặc biệt.

##### 3. Jabra (`Jabra`)
* **`attack`**: Sát thương bằng chỉ số `atk`.
* **`specialSkill`**: Chi phí `18` năng lượng.
  * Gây sát thương bằng $150\%$ chỉ số `atk`.
  * Nếu máu hiện tại của Jabra $< 30\%$ `maxHp`, sát thương kỹ năng tăng thêm $25\%$.
  * Nếu hạ gục mục tiêu, `morale` giảm thêm `5`.
* **`endTurn`**: Không có hiệu ứng đặc biệt.

##### 4. Blueno (`Blueno`)
* **`attack`**: Sát thương bằng chỉ số `atk`.
* **`specialSkill`**: Chi phí `15` năng lượng.
  * Gây sát thương bằng $130\%$ chỉ số `atk`.
  * Nếu máu hiện tại của Blueno $> 50\%$ `maxHp`, sát thương kỹ năng tăng thêm `20` đơn vị. Nếu máu hiện tại $\le 50\%$ `maxHp`, sát thương tăng thêm `40` đơn vị.
* **`endTurn`**: Không có hiệu ứng đặc biệt.

##### 5. Kalifa (`Kalifa`)
* **`attack`**: Sát thương bằng chỉ số `atk`.
* **`specialSkill`**: Chi phí `18` năng lượng.
  * Gây sát thương bằng $140\%$ chỉ số `atk`.
  * Làm giảm `morale` đi `8` đơn vị và giảm `speed` mục tiêu đi `6` đơn vị.
  * Nếu mục tiêu gánh chịu là `Nami`, `morale` giảm hẳn `12` đơn vị thay vì `8`.
* **`endTurn`**: Không có hiệu ứng đặc biệt.

##### 6. Kumadori (`Kumadori`)
* **`attack`**: Sát thương bằng chỉ số `atk`.
* **`specialSkill`**: Chi phí `16` năng lượng.
  * Gây sát thương bằng `30 + doriki / 10`.
  * Nếu máu hiện tại của Kumadori $< 40\%$ `maxHp`, sát thương tăng thêm `25` đơn vị.
* **`endTurn`**: Không có hiệu ứng đặc biệt.

##### 7. Fukurou (`Fukurou`)
* **`attack`**: Sát thương bằng chỉ số `atk`.
* **`specialSkill`**: Chi phí `14` năng lượng.
  * Gây sát thương bằng $130\%$ chỉ số `atk`.
  * Nếu mục tiêu đang là nhân vật có lượng máu hiện tại thấp nhất trong phe đối phương, sát thương tăng thêm `20` đơn vị.
  * Nếu hạ gục được mục tiêu, `morale` giảm thêm `6`.
* **`endTurn`**: Không có hiệu ứng đặc biệt.

---

### 4.4 Lớp Trạng Thái Trận Đấu (`BattleContext`)
Lưu trữ trạng thái chung của toàn bộ chiến trường.

#### Thuộc tính (`public`):
* `turnCount`: số lượt đã diễn ra.
* `morale`: tinh thần phe Mũ Rơm.
* `alarmLevel`: mức báo động tại Enies Lobby.
* `rescueProgress`: tiến độ giải cứu Robin.
* `escapeProgress`: tiến độ rút lui khỏi Enies Lobby.
* `busterCallTimer`: số lượt còn lại trước khi Buster Call kích hoạt.
* `mainGateDestroyed`: trạng thái Cổng chính (`bool`, mặc định `false`).
* `robinRescued`: trạng thái cứu Robin (`bool`, mặc định `false`).
* `bridgeOpened`: trạng thái mở cầu (`bool`, mặc định `false`).
* `battleEnded`: trạng thái kết thúc trận đánh (`bool`, mặc định `false`).
* `resultCode`: chuỗi kết quả cuối cùng (`string`).

#### Phương thức:
* Constructor mặc định: khởi tạo toàn bộ thông số ban đầu bằng 0, cờ bằng `false`, chuỗi rỗng.
* `void nextTurn();` -> Tăng `turnCount` thêm `1`.

---

### 4.5 Hệ Thống Công Trình (`Building`)
Các công trình không tham gia thứ tự đánh theo lượt nhưng tác động vào cuối lượt thông qua `applyEffect`.

#### Thuộc tính (`protected`):
* `name`: kiểu `string`, tên công trình.
* `hp`: kiểu `int`, độ bền hiện tại.
* `maxHP`: kiểu `int`, độ bền tối đa.
* `destroyed`: kiểu `bool`, trạng thái bị phá hủy.

#### Phương thức (`public`):
* `Building(string name, int hp);` -> Gán `maxHP = hp`. Nếu `hp > 0` thì `destroyed = false`, ngược lại là `true`.
* `virtual ~Building();`
* `void receiveDamage(int damage);` -> Trừ trực tiếp vào độ bền (công trình không có thủ `def`). Nếu `hp <= 0` thì `hp = 0` và `destroyed = true`.
* `bool isDestroyed() const;`
* `virtual void applyEffect(BattleContext& context) = 0;`
* `virtual void onDestroyed(BattleContext& context);` (Gọi ngay khi công trình vừa sập).
* `virtual string str() const;`
  * Định dạng: `Building [name=<name>, hp=<hp>, maxHP=<maxHP>, destroyed=<destroyed>]` (destroyed in ra `true` hoặc `false`).

#### Các công trình cụ thể:

##### 1. Cổng chính (`MainGate`)
* `applyEffect`: Nếu chưa bị phá, `rescueProgress` (tiến độ giải cứu Robin) cố định không tăng trong lượt này (mọi nguồn tăng đều không hiệu lực).
* `onDestroyed`: `mainGateDestroyed = true`, `rescueProgress` tăng thêm `20` và `morale` tăng thêm `5`.

##### 2. Tòa án (`Courthouse`)
* `applyEffect`: Nếu chưa bị phá, kết thúc mỗi lượt `alarmLevel` tự động tăng thêm `5`.
* `onDestroyed`: `alarmLevel` giảm ngay lập tức `20` đơn vị và hiệu ứng tăng `5` mỗi lượt mất hiệu lực vĩnh viễn.

##### 3. Tháp công lý (`TowerOfJustice`)
* `applyEffect`: Nếu `mainGateDestroyed == true` và Robin chưa được giải cứu (`robinRescued == false`), `rescueProgress` tự động tăng thêm `5` sau mỗi lượt. Nếu sau khi tăng, `rescueProgress >= 100`, gán `robinRescued = true` và `morale` tăng thêm `10`.

##### 4. Cầu do dự (`BridgeOfHesitation`)
* `applyEffect`: Nếu Robin đã cứu thành công (`robinRescued == true`), gán `bridgeOpened = true` và tăng `escapeProgress` thêm `5` mỗi lượt. Nếu `escapeProgress >= 100`, trận đánh lập tức kết thúc với kết quả `STRAW_HAT_WIN`.

##### 5. Tàu Buster Call (`BusterCallShip`)
* `applyEffect`: Nếu chưa bị phá hủy, `busterCallTimer` giảm `1` sau mỗi lượt. Nếu `busterCallTimer <= 0`, trận đánh kết thúc với kết quả `BUSTER_CALL`.
* `onDestroyed`: Khi bị phá hủy, kéo dài thời gian: `busterCallTimer` tăng thêm `3`.

---

### 4.6 Quản Lý Trận Đấu (`EniesLobbyBattle`)
Điều phối toàn bộ dữ liệu thực thể trên chiến trường.

#### Thuộc tính (`private`):
* `strawHats`: kiểu `Character**` (mảng động lưu tối đa 7 con trỏ thành viên Mũ Rơm).
* `strawHatCount`: số lượng thành viên hiện tại.
* `cp9Agents`: kiểu `Character**` (mảng động lưu tối đa 7 con trỏ đặc vụ CP9).
* `cp9Count`: số lượng đặc vụ hiện tại.
* `buildings`: kiểu `Building**` (mảng động lưu tối đa 5 con trỏ công trình).
* `buildingCount`: số lượng công trình hiện tại.
* `turnOrder`: danh sách liên kết đơn kiểu `TurnNode*` để giữ thứ tự lượt hành động.
* `context`: thực thể trạng thái `BattleContext`.
* `maxTurns`: số lượt giới hạn tối đa của trận đấu.

---

## 5. Quy Tắc Vận Hành Trận Chiến

### 5.1 Quy Tắc Chọn Mục Tiêu Của Phe Băng Mũ Rơm
Trong lượt của một nhân vật thuộc băng Mũ Rơm, mục tiêu được quét chọn tuần tự theo độ ưu tiên nghiêm ngặt sau:
1. Nếu `MainGate` chưa bị phá $
ightarrow$ Mục tiêu bắt buộc là `MainGate`.
2. Nếu `MainGate` đã bị phá, đồng thời `alarmLevel >= 50` và `Courthouse` chưa bị phá $
ightarrow$ Mục tiêu là `Courthouse`.
3. Nếu hai điều kiện trên không thỏa mãn, đồng thời chỉ số `busterCallTimer <= 5` và `BusterCallShip` chưa bị phá $
ightarrow$ Mục tiêu là `BusterCallShip`.
4. Nếu các điều kiện trên đều không thỏa mãn và Robin chưa được giải cứu (`robinRescued == false`) $
ightarrow$ Mục tiêu là **đặc vụ CP9 còn sống đầu tiên** tính theo thứ tự xuất hiện trong mảng `cp9Agents`.
5. Nếu Robin đã được cứu thành công (`robinRescued == true`) $
ightarrow$ Mục tiêu là `BridgeOfHesitation`. Nếu công trình này đã bị phá hủy hoàn toàn hoặc không tồn tại trong danh sách $
ightarrow$ Nhân vật quay sang tấn công **đặc vụ CP9 còn sống đầu tiên**.
6. **Trường hợp ngoại lệ đặc biệt của Chopper:** Nếu là lượt của Chopper và cậu ta **có đủ năng lượng** để thi triển kỹ năng đặc biệt (`specialSkill`), mục tiêu của kỹ năng này đổi thành **thành viên băng Mũ Rơm còn sống có chỉ số lượng máu `hp` hiện tại thấp nhất**. Nếu không đủ năng lượng để dùng skill, Chopper tuân theo quy tắc chọn mục tiêu tấn công thường từ bước 1 đến 5 như trên.

### 5.2 Quy Tắc Chọn Mục Tiêu Của Phe CP9
Trong lượt hành động của một đặc vụ CP9, mục tiêu **luôn luôn là thành viên băng Mũ Rơm còn sống đầu tiên** tính theo thứ tự trong mảng `strawHats`. Phe CP9 hoàn toàn không bao giờ tấn công hay gây sát thương lên các công trình.

### 5.3 Vòng Lặp Tiến Trình Trận Đấu (`runBattle`)
Trận đánh tiến hành lặp liên tục cho đến khi cờ `context.battleEnded == true` hoặc số lượt đã qua đạt mốc `maxTurns`. Mỗi lượt hành động của một nhân vật bao gồm các bước sau:
1. Lấy nhân vật nằm ở đầu danh sách liên kết `turnOrder`.
2. Nếu nhân vật này đã bị hạ gục (`alive == false`), bỏ qua hành động của nhân vật và lập tức chuyển sang bước 5.
3. Nếu nhân vật còn sống:
   * Tìm mục tiêu thích hợp theo đúng quy tắc chọn mục tiêu quy định cho từng phe.
   * Nếu nhân vật có đủ lượng năng lượng yêu cầu tiêu tốn của kỹ năng $
ightarrow$ Thực hiện gọi phương thức `specialSkill()`.
   * Ngược lại nếu thiếu năng lượng $
ightarrow$ Thực hiện gọi phương thức `attack()`.
   * Kết thúc chuỗi hành động, gọi phương thức xử lý cuối lượt `endTurn()` của nhân vật.
4. Áp dụng cơ chế sát thương: Nếu mục tiêu là nhân vật, gọi `receiveDamage`. Nếu mục tiêu là công trình, gọi `receiveDamage` (nếu sập lập tức gọi `onDestroyed`).
5. **Đẩy node hiện tại của nhân vật vừa kết thúc lượt xuống vị trí cuối cùng của danh sách liên kết `turnOrder`** để bảo toàn tính xoay vòng lượt đấu (áp dụng cho cả nhân vật còn sống hay đã chết).
6. Gọi phương thức `processBuildings()` để duyệt qua tất cả các công trình hiện có trong mảng `buildings` và thực thi hàm áp dụng hiệu ứng `applyEffect()` (bỏ qua các công trình đã bị phá hủy).
7. Gọi tăng số đếm lượt `context.nextTurn()`.
8. Gọi kiểm tra điều kiện kết thúc `checkEndCondition()`. Nếu chạm mốc `maxTurns` mà các cờ kết thúc khác chưa được bật, gán `battleEnded = true` và `resultCode = "TIME_OUT"`.

### 5.4 Điều Kiện Kiểm Tra Kết Thúc Trận Đấu (`checkEndCondition`)
Các điều kiện phán quyết kết quả trận chiến phải được kiểm tra theo đúng thứ tự ưu tiên từ trên xuống dưới dưới đây:

| Thứ tự ưu tiên | Điều kiện thỏa mãn | Mã kết quả trả về (`resultCode`) |
| :---: | :--- | :--- |
| **1** | `robinRescued == true` và `escapeProgress >= 100` | `STRAW_HAT_WIN` |
| **2** | `busterCallTimer <= 0` | `BUSTER_CALL` |
| **3** | Toàn bộ tất cả nhân vật trong mảng `strawHats` đã bị hạ gục | `CP9_WIN` |
| **4** | Toàn bộ tất cả nhân vật trong mảng `cp9Agents` đã bị hạ gục | `STRAW_HAT_WIN_BY_DEFEAT_CP9` |
| **5** | `turnCount >= maxTurns` | `TIME_OUT` |

---

## 6. Phương Thức Trả Về Kết Quả (`getResult`)
Sau khi vòng lặp kết thúc, hàm trả về một chuỗi thông tin kết quả có định dạng cụ thể:
`resultCode turnCount morale alarmLevel rescueProgress escapeProgress busterCallTimer`

**Ví dụ định dạng đầu ra chuẩn:** `STRAW_HAT_WIN 32 78 25 100 100 4`
