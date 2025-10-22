// * Sergi Marsol Torrent *
// * 2023-10-30 *
// * Microcontroladors i Sistemes Empotrats *
// * Universitat de Barcelona *

#include "timer.h"
#include <msp430.h>

volatile uint32_t delay;

void init_timers(void) {
    TB0CTL |= TBSSEL_2 | MC_1 | TBCLR; // Use SMCLK as the clock source, set timer to up mode, and clear the timer
    TB0CCR0 = 16000; // Configure CCR0 for a 1 ms interrupt (16 MHz / 16000 = 1 ms)
    TB0CCTL0 |= CCIE; // Enable interrupt for CCR0
}

void init_clocks(void)
{ // Configure one FRAM waitstate as required by the device datasheet for MCLK operation beyond 8MHz before configuring the clock system
    FRCTL0 = FRCTLPW | NWAITS_1;
    P2SEL1 |= BIT6 | BIT7; // P2.6~P2.7: crystal pins
    //do
    //{
        CSCTL7 &= ~(XT1OFFG | DCOFFG); // Clear XT1 and DCO fault flag
        SFRIFG1 &= ~OFIFG;
    //} while (SFRIFG1 & OFIFG); // Test oscillator fault flag
    __bis_SR_register(SCG0); // disable FLL
    CSCTL3 |= SELREF__XT1CLK; // Set XT1 as FLL reference source
    // CSCTL1 = DCOFTRIMEN_1 | DCOFTRIM0 | DCOFTRIM1 | DCORSEL_5; // DCOFTRIM=5, DCO Range = 16MHz**
    CSCTL1 = DCORSEL_5; // DCOFTRIM=5, DCO Range = 16MHz
    CSCTL2 = FLLD_0 + 487; // DCOCLKDIV = 16MHz
    __delay_cycles(3);
    __bic_SR_register(SCG0); // enable FLL
    //Software_Trim(); // Software Trim to get the best DCOFTRIM value**
    CSCTL4 = SELMS__DCOCLKDIV | SELA__REFOCLK; // set XT1 (~32768Hz) as ACLK source, ACLK = 32768Hz
    // default DCOCLKDIV as MCLK and SMCLK source
    // P1DIR |= BIT0 | BIT1; // set SMCLK, ACLK pin as output
    // P1SEL1 |= BIT0 | BIT1; // set SMCLK and ACLK pin as second function
    PM5CTL0 &= ~LOCKLPM5; // Disable the GPIO power-on default high-impedance mode to activate previously configured port settings
}

void delay_ms(uint32_t temps){ //delay function
    TB0CTL |= TBSSEL_2 | MC_1 | TBCLR;
    delay = 0;
    while (delay < temps){ // wait for delay to get to the specified time
    }
    TB0CTL &= ~MC_1;
    __no_operation(); // For debugger
}

#pragma vector=TIMER0_B0_VECTOR
__interrupt void Timer_B0_ISR(void) {
    delay++; // increase delay counter
}