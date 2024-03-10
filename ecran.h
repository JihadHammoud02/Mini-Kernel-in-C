#ifndef CONSOLE_H
#define CONSOLE_H


#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <cpu.h>
#include <stdbool.h>

extern uint32_t col;
extern uint32_t lig;

extern uint16_t *ptr_mem(uint32_t lig, uint32_t col);
extern void ecrit_car(uint32_t lig, uint32_t col, char c, int8_t color_text, int8_t color_font);
extern void efface_ecran(void);
extern void place_curseur(uint32_t lig, uint32_t col);
extern void traite_car(char c);
extern void defilement(void);
extern void console_putbytes(const char *s, int len);

#endif /* CONSOLE_H */
