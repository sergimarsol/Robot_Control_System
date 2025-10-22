// * Sergi Marsol Torrent *
// * 2023-10-30 *
// * Microcontroladors i Sistemes Empotrats *
// * Universitat de Barcelona *

#include "i2c.h"
#include <msp430.h>

uint8_t *PTxData; // Pointer to TX data
uint8_t TXByteCtr;
uint8_t *PRxData; // Pointer to RX data
uint8_t RXByteCtr;

void i2c_init() {
    P4SEL0 |= BIT7 + BIT6; // P4.6 SDA and P4.7 SCL as USCI if using USCI B1
    //P1SEL0 |= BIT3 + BIT2; // P1.2 SDA and P1.3 SCL as USCI if using USCI B0
    UCB1CTLW0 |= UCSWRST; // Stop the module
    // Configure as master, synchronous, and I2C mode; by default, it is in single-master mode
    UCB1CTLW0 |= UCMST + UCMODE_3 + UCSSEL_2; // Use SMCLK
    UCB1BR0 = 160; // fSCL = SMCLK(16MHz)/160 = ~100kHz
    UCB1BR1 = 0;
    UCB1CTLW0 &= ~UCSWRST; // Clear SW reset, resume operation
    UCB1IE |= UCTXIE0 | UCRXIE0; // Enable TX and RX interrupts
}

void I2C_send(uint8_t addr, uint8_t *buffer, uint8_t n_dades) {
    UCB1I2CSA = addr; // Set the slave address
    PTxData = buffer; // Address of the block of data to transmit
    TXByteCtr = n_dades; // Load the number of bytes to transmit
    UCB1CTLW0 |= UCTR + UCTXSTT; // I2C in TX mode, send the start condition
    __bis_SR_register(LPM0_bits + GIE); // Enter LPM0 mode, enable interrupts
    __no_operation(); // Remain in LPM0 mode until data is transmitted
    while (UCB1CTLW0 & UCTXSTP); // Ensure the stop condition has been sent
}

void I2C_receive(uint8_t addr, uint8_t *buffer, uint8_t n_dades) {
    PRxData = buffer; // Address of the buffer where received data will be stored
    RXByteCtr = n_dades; // Load the number of bytes to receive
    UCB1I2CSA = addr; // Set the slave address
    UCB1CTLW0 &= ~UCTR; // I2C in reception mode
    while (UCB1CTLW0 & UCTXSTP); // Ensure the bus is in stop condition
    UCB1CTLW0 |= UCTXSTT; // I2C start condition in reception mode
    __bis_SR_register(LPM0_bits + GIE); // Enter LPM0 mode, enable interrupts
    __no_operation(); // Remain in LPM0 mode until all data is received
}

#pragma vector = USCI_B1_VECTOR
__interrupt void ISR_USCI_I2C(void)
{
    switch(__even_in_range(UCB1IV,12))
    {
        case USCI_NONE: break; // Vector 0: No interrupts
        case USCI_I2C_UCALIFG: break; // Vector 2: ALIFG
        case USCI_I2C_UCNACKIFG: break; // Vector 4: NACKIFG
        case USCI_I2C_UCSTTIFG: break; // Vector 6: STTIFG
        case USCI_I2C_UCSTPIFG: break; // Vector 8: STPIFG
        case USCI_I2C_UCRXIFG0: // Vector 10: RXIFG
        if (RXByteCtr)
        {
            *PRxData++ = UCB1RXBUF; // Move the received data to the PRxData address
            if (RXByteCtr == 1) // Only one byte left?
                UCB1CTLW0 |= UCTXSTP; // Generate I2C stop condition
        }
        else {
            *PRxData = UCB1RXBUF; // Move the received data to the PRxData address
            __bic_SR_register_on_exit(LPM0_bits); // Exit low-power mode LPM0, activate the CPU
        }
        RXByteCtr--; // Decrement RX byte counter
        break;
        case USCI_I2C_UCTXIFG0: // Vector 12: TXIFG
        if (TXByteCtr) // Check TX byte counter
        {
            UCB1TXBUF = *PTxData++; // Load the TX buffer with the data to send
            TXByteCtr--; // Decrement TX byte counter
        }
        else
        {
            UCB1CTLW0 |= UCTXSTP; // I2C stop condition
            UCB1IFG &= ~UCTXIFG; // Clear USCI_B1 TX interrupt flag
            __bic_SR_register_on_exit(LPM0_bits); // Exit low-power mode LPM0, activate the CPU
        }
        default: break;
    }
}