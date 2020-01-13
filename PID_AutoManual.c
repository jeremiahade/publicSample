/*
 * PID_AutoManual.c
 *
 * Created: 30/12/2019 19:29:57
 *  Author: Jeremiah A.
 */ 

#include "PID_AutoManual.h"


void PID_Init(int16_t kp, int16_t ki, int16_t kd, pidDataT *pid);
int32_t PI_Control(pidDataT *pi_in, pidStatusT *piStat, int16_t setPoint, int16_t processVar);
int32_t PD_Control(pidDataT *pd_in, pidStatusT *pdStat, int16_t setPoint, int16_t processVar);
int32_t PID_Control(pidDataT *pid_in, pidStatusT *pidStat, int16_t setPoint, int16_t processVar);
void Integral_Reset(pidDataT *pid_in);


void PID_Init(int16_t kp, int16_t ki, int16_t kd, pidDataT *pid)
{
	
	#if (USE_SCALE_FACTOR == true)
	
	pid->previousProcessVar = 0;
	pid->sumError = 0;
	
	pid->kp = kp * SCALING_FACTOR;
	pid->ki = ki * SCALING_FACTOR;
	pid->kd = kd * SCALING_FACTOR;
	

#endif
	
	#if (USE_TIME_INTERVAL == true)
	
	pid->kp = kp;
	pid->ki = ki;
	pid->kd = kd;

#endif

// Limits to avoid overflow
pid->maxError = MAX_INT / (pid->kp + 1);
pid->maxSumError = MAX_I_TERM / (pid->ki + 1);

}

int32_t PI_Control(pidDataT *pi_in, pidStatusT *piStat, int16_t setPoint, int16_t processVar)
{
	int16_t pTerm, error = 0;
	int32_t iTerm, tempBuff, PI_Result = 0;
	
	error = setPoint - processVar;
	
	if (error > pi_in->maxError)
	{
		pTerm = MAX_INT;
	} 
	else if (error < -pi_in->maxError)
	{
		pTerm = -MAX_INT;
	}
	else
	{
		pTerm = pi_in->kp * error;
	}
	
	#if (USE_SCALE_FACTOR == true)
	   tempBuff = pi_in->sumError + error;
	   
	   if (tempBuff > pi_in->maxSumError)
	   {
		   iTerm = MAX_I_TERM;
		   pi_in->sumError = pi_in->maxSumError;
	   }
	   else if (tempBuff < -pi_in->maxSumError)
	   {
		   iTerm = -MAX_I_TERM;
		   pi_in->sumError = pi_in->maxSumError;
	   }
	   else
	   {
		   pi_in->sumError = tempBuff;
		   iTerm = pi_in->ki * pi_in->sumError;
	   }
	   
	   PI_Result = (pTerm + iTerm)/SCALING_FACTOR;
#endif

    #if (USE_TIME_INTERVAL == true)
	tempBuff = (pi_in->sumError + error)/pi_in->timeDuration;
	
	if (tempBuff > pi_in->maxSumError)
	{
		iTerm = MAX_I_TERM;
		pi_in->sumError = pi_in->maxSumError;
	} 
	else if(tempBuff < -pi_in->maxSumError)
	{
		iTerm = -MAX_I_TERM;
		pi_in->sumError = -pi_in->maxSumError;
	}
	else
	{
		iTerm = pi_in->ki * tempBuff;
	}
	
	PI_Result = pTerm + iTerm;

#endif
	
	
	return PI_Result;
}

