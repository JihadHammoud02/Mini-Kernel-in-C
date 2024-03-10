#ifndef PROC_H
#define PROC_H


#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "cpu.h"
#include <stdbool.h>
#include "ctx_swf.h"



enum Etatpro{
    elu,
    activable,
    endormi,
    zombie
};

struct processus {
    int32_t pid;
    char *nom;
    enum Etatpro etat;
    uint32_t registreZone[5]; // de la forme : [ebx,esp,ebp,esi.edi]
    uint32_t pile[512];
    struct processus * suiv;
    uint32_t wakeup_time;
};

struct Liste_des_activables{
    struct processus * head;
    struct processus * tail;
};

struct Liste_des_endormies{
    struct processus *head;
};
struct Liste_des_zombies{
    struct processus *head;
};

extern struct processus * actif;

extern void idle(void);
extern void proc1(void);
extern void  system__init__(void);

extern struct processus * struct_idle;
extern struct processus * struct_proc1;

extern char* mon_nom();
extern int32_t mon_pid();
extern void affiche_etats(void);

extern void ordonnance();
extern void dors(uint32_t nbr_secs);
extern void fin_processus();

#endif 