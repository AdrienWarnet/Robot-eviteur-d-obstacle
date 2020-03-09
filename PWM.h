/* 
 * File:   PWM.h
 * Author: table4
 *
 * Created on 4 septembre 2019, 15:50
 */

#ifndef PWM_H
#define	PWM_H

#define MOTEUR_DROIT 0
#define MOTEUR_GAUCHE 1


//void PWMSetSpeed(float, char);
void InitPWM(void);
void PWMUpdateSpeed(void);
void PWMSetSpeedConsigne(float , char);

#endif	/* PWM_H */

