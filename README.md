<h1 align="center">🔧 THỰC HÀNH IT4210/IT4210E - HỆ NHÚNG</h1>

<p align="center">
  <b>Tổng hợp các project thực hành Hệ nhúng trên STM32F429I-DISC1</b><br>
  GPIO • Timer • Interrupt • UART • I2C • SPI • RFID • OLED • FreeRTOS • TouchGFX
</p>

<p align="center">
  <img alt="MCU" src="https://img.shields.io/badge/MCU-STM32F429-red">
  <img alt="IDE" src="https://img.shields.io/badge/IDE-STM32CubeIDE-blue">
  <img alt="Framework" src="https://img.shields.io/badge/GUI-TouchGFX-purple">
  <img alt="RTOS" src="https://img.shields.io/badge/RTOS-FreeRTOS-green">
  <img alt="Language" src="https://img.shields.io/badge/Language-C%2FC%2B%2B-orange">
</p>

---

## 📌 Giới thiệu

Repo này lưu lại các bài thực hành, bài tự làm và một số project mở rộng trong môn **Hệ nhúng IT4210/IT4210E**.

Mục tiêu chính của repo:

- Lưu trữ mã nguồn các bài thực hành trên kit **STM32F429I-DISC1 / STM32F429ZIT6**.
- Ghi lại quá trình làm việc với các ngoại vi cơ bản và nâng cao của vi điều khiển.
- Tổng hợp các project trên lớp và các project tự mở rộng để dễ tra cứu, build lại và phát triển tiếp.
- Làm tài liệu tham khảo cá nhân khi học STM32, FreeRTOS và TouchGFX.

> Repo phục vụ mục đích học tập cá nhân. Đây không phải tài liệu chính thức của nhà trường hoặc bất kỳ tổ chức nào.

---

## 🧰 Phần cứng sử dụng

| Nhóm | Thiết bị / module |
|---|---|
| Kit chính | STM32F429I-DISC1 / STM32F429ZIT6 |
| Hiển thị | LED đơn, LED 7 thanh, OLED SH1106, LCD tích hợp trên kit |
| Giao tiếp | UART, I2C, SPI, USB HID |
| Cảm biến / ngoại vi | HS0038 IR receiver, RC522 RFID, Tiny RTC DS1307 + AT24C32, HC-SR04, servo, joystick / ADC |
| Dụng cụ | Breadboard, dây cắm, điện trở, transistor, module nguồn |

---

## 🛠️ Phần mềm sử dụng

| Công cụ | Mục đích |
|---|---|
| STM32CubeIDE | Tạo project, cấu hình ngoại vi, build và debug firmware |
| STM32CubeMX | Cấu hình clock, GPIO, timer, UART, I2C, SPI, middleware |
| TouchGFX Designer | Thiết kế giao diện đồ họa cho STM32F429 |
| FreeRTOS / CMSIS-RTOS | Lập trình đa nhiệm, task, queue |
| Hercules | Theo dõi dữ liệu UART từ kit gửi về PC |

---

## 📁 Cấu trúc repo

```txt
THUCHANH-IT4210-HENHUNG/
├─ lab1_30_manual/                 # Project mẫu / khởi động Lab 1
├─ lab31_led8_pd8_pd15/             # LED đơn PD8-PD15
├─ lab32_2x7seg_ledbar/             # LED 7 thanh + LED bar
├─ lab33_ir_nec_final/              # Giải mã remote hồng ngoại NEC
│
├─ lab02_ds1307/                    # DS1307 RTC cơ bản
├─ lab02_ds1307_step36_ok/          # Bước 3.6 - DS1307
├─ lab02_ds1307_step37_ok/          # Bước 3.7 - SH1106 OLED
├─ lab02_ds1307_step38_ok/          # Bước 3.8 - RC522 RFID
├─ lab02_ds1307_step39_basic_ok/    # Bước 3.9 - bài tổng hợp RFID door log
│
├─ lab3_0_freertos/                 # Làm quen FreeRTOS
├─ HelloTouchGFX/                   # Project TouchGFX cơ bản
├─ TouchGFX_LedButton/              # TouchGFX điều khiển LED
├─ StopwatchTouchGFX/               # Đồng hồ bấm giây TouchGFX
├─ lab3_4_racing_touchgfx/          # Game đua xe TouchGFX
│
├─ STM32F429-AnalogRead/            # Đọc ADC / analog input
├─ STM32F429-FreeRTOS-Multitask/    # FreeRTOS nhiều task
├─ STM32F429-FreeRTOS_TaskQueue/    # FreeRTOS queue
├─ STM32F429-Pong/                  # Game Pong trên STM32
├─ STM32F429-TouchGFX_Led/          # Demo TouchGFX LED
├─ TETRIS-SNAKE-STM32F4/            # Game Tetris / Snake
├─ USB_HID/                         # Thử nghiệm USB HID
├─ lab03_hcsr04_once/               # HC-SR04
├─ radar_test_fix_ui_sweep/         # Radar UI / sweep test
├─ servo_clean_test/                # Test servo
└─ README.md
```

