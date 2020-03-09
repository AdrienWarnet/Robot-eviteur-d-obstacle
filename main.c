#include<xc.h>
#include"ChipConfig.h"
#include"IO.h"
#include <stdio.h>
#include <stdlib.h>
#include "timer.h"
#include "PWM.h"
#include "Robot.h"
#include "Toolbox.h"
#include "ADC.h"
#include "UART.h"
#include "main.h"

int main(void) {

    //init de l'oscillateur

    InitOscillator();

    //config des Entré sorties
    InitIO();

    //config des timers
    InitTimer23();
    InitTimer1();
   InitTimer4();

    //init moteur 
    InitPWM();

    //init ADC
    InitADC1();

    //    LED_BLANCHE = 1;
    //    LED_BLEUE = 1;
    //    LED_ORANGE = 1;


    //    PWMSetSpeed(30, MOTEUR_DROIT);
    //    PWMSetSpeed(30, MOTEUR_GAUCHE);

//    PWMSetSpeedConsigne(30, MOTEUR_DROIT);
//    PWMSetSpeedConsigne(30, MOTEUR_GAUCHE);

    //boucle principal
    unsigned int * result;

    while (1) {
        //LED_BLANCHE = !LED_BLANCHE;
        if (ADCIsConversionFinished() == 1) {
            ADCClearConversionFinishedFlag();

            result = ADCGetResult();

            //les valeurs analogiques des capteurs infrarouges pour les visualiser dans les watches
            //            int result1 = result[0];
            //            int result2 = result[1];
            //            int result3 = result[2];

            //conversion analogique vers distance en CM
            float volts = ((float) result[0])* 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreExtremeDroit = 34 / volts - 5;
            volts = ((float) result[1])*3.3 / 4096 * 3.2;
            robotState.distanceTelemetreDroit = 34 / volts - 5;
            volts = ((float) result[2])*3.3 / 4096 * 3.2;
            robotState.distanceTelemetreCentre = 34 / volts - 5;
            volts = ((float) result[3])*3.3 / 4096 * 3.2;
            robotState.distanceTelemetreExtremeGauche = 34 / volts - 5;
            volts = ((float) result[4])*3.3 / 4096 * 3.2;
            robotState.distanceTelemetreGauche = 34 / volts - 5;
        } 
    }
}//fin main  


unsigned char stateRobot, droite_Ou_gauche = 0;

