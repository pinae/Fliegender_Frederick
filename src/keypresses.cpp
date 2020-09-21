#include <Arduino.h>
#include <HID-Project.h>

void keypressInit() {
    Keyboard.begin();
    //Consumer.begin();
}

void switchA0(bool state) {
    if (state) {
        Serial.println("A0 ON");
        //Keyboard.press(KEY_LEFT_CTRL);
        Keyboard.press(KEY_LEFT_ALT);
        Keyboard.press('A');
        Keyboard.releaseAll();
    } else {
        Serial.println("A0 OFF");
        //Keyboard.press(KEY_LEFT_CTRL);
        Keyboard.press(KEY_LEFT_ALT);
        Keyboard.press(KEY_RIGHT_SHIFT);
        Keyboard.press('A');
        Keyboard.releaseAll();
    }
}

void switchA1(bool state) {
    if (state) {
        Serial.println("A1 ON");
        //Keyboard.press(KEY_LEFT_CTRL);
        Keyboard.press(KEY_LEFT_ALT);
        Keyboard.press('B');
        Keyboard.releaseAll();
    } else {
        Serial.println("A1 OFF");
        //Keyboard.press(KEY_LEFT_CTRL);
        Keyboard.press(KEY_LEFT_ALT);
        Keyboard.press(KEY_RIGHT_SHIFT);
        Keyboard.press('B');
        Keyboard.releaseAll();
    }
}

void switchA2(bool state) {
    if (state) {
        Serial.println("A2 ON");
        //Keyboard.press(KEY_LEFT_CTRL);
        Keyboard.press(KEY_LEFT_ALT);
        Keyboard.press('C');
        Keyboard.releaseAll();
    } else {
        Serial.println("A2 OFF");
        //Keyboard.press(KEY_LEFT_CTRL);
        Keyboard.press(KEY_LEFT_ALT);
        Keyboard.press(KEY_LEFT_SHIFT);
        Keyboard.press('C');
        Keyboard.releaseAll();
    }
}

void switchB0(bool state) {
    if (state) {
        Serial.println("B0 ON");
        //Keyboard.press(KEY_LEFT_CTRL);
        Keyboard.press(KEY_LEFT_ALT);
        Keyboard.press('D');
        Keyboard.releaseAll();
    } else {
        Serial.println("B0 OFF");
        //Keyboard.press(KEY_LEFT_CTRL);
        Keyboard.press(KEY_LEFT_ALT);
        Keyboard.press(KEY_RIGHT_SHIFT);
        Keyboard.press('D');
        Keyboard.releaseAll();
    }
}

void switchB1(bool state) {
    if (state) {
        Serial.println("B1 ON");
        //Keyboard.press(KEY_LEFT_CTRL);
        Keyboard.press(KEY_LEFT_ALT);
        Keyboard.press('E');
        Keyboard.releaseAll();
    } else {
        Serial.println("B1 OFF");
        //Keyboard.press(KEY_LEFT_CTRL);
        Keyboard.press(KEY_LEFT_ALT);
        Keyboard.press(KEY_RIGHT_SHIFT);
        Keyboard.press('E');
        Keyboard.releaseAll();
    }
}

void switchB2(bool state) {
    if (state) {
        Serial.println("B2 ON");
        //Keyboard.press(KEY_LEFT_CTRL);
        Keyboard.press(KEY_LEFT_ALT);
        Keyboard.press('F');
        Keyboard.releaseAll();
    } else {
        Serial.println("B2 OFF");
        //Keyboard.press(KEY_LEFT_CTRL);
        Keyboard.press(KEY_LEFT_ALT);
        Keyboard.press(KEY_RIGHT_SHIFT);
        Keyboard.press('F');
        Keyboard.releaseAll();
    }
}

void switchC0(bool state) {
    if (state) {
        Serial.println("C0 ON");
        //Keyboard.press(KEY_LEFT_CTRL);
        Keyboard.press(KEY_LEFT_ALT);
        Keyboard.press('G');
        Keyboard.releaseAll();
    } else {
        Serial.println("C0 OFF");
        //Keyboard.press(KEY_LEFT_CTRL);
        Keyboard.press(KEY_LEFT_ALT);
        Keyboard.press(KEY_RIGHT_SHIFT);
        Keyboard.press('G');
        Keyboard.releaseAll();
    }
}

