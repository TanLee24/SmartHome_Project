#include "door_control.h"

Servo doorServo;
IRrecv irrecv(IR_PIN);
decode_results results;

String current_password = "123"; // Mật khẩu mặc định
String input_buffer = "";        // Nơi lưu các phím vừa bấm

// Định nghĩa các trạng thái của hệ thống
enum SystemState 
{
    NORMAL_MODE,
    CHANGE_PASS_MODE
};
SystemState currentState = NORMAL_MODE;

// Hàm chuyển đổi mã Hex từ Remote thành ký tự. 
// LƯU Ý: Các mã 0xFF... bên dưới chỉ là ví dụ. Bạn cần bật Serial Monitor, 
// bấm remote của bạn và thay các mã Hex thực tế in ra vào đây!
char decodeIR(uint32_t code) {
    switch (code) {
        // --- NHÓM PHÍM SỐ --- (Nhớ thay mã Hex thực tế của bạn vào đây)
        case 0xFF6897: return '0'; 
        case 0xFF30CF: return '1';
        case 0xFF18E7: return '2';
        case 0xFF7A85: return '3';
        case 0xFF10EF: return '4';
        case 0xFF38C7: return '5';
        case 0xFF5AA5: return '6';
        case 0xFF42BD: return '7';
        case 0xFF4AB5: return '8';
        case 0xFF52AD: return '9';
        
        // --- NHÓM PHÍM CHỮ CHỨC NĂNG ---
        case 0xFFA25D: return 'A'; // Nút A -> Xác nhận mở cửa
        case 0xFF629D: return 'B'; // Nút B -> Đổi mật khẩu
        case 0xFFE21D: return 'C'; // Nút C -> Xóa bộ đệm (Clear)
        
        // Các phím D, E, F tạm thời chưa dùng đến
        
        default: return '?';
    }
}

void doorControl(void *pvParameters) 
{
    // Khởi tạo Servo (ESP32 cần phân bổ timer cho PWM)
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);
    doorServo.setPeriodHertz(50); // Servo chạy ở tần số 50Hz
    doorServo.attach(SERVO_PIN, 500, 2400); 
    doorServo.write(0); // Ban đầu cho chốt cửa đóng (0 độ)

    // Khởi tạo mắt thu IR
    irrecv.enableIRIn();

    while (1) {
        if (irrecv.decode(&results)) {
            Serial.printf("IR Code Nhận Được: 0x%06X\n", results.value);
            
            char key = decodeIR(results.value);
            
            if (key != '?') {
                Serial.print("Phím bấm: "); Serial.println(key);
                
                // Nút C (Xóa/Clear) có tác dụng ở mọi trạng thái
                if (key == 'C') {
                    input_buffer = "";
                    Serial.println("Đã xóa trắng các số vừa nhập!");
                }
                // --- TRẠNG THÁI 1: BÌNH THƯỜNG ---
                else if (currentState == NORMAL_MODE) {
                    if (key >= '0' && key <= '9') {
                        input_buffer += key;
                    } 
                    else if (key == 'A') { // Bấm A để XÁC NHẬN MỞ CỬA
                        if (input_buffer == current_password) {
                            Serial.println("Mật khẩu đúng! Mở cửa...");
                            doorServo.write(90); 
                            vTaskDelay(pdMS_TO_TICKS(3000)); 
                            doorServo.write(0);  
                            Serial.println("Đã tự động đóng cửa.");
                        } else {
                            Serial.println("Mật khẩu sai!");
                        }
                        input_buffer = ""; 
                    }
                    else if (key == 'B') { // Bấm B để YÊU CẦU ĐỔI MẬT KHẨU
                        if (input_buffer == (current_password + current_password)) {
                            Serial.println("=> CHẾ ĐỘ ĐỔI MẬT KHẨU. Nhập pass mới và nhấn A.");
                            currentState = CHANGE_PASS_MODE; 
                        } else {
                            Serial.println("Sai cú pháp đổi mật khẩu (Cần nhập pass cũ 2 lần)!");
                        }
                        input_buffer = ""; 
                    }
                } 
                // --- TRẠNG THÁI 2: ĐỔI MẬT KHẨU ---
                else if (currentState == CHANGE_PASS_MODE) {
                    if (key >= '0' && key <= '9') {
                        input_buffer += key;
                    }
                    else if (key == 'A') { // Bấm A để LƯU MẬT KHẨU MỚI
                        if (input_buffer.length() > 0) {
                            current_password = input_buffer;
                            Serial.print("Đổi mật khẩu thành công! Mật khẩu mới là: ");
                            Serial.println(current_password);
                        }
                        currentState = NORMAL_MODE; 
                        input_buffer = "";
                    }
                }
            }
            irrecv.resume(); 
        }
        vTaskDelay(pdMS_TO_TICKS(50)); 
    }
}