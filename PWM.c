#include <xc.h>
#include "IO.h"
#include "PWM.h"
#include "Robot.h"
#include "Toolbox.h"

#define PWMRER 40.0
unsigned char acceleration = 4;

void InitPWM(void) {
    PTCON2bits.PCLKDIV = 0b000; // Divide by 1
    PTPER = 100 * PWMRER; //éPriode  en  pourcentage

    //réglage PWM moteur 1 sur hacheur 1
    IOCON1bits.POLH = 1; //High = 1 and  active  on low =0
    IOCON1bits.POLL = 1; //High = 1
    IOCON1bits.PMOD = 0b01; // Set PWM Mode to  Redundant
    FCLCON1 = 0x0003; //éDsactive  la  gestion  des  f a u l t s

    // Reglage PWM moteur 2  sur  hacheur 6
    IOCON6bits.POLH = 1; //High = 1*
    IOCON6bits.POLL = 1; //High = 1
    IOCON6bits.PMOD = 0b01; // Set PWM Mode to  Redundant
    FCLCON6 = 0x0003; //éDsactive  la  gestion  des  f a u l t s

    /*enable PWM module*/
    PTCONbits.PTEN = 1;

}

//void PWMSetSpeed(float vitesseEnPourcents, char motor) {
//    if (motor == MOTEUR_GAUCHE) {
//        robotState.vitesseGaucheCommandeCourante = vitesseEnPourcents;
//        if (vitesseEnPourcents >= 0) {
//            MOTEUR_GAUCHE_ENL = 0; // Pilotage  de  la  pin  en mode IO
//            MOTEUR_GAUCHE_INL = 1; //Mise à 1 de  la  pin
//            MOTEUR_GAUCHE_ENH = 1; // Pilotage  de  la  pin  en mode PWM
//        } else
//            if (vitesseEnPourcents < 0) { //pour changer le sens du moteur on change les H en L ou inversement
//            MOTEUR_GAUCHE_ENH = 0; // Pilotage  de  la  pin  en mode IO
//            MOTEUR_GAUCHE_INH = 1; //Mise à 1 de  la  pin
//            MOTEUR_GAUCHE_ENL = 1; // Pilotage  de  la  pin  en mode PWM
//        }
//        MOTEUR_GAUCHE_DUTY_CYCLE = Abs(robotState.vitesseGaucheCommandeCourante * PWMRER);
//    } else
//        if (motor == MOTEUR_DROIT) {
//        robotState.vitesseDroiteCommandeCourante = vitesseEnPourcents;
//        if (vitesseEnPourcents >= 0) {
//            MOTEUR_DROIT_ENL = 0; // Pilotage  de  la  pin  en mode IO
//            MOTEUR_DROIT_INL = 1; //Mise à 1 de  la  pin
//            MOTEUR_DROIT_ENH = 1; // Pilotage  de  la  pin  en mode PWM
//        } else
//            if (vitesseEnPourcents < 0) { //pour changer le sens du moteur on change les H en L ou inversement
//            MOTEUR_DROIT_ENH = 0; // Pilotage  de  la  pin  en mode IO
//            MOTEUR_DROIT_INH = 1; //Mise à 1 de  la  pin
//            MOTEUR_DROIT_ENL = 1; // Pilotage  de  la  pin  en mode PWM
//        }
//        MOTEUR_DROIT_DUTY_CYCLE = Abs(robotState.vitesseDroiteCommandeCourante * PWMRER);
//    }
//}

void PWMUpdateSpeed() {
    // Cette fonction est appelée sur timer et permet de suivre des rampes d?accélération
    if (robotState.vitesseDroiteCommandeCourante < robotState.vitesseDroiteConsigne)
        robotState.vitesseDroiteCommandeCourante = Min(robotState.vitesseDroiteCommandeCourante + acceleration,robotState.vitesseDroiteConsigne);
    if (robotState.vitesseDroiteCommandeCourante > robotState.vitesseDroiteConsigne)
        robotState.vitesseDroiteCommandeCourante = Max(robotState.vitesseDroiteCommandeCourante - acceleration,robotState.vitesseDroiteConsigne);

    if (robotState.vitesseDroiteCommandeCourante > 0) {
        MOTEUR_DROIT_ENH = 0; //pilotage de la pin en mode IO
        MOTEUR_DROIT_INH = 1; //Mise à 1 de la pin
        MOTEUR_DROIT_ENL = 1; //Pilotage de la pin en mode PWM
    } else {
        MOTEUR_DROIT_ENL = 0; //pilotage de la pin en mode IO
        MOTEUR_DROIT_INL = 1; //Mise à 1 de la pin
        MOTEUR_DROIT_ENH = 1; //Pilotage de la pin en mode PWM
    }
    MOTEUR_DROIT_DUTY_CYCLE = Abs(robotState.vitesseDroiteCommandeCourante) * PWMRER;

    if (robotState.vitesseGaucheCommandeCourante < robotState.vitesseGaucheConsigne)
        robotState.vitesseGaucheCommandeCourante = Min(robotState.vitesseGaucheCommandeCourante + acceleration,robotState.vitesseGaucheConsigne);
    if (robotState.vitesseGaucheCommandeCourante > robotState.vitesseGaucheConsigne)
        robotState.vitesseGaucheCommandeCourante = Max(robotState.vitesseGaucheCommandeCourante - acceleration,robotState.vitesseGaucheConsigne);

    if (robotState.vitesseGaucheCommandeCourante > 0) {
        MOTEUR_GAUCHE_ENL = 0; //pilotage de la pin en mode IO
        MOTEUR_GAUCHE_INL = 1; //Mise à 1 de la pin
        MOTEUR_GAUCHE_ENH = 1; //Pilotage de la pin en mode PWM
    } else {
        MOTEUR_GAUCHE_ENH = 0; //pilotage de la pin en mode IO
        MOTEUR_GAUCHE_INH = 1; //Mise à 1 de la pin
        MOTEUR_GAUCHE_ENL = 1; //Pilotage de la pin en mode PWM
    }
    MOTEUR_GAUCHE_DUTY_CYCLE = Abs(robotState.vitesseGaucheCommandeCourante) * PWMRER;
}

void PWMSetSpeedConsigne(float vitesseEnPourcents, char moteur){
    if(moteur == MOTEUR_DROIT)
        robotState.vitesseDroiteConsigne = vitesseEnPourcents; 
    else
        if(moteur == MOTEUR_GAUCHE)
            robotState.vitesseGaucheConsigne = vitesseEnPourcents; 
}