void OperatingSystemLoop(void) {
    if(JACK) {
        LED_ORANGE = 0;
        LED_BLANCHE = 0;
        LED_BLEUE = 0;
        
        switch (stateRobot) {
            case STATE_ATTENTE:
                timestamp = 0;
                PWMSetSpeedConsigne(0, MOTEUR_DROIT);
                PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
                stateRobot = STATE_ATTENTE_EN_COURS;

            case STATE_ATTENTE_EN_COURS:
                if (timestamp > 2000)
                    stateRobot = STATE_AVANCE;
                break;

            case STATE_AVANCE:
                PWMSetSpeedConsigne(26, MOTEUR_DROIT);
                PWMSetSpeedConsigne(25, MOTEUR_GAUCHE);
                stateRobot = STATE_AVANCE_EN_COURS;
                break;
            case STATE_AVANCE_EN_COURS:
                SetNextRobotStateInAutomaticMode();
                break;

            case STATE_TOURNE_GAUCHE:
                PWMSetSpeedConsigne(15, MOTEUR_DROIT);
                PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
                stateRobot = STATE_TOURNE_GAUCHE_EN_COURS;
                break;
            case STATE_TOURNE_GAUCHE_EN_COURS:
                SetNextRobotStateInAutomaticMode();
                break;

            case STATE_TOURNE_DROITE:
                PWMSetSpeedConsigne(0, MOTEUR_DROIT);
                PWMSetSpeedConsigne(15, MOTEUR_GAUCHE);
                stateRobot = STATE_TOURNE_DROITE_EN_COURS;
                break;
            case STATE_TOURNE_DROITE_EN_COURS:
                SetNextRobotStateInAutomaticMode();
                break;

            case STATE_TOURNE_SUR_PLACE_GAUCHE:
                PWMSetSpeedConsigne(15, MOTEUR_DROIT);
                PWMSetSpeedConsigne(-15, MOTEUR_GAUCHE);
                droite_Ou_gauche = 0;
                stateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS;
                break;
            case STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS:
                SetNextRobotStateInAutomaticMode();
                break;

            case STATE_TOURNE_SUR_PLACE_DROITE:
                PWMSetSpeedConsigne(-15, MOTEUR_DROIT);
                PWMSetSpeedConsigne(15, MOTEUR_GAUCHE);
                stateRobot = STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS;
                break;
            case STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS:
                SetNextRobotStateInAutomaticMode();
                break;
                
            case STATE_RECULE:
                if(droite_Ou_gauche == 0){
                    PWMSetSpeedConsigne(-20, MOTEUR_DROIT);
                    PWMSetSpeedConsigne(-10, MOTEUR_GAUCHE); 
                    droite_Ou_gauche = 1; }
                else if(droite_Ou_gauche == 1){
                    PWMSetSpeedConsigne(-10, MOTEUR_DROIT);
                    PWMSetSpeedConsigne(-20, MOTEUR_GAUCHE); 
                    droite_Ou_gauche = 0; }
                    stateRobot = STATE_RECULE_EN_COURS;
                break;
            case STATE_RECULE_EN_COURS:
                SetNextRobotStateInAutomaticMode();
                break; 
                
            case STATE_TOURNE__EXTREME_GAUCHE :
                PWMSetSpeedConsigne(15, MOTEUR_DROIT);
                PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
                stateRobot = STATE_TOURNE__EXTREME_GAUCHE_EN_COURS;
                break;
                
            case STATE_TOURNE__EXTREME_GAUCHE_EN_COURS :
                SetNextRobotStateInAutomaticMode();
                break;
                
                case STATE_TOURNE__EXTREME_DROIT :
                PWMSetSpeedConsigne(0, MOTEUR_DROIT);
                PWMSetSpeedConsigne(15, MOTEUR_GAUCHE);
                stateRobot = STATE_TOURNE__EXTREME_DROIT_EN_COURS;
                break;
                
            case STATE_TOURNE__EXTREME_DROIT_EN_COURS :
                SetNextRobotStateInAutomaticMode();
                break;
                
            default:
                stateRobot = STATE_ATTENTE;
                break;
        }
    } else {
        timestamp = 0;
        stateRobot = STATE_ATTENTE;
        robotState.vitesseDroiteConsigne = 0;
        robotState.vitesseGaucheConsigne = 0;
        LED_ORANGE = 1;
        LED_BLANCHE = 1;
        LED_BLEUE = 1 ;    
    }
}

unsigned char nextStateRobot = 0;

