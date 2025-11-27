#include <stdio.h>
#include <stdint.h>

#define PRESET_VALUE 0xFFFF
#define POLYNOMIAL   0x8408

unsigned int uiCrc16Cal(unsigned char const *pucY, unsigned char ucX)
{
    unsigned char ucI, ucJ;
    unsigned short int uiCrcValue = PRESET_VALUE;

    for (ucI = 0; ucI < ucX; ucI++)
    {
        uiCrcValue ^= *(pucY + ucI);
        for (ucJ = 0; ucJ < 8; ucJ++)
        {
            if (uiCrcValue & 0x0001)
                uiCrcValue = (uiCrcValue >> 1) ^ POLYNOMIAL;
            else
                uiCrcValue >>= 1;
        }
    }
    return uiCrcValue;
}

int main(void)
{
    // Frame INVENTORY-G2 tanpa CRC
    unsigned char frame[] = {
        0x0F, 0x00,  // LEN LSB first
        0x00,        // ADR
        0x01,        // CMD Inventory
        0x04,        // QValue
        0x01,        // Session
        0x02,        // MaskMem
        0x00, 0x02,  // MaskAdr
        0x04,        // MaskLen
        0x02,        // MaskData
        0x00,        // AdrTID
        0x06,        // LenTID
        0x00,        // Target
        0x80,        // Ant
        0x0A         // ScanTime
    };

    // Hitung CRC untuk SELURUH frame di atas
    unsigned int crc = uiCrc16Cal(frame, sizeof(frame));
    unsigned char crcLSB = crc & 0xFF;
    unsigned char crcMSB = (crc >> 8) & 0xFF;

    // Print full frame
    printf("Final Frame:\n");
    for (int i = 0; i < sizeof(frame); i++)
    {
        printf("%02X ", frame[i]);
    }
    printf("%02X %02X\n", crcLSB, crcMSB);

    return 0;
}
