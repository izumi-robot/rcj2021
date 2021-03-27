#include <robo2019.h>

namespace omv {
    using namespace robo::openmv;
}

omv::Reader reader(0x12);

void setup() {
    Serial.begin(9600);
    reader.setup();
}

void loop() {
    omv::Frame * frame = reader.read_frame();
    if (frame != NULL) {
        Serial.println(F("No frame"));
    } else {
        char buff[128] = "";
        frame->to_string(buff);
        Serial.println(buff);
    }
    delay(100);
}