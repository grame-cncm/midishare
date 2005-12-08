/*******************************************************************************
 * C H A M E L E O N    S. D. K.                                               *
 *******************************************************************************
 *  $Archive:: /Chameleon.sdk/system/midishare/rtems/msClockDriver.c           $
 *     $Date: 2005/12/08 13:39:26 $
 * $Revision: 1.1.2.1 $
 *-----------------------------------------------------------------------------*
 * This file is part of the Chameleon Software Development Kit                 *
 *                                                                             *
 * Copyright (C) 2001 soundart                                                 *
 * www.soundart-hot.com                                                        *
 * codemaster@soundart-hot.com                                                 *
 ******************************************************************************/

/*

  This library is free software; you can redistribute it and modify it under 
  the terms of the GNU Library General Public License as published by the 
  Free Software Foundation version 2 of the License, or any later version.

  This library is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public 
  License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

/*******************************************************************************
 *  NOTE: this driver initailizes timer2 on the MCF5206e as the 
 *  main MidiShare clock 
 ******************************************************************************/

#include <bsp.h>
#include "msClockDriver.h"
#include "msKernel.h"

extern TMSGlobalPtr gMemPtr;

/*******************************************************************************
 * Function : clock_isr
 *
 * Description : This handles the timer interrupt by clearing the timer's
 * interrupt flag and announcing the clock tick to the system.
 ******************************************************************************/
static ISR_Handler clock_isr(ISR_Vector_number vector)
{
	/* Clear pending interrupt... */
	sim->timer2.ter = MCF5206e_TIMER_TER_REF | MCF5206e_TIMER_TER_CAP;

	/* Announce the clock tick */
	ClockHandler(gMemPtr);
}


/*******************************************************************************
 * Function : msOpenClockDriver
 *
 * Description : This initialises timer2 with the MidiShare timeout value
 * as a reference and sets up the interrupt handler for clock ticks.
 ******************************************************************************/
void msOpenClockDriver(void)
{
	ISR_Handler* old_isr;

	/* Configure timer2 interrupts */
	sim->icr10 = MCF5206e_SIM_ICR_AVEC                   |
				 MCF5206e_SIM_ICR_IL(TIMER2_IRQ_LEVEL)   |
				 MCF5206e_SIM_ICR_IP(TIMER2_IRQ_PRIORITY); 

	/* Register the interrupt handler */
	_ISR_Install_vector(TIMER2_IRQ_VECTOR, clock_isr, &old_isr);

	/* Reset timer2 */
	sim->timer2.tmr = MCF5206e_TIMER_TMR_RST;    
	sim->timer2.tmr = MCF5206e_TIMER_TMR_CLK_STOP;   
	sim->timer2.tmr = MCF5206e_TIMER_TMR_RST;    
	sim->timer2.tcn = 0; /* zero count */
	sim->timer2.ter = MCF5206e_TIMER_TER_REF | MCF5206e_TIMER_TER_CAP; /* clear pending */

	/*
	 * Prescale Timer2 by 1/sysclk so that it counts in microsecs
	 * for the SBC5206 as supplied this will be 1/25
	 */
	sim->timer2.tmr = MCF5206e_TIMER_TMR_PS(MCF5206e_CLOCK_SPEED/1000000 - 1)	|
			  MCF5206e_TIMER_TMR_CE_NONE					|
			  MCF5206e_TIMER_TMR_ORI					|
			  MCF5206e_TIMER_TMR_FRR					|
			  MCF5206e_TIMER_TMR_RST;

	/* Set the timer timeout value in microseconds */
	sim->timer2.trr = 1000 - 1;

	/* Enable timer 2 interrupts */
	sim->imr &= (~MCF5206e_SIM_IMR_TIMER2);

	/* Kick it into life! */
	sim->timer2.tmr |= MCF5206e_TIMER_TMR_CLK_MSCLK;
}


/*******************************************************************************
 * Function : msCloseClockDriver
 *
 * Description : This shuts down the timer if it was enabled and removes it
 * from the MCF5206 interrupt mask.
 ******************************************************************************/
void msCloseClockDriver(void)
{
	/* disable all timer2 interrupts */
	sim->imr = sim->imr | MCF5206e_SIM_IMR_TIMER2;   

	/* reset timer2... */
	sim->timer2.tmr = MCF5206e_TIMER_TMR_CLK_STOP; 

	/* clear pending */
	sim->timer2.ter = MCF5206e_TIMER_TER_REF | MCF5206e_TIMER_TER_CAP; 
}
