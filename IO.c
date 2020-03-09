/*
 * File:   IO.c
 */

#include <xc.h>
#include "IO.h"
//#include "main.h"

void InitIO()
{
    // IMPORTANT : désactiver les entrées analogiques, sinon on perd les entrées numériques
    ANSELA = 0; // 0 desactive
    ANSELB = 0;
    ANSELC = 0;
    ANSELD = 0;
    ANSELE = 0;
    ANSELF = 0;
    ANSELG = 0;

    //********** Configuration des sorties ********************************
    // LED
    _TRISC10 = 0;  // LED Orange
    _TRISG6 = 0; //LED Blanche
    _TRISG7 = 0; // LED Bleue
    
    // Moteurs Gauche
    _TRISB15 = 0;   //IN2
    _TRISB14 = 0;   //IN1
    
    //Moteur Droit   
    _TRISC7 = 0;   //IN2
    _TRISC6 = 0;   //IN1
    
    

    //********** Configuration des entrées ********************************   
    
    //Jack
    _TRISA11 = 1;
}

