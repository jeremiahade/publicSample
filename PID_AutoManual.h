/*
 * PID_AutoManual.h
 *
 * Created: 30/12/2019 19:30:46
 *  Author: Jeremiah A.
 */ 


#ifndef PID_AUTOMANUAL_H_
#define PID_AUTOMANUAL_H_

#ifdef _cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <atmel_start.h>


#define USE_SCALE_FACTOR false
#define USE_TIME_INTERVAL true




#if (USE_SCALE_FACTOR == true && USE_TIME_INTERVAL == true)
#error "scale factor and time interval is true"
#endif

#if (USE_SCALE_FACTOR == false && USE_TIME_INTERVAL == false)
#error "scale factor and time interval is false"
#endif

#if (USE_SCALE_FACTOR == true)
#ifdef TIME_INTERVAL
#undef TIME_INTERVAL
#endif
#define SCALING_FACTOR 180
#endif

#if (USE_TIME_INTERVAL == true)
#ifdef SCALING_FACTOR
#undef SCALING_FACTOR
#endif
#define TIME_INTERVAL  1
#endif


typedef struct PID_DATA
{
	int16_t kp;
	int16_t ki;
	int16_t kd;
	int16_t proportionalTerm;
	int16_t integralTerm;
	int16_t derivativeTerm;
	int16_t previousProcessVar;
	int16_t sumError;
	int16_t maxSumError;
	int16_t maxError;
	int16_t timeDuration;
	int16_t maxPIDlimit;
	int16_t minPIDlimit;
	}pidDataT;
	
typedef struct PID_STATUS
{
	bool Stable;
	bool TuningInProgress;
	bool TuningCompleted;
	bool Oscillating;
	bool unstable;
	bool Overshoot;
	bool undershoot;
	bool positiveProgress;
	bool negativeProgress;
	}pidStatusT;

// Maximum value of variables
#define MAX_INT         INT16_MAX
#define MAX_LONG        INT32_MAX
#define MAX_I_TERM      (MAX_LONG / 2)

#endif /* PID_AUTOMANUAL_H_ */