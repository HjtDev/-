//
// Created by HJT2 on 2/2/2024.
//
#include "Definitions.h"
float pins::controls::AIR_CLEANER_TIMER = 1.0;
float pins::controls::MICROSWITCH_TIMER = 5.0;
int8_t pins::controls::menu = 1;
bool pins::controls::TIMER_TRIGERED = false;
bool pins::controls::air_cleaner = false;
String pins::controls::status = "0000000000000000";
LiquidCrystal_I2C* display = new LiquidCrystal_I2C(0x27, 16, 2);

namespace Icons {
    byte LOADING_START[] = {
        0x0F,
        0x18,
        0x10,
        0x10,
        0x10,
        0x10,
        0x18,
        0x0F
    };
    byte LOADING_START_FILLED[] = {
        0x0F,
        0x18,
        0x13,
        0x17,
        0x17,
        0x13,
        0x18,
        0x0F
    };
    byte LOADING_MIDDLE[] = {
        0x1F,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x1F,
    };
    byte LOADING_MIDDLE_FILLED[] = {
        0x1F,
        0x00,
        0x1F,
        0x1F,
        0x1F,
        0x1F,
        0x00,
        0x1F,
    };
    byte LOADING_END[] = {
        0x1E,
        0x03,
        0x01,
        0x01,
        0x01,
        0x01,
        0x03,
        0x1E,
    };
    byte LOADING_END_FILLED[] = {
        0x1E,
        0x03,
        0x19,
        0x1D,
        0x1D,
        0x19,
        0x03,
        0x1E,
    };
}

inline bool air_cleaner_status() {
    return digitalRead(AIR_CLEANER_OUTPUT);
}
inline bool unit_motor_status() {
    return digitalRead(UNIT_MOTOR_OUTPUT);
}
inline bool pump_up_status() {
    return digitalRead(PUMP_UP_OUTPUT);
}
inline bool pump_down_status() {
    return digitalRead(PUMP_DOWN_OUTPUT);
}
inline MainSelectorStatus main_selector_status() {
    if(!digitalRead(pins::inputs::MAIN_SELECTOR_MANUAL))
        return MainSelectorStatus::MANUAL;
    else if(!digitalRead(pins::inputs::MAIN_SELECTOR_AUTOMATIC))
        return MainSelectorStatus::AUTOMATIC;
    else
        return MainSelectorStatus::MAIN_OFF;
}
inline AutomaticSelectorStatus automatic_selector_status() {
    if(!digitalRead(pins::inputs::AUTO_SELECTOR_PADDLE))
        return AutomaticSelectorStatus::PADDLE;
    else if(!digitalRead(pins::inputs::AUTO_SELECTOR_AUTOMATIC))
        return AutomaticSelectorStatus::AUTO_AUTO;
    else
        return AutomaticSelectorStatus::AUTO_OFF;
}
inline TimerSelector timer_selector_status() {
    if(!digitalRead(pins::inputs::HIGHLEVEL_SELECTOR_TIMER))
        return TimerSelector::Timer;
    else
        return TimerSelector::MICROSWITCH;
}
inline bool start_button_status() {
    return !digitalRead(pins::inputs::MOTOR_START);
}
inline bool stop_button_status() {
    return digitalRead(pins::inputs::MOTOR_STOP);
}
inline bool pump_up_button_status() {
    return !digitalRead(pins::inputs::PUMP_UP);
}
inline bool pump_down_button_status() {
    return !digitalRead(pins::inputs::PUMP_DOWN);
}
inline bool highlevel_microswitch_status() {
    return digitalRead(pins::inputs::HIGHLEVEL_MICTORSWITCH);
}
inline bool lowlevel_microswitch_status() {
    return digitalRead(pins::inputs::LOWLEVEL_MICTORSWITCH);
}
inline bool phase_status() {
    return !digitalRead(pins::inputs::PHASE_CONTROL);
}
inline bool paddle_status() {
    return !digitalRead(pins::inputs::PADDLE);
}

inline void setOutput(OutputPins pin, bool value) {
    digitalWrite(pin, value);
}

inline MenuKey get_menu_key() {
    if(!digitalRead(pins::inputs::MENU_UP))
        return MenuKey::UP;
    if(!digitalRead(pins::inputs::MENU_DOWN))
        return MenuKey::DOWN;
    // if(!digitalRead(pins::inputs::MENU_ENTER))
    //     return MenuKey::ENTER;
    if(!digitalRead(pins::inputs::MENU_NEXT))
        return MenuKey::NEXT;
    return MenuKey::NO_KEY;
}

