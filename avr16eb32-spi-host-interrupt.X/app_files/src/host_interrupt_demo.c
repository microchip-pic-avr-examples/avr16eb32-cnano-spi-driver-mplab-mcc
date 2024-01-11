/**
 * Interrupt Demo Application Source File
 * 
 * @file host_interrupt_demo.c
 * 
 * @ingroup hostinterruptdemo
 * 
 * @brief SPI interrupt mode demonstration implementation.
 * 
 * @version Interrupt Demo Application Version v1.0.0
 * 
 */

/*
© [2024] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS "AS IS". 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/

// Included files
#include <stdint.h>

#include "../../mcc_generated_files/system/pins.h"
#include "../../mcc_generated_files/spi/spi_interface.h"
#include "../host_interrupt_demo.h"


// Functions
void DemoInitialize(void)
{
    SPI_Host.Initialize();
    SPI_Host.Open(SEVENSEG);
    SPI_Host.RxCompleteCallbackRegister(DisplayCompleteCallback);
}

bool DeassertDevice(void)
{
    bool returnValue;
    if (SPI_Host.IsTxReady() == true)
    {
        CS1_SetHigh();
        returnValue = true;
    }
    else
    {
        returnValue = false;
    }
    return returnValue;
}

void DisplayCustomCharacter(uint8_t input)
{
    uint8_t recVal = 0x00;
    CS1_SetLow();
    recVal = SPI_Host.ByteExchange(input);
    CS1_SetHigh();
}

bool DisplayNumber(uint8_t numberInput)
{
    bool returnValue = false;
    static const uint8_t sevenSegNumbers[] = {
        0x7E,   /**< Hex for digit 0 */
        0x0A,   /**< Hex for digit 1 */
        0xB6,   /**< Hex for digit 2 */
        0x9E,   /**< Hex for digit 3 */
        0xCA,   /**< Hex for digit 4 */
        0xDC,   /**< Hex for digit 5 */
        0xFC,   /**< Hex for digit 6 */
        0x0E,   /**< Hex for digit 7 */
        0xFE,   /**< Hex for digit 8 */
        0xDE    /**< Hex for digit 9 */
    };
    if (SPI_Host.IsTxReady() == true)
    {
        uint8_t static displayNumber[2];
        uint8_t digit;
        digit = numberInput%10u;
        displayNumber[0] = sevenSegNumbers[digit];
        digit = numberInput/10u;
        displayNumber[1] = sevenSegNumbers[digit];
        CS1_SetLow();
        SPI_Host.BufferExchange(displayNumber, 2);
        returnValue= true;
    }
    else
    {
        returnValue = false;
    }
    return returnValue;
}

void DisplayCompleteCallback(void)
{
    LED0_Toggle();
}

bool ButtonPress(void)
{   
    static uint8_t debounce = 10u;
    static uint8_t buttonState = 0u;
    static bool buttonPressEnabled = true;
    bool returnValue = false;

    if(0u == SW0_GetValue())
    {       
        if(buttonState < debounce)
        {
            buttonState++;
        }
        else if(true == buttonPressEnabled)
        {
            buttonPressEnabled = false;
            returnValue = true;
        }
        else
        {
            // Do Nothing
        }
    }
    else if(buttonState > 0u)
    {
        buttonState--;
    }
    else
    {
        buttonPressEnabled = true;
    }
    return returnValue;
}