void SetNextRobotStateInAutomaticMode() {
    unsigned char positionObstacle = PAS_D_OBSTACLE;
    //Détermination de la position des obstacles en fonction des télémètres
//    if (robotState.distanceTelemetreDroit < 33 &&
//            robotState.distanceTelemetreCentre > 36 &&
//            robotState.distanceTelemetreGauche > 33) //Obstacle à droite
//        positionObstacle = OBSTACLE_A_DROITE;
//    else if (robotState.distanceTelemetreDroit > 33 &&
//            robotState.distanceTelemetreCentre > 36 &&
//            robotState.distanceTelemetreGauche < 33) //Obstacle à gauche
//        positionObstacle = OBSTACLE_A_GAUCHE;
//    else if (robotState.distanceTelemetreExtremeDroit < 15 )
//       positionObstacle = OBSTACLE_A_DROITE;
//    else if(robotState.distanceTelemetreExtremeGauche < 15)
//        positionObstacle = OBSTACLE_A_GAUCHE;
//    else if (robotState.distanceTelemetreCentre < 36) //Obstacle en face
//        positionObstacle = OBSTACLE_EN_FACE;
//    else if (robotState.distanceTelemetreDroit > 33 &&
//            robotState.distanceTelemetreCentre > 36 &&
//            robotState.distanceTelemetreGauche > 33) //pas d?obstacle
//        positionObstacle = PAS_D_OBSTACLE;
    
    
    if (robotState.distanceTelemetreExtremeDroit < 25 && robotState.distanceTelemetreDroit > 33 && robotState.distanceTelemetreCentre > 36 && robotState.distanceTelemetreGauche > 33 && robotState.distanceTelemetreExtremeGauche > 25 ) //Obstacle à extreme droite
    positionObstacle = OBSTACLE_EXTREME_DROIT;
    else if (robotState.distanceTelemetreExtremeGauche < 25 && robotState.distanceTelemetreDroit > 33 && robotState.distanceTelemetreCentre > 36 && robotState.distanceTelemetreGauche > 33 && robotState.distanceTelemetreExtremeDroit > 25 ) //Obstacle à extreme gauche
    positionObstacle = OBSTACLE_EXTREME_GAUCHE;
    else if (robotState.distanceTelemetreDroit < 33 && robotState.distanceTelemetreCentre > 36 && robotState.distanceTelemetreGauche > 33) //Obstacle à droite
    positionObstacle = OBSTACLE_A_DROITE;
    else if (robotState.distanceTelemetreDroit > 33 && robotState.distanceTelemetreCentre > 36 && robotState.distanceTelemetreGauche < 33) //Obstacle à gauche
    positionObstacle = OBSTACLE_A_GAUCHE;
    else if (robotState.distanceTelemetreCentre < 36 && robotState.distanceTelemetreExtremeDroit < robotState.distanceTelemetreExtremeGauche ) //Obstacle en face
    positionObstacle = OBSTACLE_EN_FACE_DROIT;
    else if (robotState.distanceTelemetreCentre < 36 && robotState.distanceTelemetreExtremeDroit > robotState.distanceTelemetreExtremeGauche ) //Obstacle en face
    positionObstacle = OBSTACLE_EN_FACE_GAUCHE;
    else if (robotState.distanceTelemetreCentre < 36 && robotState.distanceTelemetreExtremeDroit == robotState.distanceTelemetreExtremeGauche  ) //Obstacle en face
    positionObstacle = OBSTACLE_EN_FACE;
    else if (robotState.distanceTelemetreDroit > 33 && robotState.distanceTelemetreCentre > 36 && robotState.distanceTelemetreGauche > 33) //pas d?obstacle
    positionObstacle = PAS_D_OBSTACLE;
    
    
    //Détermination de l?état à venir du robot+9
    if (positionObstacle == PAS_D_OBSTACLE)
        nextStateRobot = STATE_AVANCE;
    else if (positionObstacle == OBSTACLE_A_DROITE)
        nextStateRobot = STATE_TOURNE_GAUCHE;
    else if (positionObstacle == OBSTACLE_A_GAUCHE)
        nextStateRobot = STATE_TOURNE_DROITE;
    else if (positionObstacle == OBSTACLE_EXTREME_DROIT)
        nextStateRobot = STATE_TOURNE__EXTREME_GAUCHE;
    else if (positionObstacle == OBSTACLE_EXTREME_GAUCHE)
        nextStateRobot = STATE_TOURNE__EXTREME_DROIT;
     else if (positionObstacle == OBSTACLE_EN_FACE)
        nextStateRobot = STATE_RECULE;
    else if (positionObstacle == OBSTACLE_EN_FACE_DROIT)
        nextStateRobot =STATE_TOURNE_SUR_PLACE_GAUCHE;
    else if (positionObstacle == OBSTACLE_EN_FACE_GAUCHE)
        nextStateRobot =STATE_TOURNE_SUR_PLACE_DROITE;
    

    //Si l?on n?est pas dans la transition de l?étape en cours
    if (nextStateRobot != stateRobot - 1)
        stateRobot = nextStateRobot;
}
