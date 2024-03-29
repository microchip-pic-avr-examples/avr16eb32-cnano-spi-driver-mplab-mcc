/*
 * MAIN Generated Driver File
 *
 * @file main.c
 *
 * @defgroup main MAIN
 *
 * @brief This is the generated driver implementation file for the MAIN driver.
 *
 * @version MAIN Driver Version 1.0.0
 */

/*
� [2024] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip
    software and any derivatives exclusively with Microchip products.
    You are responsible for complying with 3rd party license terms
    applicable to your use of 3rd party software (including open source
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.?
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR
    THIS SOFTWARE.
 */
#include "mcc_generated_files/system/system.h"

/*
    Main application
 */

#define DATA_TO_SEND        0XAB
#define EXPECTED_RESULT1    0XCD
#define EXPECTED_RESULT2    0XEF

int main(void)
{
    SYSTEM_Initialize();

    uint8_t receivedValue = 0;

    LED0_SetHigh();

    SPI_Client.Initialize();
    SPI_Client.Open(CLIENT_CONFIG);

    printf("\e[1;1H\e[2J"); // Clear Screen

    while (1)
    {
        while (false == SPI_Client.IsTxReady())
        {
            // Wait for operation to finish
        }
        receivedValue = SPI_Client.ByteExchange(DATA_TO_SEND);
        while (false == SPI_Client.IsTxReady())
        {
            // Wait for operation to finish
        }

        printf("\t...Working...\n");
        printf("Sent by Device: \t0x%x\t(CLIENT)\n", DATA_TO_SEND);
        printf("Received from Tester:  \t0x%x\t(HOST)\n", receivedValue);
        if ((uint8_t) EXPECTED_RESULT1 == receivedValue)
        {
            LED0_SetLow();
            printf("\tSUCCESS   \n");
        }
        else
        {
            LED0_SetHigh();
            printf("\tFAILURE\n");
        }
        printf("\033[0;0H"); // Go to home
        printf("\33[2K\r"); // Clear Line
    }
}