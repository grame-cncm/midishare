#ifndef __INITVAL_H
#define __INITVAL_H

/*
 *  Init values for soundcard modules
 *  Copyright (c) by Jaroslav Kysela <perex@suse.cz>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#define SND_DEFAULT_IDX1 (-1)
#define SND_DEFAULT_IDX { [0 ... (SND_CARDS-1)] = -1 }
#define SND_DEFAULT_STR1 NULL
#define SND_DEFAULT_STR { [0 ... (SND_CARDS-1)] = NULL }
#define SND_DEFAULT_PORT1 SND_AUTO_PORT
#define SND_DEFAULT_PORT { SND_AUTO_PORT, [1 ... (SND_CARDS-1)] = -1 }
#define SND_DEFAULT_IRQ1 SND_AUTO_IRQ
#define SND_DEFAULT_IRQ { [0 ... (SND_CARDS-1)] = SND_AUTO_IRQ }
#define SND_DEFAULT_DMA1 SND_AUTO_DMA
#define SND_DEFAULT_DMA { [0 ... (SND_CARDS-1)] = SND_AUTO_DMA }
#define SND_DEFAULT_DMA_SIZE1 SND_AUTO_DMA_SIZE
#define SND_DEFAULT_DMA_SIZE { [0 ... (SND_CARDS-1)] = SND_AUTO_DMA_SIZE }
#define SND_DEFAULT_PTR1 SND_DEFAULT_STR1
#define SND_DEFAULT_PTR SND_DEFAULT_STR

#ifndef SND_SKIP_EXPORT_NO_SYMBOLS
EXPORT_NO_SYMBOLS;
#endif

#endif				/* __INITVAL_H */