void update_display() {
    using namespace pins::controls;
    float* selected;
    // print("next", {0, 0}, true, 150);
    if(menu == 2) {
        std::vector<String> data = {"X", "T", "C", "E", "L", "H", "K", "D", "U", "O", "S", "Z", "P", "A", "M", " "};
        String akbar = "";
        akbar += String(!digitalRead(pins::inputs::PADDLE));
        akbar += String(!digitalRead(pins::inputs::HIGHLEVEL_SELECTOR_TIMER));
        akbar += String(!digitalRead(pins::inputs::PHASE_CONTROL));
        akbar += String(!digitalRead(pins::inputs::HIGHLEVEL_SELECTOR_MICROSWITCH));
        akbar += String(!digitalRead(pins::inputs::LOWLEVEL_MICTORSWITCH));
        akbar += String(!digitalRead(pins::inputs::HIGHLEVEL_MICTORSWITCH));
        akbar += String(!digitalRead(pins::inputs::AIR_CLEANER_BUTTON));
        akbar += String(!digitalRead(pins::inputs::PUMP_DOWN));
        akbar += String(!digitalRead(pins::inputs::PUMP_UP));
        akbar += String(!digitalRead(pins::inputs::MOTOR_STOP));
        akbar += String(!digitalRead(pins::inputs::MOTOR_START));
        akbar += String(!digitalRead(pins::inputs::AUTO_SELECTOR_AUTOMATIC));
        akbar += String(!digitalRead(pins::inputs::AUTO_SELECTOR_PADDLE));
        akbar += String(!digitalRead(pins::inputs::MAIN_SELECTOR_AUTOMATIC));
        akbar += String(!digitalRead(pins::inputs::MAIN_SELECTOR_MANUAL));
        if(akbar != status) {
            display->clear();
            for(uint8_t i = 0; i < data.size(); i++) {
                print(data[i], {i, 0}, false);
            }
            print(akbar, {0, 1}, false);
            status = akbar;
        }
    } else if(menu == 3) {
        print("UNIT:" + String(unit_motor_status() ? "ON " : "OFF"), {0, 0}, false);
        print("AIR:" + String(air_cleaner_status() ? "ON " : "OFF"), {9, 0}, false);
        print("UP:" + String(pump_up_status() ? "ON " : "OFF"), {0, 1}, false);
        print("DOWN:" + String(pump_down_status() ? "ON " : "OFF"), {8, 1}, false);
    } else if(menu == 4) {
        selected = &MICROSWITCH_TIMER;
        print("Highlevel Timer:", {0, 0}, false);
        print(*selected, {0, 1}, false, 35);
    } else if(menu == 5) {
        selected = &AIR_CLEANER_TIMER;
        print("Cleaning Time:", {0, 0}, false);
        print(*selected, {0, 1}, false, 35);
    } else if(menu == 1) {
        display->setCursor(1, 0);
        display->print("Sanat Gostaran");
        display->setCursor(0, 1);
        display->print("Ph: 09133088089");
    }
}

//
int calculateSpace(String str) {return ((16 - str.length()) / 2);}

void loading() {
    display->setCursor(2, 1); 
    display->write(specialCharacters::LOADING_START);
    for(uint8_t col = 3; col < 13; col++) {
        display->setCursor(col, 1);
        display->write(specialCharacters::LOADING_MIDDLE);
    }
    display->write(specialCharacters::LOADING_END);
    makeDelay();
    display->setCursor(2, 1);
    display->write(specialCharacters::LOADING_START_FILLED);
    for(uint8_t col = 3; col < 13; col++) {
        makeDelay();
        display->setCursor(col, 1);
        display->write(specialCharacters::LOADING_MIDDLE_FILLED);
    }
    makeDelay();
    display->write(specialCharacters::LOADING_END_FILLED);
    makeDelay();
};
//
void makeDelay() {
    delay(100);
    ulong* rand_input = new ulong(random(0, 750));
    delay(*rand_input);
    delete rand_input;
};

void stage(String str) {
    display->clear();
    uint8_t space = calculateSpace(str);
    display->setCursor(space, 0);
    display->print(str);
    loading();
};

