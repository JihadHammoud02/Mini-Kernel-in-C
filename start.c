#include "proc.h"
#include "FREQ.h"
void kernel_start(void)
{
// initialisations
    masque_IRQ(0,0);
    confHorloge();
    init_traitant_IT(32, traitant_IT_32); // Initialisation de la table d'interruption
    sti();
// boucle d’attente
while (1) hlt();

}