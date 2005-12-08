/*******************************************************************************
 * C H A M E L E O N    S. D. K.                                               *
 *******************************************************************************
 *  $Archive:: /Chameleon.sdk/system/midishare/rtems/msMidiDriver.c            $
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

#include <bsp.h>
#include "MidiShareKernel.h"
#include "msMidiDriver.h"

#define MIDI_BAUD		31250

static MidiDrv gDrvData;

static ISR_Handler midi_isr(ISR_Vector_number vec);


static void KeyOffTask(long date, short refNum, long e, long data, long a3)
{
	register MidiSlotPtr pSlot = (MidiSlotPtr) data;

	fifoput(&pSlot->fifoXmt, (cell *) e);
	midi_isr(UART1_IRQ_VECTOR);
}

/*******************************************************************************
 * midi interrupt...
 ******************************************************************************/
static ISR_Handler midi_isr(ISR_Vector_number vec)
{
	register MidiDrvPtr  pDrv;
	register MidiSlotPtr pSlot;
	volatile uartstruct* uart = &sim->uart1;

	pDrv = &gDrvData;
	pSlot = &pDrv->slotData;

	if (uart->read.usr & MCF5206e_UART_USR_RXRDY)
	{
		register MidiEvPtr ev;

		while ((uart->read.usr & MCF5206e_UART_USR_RXRDY) != 0) 
		{
			ev = MidiParseByte(&pSlot->rcv, uart->read.urbuf);
			if (ev)
			{
				Port(ev) = pSlot->slot.slotRef;
				MidiSendIm(pDrv->drv, ev);
			}
		}
	}
	else if (uart->read.usr & MCF5206e_UART_USR_TXRDY)
	{
		register MidiEvPtr ev;
		BYTE c;

		while (uart->read.usr & MCF5206e_UART_USR_TXRDY)
		{
			if (!MidiStreamCountByte(&pSlot->xmt))
			{
				if (!fifoavail(&pSlot->fifoXmt))
					break;

				ev = MidiStreamPutEvent(&pSlot->xmt, (MidiEvPtr) fifoget(&pSlot->fifoXmt));
				if (ev) 
					MidiTask(KeyOffTask, Date(ev), 0, (long) ev, (long) pSlot, 0);
			}

			if (MidiStreamGetByte(&pSlot->xmt, &c))
				uart->write.utbuf = c;
		}

		if (!fifoavail(&pSlot->fifoXmt) && !MidiStreamCountByte(&pSlot->xmt))
			uart->write.uimr =	MCF5206e_UART_UIMR_FFULL;	/* disable TXRDY interrupt */
		else
			uart->write.uimr =	MCF5206e_UART_UIMR_FFULL |
						MCF5206e_UART_UIMR_TXRDY;	/* enable TXRDY interrupt */
	}
}

static void midi_isr_enable(rtems_boolean enable)
{
	if (enable)
		sim->imr &= ~(MCF5206e_SIM_IMR_UART1);
	else
		sim->imr |= (MCF5206e_SIM_IMR_UART1);
}

static rtems_boolean midi_setup(void)
{
	static rtems_boolean	first_time = TRUE;
	volatile uartstruct*    uart = &sim->uart1;
	ISR_Handler*		old_isr;
	long                    ubg;
	int			i;

	if (!first_time)
		return TRUE;

	/*
	 * compute baud rate (using sysclock/32)...
	 */
	ubg = (MCF5206e_CLOCK_SPEED + 16 * MIDI_BAUD) / (32 * MIDI_BAUD);

	if (ubg < 2)
		return FALSE;

	/*
	 * reset everything:
	 *  reset cmds are misc[2:0] = 001b .. 101b
	 */
	for (i=5; i >= 1 ;i--)
		uart->write.ucr = (i<<4);

	/*
	 * init interrupt:
	 */
	sim->icr12 =	MCF5206e_SIM_ICR_IL(UART1_IRQ_LEVEL)   |	/* AVEC=0 */
			MCF5206e_SIM_ICR_IP(UART1_IRQ_PRIORITY); 

	uart->write.uivr = UART1_IRQ_VECTOR;			/* set intr vec */
	uart->write.uacr = 0x00;				/* disallow CTS interrupts */
	uart->write.uimr = MCF5206e_UART_UIMR_FFULL;		/* enable FFULL interrupt */

	/*
	 * setup baud rate:
	 */
	uart->write.ucsr = MCF5206e_UART_UCSR_RCS(0xD) | MCF5206e_UART_UCSR_TCS(0xD);	/* use system_clock/32 */
	uart->write.ubg1 = (ubg>>8) & 0xFF;	/* USB of baud rate */
	uart->write.ubg2 = (ubg>>0) & 0xFF;	/* LSB of baud rate */

	/*
	 * set mode:
	 *  umr1 = 0xF3 = RxRTS=0 RxIRQ=0 ERR=1 PM=10 PT=0 B/C=11
	 *  umr2 = 0x17 = CM=00 TxRTS=0 TxCTS=0 SB=0111
	 */
	uart->write.ucr =	MCF5206e_UART_UCR_RESET_MR;		/* select umr1 */

	/* Full vs. Ready set here and other places */
	uart->write.umr =	0							|
				MCF5206e_UART_UMR1_ERR		|
				MCF5206e_UART_UMR1_PM_NONE	|
				MCF5206e_UART_UMR1_BC_8;		/* 8 bit no parity */

	uart->write.umr =	0							 |
				MCF5206e_UART_UMR2_CM_NORMAL |
				MCF5206e_UART_UMR2_STOP_BITS_1;		/* 1 stop bit */

	uart->write.ucr  =	MCF5206e_UART_UCR_TX_ENABLED | 
				MCF5206e_UART_UCR_RX_ENABLED;		/* enable Tx & Rx */

	/*
	 * resets don't seem to work: so input doesn't get flushed!
	 */
	for (i=0; (i < 5) && (uart->read.usr & 0x01) ;i++)
		ubg = uart->read.urbuf;

	_ISR_Install_vector(UART1_IRQ_VECTOR, midi_isr, &old_isr);
#if 0
	/* make sure the midi is awake! */
	midi_isr(UART1_IRQ_VECTOR);
#endif
	first_time = FALSE;

	return TRUE;
}

