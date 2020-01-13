/***********************************
 * PFC_Module.h
 *
 * Created: 29/12/2019 20:46:48
 *  Author: Jeremiah A.
 **********************************/ 


#ifndef PFC_MODULE_H_
#define PFC_MODULE_H_

#include "atmel_start.h"
#include <math.h>

#ifdef __cplusplus
extern "C" {
	#endif
	
#define PI    180
#define PID_OUTPUT_NEGATE	180
#define TRIGGER_STOP_DELAY	250

#ifndef PI
#error "PI must be defined"
#endif

#if (PI != 180)
#error "defined PI must be equal to 180"
#endif

#ifndef TRIGGER_STOP_DELAY
#error "Trigger delay must be defined"
#endif

#ifndef PID_OUTPUT_NEGATE
#define PID_OUTPUT_NEGATE 180 
#endif


 double FiringAngleConstant (double PIDOuptut);
 uint8_t TriggerDelay (double firingAngleConstant);
 uint8_t FiringAngle (double firingAngleConstant);
 bool PhaseFiringOutput (bool zeroCrossingDetector, bool *firingOutput, uint8_t triggerDelay);
 uint8_t LoadVoltageRms(uint8_t supplyVoltage, uint8_t firingAngle);





#endif /* PFC_MODULE_H_ */