inline String set_status() {
    using namespace pins;
    String status = "";
    status += "00";
    status += String(main_selector_status() == MainSelectorStatus::MANUAL);
    status += String(main_selector_status() == MainSelectorStatus::AUTOMATIC);
    status += String(automatic_selector_status() == AutomaticSelectorStatus::PADDLE);
    status += String(automatic_selector_status() == AutomaticSelectorStatus::AUTO_AUTO);
    status += String(!stop_button_status());   
    status += String(start_button_status());   
    status += String(pump_up_button_status());   
    status += String(pump_down_button_status());   
    status += String(!highlevel_microswitch_status());   
    status += String(!lowlevel_microswitch_status());   
    status += String(phase_status());   
    status += String(timer_selector_status() == TimerSelector::MICROSWITCH);
    status += String(timer_selector_status() == TimerSelector::Timer);
    status += String(paddle_status());   
    // status += String(unit_motor_status());   
    // status += String(pump_up_status());   
    // status += String(pump_down_status());   
    // status += String(air_cleaner_status());   
    if(status != controls::status) {
        controls::status = status;
    }
    return controls::status;
}


inline uint32_t convert_string_to_decimal(String status) {
    uint32_t value = 0;
    uint8_t power = 0;
    for(int8_t i = status.length() - 1; i >= 0; i--) {
        value += (status[i] - '0') * pow(2, power);
        power++;
    }
    return value;
}

template<class data>
void print(data msg, std::vector<uint8_t> cursor, bool clear, unsigned long delay) {
    if(clear) {
        display->clear();
    }
    display->setCursor(cursor[0], cursor[1]);
    display->print(msg);
    vTaskDelay(delay / portTICK_PERIOD_MS);
}

void task1(void* pvParameters) {
    while(true) {
        if(stop_button_status() || !phase_status()) {
            setOutput(UNIT_MOTOR_OUTPUT, LOW);
        }
        if(start_button_status() && phase_status() && !stop_button_status()) {
            setOutput(UNIT_MOTOR_OUTPUT, HIGH);
        } 
    }
}

