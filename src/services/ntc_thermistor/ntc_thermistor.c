/**
 * @file myNTC_Thermistor.c
 * @author Jackson Kenedi Lewandowski (jackson.lewandowski@komlog.com/jacksonlewandowski.work@gmail.com)
 * @brief   Essa biblioteca utiliza é utilizada para converter o valor medido no adc em temperatura.
 * @version 0.1
 * @date 2024-11-11
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "ntc_thermistor.h"
#include <stdlib.h>
/**
 * @brief Cria um novo objeto Thermistor para auxiliar na conversão da medida do ADC em temperatura.
 * 
 * Este objeto utiliza um circuito divisor de tensão para a medição de temperatura com um termistor NTC. 
 * Abaixo está a referência do circuito utilizado:
 *
 *       [VCC]  <- voltageRef
 *         |
 *       [Rf]   <- refResistance
 *         |
 *         +---------------------------+------ <- [ADC]
 *         |                           |
 *       [Rp] <- parallelResistance   [T] <- Thermistor
 *         |                           |
 *         +---------------------------+
 *         |
 *       [GND]
 *
 * @param ptrAux Ponteiro para o objeto Thermistor a ser configurado.
 * @param parallelResistance Resistência em paralelo com o Thermistor (Rp).
 * @param refResistance Resistência de referência utilizada para medir a tensão (Rf).
 * @param voltageRef Tensão de alimentação do circuito de medida.
 *
 * As variáveis adicionais são usadas para garantir maior precisão na conversão de resistência para temperatura.
 * O vetor de resistências do NTC foi extraído do datasheet do componente e está presente na biblioteca `NTC_10K_3380.h`.
 * 
 * O vetor de resistências cobre um intervalo de 180 valores, de 486088 Ω (-55°C) até 538 Ω (125°C), com espaçamento de 1°C entre 
 * cada valor. Cada posição do vetor representa uma resistência para uma temperatura específica, onde a posição do valor (índice) 
 * somada ao offset (-55) corresponde à temperatura medida. 
 * 
 * Exemplo: Se a resistência medida corresponde ao valor da posição 63 do vetor, a temperatura será 8°C.
 * 
 * @param offset Temperatura mínima presente no vetor.
 * @param max Temperatura máxima presente no vetor.
 * @param NTC Vetor de resistências para cada temperatura, utilizado na conversão de resistência para temperatura.
 */
void thermistor_new(NTC_Thermistor *ptrAux, int parallelResistance, int refResistance, 
                        short voltageRef, int offset, int max,  const int *NTC)
{
    ptrAux->_NTC = NTC; 
    ptrAux->_parallelResistance = parallelResistance;
    ptrAux->_refResistance = refResistance;
    ptrAux->_voltageRef = voltageRef;
    ptrAux->_offset = offset;
    ptrAux->_max = max;
}
/**
 * @brief Busca a temperatura referente a resistencia medida
 * 
 * @param ptrThermistor Ponteiro para o Thermistor de referencia
 * @param resistance Resistencia medida
 * @return short temperatura em valor short, por exemplo:
 *          Se a temperatura medida é 25.487°C o valor retornado é 2548. Não é feito o arredondamento da ultima casa decimal!
 * Os valores retornados podem saturar, como exemplo:
 *   Retorno 
 *  -80 |
 *  -55 |----------
 *      |          \
 *      |           \
 *      |            \
 *      |             \
 *      |              \  
 *      |               \
 *      |                \
 *      |                 \
 *      |                  \    
 *  125 |                   ----------------
 *  150 |-----------------------------------
 *       -100 -80 -55      125  150   200 ->    Temperatura (°C)
 * 
 */
short _findTemp(NTC_Thermistor *ptrThermistor, int resistance)
{
    int size = (ptrThermistor->_max-ptrThermistor->_offset); //Descobrindo o tamanho do vetor do Thermisto Tmax-Tmin -> no caso 125-(-55) = 180
    for (short i = 0; i < size; i++)//Percorre o vetor procurando o valor mais proximo da resistencia medida
    {
        if (ptrThermistor->_NTC[i] == resistance)  //Se o valor for igual a resistencia medida 
            return (i+ptrThermistor->_offset)*100; //Retorna (short) (index+offset)*100
            else if (ptrThermistor->_NTC[i] < resistance) //O NTC diminui a resistencia conforme a temperatura sobe, portanto se a resistencia for menor quer dizer que chegamos proximo da temperatura.
            {
                if (i == 0)//Se é a primeira interação então a temperatura é mínima (no nosso caso -55°C)
                    return ptrThermistor->_offset*100;
                long int delta = (ptrThermistor->_NTC[i-1]-ptrThermistor->_NTC[i]); //Faz a aproximação da temperatura medida caso a resistencia esteja entre dois valores do vetor
                delta = (((resistance-ptrThermistor->_NTC[i])*1000) / delta)/10;
                return ((100*(i+ptrThermistor->_offset)) - delta);
            }   
    }
    return ptrThermistor->_max*100;//Se estiver fora da faixa retorna a temperatura maxima presente na biblioteca
}
/**
 * @brief Faz a conversão da medida de tensão em Graus Celscius
 * 
 * @param ptrThermistor Ponteiro para o thermistor que está sendo medido
 * @param measuredValue Valor medido pelo adc em mV
 * @return short Temperatura medida
 */
short convert_mV_to_celcius(NTC_Thermistor *ptrThermistor, short measuredValue)
{
/*
      [VCC]  <- voltageRef
        |
       [Rf]   <- refResistance
        |
        +---------------------------+------ <- [ADC]
        |                           |
       [Rp] <- parallelResistance   [T] <- Thermistor
        |                           |
        +---------------------------+
        |
       [GND]   
    */
    long int mvTo_nv = 1000;//Variavel utilizada para transformar mV me nV
    long int voltageValue = mvTo_nv*(ptrThermistor->_voltageRef - measuredValue);//Tensão sobre os terminais do resistor de referencia
    long int LongAux = voltageValue/ptrThermistor->_refResistance;//Corrente que está passando no circuito de medição
    LongAux = (mvTo_nv*measuredValue)/LongAux;//Calculo para identificar a relação de resistencia em do NTC com a resistencia em paralelo
    LongAux = ((LongAux * ptrThermistor->_parallelResistance) / (ptrThermistor->_parallelResistance - LongAux)); //Calculo para identificar a resistencia do NTC
    return _findTemp(ptrThermistor, (int)LongAux);//Busca a temperatura referente a resistencia encontrada e retorna esse valor
}