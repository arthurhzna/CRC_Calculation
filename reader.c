//read single tag

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

    // 0C 00 02 00 02 00 02 00 00 00 00
    unsigned char frame_no_crc[] = {
        0x0C, 0x00, 0x02, 0x00, 0x02,
        0x00, 0x02, 0x00, 0x00, 0x00, 0x00
    };


    unsigned int crc = uiCrc16Cal(frame_no_crc, sizeof(frame_no_crc));
    unsigned char crcLSB = crc & 0xFF;
    unsigned char crcMSB = (crc >> 8) & 0xFF;

    printf("CRC dihitung  : %02X %02X\n", crcLSB, crcMSB);

    unsigned char frame_full[13];
    for (int i = 0; i < 11; ++i) {
        frame_full[i] = frame_no_crc[i];
    }
    frame_full[11] = crcLSB;  // 0x1D
    frame_full[12] = crcMSB;  // 0xA8

    printf("Frame lengkap : ");
    for (int i = 0; i < 13; ++i) {
        printf("%02X ", frame_full[i]);
    }
    printf("\n");


    unsigned int verify = uiCrc16Cal(frame_full, sizeof(frame_full));
    printf("Hasil verifikasi CRC (harus 0): 0x%04X\n", verify);

    return 0;
}
