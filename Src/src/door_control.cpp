#include "door_control.h"

Servo doorServo;
IRrecv irrecv(IR_PIN);
decode_results results;

String current_password = "123"; 
String input_buffer = "";        

// Định nghĩa 4 trạng thái của hệ thống
enum SystemState {
    IDLE_MODE,                  // Chờ lệnh (Không làm gì cả)
    ENTER_PASS_TO_OPEN,         // Đang nhập mật khẩu mở cửa
    ENTER_OLD_PASS_TO_CHANGE,   // Đang nhập pass cũ để xác thực đổi pass
    ENTER_NEW_PASS              // Đang nhập pass mới
};
SystemState currentState = IDLE_MODE;

// Hàm giải mã (Bạn nhớ thay các mã Hex thực tế của remote vào đây nhé)
char decodeIR(uint32_t code) {
    switch (code) {
        // --- NHÓM PHÍM SỐ --- 
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
        case 0xFFA25D: return 'A'; // Nút A -> Kích hoạt chế độ mở cửa
        case 0xFF629D: return 'B'; // Nút B -> Kích hoạt chế độ đổi pass
        case 0xFFE21D: return 'C'; // Nút C -> Hủy bỏ (Cancel/Clear)
        case 0xFF22DD: return 'D'; // Nút D -> Xác nhận (Confirm/Enter)
        
        default: return '?';
    }
}

void doorControl(void *pvParameters) {
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);
    doorServo.setPeriodHertz(50); 
    doorServo.attach(SERVO_PIN, 500, 2400); 
    doorServo.write(0); // Đóng cửa

    irrecv.enableIRIn();
    Serial.println("--- HỆ THỐNG KHÓA CỬA ĐÃ SẴN SÀNG ---");

    while (1) {
        if (irrecv.decode(&results)) {
            char key = decodeIR(results.value);
            
            if (key != '?') {
                Serial.print("Phím bấm: "); Serial.println(key);
                
                // Nút C luôn có tác dụng HỦY BỎ thao tác đang làm dở và trở về trạng thái nghỉ
                if (key == 'C') {
                    input_buffer = "";
                    currentState = IDLE_MODE;
                    Serial.println("Đã hủy thao tác! Trở về trạng thái chờ lệnh.");
                }
                
                // --- TRẠNG THÁI 0: CHỜ LỆNH ---
                else if (currentState == IDLE_MODE) {
                    if (key == 'A') {
                        Serial.println("=> CHẾ ĐỘ MỞ CỬA: Mời nhập mật khẩu, sau đó nhấn D để xác nhận.");
                        input_buffer = "";
                        currentState = ENTER_PASS_TO_OPEN;
                    } 
                    else if (key == 'B') {
                        Serial.println("=> CHẾ ĐỘ ĐỔI MẬT KHẨU: Mời nhập mật khẩu CŨ để xác minh, sau đó nhấn D.");
                        input_buffer = "";
                        currentState = ENTER_OLD_PASS_TO_CHANGE;
                    }
                }
                
                // --- TRẠNG THÁI 1: NHẬP PASS MỞ CỬA ---
                else if (currentState == ENTER_PASS_TO_OPEN) {
                    if (key >= '0' && key <= '9') {
                        input_buffer += key;
                        Serial.print("*"); // In dấu sao che mật khẩu trên màn hình
                    } 
                    else if (key == 'D') { // Bấm D để Xác nhận
                        Serial.println(); // Xuống dòng
                        if (input_buffer == current_password) {
                            Serial.println("Mật khẩu đúng! Đang mở cửa...");
                            doorServo.write(90); 
                            vTaskDelay(pdMS_TO_TICKS(3000)); 
                            doorServo.write(0);  
                            Serial.println("Đã chốt cửa.");
                        } else {
                            Serial.println("SAI MẬT KHẨU!");
                        }
                        currentState = IDLE_MODE; // Xong việc thì về trạng thái chờ
                        input_buffer = ""; 
                    }
                }
                
                // --- TRẠNG THÁI 2: XÁC MINH PASS CŨ ---
                else if (currentState == ENTER_OLD_PASS_TO_CHANGE) {
                    if (key >= '0' && key <= '9') {
                        input_buffer += key;
                        Serial.print("*");
                    }
                    else if (key == 'D') {
                        Serial.println();
                        if (input_buffer == current_password) {
                            Serial.println("Xác minh thành công! Mời nhập mật khẩu MỚI, sau đó nhấn D.");
                            currentState = ENTER_NEW_PASS;
                        } else {
                            Serial.println("SAI MẬT KHẨU CŨ! Đã hủy thao tác đổi pass.");
                            currentState = IDLE_MODE;
                        }
                        input_buffer = "";
                    }
                }

                // --- TRẠNG THÁI 3: LƯU PASS MỚI ---
                else if (currentState == ENTER_NEW_PASS) {
                    if (key >= '0' && key <= '9') {
                        input_buffer += key;
                        Serial.print(key); // Hiển thị số đang nhập để chắc chắn không bấm nhầm
                    }
                    else if (key == 'D') {
                        Serial.println();
                        if (input_buffer.length() > 0) {
                            current_password = input_buffer;
                            Serial.print("Đổi mật khẩu THÀNH CÔNG! Mật khẩu mới là: ");
                            Serial.println(current_password);
                        } else {
                            Serial.println("Mật khẩu mới không được để trống!");
                        }
                        currentState = IDLE_MODE; 
                        input_buffer = "";
                    }
                }
            }
            irrecv.resume(); 
        }
        vTaskDelay(pdMS_TO_TICKS(50)); 
    }
}