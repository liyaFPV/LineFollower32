#include "config.h"
#include "sensors.h"
#include "pid.h"
#include "motors.h"
#include "bluetooth.h"

int BaseSpeed = 140;
int TurboSpeed = 140;
int trim = 0;
int timeslep = 10;
bool robotRun = false;

int lastErr = 0;
static const uint32_t EEPROM_MAGIC = 0xA5A55A5A;

void saveSettings() {
    int addr = 0;
    EEPROM.put(addr, EEPROM_MAGIC); addr += sizeof(uint32_t);

    EEPROM.put(addr, P); addr += sizeof(float);
    EEPROM.put(addr, I); addr += sizeof(float);
    EEPROM.put(addr, D); addr += sizeof(float);

    EEPROM.put(addr, BaseSpeed); addr += sizeof(int);
    EEPROM.put(addr, TurboSpeed); addr += sizeof(int);

    EEPROM.put(addr, sensorThreshold); addr += sizeof(int);

    // NEW SETTINGS
    EEPROM.put(addr, trim); addr += sizeof(int);
    EEPROM.put(addr, timeslep); addr += sizeof(int);

    for(int i = 0; i < 8; i++) {
        EEPROM.put(addr, sensorMin[i]); addr += sizeof(int);
    }

    for(int i = 0; i < 8; i++) {
        EEPROM.put(addr, sensorMax[i]); addr += sizeof(int);
    }

    EEPROM.commit();
}

void loadSettings() {
    int addr = 0;
    uint32_t magic = 0;

    EEPROM.get(addr, magic);
    if(magic != EEPROM_MAGIC) {
        // defaults
        for(int i = 0; i < 8; i++) {
            sensorMin[i] = 0;
            sensorMax[i] = 4095;
        }

        sensorThreshold = 200;
        trim = 0;          // NEW DEFAULT
        timeslep = 10;     // NEW DEFAULT
        return;
    }

    addr += sizeof(uint32_t);

    EEPROM.get(addr, P); addr += sizeof(float);
    EEPROM.get(addr, I); addr += sizeof(float);
    EEPROM.get(addr, D); addr += sizeof(float);

    EEPROM.get(addr, BaseSpeed); addr += sizeof(int);
    EEPROM.get(addr, TurboSpeed); addr += sizeof(int);

    EEPROM.get(addr, sensorThreshold); addr += sizeof(int);

    // NEW SETTINGS
    EEPROM.get(addr, trim); addr += sizeof(int);
    EEPROM.get(addr, timeslep); addr += sizeof(int);

    for(int i = 0; i < 8; i++) {
        EEPROM.get(addr, sensorMin[i]); addr += sizeof(int);
    }

    for(int i = 0; i < 8; i++) {
        EEPROM.get(addr, sensorMax[i]); addr += sizeof(int);
    }
}

void setup(){
    Serial.begin(115200);
    EEPROM.begin(EEPROM_SIZE);
    loadSettings();

    pinMode(BTN_START, INPUT_PULLUP);

    motorsInit();
    btInit();
    delay(1000);
}

void processLine(int err) {
    if(err == 5000){
        setMotor(BaseSpeed,BaseSpeed);
        delay(timeslep);
        return;
    }
    if(err==4000){
        if(lastErr>0)
            setMotor(BaseSpeed,0);
        else
            setMotor(0,BaseSpeed);
        return;
    }

    float correction = computePID(err+trim);

    int currentSpeed = (abs(err) < 500) ? TurboSpeed : BaseSpeed; // Турбо на прямых

    int L = currentSpeed + correction;
    int R = currentSpeed - correction;

    setMotor(L,R);

    lastErr = err;
}

void loop(){

    btTick();

    if(digitalRead(BTN_START)==LOW){

        robotRun=!robotRun;

        delay(300);

    }

    int err = readLine();

    processLine(err);

}