#include "bluetooth.h"
#include "pid.h"
#include "sensors.h"
#include "motors.h"
#include <BluetoothSerial.h>

extern int BaseSpeed;
extern int TurboSpeed;
extern bool robotRun;

BluetoothSerial SerialBT;
static bool btIntroShown = false;

void printHelp(){

SerialBT.println("===== LINE FOLLOWER =====");

SerialBT.println("AT COMMANDS:");

SerialBT.println("AT -> ping");
SerialBT.println("AT+START");
SerialBT.println("AT+STOP");

SerialBT.println("AT+P=value");
SerialBT.println("AT+I=value");
SerialBT.println("AT+D=value");

SerialBT.println("AT+B=value (base speed)");
SerialBT.println("AT+T=value (turbo speed)");

SerialBT.println("AT+CAL");
SerialBT.println("AT+SENS -> start manual sensitivity measurement (500 ms average output)");
SerialBT.println("AT+SENS=value -> set sensitivity threshold and save");
SerialBT.println("AT+CONF");
SerialBT.println("AT+SAVE");
SerialBT.println("=========================");
}

void printConfig(){

SerialBT.println("CONFIG:");

SerialBT.print("P="); SerialBT.println(P);
SerialBT.print("I="); SerialBT.println(I);
SerialBT.print("D="); SerialBT.println(D);

SerialBT.print("BaseSpeed=");
SerialBT.println(BaseSpeed);

SerialBT.print("TurboSpeed=");
SerialBT.println(TurboSpeed);

SerialBT.print("RUN=");
SerialBT.println(robotRun);

}

void parse(String cmd){

cmd.trim();

if(cmd=="AT") SerialBT.println("OK");

else if(cmd=="AT+START"){
robotRun=true;
SerialBT.println("RUN");
}

else if(cmd=="AT+STOP"){
robotRun=false;
stopMotors();
SerialBT.println("STOP");
}

else if(cmd.startsWith("AT+P=")){
P=cmd.substring(5).toFloat();
}

else if(cmd.startsWith("AT+I=")){
I=cmd.substring(5).toFloat();
}

else if(cmd.startsWith("AT+D=")){
D=cmd.substring(5).toFloat();
}

else if(cmd.startsWith("AT+B=")){
BaseSpeed=cmd.substring(5).toInt();
}

else if(cmd.startsWith("AT+T=")){
TurboSpeed=cmd.substring(5).toInt();
}

else if(cmd=="AT+CAL"){
calibrateSensors();
}

else if(cmd=="AT+SENS"){
startManualSensitivity();
}

else if(cmd.startsWith("AT+SENS=")){
int value = cmd.substring(8).toInt();
setSensorThreshold(value);
saveSettings();
SerialBT.println("SAVED");
}

else if(cmd=="AT+CONF"){
printConfig();
printSensorThreshold();
}

else if(cmd=="AT+SAVE"){
saveSettings();
SerialBT.println("SAVED");
}

else if(cmd=="AT+HELP"){
printHelp();
}

}

void btInit(){

SerialBT.begin("LineFollower");
btIntroShown = false;
}

void btTick(){

    if(SerialBT.hasClient() && !btIntroShown) {
        SerialBT.println("READY");
        printHelp();
        printConfig();
        printSensorThreshold();
        btIntroShown = true;
    }

    manualSensitivityTick();

    if(!SerialBT.available()) return;

    String cmd=SerialBT.readStringUntil('\n');

    parse(cmd);

    SerialBT.print("> ");

}