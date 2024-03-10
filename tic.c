#include "tic.h"
#include "proc.h"
uint32_t compteur = 0;
uint32_t hour = 0;
uint32_t min = 0;
uint32_t sec = 0;
char str[20];

void ecrit_temps(const char *s, int len){

    // Initialize column on the upper right
    int current_col=col-len;
    for(size_t i=0;i<len;i++){
        ecrit_car(1,current_col,s[i],15,0);
        current_col++;
}}

uint32_t nbr_secondes(){
    return hour*3600 + min*60 + sec; 
}

// La fonction tic_PIT est responsable de gérer l'interruption de l'horloge.
void tic_PIT(void)
{
    outb(0x20, 0x20);
    if(compteur == 50){
        
        sec++;
        if(sec==60){
            min++;
            sec=0;
        }
        if(min==60){
            hour++;
            min=0;
        }

       sprintf(str,"%d : %d : %d",hour,min,sec);
       ecrit_temps(str,20);
       compteur=0;
    }
    compteur++;
    
}
