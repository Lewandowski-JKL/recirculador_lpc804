#ifndef MYNTC_THERMISTOR_H
#define MYNTC_THERMISTOR_H
#include "NTC_10K_3380.h"

typedef struct NTC_Thermistor
{
    float _parallelResistance;
    float _refResistance;
    float _voltageRef;
    const int *_NTC;
}NTC_Thermistor;

NTC_Thermistor *newThermistor(float parallelResistance, float refResistance, float voltageRef, const int *NTC);
float readTemp_celcius(NTC_Thermistor *ptrThermistor, float measuredValue);

#endif