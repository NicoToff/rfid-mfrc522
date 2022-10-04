#include <Arduino.h>
#include <WiFi.h>
#include "ssid_password.h"
#include <PubSubClient.h>

#define RX 16
#define TX 17

WiFiClient wifiClient;
PubSubClient mqttClient("public.mqtthq.com", 1883, wifiClient);

IPAddress initWiFi()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi ..");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print('.');
        delay(1000);
    }
    Serial.println(WiFi.localIP());

    return WiFi.localIP();
}

bool mqttConnect()
{
    bool connected = mqttClient.connected();
    if (!connected)
    {
        unsigned long startTime = millis();
        Serial.print("Connecting to MQTT...");
        // Tentative de connexion pendant 5 secondes
        while (!mqttClient.connected() && (millis() - startTime < 5000))
        {
            mqttClient.connect("esp32_NicoToff");
            Serial.print('.');
            delay(100);
        }
        connected = mqttClient.connected();
        connected ? Serial.println(" Connected!") : Serial.println(" FAILED!!!");
    }
    return connected;
}

bool sendToMqtt(String topic, String message)
{
    bool sent = false;
    if (mqttConnect())
    {
        sent = mqttClient.publish(topic.c_str(), message.c_str());
    }
    if (!sent)
    {
        Serial.println("Couldn't send to MQTT!");
    }
    return sent;
}

void setup()
{
    Serial.begin(115200);
    Serial2.begin(115200, SERIAL_8N1, RX, TX);
    Serial.println("Hello by ESP32");
    initWiFi();
    mqttConnect();
    Serial.println("Setup done");
}

void loop()
{
    if (Serial2.available())
    {
        String message = Serial2.readString();
        Serial.println(message);
        sendToMqtt("helha/iot/tp2", message);
        delay(2000);
    }
    delay(10);
}