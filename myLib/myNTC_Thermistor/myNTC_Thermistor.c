#include "myNTC_Thermistor.h"
#include <stdlib.h>

NTC_Thermistor *newThermistor(float parallelResistance, float refResistance, float voltageRef, const int *NTC)
{
    NTC_Thermistor *ptrAux = (NTC_Thermistor*)malloc(sizeof(NTC_Thermistor));
    ptrAux->_NTC = NTC; 
    ptrAux->_parallelResistance = parallelResistance;
    ptrAux->_refResistance = refResistance;
    ptrAux->_voltageRef = voltageRef;
    return ptrAux;
}

float readTemp_celcius(NTC_Thermistor *ptrThermistor, float measuredValue)
{
    float floatAux = 0.0;
    //calcular corrente

    return 1.0;
}