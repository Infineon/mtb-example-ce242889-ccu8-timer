/******************************************************************************
* File Name:   main.c
*
* Description: This is the source code for the PSOC Control C1 MCU: CCU8 Timer Example
*              for ModusToolbox.
*
* Related Document: See README.md
*
*******************************************************************************
* (c) 2026, Infineon Technologies AG, or an affiliate of Infineon
* Technologies AG. All rights reserved.
* This software, associated documentation and materials ("Software") is
* owned by Infineon Technologies AG or one of its affiliates ("Infineon")
* and is protected by and subject to worldwide patent protection, worldwide
* copyright laws, and international treaty provisions. Therefore, you may use
* this Software only as provided in the license agreement accompanying the
* software package from which you obtained this Software. If no license
* agreement applies, then any use, reproduction, modification, translation, or
* compilation of this Software is prohibited without the express written
* permission of Infineon.
*
* Disclaimer: UNLESS OTHERWISE EXPRESSLY AGREED WITH INFINEON, THIS SOFTWARE
* IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* INCLUDING, BUT NOT LIMITED TO, ALL WARRANTIES OF NON-INFRINGEMENT OF
* THIRD-PARTY RIGHTS AND IMPLIED WARRANTIES SUCH AS WARRANTIES OF FITNESS FOR A
* SPECIFIC USE/PURPOSE OR MERCHANTABILITY.
* Infineon reserves the right to make changes to the Software without notice.
* You are responsible for properly designing, programming, and testing the
* functionality and safety of your intended application of the Software, as
* well as complying with any legal requirements related to its use. Infineon
* does not guarantee that the Software will be free from intrusion, data theft
* or loss, or other breaches ("Security Breaches"), and Infineon shall have
* no liability arising out of any Security Breaches. Unless otherwise
* explicitly approved by Infineon, the Software may not be used in any
* application where a failure of the Product or any consequences of the use
* thereof can reasonably be expected to result in personal injury.
*******************************************************************************/

#include "cybsp.h"
#include "cy_utils.h"
#include <stdio.h>
#include "cy_retarget_io.h"

/*******************************************************************************
* Macros
*******************************************************************************/

/* Defines priority level of the period match event interrupt */
#define TIMER_0_PERIOD_MATCH_EVENT_PRIORITY     3

/* Defines handler of the period match event interrupt */
#define TIMER_0_PERIOD_MATCH_EVENT_HANDLER      CCU80_SR0_INTERRUPT_HANDLER

/* Defines IRQ number of the period match event interrupt */
#define TIMER_0_PERIOD_MATCH_EVENT_IRQN         CCU80_SR0_IRQN

/*******************************************************************************
* Global Variables
*******************************************************************************/
 /* Timer interrupt flag to indicate CCU8 timer period match event */
static volatile bool timer_interrupt_flag = false;

/* Define macro to enable/disable printing of debug messages */
#define ENABLE_DEBUG_PRINT                      (0)

#if ENABLE_DEBUG_PRINT
/* Print debug message only once after entering the main loop */
static bool ENTER_LOOP = false;
#endif

/*******************************************************************************
* Function Name: TIMER_0_PERIOD_MATCH_EVENT_HANDLER
********************************************************************************
* Summary:
* This is the interrupt handler function for the CCU8 timer period match interrupt.
*
* Parameters:
*  none
*
* Return:
*  void
*
*******************************************************************************/
void TIMER_0_PERIOD_MATCH_EVENT_HANDLER(void)
{
    timer_interrupt_flag = true;
}

/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
* This is the main function.It sets up a CCU8 timer to period match interrupt.
* The main while loop checks for the elapsed time due to CCU8 timer period match
* event and toggles an LED at approximately 1Hz.
*
* Parameters:
*  none
*
* Return:
*  int
*
*******************************************************************************/
int main(void)
{
    cy_rslt_t result;

    /* Initialize the device and board peripherals */
    result = cybsp_init();
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* Initialize printf retarget */
    result = cy_retarget_io_init(CYBSP_DEBUG_UART_HW);
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    #if ENABLE_DEBUG_PRINT
    printf("Initialization done\r\n");
    #endif

    /* Set priority */
    NVIC_SetPriority(TIMER_0_PERIOD_MATCH_EVENT_IRQN, TIMER_0_PERIOD_MATCH_EVENT_PRIORITY);

    /* Enable IRQ */
    NVIC_EnableIRQ(TIMER_0_PERIOD_MATCH_EVENT_IRQN);

    /* Infinite loop */
    while(1)
    {
        /* Check if timer elapsed (interrupt fired) and toggle the LED */
        if (timer_interrupt_flag)
        {
            /* Clear the flag */
            timer_interrupt_flag = false;
            
            /* Toggle the USER LED state */
            Cy_GPIO_ToggleOutput(CYBSP_USER_LED_PORT, CYBSP_USER_LED_PIN);

            #if ( ENABLE_DEBUG_PRINT )
            if(!ENTER_LOOP)
            {
                printf("User LED toggled\r\n");
                ENTER_LOOP = true;
            }
            #endif
        }
    }
}

/* [] END OF FILE */
