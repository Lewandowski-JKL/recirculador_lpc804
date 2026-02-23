#include "myADC.h"



int readADC(myADC *adc)
{
    unsigned int adc_result = 0;
    unsigned int value = 0;
    for (unsigned short i = 0; i < adc->_measureLength; i++)
    {
        //	Step	6.	read	result	bits	in	DAT8	(data	for	channel	8)	for	conversion	result.
        ADC0->SEQ_CTRL[0]	&=	~(1UL<<ADC_SEQ_CTRL_START_SHIFT);		//	start	bit	to	0
        ADC0->SEQ_CTRL[0]	|=	(1UL<<ADC_SEQ_CTRL_START_SHIFT);		//	start	bit	to	1
        //	Read	the	captured	value	on	ADC	ch	8.	Assign	it	to	a	variable.
        adc_result	= ((ADC0->DAT[((myADC*)adc)->_channel])&(ADC_DAT_RESULT_MASK));//	isolate	bits	15:4	(data)
        value += (adc_result>>ADC_DAT_RESULT_SHIFT);//	shift	right;get	true	numeric	value
    }
    return (value/adc->_measureLength);
}
unsigned char adcOn = 0;
float readMiliVolts(myADC *adc)
{
    return adc->_convertTo_mv * (float)readADC(adc);
}

void adcBegin()
{
    //inicia o ADC
    SYSCON->PDRUNCFG &=	~(SYSCON_PDRUNCFG_ADC_PD_MASK);
    SYSCON->SYSAHBCLKCTRL0 |= (SYSCON_SYSAHBCLKCTRL0_ADC_MASK | SYSCON_SYSAHBCLKCTRL0_SWM_MASK);
    //	Reset	the	ADC	module.
    //	Table	64	in	User	Manual.
    //	Bit	24	in	Peripheral	Reset	Control	Register	0:	go	0,	then	1	to	reset	and	clear.
    SYSCON->PRESETCTRL0 &=	~(SYSCON_PRESETCTRL0_ADC_RST_N_MASK);//	Assert reset	(0)
    SYSCON->PRESETCTRL0 |=	(SYSCON_PRESETCTRL0_ADC_RST_N_MASK);//	Remove	reset(1)
    //	Set	to	00 to	use	the	FRO	(yes!);	01	to	use	an	external	clock	(no!);	11	is	no	clock.	(no!)
    //	FRO	runs	at	750	kHz.
    SYSCON->ADCCLKSEL &= ~(SYSCON_ADCCLKSEL_SEL_MASK);	//	Use	fro_clk	as	source	for	ADC	async clock
    //	Divide	the	FRO	clock	into	the	ADC.		If	0	it	shuts	down	the	ADC	clock?
    SYSCON->ADCCLKDIV =	1;//	divide	by	1	(values:	0	to	255)
    adcOn = 1;
}

myADC *newADC(unsigned char channel, unsigned short measureLength)
{
    if (~adcOn)
        adcBegin();
    myADC *ADCaux = (myADC*)malloc(sizeof(myADC));
    ADCaux->_channel = channel;
    ADCaux->_measureLength = measureLength;
    ADCaux->_convertTo_mv = 3.3/((double)0xFFF);

    SWM0->PINENABLE0 &=	~(1UL <<(SWM_PINENABLE0_ADC_0_SHIFT+channel));//
    ADC0->SEQ_CTRL[0] |= (1UL << channel);
    //Set	TRIGPOL	to	1	and	SEQ_ENA	to	1	in	SEQA_CTRL	register
    ADC0->SEQ_CTRL[0] |= (1UL<<ADC_SEQ_CTRL_TRIGPOL_SHIFT);	//	trig	pol	set	to	1.
    ADC0->SEQ_CTRL[0] |= (1UL<<ADC_SEQ_CTRL_SEQ_ENA_SHIFT);	//	Sequence	A	turned	ON.
    //set	START	bit	to	1	in	SEQA_CTRL	register
    //This	bit	can	only	be	set	momentarily.		It	immediately	goes	back	to 0
    //and	so	always	gets	read	back	as	0.
    ADC0->SEQ_CTRL[0]	|=	(1UL<<ADC_SEQ_CTRL_START_SHIFT);		//	start	bit	to	1
    return ADCaux;
}



