
#ifndef TIC_H
#define TIC_H

#include "ecran.h"


extern uint32_t compteur;
extern uint32_t hour;
extern uint32_t min;
extern uint32_t sec;
extern char str[20];


extern uint32_t nbr_secondes();
extern void ecrit_temps(const char *s, int len);
extern void tic_PIT(void);


#endif /* TIC_H */