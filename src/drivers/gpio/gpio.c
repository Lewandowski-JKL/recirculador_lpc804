#include "gpio.h"
#include "board.h"
#include <stdbool.h>

bool gpioOn=false; //Flag de referencia se a GPIO do micontrolador ja foi Iniciada
unsigned int inputRefPin = 0;
unsigned int outputRefPin = 0;
/**
 * @brief Faz a inicialização da GPIO do microcontrolador
 * 
 */
void gpio_enableClock()
{
    SYSCON->SYSAHBCLKCTRL0 |= (SYSCON_SYSAHBCLKCTRL0_GPIO0_MASK);
    
    SYSCON->PRESETCTRL0 &= ~SYSCON_PRESETCTRL0_GPIO0_RST_N_MASK;// reset GPIO (bit=0)
    SYSCON->PRESETCTRL0 |= SYSCON_PRESETCTRL0_GPIO0_RST_N_MASK;// clear reset (bit=1)
    gpioOn = true;
}
/**
 * @brief Define o Pino do microcontrolador como entrada
 * 
 * @param pin Pino que será definido como entrada
 */
void gpio_ConfigPinInput(unsigned char pin)
{
    if (!gpioOn) //Verifica se a GPIO ja foi iniciada
        gpio_enableClock();
    //GPIO->DIRSET[0] &= ~(1UL<<pin); //define o pino como entrada
    inputRefPin |= (1UL<<pin); //Seta flag dos pinos de entrada
    outputRefPin &= ~(1UL<<pin); //Limpa flag dos pinos de saida
    GPIO->DIRCLR[0] = (1UL<<pin); //define o pino como entrada
    gpioOn = true;//Seta a flag da GPIO
}
/**
 * @brief Define a Porta interira do microcontrolador como entrada
 * 
 * @param port Porta a ser definida compo entrada
 */
void gpio_ConfigPortInput(unsigned char port)
{
    if (!gpioOn) //Verifica de a GPIO ja foi iniciada
        gpio_enableClock(); //Inicia a GPIO
    inputRefPin = 0xFFFF;//Marca todos os pinos como entrada
    outputRefPin = 0UL;//Limpa todos os pinos de saída
    GPIO->DIRCLR[port] = 0xFFFF; //define a porta como entrada
    gpioOn = true;  //Seta a flag de inicialização de GPIO
}
/**
 * @brief Define o pino como Saída
 * 
 * @param pin 
 */
void gpio_ConfigPinOutput(unsigned char pin)
{
    if (!gpioOn)
        gpio_enableClock();
    inputRefPin &= ~(1UL<<pin);//Limpa o estado de entrada do pino na variavel
    outputRefPin |= (1UL<<pin);//Escreve o estado de saída do pino na variavel
    GPIO->DIRSET[0] = (1UL<<pin); //define o pino como saída
    gpioOn = true;  
}
/**
 * @brief Define a porta como saída
 * 
 * @param port 
 */
void gpio_ConfigPortOutput(unsigned char port)
{
    if (!gpioOn)//Verifica se a GPIO ja foi iniciada
        gpio_enableClock();//Inicia a GPIO
    inputRefPin = 0UL;//Limpa a variavel dos pinos de entrada
    outputRefPin = 0xFFFF;//Marca todos os pinos como saída
    GPIO->DIRSET[port] =  0xFFFF; //define a porta como saída
    gpioOn = true;  
}
/**
 * @brief Faz a leitura de um pino definido como entrada
 * 
 * @param pin 
 * @return true 
 * @return false 
 */
bool gpio_ReadPin(unsigned char pin)
{
    if ((1UL<<pin)&inputRefPin)//Verifica se o pino ta como entrada
        return (bool)(GPIO->B[0][pin]);//retorna valor lido
    return false;
}
/**
 * @brief 
 * 
 * @param port 
 * @return 
 */
unsigned int gpio_ReadPort(unsigned char port){return 0;}
/**
 * @brief Escreve valor no pino definido como saída
 * 
 * @param pin 
 * @param value 
 */
void gpio_WritePin(unsigned char pin, bool value)
{
    if ((1UL<<pin)&outputRefPin)//Verifica se o pino esta definido como saída
        value ? gpio_SetPin(pin) : gpio_ClearPin(pin);
}
/**
 * @brief Coloca o pino em estado HIGH ou '1'
 * 
 * @param pin 
 */
void gpio_SetPin(unsigned char pin)
{
    if ((1UL<<pin)&outputRefPin)//verifica se é um pino de saída
        GPIO->SET[0] = (1UL<<pin);
}
/**
 * @brief Coloca o pino em estado LOW ou '0'
 * 
 * @param pin 
 */
void gpio_ClearPin(unsigned char pin)
{
    if ((1UL<<pin)&outputRefPin)//verifica se é um pino de saída
        GPIO->CLR[0] = (1UL<<pin);
}
/**
 * @brief Altera o estado pino do microcontrolador
 * 
 * @param pin 
 */
void gpio_TogglePin(unsigned char pin)
{   
    if ((1UL<<pin)&outputRefPin)//verifica se é um pino de saída
        GPIO->NOT[0] = (1UL << pin);
}
/**
 * @brief Escreve valor na porta do microcontolador
 * 
 * @param port 
 * @param value 
 */
void gpio_WritePort(unsigned char port, unsigned int value)
{
    GPIO->CLR[port] = ~value;
    GPIO->SET[port] = value;
}
/**
 * @brief Coloca todos os pinos da porta do microcontrolador em HIGH
 * 
 * @param port 
 */
void gpio_SetPort(unsigned char port, unsigned int mask)
{
    GPIO->SET[0] = mask;//0xFFFF
}
/**
 * @brief Coloca todos os pinos da porta do microcontrolador em LOW
 * 
 * @param port 
 */
void gpio_ClearPort(unsigned char port, unsigned int mask)
{
    GPIO->CLR[0] = mask;//0xFFFF
}
/**
 * @brief Altera o estado da porta do microcontrolador
 * 
 * @param port 
 */
void gpio_TogglePort(unsigned char port, unsigned int mask)
{
    GPIO->NOT[port] = mask;//0xFFFF
}
void gpio_resetDisable()
{
    SWM0->PINENABLE0 |= (1<<6);
}
void gpio_resetEnable()
{
    SWM0->PINENABLE0 &= ~(1<<6);
}