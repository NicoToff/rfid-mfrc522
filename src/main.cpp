#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 5
#define SS_PIN 53

MFRC522 mfrc522(SS_PIN, RST_PIN);

typedef struct
{
    const char *name[10];
    MFRC522::Uid rfid;
} dbItem;

const byte BYTE_NBR = 4;
const byte SAK_TYPE = 8;

const dbItem DB[] = {
    {{"Nico"}, {BYTE_NBR, {0xE, 0xC5, 0x51, 0x3}, SAK_TYPE}},
    {{"Sajad"}, {BYTE_NBR, {0x4, 0x98, 0x51, 0x3}, SAK_TYPE}},
    {{"Ronaldo"}, {BYTE_NBR, {0x1A, 0x5A, 0xCD, 0x18}, SAK_TYPE}}};

const char *getCardName(MFRC522::Uid readUID)
{
    const unsigned long NBR_OF_ENTRIES = sizeof(DB) / sizeof(dbItem);
    for (unsigned int entry = 0; entry < NBR_OF_ENTRIES; entry++)
    {
        if (DB[entry].rfid.size == readUID.size && DB[entry].rfid.sak == readUID.sak)
        {
            bool found = true;
            for (int byte = 0; byte < readUID.size; byte++)
            {
                if (DB[entry].rfid.uidByte[byte] != readUID.uidByte[byte])
                {
                    found = false;
                    break;
                }
            }

            if (found)
            {
                return DB[entry].name[0];
            }
        }
    }
    return "Inconnu";
}

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
    Serial.println("RFID ready");
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
        Serial.println();
        Serial.print("Tag Name: ");
        Serial.println(getCardName(mfrc522.uid));
        delay(1000);
    }
    delay(10);
}