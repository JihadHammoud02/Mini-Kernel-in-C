#include "proc.h"
#include "tic.h"
#include "tinyalloc.h"
struct processus * struct_idle;
struct processus * struct_proc1;
#define NUMBER_OF_PROCESSES 8
int32_t NUMBER_OF_CURRENT_PROCESSES=0;
struct Liste_des_activables * listeActiv;
struct Liste_des_endormies *listeEnd;
struct Liste_des_zombies *listeZomb;
struct processus * processTable[NUMBER_OF_PROCESSES];
struct processus * actif;


void idle()
{
for (;;) {
sti();
hlt();
cli();
}
}
void proc1(void)
{
for (int32_t i = 0; i < 2; i++) {
printf("[temps = %u] processus %s pid = %i\n", nbr_secondes(),
mon_nom(), mon_pid());
dors(2);
}
}
void proc2(void)
{
for (;;) {
printf("[temps = %u] processus %s pid = %i\n", nbr_secondes(),
mon_nom(), mon_pid());
dors(3);
}
}
void proc3(void)
{
for (;;) {
printf("[temps = %u] processus %s pid = %i\n", nbr_secondes(),
mon_nom(), mon_pid());
dors(5);
}
}
void proc4(void) {
for (;;) {
printf("[%s] pid = %i\n", mon_nom(), mon_pid());
sti();
hlt();
cli();
}
}

void proc5(void) {
for (;;) {
printf("[%s] pid = %i\n", mon_nom(), mon_pid());
sti();
hlt();
cli();
}
}
void proc6(void) {
for (;;) {
printf("[%s] pid = %i\n", mon_nom(), mon_pid());
sti();
hlt();
cli();
}
}
void proc7(void) {
for (;;) {
printf("[%s] pid = %i\n", mon_nom(), mon_pid());
sti();
hlt();
cli();
}
}


int32_t cree_processus(void (*code)(void), char *nom,void (*fin)(void)){

    if(NUMBER_OF_CURRENT_PROCESSES >= NUMBER_OF_PROCESSES ){
        return -1;
    }
    else{
        struct processus * struct_du_processus=malloc(sizeof(struct processus));
        struct_du_processus->pid=NUMBER_OF_CURRENT_PROCESSES+1;
        // adding this process as a successor of the previous one using suiv

        listeActiv->tail->suiv=struct_du_processus;
        // New process configurations
        struct_du_processus->nom=nom;
        struct_du_processus->etat=activable;
        struct_du_processus->pile[510]=(uint32_t)fin;
        struct_du_processus->pile[511]=(uint32_t)code;
        struct_du_processus->registreZone[1]=(uint32_t)&struct_du_processus->pile[511];
        // Update process table and number of activable processes
        NUMBER_OF_CURRENT_PROCESSES=NUMBER_OF_CURRENT_PROCESSES+1;
        // Update the list tail
        struct_du_processus->suiv=NULL;
        listeActiv->tail=struct_du_processus;
        processTable[NUMBER_OF_CURRENT_PROCESSES]=struct_du_processus;
        return struct_du_processus->pid;
    }
}

void system__init__(){
    // Initialisation de idle
    struct_idle = malloc(sizeof(struct processus));
    actif= struct_idle;
    struct_idle->pid=0;
    struct_idle->nom="idle";
    struct_idle->etat=elu;

    listeActiv = malloc(sizeof(struct Liste_des_activables));
    listeEnd= malloc(sizeof(struct Liste_des_endormies));
    listeActiv->head=struct_idle;
    listeActiv->tail=struct_idle;

    listeZomb=malloc(sizeof(struct Liste_des_zombies));
    processTable[0]=struct_idle;

    // Fin d'initialisation de idle
    
    // Intialisation des autres processsus
    cree_processus(proc1 , "proc1",fin_processus);
    cree_processus(proc2 , "proc2",NULL);
    cree_processus(proc3 , "proc3",NULL);
    cree_processus(proc4 , "proc4",NULL);
    cree_processus(proc5 , "proc5",NULL);
    cree_processus(proc6 , "proc6",NULL);
    cree_processus(proc7 , "proc7",NULL);

}