void task2(void* pvParameters) {
    while(true) {
        /* --------------------------- MAIN SELECTOR MANUAL --------------------------- */    
        if(main_selector_status() == MainSelectorStatus::MANUAL) {
            while(main_selector_status() == MainSelectorStatus::MANUAL && pump_up_button_status() && !highlevel_microswitch_status() && unit_motor_status()) {
                setOutput(PUMP_UP_OUTPUT, HIGH);
                yield();
            }
            setOutput(PUMP_UP_OUTPUT, LOW);
            while(main_selector_status() == MainSelectorStatus::MANUAL && pump_down_button_status() && !lowlevel_microswitch_status() && !digitalRead(pins::inputs::HIGHLEVEL_SELECTOR_MICROSWITCH) && unit_motor_status()) {
                setOutput(PUMP_DOWN_OUTPUT, HIGH);
                yield();
            }
            setOutput(PUMP_DOWN_OUTPUT, LOW);
        /* --------------------------- MAIN SELECTOR MANUAL --------------------------- */ 
        /* --------------------------- MAIN SELECTOR AUTOMATIC --------------------------- */    
        } else if(main_selector_status() == MainSelectorStatus::AUTOMATIC) {
        /* --------------------------- AUTOMATIC SELECTOR PADDLE --------------------------- */    
            if(automatic_selector_status() == AutomaticSelectorStatus::PADDLE) {
                while(!highlevel_microswitch_status() && unit_motor_status() && !pins::controls::TIMER_TRIGERED && automatic_selector_status() == AutomaticSelectorStatus::PADDLE) {
                    setOutput(PUMP_UP_OUTPUT, HIGH);
                    yield();
                }
                setOutput(PUMP_UP_OUTPUT, LOW);
                if(paddle_status()) {
                    pins::controls::TIMER_TRIGERED = false;
                    while(automatic_selector_status() == AutomaticSelectorStatus::PADDLE && !lowlevel_microswitch_status() && !digitalRead(pins::inputs::HIGHLEVEL_SELECTOR_MICROSWITCH) && unit_motor_status()) {
                        setOutput(PUMP_DOWN_OUTPUT, HIGH);
                        yield();
                    }
                    setOutput(PUMP_DOWN_OUTPUT, LOW);
                    if(automatic_selector_status() == AutomaticSelectorStatus::PADDLE) {
                        pins::controls::air_cleaner = true;
                    }
                    yield();
                    uint32_t current_time = millis();
                    while(automatic_selector_status() == AutomaticSelectorStatus::PADDLE && !highlevel_microswitch_status() && unit_motor_status()) {
                        if(timer_selector_status() != TimerSelector::Timer) {
                            setOutput(PUMP_UP_OUTPUT, HIGH);
                        } else {
                            if(millis() - current_time <= int(pins::controls::MICROSWITCH_TIMER * 1000)) {
                                setOutput(PUMP_UP_OUTPUT, HIGH);
                            } else {
                                // display->clear();
                                // display->setCursor(0, 0);
                                // display->print("Timer");
                                setOutput(PUMP_UP_OUTPUT, LOW);
                                pins::controls::TIMER_TRIGERED = true;
                                break;
                            }
                        }
                        yield();
                    }
                    // display->clear();
                    // display->setCursor(0, 0);
                    // display->print("End Paddle");
                    setOutput(PUMP_UP_OUTPUT, LOW);
                    // while(paddle_status()){yield();}
                }
        /* --------------------------- AUTOMATIC SELECTOR AUTOMATIC --------------------------- */
            } else if(automatic_selector_status() == AutomaticSelectorStatus::AUTO_AUTO) {
                uint32_t current_time = millis();
                while(automatic_selector_status() == AutomaticSelectorStatus::AUTO_AUTO && !highlevel_microswitch_status() && unit_motor_status()) {
                    if(timer_selector_status() != TimerSelector::Timer) {
                        setOutput(PUMP_UP_OUTPUT, HIGH);
                    } else {
                        if(millis() - current_time <= int(pins::controls::MICROSWITCH_TIMER * 1000)) {
                            setOutput(PUMP_UP_OUTPUT, HIGH);
                        } else {
                            // display->clear();
                            // display->setCursor(0, 0);
                            // display->print("Timer");
                            setOutput(PUMP_UP_OUTPUT, LOW);
                            pins::controls::TIMER_TRIGERED = false;
                            break;
                        }
                    }
                    yield();
                }
                setOutput(PUMP_UP_OUTPUT, LOW);
                pins::controls::TIMER_TRIGERED = false;
                while(automatic_selector_status() == AutomaticSelectorStatus::AUTO_AUTO && !lowlevel_microswitch_status() && !digitalRead(pins::inputs::HIGHLEVEL_SELECTOR_MICROSWITCH) &&unit_motor_status()) {
                    setOutput(PUMP_DOWN_OUTPUT, HIGH);
                    yield();
                }
                setOutput(PUMP_DOWN_OUTPUT, LOW);
                if(automatic_selector_status() == AutomaticSelectorStatus::AUTO_AUTO) {
                    pins::controls::air_cleaner = true;
                }
                yield();
            } else if(automatic_selector_status() == AutomaticSelectorStatus::AUTO_OFF) {
                yield();
            }
        /* --------------------------- AUTOMATIC SELECTOR OFF --------------------------- */
        }
        if(main_selector_status() == MainSelectorStatus::MAIN_OFF || automatic_selector_status() == AutomaticSelectorStatus::AUTO_OFF) {
            setOutput(PUMP_UP_OUTPUT, LOW);
            setOutput(PUMP_DOWN_OUTPUT, LOW);
            // setOutput(AIR_CLEANER_OUTPUT, LOW);
        }
    }
}