---

## 🧪 Nội dung các bài thực hành chính

### 🟦 Bài thực hành 1 - GPIO, Interrupt, Timer

Nội dung chính:

- Cấu hình GPIO output cho dãy LED đơn.
- Điều khiển 8 LED trên các chân PD8-PD15.
- Ghép nối 2 LED 7 thanh.
- Sử dụng timer để quét LED 7 thanh.
- Xử lý nút bấm B1 bằng interrupt.
- Tìm hiểu và giải mã tín hiệu remote hồng ngoại theo chuẩn NEC.
- Hiển thị mã nút bấm lên LED 7 thanh.
- Bật/tắt LED hoặc đổi hiệu ứng LED theo lệnh remote.

Một số project liên quan:

| Thư mục | Nội dung |
|---|---|
| `lab1_30_manual` | Project mẫu / kiểm tra cấu hình ban đầu |
| `lab31_led8_pd8_pd15` | Điều khiển dãy 8 LED đơn |
| `lab32_2x7seg_ledbar` | LED 7 thanh và LED bar |
| `lab33_ir_nec_final` | Giải mã remote hồng ngoại NEC |

---

### 🟩 Bài thực hành 2 - Ghép nối nối tiếp

Nội dung chính:

- Tìm hiểu bus **I2C**, **SPI**, **UART**.
- Ghép nối module **Tiny RTC DS1307 + AT24C32**.
- Đọc và đặt thời gian thực từ DS1307.
- Hiển thị dữ liệu lên màn hình **OLED SH1106**.
- Ghép nối module **RFID RC522** qua SPI.
- Đọc mã thẻ RFID 13.56 MHz.
- Xây dựng ứng dụng mô phỏng đóng/mở cửa bằng RFID.
- Lưu log mở cửa gồm thời gian và mã thẻ.
- Giao tiếp với PC qua UART/Hercules để xem log hoặc cấu hình mã thẻ.

Một số project liên quan:

| Thư mục | Nội dung |
|---|---|
| `lab02_ds1307` | DS1307 cơ bản |
| `lab02_ds1307_step36_ok` | Đọc/ghi thời gian DS1307 |
| `lab02_ds1307_step37_ok` | Hiển thị dữ liệu lên OLED SH1106 |
| `lab02_ds1307_step38_ok` | Đọc thẻ RFID RC522 |
| `lab02_ds1307_step39_basic_ok` | Bài tổng hợp mô phỏng hệ thống mở cửa |

---

### 🟥 Bài thực hành 3 - FreeRTOS và TouchGFX

Nội dung chính:

- Làm quen với **FreeRTOS** trên STM32F429.
- Tạo nhiều task và quan sát hoạt động đa nhiệm.
- Gửi dữ liệu qua UART từ task người dùng.
- Sử dụng queue để truyền dữ liệu giữa task xử lý phần cứng và giao diện.
- Làm quen với **TouchGFX Designer**.
- Tạo giao diện điều khiển LED trên LCD.
- Xử lý sự kiện button và tick event trong TouchGFX.
- Xây dựng đồng hồ bấm giây bằng Texture Mapper.
- Điều khiển animation bằng nút USER_BUTTON.
- Mở rộng với game / giao diện đồ họa như Racing, Pong, Tetris, Snake.

Một số project liên quan:

