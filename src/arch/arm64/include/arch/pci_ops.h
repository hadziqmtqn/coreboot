/*
 * This file is part of the coreboot project.
 *
 * Copyright 2013 Google Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef ARCH_ARM64_PCI_OPS_H
#define ARCH_ARM64_PCI_OPS_H

#include <stdint.h>

typedef u32 pci_devfn_t;

#ifdef __SIMPLE_DEVICE__
u8 pci_read_config8(pci_devfn_t dev, unsigned int where);
u16 pci_read_config16(pci_devfn_t dev, unsigned int where);
u32 pci_read_config32(pci_devfn_t dev, unsigned int where);
void pci_write_config8(pci_devfn_t dev, unsigned int where, u8 val);
void pci_write_config16(pci_devfn_t dev, unsigned int where, u16 val);
void pci_write_config32(pci_devfn_t dev, unsigned int where, u32 val);
#endif

#endif