void task3(void* pvParameters) {
    using namespace pins::controls;
    float* selected;
    while(true) {
        // print("task3", {0, 0}, true, 150);
        if(menu == 4) {
            selected = &MICROSWITCH_TIMER;
        }
        if(menu == 5) {
            selected = &AIR_CLEANER_TIMER;
        }
        if(get_menu_key() == MenuKey::UP && (menu == 5 || menu == 4)) {
            // print("up", {0, 0}, true, 150);
            display->clear();
            status = "";
            *selected += .1;
            controlVariable(*selected, 0.1, 9.9);
            // update_display();
        }
        if(get_menu_key() == MenuKey::DOWN && (menu == 5 || menu == 4)) {
            // print("down", {0, 0}, true, 150);
            display->clear();
            status = "";
            *selected -= .1;
            controlVariable(*selected, 0.1, 9.9);
            // update_display();
        }
        if(get_menu_key() == MenuKey::NEXT) {
            // print("next", {0, 0}, true, 150);
            display->clear();
            status = "";
            uint32_t current = millis();
            while(get_menu_key() == MenuKey::NEXT) {
                if(millis() - current >= 3000) {
                    writeByte(0x1000, int(pins::controls::AIR_CLEANER_TIMER * 10));
                    writeByte(0x2000, int(pins::controls::MICROSWITCH_TIMER * 10));
                    print("   Saved Data   ", {0, 0}, true, 1000);
                    break;
                }   
            }
            menu++;
            controlVariable(menu, 1, 5);
            // update_display();
        }
        update_display();
        vTaskDelay(40 / portTICK_PERIOD_MS);
        // if(get_menu_key() == MenuKey::ENTER) {
        //     // nothing to do
        //     // update_display();
        // }
        // float* selected = (pins::controls::menu ? &pins::controls::AIR_CLEANER_TIMER : &pins::controls::MICROSWITCH_TIMER);
        // if(get_menu_key() == MenuKey::UP && *selected < 9.9) {
        //     *selected += .1;
        //     update_display();
        // }
        // if(get_menu_key() == MenuKey::DOWN && *selected > 0.0) {
        //     *selected -= .1;
        //     update_display();
        // }
        // if(get_menu_key() == MenuKey::NEXT) {
        //     uint32_t current = millis();
        //     while(get_menu_key() == MenuKey::NEXT) {
        //         if(millis() - current >= 3000) {
        //             writeByte(0x1000, int(pins::controls::AIR_CLEANER_TIMER * 10));
        //             writeByte(0x2000, int(pins::controls::MICROSWITCH_TIMER * 10));
        //             // display->clear();
        //             // display->setCursor(0, 0);
        //             // display->print("Save");
        //             vTaskDelay(1500 / portTICK_PERIOD_MS);
        //             break;
        //         }   
        //     }
        //     pins::controls::menu = !pins::controls::menu;
        //     update_display();
        // }
        // if(get_menu_key() == MenuKey::ENTER && *selected < 9.9) {
        //     // nothing to do
        //     update_display();
        // }


        // Working
        // String akbar = "00";
        // akbar += String(!digitalRead(pins::inputs::MAIN_SELECTOR_MANUAL));
        // akbar += String(!digitalRead(pins::inputs::MAIN_SELECTOR_AUTOMATIC));
        // akbar += String(!digitalRead(pins::inputs::AUTO_SELECTOR_PADDLE));
        // akbar += String(!digitalRead(pins::inputs::AUTO_SELECTOR_AUTOMATIC));
        // akbar += String(!digitalRead(pins::inputs::MOTOR_START));
        // akbar += String(!digitalRead(pins::inputs::MOTOR_STOP));
        // akbar += String(!digitalRead(pins::inputs::PUMP_UP));
        // akbar += String(!digitalRead(pins::inputs::PUMP_DOWN));
        // akbar += String(!digitalRead(pins::inputs::HIGHLEVEL_MICTORSWITCH));
        // akbar += String(!digitalRead(pins::inputs::LOWLEVEL_MICTORSWITCH));
        // akbar += String(!digitalRead(pins::inputs::PHASE_CONTROL));
        // akbar += String(!digitalRead(pins::inputs::HIGHLEVEL_SELECTOR_TIMER));
        // akbar += String(!digitalRead(pins::inputs::HIGHLEVEL_SELECTOR_MICROSWITCH));
        // akbar += String(!digitalRead(pins::inputs::PADDLE));
        // display->setCursor(0, 0);
        // display->print(digitalRead(pins::outputs::OUTPUT1));
        // display->print(digitalRead(pins::outputs::OUTPUT2));
        // display->print(digitalRead(pins::outputs::OUTPUT3));
        // display->print(digitalRead(pins::outputs::OUTPUT4));
        // display->setCursor(0, 1);
        // display->print(akbar);
        // vTaskDelay(1000 / portTICK_PERIOD_MS);


        // akbar += String(digitalRead(pins::inputs::MAIN_SELECTOR_MANUAL));
        // akbar += String(digitalRead(pins::inputs::MAIN_SELECTOR_AUTOMATIC));
        // akbar += String(digitalRead(pins::inputs::AUTO_SELECTOR_PADDLE));
        // akbar += String(digitalRead(pins::inputs::AUTO_SELECTOR_AUTOMATIC));
        // akbar += String(digitalRead(pins::inputs::MOTOR_START));
        // akbar += String(digitalRead(pins::inputs::MOTOR_STOP));
        // akbar += String(digitalRead(pins::inputs::PUMP_UP));
        // akbar += String(digitalRead(pins::inputs::PUMP_DOWN));
        // akbar += String(digitalRead(pins::inputs::HIGHLEVEL_MICTORSWITCH));
        // akbar += String(digitalRead(pins::inputs::LOWLEVEL_MICTORSWITCH));
        // akbar += String(digitalRead(pins::inputs::PHASE_CONTROL));
        // akbar += String(digitalRead(pins::inputs::HIGHLEVEL_SELECTOR_TIMER));
        // akbar += String(digitalRead(pins::inputs::HIGHLEVEL_SELECTOR_MICROSWITCH));
        // akbar += String(digitalRead(pins::inputs::PADDLE));

        

        // digitalWrite(pins::controls::latch, LOW);
        // shiftOut(pins::controls::data_pin, pins::controls::clock_pin, LSBFIRST, convert_string_to_decimal(akbar) >> 8);
        // shiftOut(pins::controls::data_pin, pins::controls::clock_pin, LSBFIRST, convert_string_to_decimal(akbar));
        // digitalWrite(pins::controls::latch, HIGH);
        // vTaskDelay(100 / portTICK_PERIOD_MS);

    }
}