int32_t PD_Control(pidDataT *pd_in, pidStatusT *pdStat, int16_t setPoint, int16_t processVar)
{
	int16_t pTerm, error = 0;
	int32_t dTerm,  PD_Result = 0;
	
	error = setPoint - processVar;
	
	if (error > pd_in->maxError)
	{
		pTerm = MAX_INT;
	}
	else if (error < -pd_in->maxError)
	{
		pTerm = -MAX_INT;
	}
	else
	{
		pTerm = pd_in->kp * error;
	}
	
	#if (USE_SCALE_FACTOR == true)
	dTerm = pd_in->kd * (processVar - pd_in->previousProcessVar);
	PD_Result = (pTerm + dTerm)/SCALING_FACTOR;
	
	if (PD_Result > pd_in->maxPIDlimit)
	{
		PD_Result = pd_in->maxPIDlimit;
	} 
	else if (PD_Result < pd_in->minPIDlimit)
	{
		PD_Result = pd_in->minPIDlimit;
	}
#endif

    #if (USE_TIME_INTERVAL == true)
	int32_t termBuff = 0;
	
	termBuff = (processVar - pd_in->previousProcessVar)/pd_in->timeDuration;
	dTerm = pd_in->kd * termBuff;
	PD_Result = pTerm + dTerm;
	
	if (PD_Result > pd_in->maxPIDlimit)
	{
		PD_Result = pd_in->maxPIDlimit;
	}
	else if (PD_Result < pd_in->minPIDlimit)
	{
		PD_Result = pd_in->minPIDlimit;
	}
#endif
	
	return PD_Result;
}

int32_t PID_Control(pidDataT *pid_in, pidStatusT *pidStat, int16_t setPoint, int16_t processVar)
{
    int16_t pTerm, error = 0;
	int32_t dTerm, iTerm, tempBuff, PID_Result = 0;
	
	error = setPoint - processVar;
	
	if (error > pid_in->maxError)
	{
		pTerm = MAX_INT;
	}
	else if (error < -pid_in->maxError)
	{
		pTerm = -MAX_INT;
	}
	else
	{
		pTerm = pid_in->kp * error;
	}
	
	#if (USE_SCALE_FACTOR == true)
	 tempBuff = pid_in->sumError + error;
	 
	 if (tempBuff > pid_in->maxSumError)
	 {
		 iTerm = MAX_I_TERM;
		 pid_in->sumError = pid_in->maxSumError;
	 }
	 else if (tempBuff < -pid_in->maxSumError)
	 {
		 iTerm = -MAX_I_TERM;
		 pid_in->sumError = pid_in->maxSumError;
	 }
	 else
	 {
		 pid_in->sumError = tempBuff;
		 iTerm = pid_in->ki * pid_in->sumError;
	 }
	 
	dTerm = pid_in->kd * (processVar - pid_in->previousProcessVar);
	
	if (PID_Result > pid_in->maxPIDlimit)
	{
		PID_Result = pid_in->maxPIDlimit;
	} 
	else if (PID_Result < pid_in->minPIDlimit)
	{
		PID_Result = pid_in->minPIDlimit;
	}
	else
	{
		PID_Result = (pTerm + iTerm + dTerm)/SCALING_FACTOR;
	}
#endif

    #if (USE_TIME_INTERVAL == true)
	int32_t termBuff = 0;
	
	tempBuff = (pid_in->sumError + error)/pid_in->timeDuration;
	
	if (tempBuff > pid_in->maxSumError)
	{
		iTerm = MAX_I_TERM;
		pid_in->sumError = pid_in->maxSumError;
	}
	else if(tempBuff < -pid_in->maxSumError)
	{
		iTerm = -MAX_I_TERM;
		pid_in->sumError = -pid_in->maxSumError;
	}
	else
	{
		iTerm = pid_in->ki * tempBuff;
	}
	
	termBuff = (processVar - pid_in->previousProcessVar)/pid_in->timeDuration;
	dTerm = pid_in->kd * termBuff;
	PID_Result = pTerm + iTerm + dTerm;
	
	if (PID_Result > pid_in->maxPIDlimit)
	{
		PID_Result = pid_in->maxPIDlimit;
	}
	else if (PID_Result < pid_in->minPIDlimit)
	{
		PID_Result = pid_in->minPIDlimit;
	}
#endif	

return PID_Result;
}