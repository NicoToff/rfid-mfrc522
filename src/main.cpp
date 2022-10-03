#include <Arduino.h>
#define RX 16
#define TX 17

void setup()
{
    Serial.begin(115200);
    Serial2.begin(115200, SERIAL_8N1, RX, TX);
    Serial.println("Hello by ESP32");
}

void loop()
{
    if (Serial2.available())
    {
        Serial.print("ESP32 sees: ");
        Serial.println(Serial2.readString());
    }
}