void air_cleaner(void* pvParameters) {
    while(true) {
        while(!digitalRead(pins::inputs::AIR_CLEANER_BUTTON)) {
            setOutput(AIR_CLEANER_OUTPUT, HIGH);
        }
        setOutput(AIR_CLEANER_OUTPUT, LOW);
        if(pins::controls::air_cleaner) {
            uint32_t current_time = millis();
            while(millis() - current_time <= int(pins::controls::AIR_CLEANER_TIMER * 1000) && unit_motor_status()) {
                setOutput(AIR_CLEANER_OUTPUT, HIGH);
                yield();
            }
            setOutput(AIR_CLEANER_OUTPUT, LOW);
            pins::controls::air_cleaner = false;
        } else {
            setOutput(AIR_CLEANER_OUTPUT, LOW);
        }
    }
}

void writeByte(uint32_t address, uint8_t value) {
    uint8_t r = readByte(address);
    if(r == value) {
        return;
    }
    eraseSector(address);
    digitalWrite(pins::controls::chipSelectPin, LOW);  // Select the chip

    SPI.transfer(0x06);  // Write Enable command
    digitalWrite(pins::controls::chipSelectPin, HIGH);  // Deselect the chip
    delayMicroseconds(10);

    digitalWrite(pins::controls::chipSelectPin, LOW);  // Select the chip
    SPI.transfer(0x02);  // Page Program command
    SPI.transfer((address >> 16) & 0xFF);  // Send the address bytes
    SPI.transfer((address >> 8) & 0xFF);
    SPI.transfer(address & 0xFF);

    SPI.transfer(value);

    digitalWrite(pins::controls::chipSelectPin, HIGH);  // Deselect the chip
    delay(5);  // Page program time
}

uint8_t readByte(uint32_t address) {
    digitalWrite(pins::controls::chipSelectPin, LOW);  // Select the chip

    SPI.transfer(0x03);  // Read command
    SPI.transfer((address >> 16) & 0xFF);  // Send the address bytes
    SPI.transfer((address >> 8) & 0xFF);
    SPI.transfer(address & 0xFF);

    uint8_t readValue = SPI.transfer(0x00);  // Send dummy byte to read data

    digitalWrite(pins::controls::chipSelectPin, HIGH);  // Deselect the chip
    return readValue;
}

void eraseSector(uint32_t sectorAddress) {
    digitalWrite(pins::controls::chipSelectPin, LOW);  // Select the chip

    SPI.transfer(0x06);  // Write Enable command
    digitalWrite(pins::controls::chipSelectPin, HIGH);  // Deselect the chip
    delayMicroseconds(10);

    digitalWrite(pins::controls::chipSelectPin, LOW);  // Select the chip
    SPI.transfer(0x20);  // Sector Erase command
    SPI.transfer((sectorAddress >> 16) & 0xFF);  // Send the sector address bytes
    SPI.transfer((sectorAddress >> 8) & 0xFF);
    SPI.transfer(sectorAddress & 0xFF);

    digitalWrite(pins::controls::chipSelectPin, HIGH);  // Deselect the chip
    delay(100);  // Sector erase time
}