#include "ecran.h"
uint32_t col;
uint32_t lig;



uint16_t *ptr_mem(uint32_t lig, uint32_t col)
{
    uint16_t *addressStart = (uint16_t *)0xb8000;
    uint16_t *ptr = addressStart + (lig * 80 + col);
    return ptr;
};

void ecrit_car(uint32_t lig, uint32_t col, char c, int8_t color_text, int8_t color_font)
{
    uint16_t *address;
    address = ptr_mem(lig, col);
    *address = c | (color_text << 8) | (color_font << 3) | (0 << 1);
};

void efface_ecran(void)
{

    col = 0;
    lig = 0;
    while (lig < 25)
    {
        while (col < 80)
        {
            ecrit_car(lig, col, ' ', 15, 0);
            col = col + 1;
        }
        col = 0;
        lig = lig + 1;
    }
    col = 0;
    lig = 0;
};

void place_curseur(uint32_t lig, uint32_t col)
{
    uint16_t pos;
    pos = col + lig * 80;
    uint16_t pos_low = pos & 0000000011111111;
    uint16_t pos_high = pos & 1111111100000000;

    outb(0x0F, 0x3D4);
    outb(pos_low, 0x3D4);

    outb(0x0E, 0x3D4);
    outb(pos_high, 0x3D5);
};

void traite_car(char c)
{
    int asciiCode = c;
    if (asciiCode == 8)
    {
        if (col != 0)
        {
            col = col - 1;
        }
    }
    else
    {
        if (asciiCode == 9)
        {
            col = (col / 8) * 8;
            col = col * 8;
        }
        else
        {
            if (asciiCode == 10)
            {
                col = 0;
                lig = lig + 1;
                place_curseur(lig, col);
            }
            else
            {
                if (asciiCode == 12)
                {
                    efface_ecran();
                    col = 0;
                    lig = 0;
                }
                else
                {
                    if (asciiCode == 13)
                    {
                        col = 0;
                        lig = 0;
                    }
                    else
                    {

                        ecrit_car(lig, col, c, 15, 0);
                    }
                }
            }
        }
    }
}

void defilement(void)
{
    uint16_t *destination;
    uint16_t *source;
    size_t size;

    destination = ptr_mem(0, 0);  // Start from the beginning of the screen
    source = ptr_mem(1, 0);       // Move up one line
    size = 24 * 80 * 2;           // Total size of the screen in bytes

    memmove(destination, source, size);

}


void console_putbytes(const char *s, int len)
{

    if (col == 0 && lig == 0)
    {
        efface_ecran();
    }
    int32_t ind = 0;
    while (ind < len)
    {
        if (col == 79)
        {
            lig = lig + 1;
            col = 0;
            place_curseur(lig, col);
        }
        if (lig == 24)
        {
            defilement();
            lig = lig - 1;
        }
        traite_car(s[ind]);
        col = col + 1;
        ind++;
    }
}

