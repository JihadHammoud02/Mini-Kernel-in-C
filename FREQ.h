#ifndef FREQ_H
#define FREQ_H

#include "traitants.h" 
#include "proc.h"
extern uint32_t CLOCKFREQ;
extern uint32_t QUARTZ;

extern void init_traitant_IT(uint32_t num_IT, void (*traitant)(void));
extern void masque_IRQ(uint8_t num ,bool masque);
extern void confHorloge(void);

#endif /* FREQ_H */
