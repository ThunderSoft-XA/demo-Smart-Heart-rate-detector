#include "plusensor.h"


#define true 1
#define false 0
int BPM;                   // used to hold the pulse rate
int Signal;                // holds the incoming raw data
int IBI = 600;             // holds the time between beats, must be seeded! 
unsigned char Pulse = false;     // true when pulse wave is high, false when it's low
unsigned char QS = false;        // becomes true when Arduoino finds a beat.
int rate[10];                    // array to hold last ten IBI values
unsigned long sampleCounter = 0;          // used to determine pulse timing
unsigned long lastBeatTime = 0;           // used to find IBI
int P =512;                      // used to find peak in pulse wave, seeded
int T = 512;                     // used to find trough in pulse wave, seeded
int thresh = 512;                // used to find instant moment of heart beat, seeded
int amp = 100;                   // used to hold amplitude of pulse waveform, seeded
int Num;
unsigned char firstBeat = true;        // used to seed rate array so we startup with reasonable BPM
unsigned char secondBeat = false;      // used to seed rate array so we startup with reasonable BPM

extern qapi_ADC_Handle_t adc_handle;
extern qapi_Adc_Input_Properties_Type_t Properties_ADC1;
extern qapi_Adc_Input_Properties_Type_t Properties_ADC0;




void Pulsesensor_init()
{
	qapi_Status_t status = QAPI_ERROR;
	const char *Channel_Name_ADC0 = ADC_INPUT_ADC0;
	qapi_Timer_Sleep(2, QAPI_TIMER_UNIT_SEC, true);
	status = adc_open_handle();
	if(status != QAPI_OK)
    {
		//IOT_DEBUG("Get ADC Handle ERROR!");
		gizLog(LOG_INFO,"adc open handle error...\n");
		return;
    }
	status = adc_get_properties(Channel_Name_ADC0, &Properties_ADC0);
    if(status != QAPI_OK)
    {
		//IOT_DEBUG("Get ADC channel-%s Configuration ERROR!", Channel_Name_ADC1);
		gizLog(LOG_INFO,"Get ADC channel-%s Configuration ERROR...\n", Channel_Name_ADC0);
		return;
    }
}


uint8_t getHeartRateValue(uint32_t* heartrate )			
{
	gizLog(LOG_INFO,"in getHeartRateValue...\n");
	qapi_Status_t status = QAPI_ERROR;
	qapi_ADC_Read_Result_t result;
	uint8_t i = 0;
	unsigned int runningTotal;
	//if(htim->Instance==htim3.Instance)
	//{
	//Signal=HAL_ADC_GetValue(&hadc1)>>2;					 // read the Pulse Sensor

	memset(&result, 0, sizeof(result));
	status = qapi_ADC_Read_Channel(adc_handle, &Properties_ADC0, &result);
	if(QAPI_OK == status)
	{
		if(ADC_RESULT_VALID == result.eStatus)
		{
			Signal = result.nMicrovolts / 1000;
			gizLog(LOG_INFO,"get ADC value , Signal = %d\n", result.nMicrovolts); 
		}
	}
	else
	{
		gizLog(LOG_INFO,"Read Adc error\n");
		return -1;
	}
	sampleCounter += 10;                         // keep track of the time in mS with this variable
	Num = sampleCounter - lastBeatTime;       // monitor the time since the last beat to avoid noise
		//HAL_ADC_Start(&hadc1);									//restart ADC conversion

		//  find the peak and trough of the pulse wave
	if(Signal < thresh && Num > (IBI/5)*3)		// avoid dichrotic noise by waiting 3/5 of last IBI
	{  
		if(Signal < T)													// T is the trough	
		{                       
			T = Signal;                      						   // keep track of lowest point in pulse wave 
			gizLog(LOG_INFO,"Find trough, T = %d\n", T);
		}
	}

	if(Signal > thresh && Signal > P)		// thresh condition helps avoid noise
	{          
		P = Signal;                             // P is the peak,keep track of highest point in pulse wave
		gizLog(LOG_INFO,"Find peak, P = %d\n", P);
	}                                        

		//  NOW IT'S TIME TO LOOK FOR THE HEART BEAT		
		// signal surges up in value every time there is a pulse
	if (Num > 600)				// 
	{                                   // avoid high frequency noise	
		if ( (Signal > thresh) && (Pulse == false) && (Num > (IBI/5)*3) )
		{        
			Pulse = true;                               // set the Pulse flag when we think there is a pulse
			//HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);                // turn on pin 13 LED
			IBI = sampleCounter - lastBeatTime;         // measure time between beats in mS
			lastBeatTime = sampleCounter;               // keep track of time for next pulse

			if(secondBeat)									// if this is the second beat, if secondBeat == TRUE
			{                        
				secondBeat = false;                  // clear secondBeat flag
				for(int i=0; i<=9; i++)
				{             
					rate[i] = IBI;     				// seed the running total to get a realisitic BPM at startup                 
				}
			}

			if(firstBeat)							// if it's the first time we found a beat, if firstBeat == TRUE
			{                         
				firstBeat = false;                   // clear firstBeat flag
				secondBeat = true;                   // set the second beat flag
					// sei();                              				 // enable interrupts again
				return 0;                           // IBI value is unreliable so discard it
			}   

				// keep a running total of the last 10 IBI values
			runningTotal = 0;                  // clear the runningTotal variable 
			for(int i=0; i<=8; i++)							// shift data in the rate array
			{                
				rate[i] = rate[i+1];                 		 	// and drop the oldest IBI value 
				runningTotal += rate[i];              // add up the 9 oldest IBI values
			}
			rate[9] = IBI;                          				// add the latest IBI to the rate array
			runningTotal += rate[9];               	 // add the latest IBI to runningTotal
			runningTotal /= 10;                     		// average the last 10 IBI values 
			BPM = 60000/runningTotal;        	 // how many beats can fit into a minute? that's BPM!
			*heartrate = BPM;
			gizLog(LOG_INFO,"BPM = %d\n", *heartrate); 
			QS = true;                            					 // set Quantified Self flag 
			 // QS FLAG IS NOT CLEARED INSIDE THIS ISR
		}                       
	}

	if (Signal < thresh && Pulse == true)
	{   // when the values are going down, the beat is over
		//HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);            // turn off pin 13 LED
		Pulse = false;                         // reset the Pulse flag so we can do it again
		amp = P - T;                           // get amplitude of the pulse wave
		thresh = amp/2 + T;                    // set thresh at 50% of the amplitude
		P = thresh;                            // reset these for next time
		T = thresh;
	}

	if (Num > 2500)
	{                           // if 2.5 seconds go by without a beat
		thresh = 512;                          // set thresh default
		P = 512;                               // set P default
		T = 512;                               // set T default
		lastBeatTime = sampleCounter;          // bring the lastBeatTime up to date        
		firstBeat = true;                      // set these to avoid noise
		secondBeat = false;                    // when we get the heartbeat back
	}

}
