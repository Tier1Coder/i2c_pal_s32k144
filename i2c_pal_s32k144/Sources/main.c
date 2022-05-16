
/* Components includes */
#include "Cpu.h"
#include "clockMan1.h"
#include "pin_mux.h"
#if CPU_INIT_CONFIG
#include "Init_Config.h"
#endif

/* User includes */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <i2c2.h>
#include <library.h>

/* Defines */

#define TRANSFER_SIZE 8

/* Slave buffers */
uint8_t slaveTxBuffer[TRANSFER_SIZE] = { 0x0, 0x1, 0x02, 0x3, 0x4, 0x5, 0x6, 0x7};
uint8_t slaveRxBuffer[TRANSFER_SIZE] = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

/*
 	 I2C Slave Callback

 	 instance   I2C instance number
 	 slaveEvent Event received on the I2C bus
 	 userData   User defined data that is passed to the callback

 	 This function will be called by I2C interrupt handler and it
 	 will assign the buffer for TX or RX events.
 	 If an error event occurs, it will abort the current transfer.
 */

void i2c1_SlaveCallback0(i2c_slave_event_t slaveEvent, void *userData)
{
	i2c_instance_t * instance; /* instance from userData */
	instance = (i2c_instance_t *) userData;

	/* RX / TX check */
	if (slaveEvent == I2C_SLAVE_EVENT_RX_REQ)
		I2C_SlaveSetRxBuffer(instance, slaveRxBuffer, TRANSFER_SIZE);
	if (slaveEvent == I2C_SLAVE_EVENT_TX_REQ)
		I2C_SlaveSetTxBuffer(instance, slaveTxBuffer, TRANSFER_SIZE);
}

volatile int exit_code = 0;

/*
 The startup initialization sequence is the following:
 * - __start (startup asm routine)
 * - __init_hardware()
 * - main()
 *   - PE_low_level_init()
 *     - Common_Init()
 *     - Peripherals_Init()
 */
int main(void) {

	uint8_t masterTxBuffer[TRANSFER_SIZE];		/* I2C Transfer Buffer */
	uint8_t cnt;								/* Initialize Buffers */
	uint16_t i;

	volatile bool isTransferOk = true;


	i2c1_SlaveConfig0.callbackParam = (uint32_t *) &i2c1_instance;	/* Use as callback parameter for slave module the I2C instance number */


	/*  */
	extension_flexio_for_i2c_t extension;
	extension.sclPin = 1;							/* Configure FLEXIO pins routing */
	extension.sdaPin = 0;
	i2c2_MasterConfig0.extension = &extension;
	/*  */


	/* Initialize and configure clocks
	 *  - Configure system clocks and dividers
	 *  - Configure LPI2C clock gating
	 *  -   see clock manager component for details
	 */


	CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT,
			g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
	CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_FORCIBLE);

	/* Initialize pins
	 *  - Configure I2C pins
	 *  -   See PinSettings component for more info
	 */

	PINS_DRV_Init(2, g_pin_mux_InitConfigArr);

	/* Initializes I2C master for FlexIO
	 *  See I2C PAL component for configuration details
	 */
	I2C_MasterInit(&i2c2_instance, &i2c2_MasterConfig0);

	 /* Initialize I2C slave instance for LPI2C driver
	  * See I2C PAL component for configuration details
	  */
	I2C_SlaveInit(&i2c1_instance, &i2c1_SlaveConfig0);

    /* Initialize the data buffer */
    for (i = 0u; i < TRANSFER_SIZE; i++)
    {
        masterTxBuffer[i] = i;
    }

    /* FlexIO master sends masterTxBuffer to LPI2C0 configured as slave */
    I2C_MasterSendDataBlocking(&i2c2_instance, masterTxBuffer, TRANSFER_SIZE, true, 0xFFFF);

    /* Check if transfer is completed with no errors */
    for (cnt = 0U; cnt < TRANSFER_SIZE; cnt++)
    {
        /* If the values are not equal, break the loop and set isTransferOk to false */
        if((masterTxBuffer[cnt] != slaveRxBuffer[cnt]))
        {
            isTransferOk = false;
            break;
        }


    }

    /* char greeting[] = "Hello"; */
	McuSSD1306_Init();
	McuSSD1306_UpdateFull();
	McuSSD1306_PrintString(0, 0, 0);
	McuSSD1306_UpdateFull();


    /* Cast isTransferOk to avoid "set but not used" warnings */
    (void)isTransferOk;

    /* End of the driver example */

	/*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
#ifdef PEX_RTOS_INIT
	PEX_RTOS_INIT(); /* Initialization of the selected RTOS. Macro is defined by the RTOS component. */
#endif
	/*** End of Processor Expert internal initialization.                    ***/


	/*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;) {
    if(exit_code != 0) {
      break;
    }
  }
  return exit_code;
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
 ** @}
 */
/*
 ** ###################################################################
 **
 **     This file was created by Processor Expert 10.1 [05.21]
 **     for the Freescale S32K series of microcontrollers.
 **
 ** ###################################################################
 */