| Thư mục | Nội dung |
|---|---|
| `lab3_0_freertos` | Làm quen FreeRTOS |
| `HelloTouchGFX` | Project TouchGFX đầu tiên |
| `TouchGFX_LedButton` | Button trên giao diện điều khiển LED |
| `StopwatchTouchGFX` | Đồng hồ bấm giây TouchGFX |
| `lab3_4_racing_touchgfx` | Game đua xe với TouchGFX |
| `STM32F429-FreeRTOS-Multitask` | Demo nhiều task |
| `STM32F429-FreeRTOS_TaskQueue` | Demo queue trong FreeRTOS |

---

## 🚀 Cách mở project trong STM32CubeIDE

### Cách 1: Import project có sẵn

1. Mở **STM32CubeIDE**.
2. Chọn:

```txt
File → Import → General → Existing Projects into Workspace
```

3. Ở phần **Select root directory**, chọn thư mục project cần mở, ví dụ:

```txt
lab33_ir_nec_final
```

4. Tick project xuất hiện trong danh sách.
5. Bấm **Finish**.
6. Build project bằng:

```txt
Project → Build Project
```

7. Nạp xuống kit bằng nút **Run** hoặc **Debug**.

---

### Cách 2: Mở project TouchGFX

Với các project TouchGFX, thường cần mở đúng project nằm trong thư mục STM32CubeIDE:

```txt
<ProjectName>/STM32CubeIDE/.project
```

Hoặc import thư mục:

```txt
<ProjectName>/STM32CubeIDE
```

Sau khi chỉnh giao diện trong TouchGFX Designer, cần bấm:

```txt
Generate Code / F4
```

rồi quay lại STM32CubeIDE để build và nạp chương trình.

---

## ✅ Kết quả mong đợi

| Bài | Kết quả |
|---|---|
| Lab 1 | LED đơn chạy hiệu ứng, LED 7 thanh hiển thị số, remote IR điều khiển được LED / hiển thị mã phím |
| Lab 2 | Đọc thời gian DS1307, hiển thị OLED SH1106, đọc mã thẻ RFID RC522, mô phỏng hệ thống mở cửa |
| Lab 3 | Chạy FreeRTOS nhiều task, giao diện TouchGFX hoạt động, button điều khiển LED, stopwatch / game chạy trên LCD |

---

## 🧠 Kiến thức đã luyện tập

- Cấu hình clock STM32F4.
- Lập trình GPIO input/output.
- Xử lý external interrupt.
- Sử dụng timer và timer interrupt.
- Giao tiếp UART với Hercules.
- Giao tiếp I2C với RTC và OLED.
- Giao tiếp SPI với RFID RC522.
- Tổ chức code theo project STM32CubeIDE.
- Lập trình HAL driver.
- Làm việc với FreeRTOS task và queue.
- Thiết kế giao diện nhúng bằng TouchGFX.
- Kết hợp phần cứng và giao diện đồ họa.

---

## ⚠️ Lưu ý khi build / nạp code

- Kiểm tra đúng board và chip đang dùng: **STM32F429I-DISC1 / STM32F429ZIT6**.
- Với project dùng breadboard, cần kiểm tra kỹ **VCC** và **GND** trước khi cấp nguồn.
- Một số project có thể phụ thuộc vào đường dẫn workspace hoặc version STM32CubeIDE/TouchGFX.
- Nếu project TouchGFX lỗi build, hãy mở TouchGFX Designer và generate code lại.
- Với UART, kiểm tra đúng COM port và baudrate trong Hercules.
- Với OLED SH1106, chú ý một số module có thứ tự chân **SCL/SDA** khác nhau.

---

## 👤 Tác giả

**Pham Ngoc Hung**

- GitHub: `@phamhungcrab`
- Mục đích repo: lưu trữ bài thực hành, bài tự làm và project học tập môn Hệ nhúng.

---

## 📄 License / bản quyền

Repo này dùng cho mục đích học tập cá nhân. Nếu sử dụng lại code hoặc tài liệu từ repo, vui lòng ghi nguồn phù hợp.

---

## ⭐ Ghi chú

Repo vẫn có thể tiếp tục được cập nhật thêm các project nhỏ, bài test phần cứng, game TouchGFX hoặc demo ngoại vi STM32F4 khác.
