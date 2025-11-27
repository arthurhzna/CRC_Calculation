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
    // Frame command bytes BEFORE CRC
    unsigned char frame[] = {0x04, 0xFF, 0x21};

    // CRC 3 byte
    unsigned int crc = uiCrc16Cal(frame, 3);

    unsigned char crcLSB = crc & 0xFF;
    unsigned char crcMSB = (crc >> 8) & 0xFF;

    printf("CRC result: 0x%02X 0x%02X\n", crcLSB, crcMSB);
    printf("Final Frame: ");
    printf("%02X %02X %02X %02X %02X\n",
        frame[0], frame[1], frame[2], crcLSB, crcMSB);
    return 0;
}
