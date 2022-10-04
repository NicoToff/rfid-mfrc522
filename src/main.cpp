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
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print('.');
        delay(100);
    }

    return WiFi.localIP();
}

bool mqttConnect()
{
    bool connected = mqttClient.connected();
    if (!connected)
    {
        Serial.print("Connecting to MQTT...");
        mqttClient.connect("esp32_NicoToff");
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

    Serial.print("Connecting to WiFi..");
    IPAddress ip = initWiFi();
    Serial.print("IP address: ");
    Serial.println(ip);
    Serial.print("On SSID: ");
    Serial.println(WiFi.SSID());

    mqttConnect();
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