/*******************************************************************************
 * MidiShare part
 ******************************************************************************/
static void ReceiveEvents(short r)
{
	register MidiDrvPtr pDrv = &gDrvData;
	MidiEvPtr ev = MidiGetEv(r);
	rtems_boolean rcv = FALSE;

	while (ev) 
	{
		if (pDrv->slotData.slot.slotRef == Port(ev)) 
		{
			fifoput(&pDrv->slotData.fifoXmt, (cell *) ev);
			rcv = TRUE;
		}
		else 
			MidiFreeEv(ev);

		ev = MidiGetEv(r);
	}

	if (rcv)
		midi_isr(UART1_IRQ_VECTOR);
}

/******************************************************************************/
static void SetupFilter(MidiFilterPtr filter)
{
	short i;

	for (i = 0; i<256; i++)
	{
		RejectBit(filter->evType, i);
		RejectBit(filter->port, i);
	}

	for (i = 0; i<16; i++)
		AcceptBit(filter->channel, i);
		
	for (i = 0; i<typeProcess; i++)
		AcceptBit(filter->evType, i);
	
	for (i = typeQuarterFrame; i<=typeRegParam; i++)
		AcceptBit(filter->evType, i);

	AcceptBit(filter->port, 0);
	AcceptBit(filter->port, 1);
}

static void DrvWakeUp(short r)
{
	MidiDrvPtr  pDrv = &gDrvData;
	MidiSlotPtr pSlot;
	SlotRefNum  slot; 

	/* Init driver */

	pDrv->drv = r;

	pSlot = &pDrv->slotData;

	pSlot->slot.slotRef = -1;
	MidiParseInit(&pSlot->rcv, pDrv->rTbl, pDrv->s2t);
	MidiStreamInit(&pSlot->xmt, pDrv->lTbl);
	fifoinit(&pSlot->fifoXmt);

	/* Initialize External Midi Slot */

	slot = MidiAddSlot(r, MidiSlotName, MidiInputOutputSlot);
	if (slot.slotRef >= 0) 
		pDrv->slotData.slot = slot;

	midi_isr_enable(FALSE);
	midi_setup();
	midi_isr_enable(TRUE);

	MidiSetRcvAlarm(r, ReceiveEvents);

	SetupFilter(&pDrv->filter);
	MidiSetFilter(r, &pDrv->filter);

	MidiConnect(MidiShareDriverRef, r, TRUE);
	MidiConnect(r, MidiShareDriverRef, TRUE);

	MidiConnectSlot(0, pDrv->slotData.slot, TRUE);
}

/******************************************************************************/

static void DrvSleep(short r)
{
	MidiDrvPtr  pDrv = &gDrvData;

	MidiConnectSlot(0, pDrv->slotData.slot, FALSE);

	MidiConnect(MidiShareDriverRef, r, FALSE);
	MidiConnect(r, MidiShareDriverRef, FALSE);

	midi_isr_enable(FALSE);
	MidiParseReset(&pDrv->slotData.rcv);
	MidiStreamReset(&pDrv->slotData.xmt);
}

/******************************************************************************/
BOOL msOpenMidiDriver(void)
{
	TDriverInfos infos = { MidiDriverName, 100, 0};
	TDriverOperation op = { DrvWakeUp, DrvSleep, { 0, 0, 0 } }; 
	short drv;

	gDrvData.drv = 0;

	MidiParseInitMthTbl(gDrvData.rTbl);
	MidiParseInitTypeTbl(gDrvData.s2t);
	MidiStreamInitMthTbl(gDrvData.lTbl);
	
	if (MidiGetNamedAppl(MidiDriverName) > 0)
		return TRUE;

	drv = MidiRegisterDriver(&infos, &op);
	if (drv == MIDIerrSpace)
		return FALSE;

	return TRUE;
}

/******************************************************************************/
void msCloseMidiDriver(void)
{
	short drv = gDrvData.drv;

	gDrvData.drv = 0;
	if (drv > 0) 
		MidiUnregisterDriver(drv);
}

/******************************************************************************/
BOOL GetPortState(short port)
{
	if (port == 0)
		return MidiIsSlotConnected(port, gDrvData.slotData.slot);

	return FALSE;
}

void SetPortState(short port, BOOL state)
{
	if (port == 0)
		MidiConnectSlot(port, gDrvData.slotData.slot, state);
}