// probelem might be here VVVV

struct processus * extract_head(){
    struct processus * old_head=listeActiv->head;
    listeActiv->head=old_head->suiv;
    old_head->etat=activable;
    listeActiv->head->etat=elu;
    actif=listeActiv->head;
    return old_head;
}

void insert_tail(struct processus ** old_head){
    struct processus * old_tail= listeActiv->tail;
    listeActiv->tail=*old_head;
    old_tail->suiv=*old_head;
    (*old_head)->suiv=NULL;
}

// ---------------------------------------------------------------------
// Ajouter le processus dans la liste des processuses endormies dans le bon ordre 
void add_endormie(struct processus *proc,uint32_t nbresec){
    proc->wakeup_time=nbresec;
    if(listeEnd->head == NULL){
        listeEnd->head=proc;
    }
    else{
        struct processus * current_node=listeEnd->head;
        struct processus * previous_node=current_node;
        bool inserted = false;
        while (current_node != NULL && !inserted) {
        if (proc->wakeup_time < current_node->wakeup_time) {
            proc->suiv = current_node;
            // If the current node is the head, the new head is proc
            if (listeEnd->head == current_node) {
                listeEnd->head = proc;
                inserted = true;
            } else {
                previous_node->suiv = proc;
                inserted = true;
            }
        } else {
            previous_node = current_node;
            current_node = current_node->suiv;
        }


}
        if(!inserted){
            previous_node->suiv=proc;
            proc->suiv=NULL;
        }
        
    }
}

// Reveiller le processus endormie
void extract_endormie(struct processus * proc){
     struct processus *current=listeEnd->head;
     struct processus *previous=current;
     if(proc==current){
        listeEnd->head=current->suiv;
     }
     else{
     while(current!=proc && current!=NULL){
        previous=current;
        current=current->suiv;
     }
     previous->suiv=current->suiv;
     }
}

// Verifier si la duree d'endormissement d'un processus est passe , si oui le reveiller
void wakeupVerification(){
    uint32_t total_sec=nbr_secondes();
    struct processus *current=listeEnd->head;
    while(current != NULL){
        if(current->wakeup_time<total_sec){
            extract_endormie(current);
            insert_tail(&current);
        }
        current=current->suiv;
    }
}


void ordonnance(){
    wakeupVerification();
    struct processus *old_processus=extract_head(); // proc 3 is the actif
    // printf("Extracted head : %p",old_processus);
    insert_tail(&old_processus);
    ctx_sw(listeActiv->tail->registreZone, listeActiv->head->registreZone);
}

void dors(uint32_t nbr_secs){
    struct processus *running_proc=extract_head();
    running_proc->etat=endormi;
    add_endormie(running_proc,nbr_secs);//proc2 is the actif 
    ctx_sw(running_proc->registreZone, listeActiv->head->registreZone);
}
// -------------------------------------------------------------------------

void addZombie(struct processus *proc ){
    if(listeZomb->head == NULL){
        listeZomb->head=proc;
    }
    else{
        struct processus * current=listeZomb->head;
        while(current->suiv!= NULL){
            current=current->suiv;
        }
        current->suiv=proc;
        proc->suiv=NULL;

    }
}

void fin_processus(){
    struct processus *running_proc=extract_head();
    addZombie(running_proc);
    ctx_sw(running_proc->registreZone, listeActiv->head->registreZone);
}


void affiche_etats(void){
    for(int i=0;i<NUMBER_OF_PROCESSES;i++){
        ecrit_car(0,10,processTable[i]->nom[i],15,0);
    }
}


char* mon_nom(){

    return actif->nom;
}

int32_t mon_pid(){
    return actif->pid;
}