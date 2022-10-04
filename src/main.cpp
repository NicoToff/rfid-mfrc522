#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include "RFID_DB_TP2.h"

#define RST_PIN 5
#define SS_PIN 53

MFRC522 mfrc522(SS_PIN, RST_PIN);

boolean newUidDetected()
{
    if (!mfrc522.PICC_IsNewCardPresent())
        return false;
    if (!mfrc522.PICC_ReadCardSerial()) // Stocke les données du tag dans `mfrc522.uid` et renvoie `true` si tout est OK
        return false;
    mfrc522.PICC_HaltA(); // Stop reading
    return true;
}

void setup()
{
    Serial.begin(115200);
    SPI.begin();
    mfrc522.PCD_Init();
    Serial.println("RFID ready on Mega");
}

void loop()
{
    if (newUidDetected())
    {
        Serial.print("Tag ID: ");
        for (int i = 0; i < mfrc522.uid.size; i++)
        {
            Serial.print(mfrc522.uid.uidByte[i], HEX);
            Serial.print(" ");
        }
        Serial.print("-> ");
        Serial.println(getCardName(mfrc522.uid));
        delay(1000);
    }
    delay(10);
}