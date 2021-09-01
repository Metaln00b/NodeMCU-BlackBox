#include "BluetoothSerial.h"
#define CLIENT_NAME     "BT-DASH-E12"

BluetoothSerial SerialBT;
bool connected;

void setup() {
    Serial.begin(115200);
    SerialBT.begin("CAN-MASTER", true); 
    connected = SerialBT.connect(CLIENT_NAME);
  
    if (connected)
    {
        Serial.println("Connected Succesfully!");
    }
    else
    {
        while(!SerialBT.connected(10000)) 
        {
            Serial.println("Failed to connect. Make sure remote device is available and in range, then restart app."); 
        }
    }

    if (SerialBT.disconnect())
    {
        Serial.println("Disconnected Succesfully!");
    }

    SerialBT.connect();
}

void loop() {
    if (Serial.available())
    {
        SerialBT.write(Serial.read());
    }
}
