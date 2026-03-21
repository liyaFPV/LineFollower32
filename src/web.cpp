#include <Arduino.h>
#include "config.h"
#include <SettingsGyver.h>
#include <EEPROM.h>
#include "web.h"
#include "EEPROM_part.h"

SettingsGyver sett("Line Follower 32");

float Pk, Ik, Dk;
int BaseSpeed,Sk;
bool swit;
String Skt="Sk"+String(analogRead(S0));

void save(){
    EEPROM.begin(50);
    EEPROM.writeInt(pka, Pk);
    EEPROM.writeInt(ika, Ik);
    EEPROM.writeInt(dka, Dk);
    EEPROM.writeInt(ska, Sk);
    EEPROM.writeInt(baseSpeeda, BaseSpeed);
    EEPROM.commit();
}

void load(){
    EEPROM.begin(50);
    Pk = EEPROM.readInt(pka);
    Ik = EEPROM.readInt(ika);
    Dk = EEPROM.readInt(dka);
    Sk = EEPROM.readInt(ska);
    BaseSpeed = EEPROM.readInt(baseSpeeda);
}

void update(sets::Updater& upd) {
    upd.update("Sk"_h, analogRead(S0));
}

void build(sets::Builder& b) {
    b.Slider("Pk", 0, 150, 1, "", &Pk);
    b.Slider("Ik", 0, 150, 1, "", &Ik);
    b.Slider("Dk", 0, 150, 1, "", &Dk);
    b.Slider("Sk", 0, 4095, 1, "", &Sk);
    b.Label("Sk"_h, "agv");
    b.Slider("Base speed", 0, 255, 1, "", &BaseSpeed);
    b.Switch("start", &swit);
    if(b.Button("save")) {
        Serial.println("Pk: " + String(Pk));
        Serial.println("Ik: " + String(Ik));
        Serial.println("Dk: " + String(Dk));
        Serial.println("Sk: " + String(Skt));
        Serial.println("BaseSpeed: " + String(BaseSpeed));
        save();
    }
}
void setupWEB() {
    load();
    Serial.println();

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println();
    Serial.print("Connected: ");
    Serial.println(WiFi.localIP());

    sett.begin();
    sett.onBuild(build);
    sett.onUpdate(update);
}

void webTick() {
    sett.tick();
}

