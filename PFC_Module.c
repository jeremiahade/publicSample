/***********************************
 * PFC_Module.c
 *
 * Created: 29/12/2019 20:46:18
 *  Author: Jeremiah A.
 **********************************/ 

#include "PFC_Module.h"
#include <util/delay.h>



double	vLoadRms	=	0;
double	vSupply	=	0;
		
		
 double FiringAngleConstant (double PIDOuptut);
 uint8_t TriggerDelay (double firingAngleConstant);
 uint8_t FiringAngle (double firingAngleConstant);
 bool PhaseFiringOutput (bool zeroCrossingDetector, bool *firingOutput, uint8_t triggerDelay);
 uint8_t LoadVoltageRms(uint8_t supplyVoltage, uint8_t firingAngle);

 
 void my_delay_ms(uint8_t delay) {
	 while(delay--) {
		 _delay_ms(1);
	 }
 }
 
 double FiringAngleConstant(double PIDOuptut)
 {
	 double	k	=	0;   //Firing angle constant K
	 double negatedPIDOutput	=	0;
	 
	 negatedPIDOutput	=	PID_OUTPUT_NEGATE - PIDOuptut;
	 k	=	negatedPIDOutput/PI;
	 
	 return k;
 }
 
 uint8_t TriggerDelay(double firingAngleConstant)
 {
	 uint8_t  triggerDelay	=	0;
	 
	 triggerDelay	=	((uint8_t)firingAngleConstant * 1000)/100;
	 
	 return triggerDelay;
 }
 
 uint8_t FiringAngle(double firingAngleConstant)
 {
	 uint8_t firingAngle = 0;
	 
	 firingAngle	=	firingAngleConstant * PI;
	 
	 return firingAngle;
 }
 
 bool PhaseFiringOutput(bool zeroCrossingDetector, bool *firingOutput, uint8_t triggerDelay)
 {
	 bool zeroCrossingReset	=	true ;
	 
	 if (zeroCrossingDetector == true)
	 {
		 my_delay_ms(triggerDelay);
		 *firingOutput = true;
		 _delay_us (TRIGGER_STOP_DELAY);
		 *firingOutput = false;
		 zeroCrossingReset = false;
	 }
	 
	 return zeroCrossingReset;
 }
 
 uint8_t LoadVoltageRms(uint8_t supplyVoltage, uint8_t firingAngle)
 {
	uint8_t loadVoltageRms	=	0;
	double	interimResultVlrms = 0;
	double	squarerootInterimResultVlrms	=	0;
	double	sineResult	=	0;
	double	sineInput	=	0;
	
	sineInput = 2 * (double)firingAngle;
	sineResult = sin(sineInput);
	interimResultVlrms	=	(1/PI)*(PI - firingAngle + sineResult);
	squarerootInterimResultVlrms	=	sqrt(interimResultVlrms);
	loadVoltageRms	=	(uint8_t) (supplyVoltage * squarerootInterimResultVlrms); 
		
	return loadVoltageRms;
 }