/* SPDX-License-Identifier: GPL-2.0-or-later */
/* This file is part of the coreboot project. */

#ifndef SUPERIO_NSC_PC87392_H
#define SUPERIO_NSC_PC87392_H

#define PC87392_FDC  0x00
#define PC87392_PP   0x01
#define PC87392_SP2  0x02
#define PC87392_SP1  0x03
#define PC87392_GPIO 0x07
#define PC87392_WDT  0x0A

#define PC87392_GPIO_PIN_OE 0x01
#define PC87392_GPIO_PIN_TYPE_PUSH_PULL 0x02
#define PC87392_GPIO_PIN_PULLUP 0x04
#define PC87392_GPIO_PIN_LOCK 0x08
#define PC87392_GPIO_PIN_TRIG_LEVEL 0x10
#define PC87392_GPIO_PIN_TRIG_LOW 0x20
#define PC87392_GPIO_PIN_DEBOUNCE 0x40

#define PC87392_GPIO_PIN_TRIGGERS_IRQ 0x01
#define PC87392_GPIO_PIN_TRIGGERS_SMI 0x02

#endif
