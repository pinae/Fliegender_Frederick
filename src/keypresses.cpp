#include <Arduino.h>

void switchA0(bool state) {
    if (state) {
        Serial.println("A0 ON");
    } else {
        Serial.println("A0 OFF");
    }
}

void switchA1(bool state) {
    if (state) {
        Serial.println("A1 ON");
    } else {
        Serial.println("A1 OFF");
    }
}

void switchA2(bool state) {
    if (state) {
        Serial.println("A2 ON");
    } else {
        Serial.println("A2 OFF");
    }
}

void switchB0(bool state) {
    if (state) {
        Serial.println("B0 ON");
    } else {
        Serial.println("B0 OFF");
    }
}

void switchB1(bool state) {
    if (state) {
        Serial.println("B1 ON");
    } else {
        Serial.println("B1 OFF");
    }
}

void switchB2(bool state) {
    if (state) {
        Serial.println("B2 ON");
    } else {
        Serial.println("B2 OFF");
    }
}

void switchC0(bool state) {
    if (state) {
        Serial.println("C0 ON");
    } else {
        Serial.println("C0 OFF");
    }
}

void switchC1(bool state) {
    if (state) {
        Serial.println("C1 ON");
    } else {
        Serial.println("C1 OFF");
    }
}

void switchD0(bool state) {
    if (state) {
        Serial.println("D0 ON");
    } else {
        Serial.println("D0 OFF");
    }
}

void switchD1(bool state) {
    if (state) {
        Serial.println("D1 ON");
    } else {
        Serial.println("D1 OFF");
    }
}

void trimWheelStepRight() {
    Serial.println(" -->");
    //Consumer.write(MEDIA_VOL_UP);
}

void trimWheelStepLeft() {
    Serial.println("<-- ");
    //Consumer.write(MEDIA_VOL_DOWN);
}

void trimWheelSwitchPress() {
    Serial.println("- SWITCH -");
    //Consumer.write(MEDIA_VOL_MUTE);
}