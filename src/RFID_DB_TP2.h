
#include <MFRC522.h>

#define BYTE_NBR 4
#define SAK_TYPE 8

typedef struct
{
    const char *name[10];
    MFRC522::Uid rfid;
} dbItem;

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