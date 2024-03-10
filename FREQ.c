#include "FREQ.h"
uint32_t CLOCKFREQ = 50;
uint32_t QUARTZ = 1193181;

void confHorloge(void){
    outb(0x34, 0x43); // Reglage de la fonction horloge
    outb((QUARTZ / CLOCKFREQ) & 0xFF, 0x40);
    outb(((QUARTZ / CLOCKFREQ )>> 8), 0x40);
}



void init_traitant_IT(uint32_t num_IT, void (*traitant)(void))
{

  
    uint32_t *idt_table = (uint32_t *)0x1000;

 
    uint32_t *idt_entry = idt_table + num_IT * 2;

 
    uint32_t traitant_low = (uint32_t)traitant & 0xFFFF;
    uint32_t traitant_high = ((uint32_t)traitant) & 0xFFFF0000;
                                                        

    idt_entry[0] = (traitant_low | (0x10 << 16));
    idt_entry[1] = (traitant_high | 0x8E00);

}



void masque_IRQ(uint8_t num ,bool masque)
{
    uint8_t masqueActuelle = inb(0x21);
    bool *IRQ_Table= (bool *)&masqueActuelle;
    IRQ_Table[num]=masque;
    outb(masqueActuelle, 0x21);
}
