#ifndef NTC_THERMISTOR_H
#define NTC_THERMISTOR_H

typedef struct NTC_Thermistor
{
    int _parallelResistance;
    int _refResistance;
    short _voltageRef;
    const int *_NTC;
    int _offset;
    int _max;
}NTC_Thermistor;

void newThermistor(NTC_Thermistor *ptrAux, int parallelResistance, int refResistance, 
                        short voltageRef, int offset, int max, const int *NTC);
short convert_mV_to_celcius(NTC_Thermistor *ptrThermistor, short measuredValue);

#endif