void switchC1(bool state) {
    if (state) {
        Serial.println("C1 ON");
        //Keyboard.press(KEY_LEFT_CTRL);
        Keyboard.press(KEY_LEFT_ALT);
        Keyboard.press('H');
        Keyboard.releaseAll();
    } else {
        Serial.println("C1 OFF");
        //Keyboard.press(KEY_LEFT_CTRL);
        Keyboard.press(KEY_LEFT_ALT);
        Keyboard.press(KEY_RIGHT_SHIFT);
        Keyboard.press('H');
        Keyboard.releaseAll();
    }
}

void switchD0(bool state) {
    if (state) {
        Serial.println("D0 ON");
        //Keyboard.press(KEY_LEFT_CTRL);
        Keyboard.press(KEY_LEFT_ALT);
        Keyboard.press('I');
        Keyboard.releaseAll();
    } else {
        Serial.println("D0 OFF");
        //Keyboard.press(KEY_LEFT_CTRL);
        Keyboard.press(KEY_LEFT_ALT);
        Keyboard.press(KEY_RIGHT_SHIFT);
        Keyboard.press('I');
        Keyboard.releaseAll();
    }
}

void switchD1(bool state) {
    if (state) {
        Serial.println("D1 ON");
        //Keyboard.press(KEY_LEFT_CTRL);
        Keyboard.press(KEY_LEFT_ALT);
        Keyboard.press('J');
        Keyboard.releaseAll();
    } else {
        Serial.println("D1 OFF");
        //Keyboard.press(KEY_LEFT_CTRL);
        Keyboard.press(KEY_LEFT_ALT);
        Keyboard.press(KEY_RIGHT_SHIFT);
        Keyboard.press('J');
        Keyboard.releaseAll();
    }
}

void flaps(unsigned int state) {
    switch (state) {
        case 0: 
            Serial.println("Flaps: 0"); 
            Keyboard.press(KEY_LEFT_CTRL);
            Keyboard.press(KEY_LEFT_ALT);
            Keyboard.press(KEY_LEFT_SHIFT);
            Keyboard.press('0');
            Keyboard.releaseAll();
            break;
        case 1: 
            Serial.println("Flaps: 1"); 
            Keyboard.press(KEY_LEFT_CTRL);
            Keyboard.press(KEY_LEFT_ALT);
            Keyboard.press(KEY_LEFT_SHIFT);
            Keyboard.press('1');
            Keyboard.releaseAll();
            break;
        case 2: 
            Serial.println("Flaps: 2"); 
            Keyboard.press(KEY_LEFT_CTRL);
            Keyboard.press(KEY_LEFT_ALT);
            Keyboard.press(KEY_LEFT_SHIFT);
            Keyboard.press('2');
            Keyboard.releaseAll();
            break;
        case 3: 
            Serial.println("Flaps: 3"); 
            Keyboard.press(KEY_LEFT_CTRL);
            Keyboard.press(KEY_LEFT_ALT);
            Keyboard.press(KEY_LEFT_SHIFT);
            Keyboard.press('3');
            Keyboard.releaseAll();
            break;
    }
}

void landingGear(bool state) {
    switch (state) {
        case true: 
            Serial.println("Landing Gear OUT"); 
            Keyboard.press(KEY_LEFT_CTRL);
            Keyboard.press(KEY_LEFT_ALT);
            Keyboard.press(KEY_PAGE_DOWN);
            Keyboard.releaseAll();
            break;
        case false: 
            Serial.println("Landing Gear RETRACTED"); 
            Keyboard.press(KEY_LEFT_CTRL);
            Keyboard.press(KEY_LEFT_ALT);
            Keyboard.press(KEY_PAGE_UP);
            Keyboard.releaseAll();
            break;
    }
}

void trimWheelStepRight() {
    Serial.println(" -->");
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.press(KEY_UP_ARROW);
    Keyboard.releaseAll();
}

void trimWheelStepLeft() {
    Serial.println("<-- ");
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.press(KEY_DOWN_ARROW);
    Keyboard.releaseAll();
}

void trimWheelSwitchPress() {
    Serial.println("- SWITCH -");
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.press(KEY_LEFT_ARROW);
    Keyboard.releaseAll();
}