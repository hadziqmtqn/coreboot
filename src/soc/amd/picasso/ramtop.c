/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2015 Intel Corp.
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

#define __SIMPLE_DEVICE__

#include <assert.h>
#include <stdint.h>
#include <console/console.h>
#include <cpu/x86/msr.h>
#include <cpu/x86/smm.h>
#include <cpu/amd/msr.h>
#include <cpu/amd/mtrr.h>
#include <cbmem.h>
#include <arch/bert_storage.h>
#include <soc/northbridge.h>
#include <soc/iomap.h>
#include <amdblocks/acpimmio.h>

void backup_top_of_low_cacheable(uintptr_t ramtop)
{
	biosram_write32(BIOSRAM_CBMEM_TOP, ramtop);
}

uintptr_t restore_top_of_low_cacheable(void)
{
	return biosram_read32(BIOSRAM_CBMEM_TOP);
}

#if CONFIG(ACPI_BERT)
 #if CONFIG_SMM_TSEG_SIZE == 0x0
  #define BERT_REGION_MAX_SIZE 0x100000
 #else
  /* SMM_TSEG_SIZE must stay on a boundary appropriate for its granularity */
  #define BERT_REGION_MAX_SIZE CONFIG_SMM_TSEG_SIZE
 #endif
#else
 #define BERT_REGION_MAX_SIZE 0
#endif

void bert_reserved_region(void **start, size_t *size)
{
	if (CONFIG(ACPI_BERT))
		*start = cbmem_top();
	else
		start = NULL;
	*size = BERT_REGION_MAX_SIZE;
}

void *cbmem_top(void)
{
	msr_t tom = rdmsr(TOP_MEM);

	if (!tom.lo)
		return 0;

	/* 8MB alignment to keep MTRR usage low */
	return (void *)ALIGN_DOWN(restore_top_of_low_cacheable()
			- CONFIG_SMM_TSEG_SIZE
			- BERT_REGION_MAX_SIZE, 8*MiB);
}

static uintptr_t smm_region_start(void)
{
	return (uintptr_t)cbmem_top() + BERT_REGION_MAX_SIZE;
}

static size_t smm_region_size(void)
{
	return CONFIG_SMM_TSEG_SIZE;
}

void smm_region(uintptr_t *start, size_t *size)
{
	*start = smm_region_start();
	*size = smm_region_size();
}

/*
 * For data stored in TSEG, ensure TValid is clear so R/W access can reach
 * the DRAM when not in SMM.
 */
static void clear_tvalid(void)
{
	msr_t hwcr = rdmsr(HWCR_MSR);
	msr_t mask = rdmsr(SMM_MASK_MSR);
	int tvalid = !!(mask.lo & SMM_TSEG_VALID);

	if (hwcr.lo & SMM_LOCK) {
		if (!tvalid) /* not valid but locked means still accessible */
			return;

		printk(BIOS_ERR, "Error: can't clear TValid, already locked\n");
		return;
	}

	mask.lo &= ~SMM_TSEG_VALID;
	wrmsr(SMM_MASK_MSR, mask);
}

int smm_subregion(int sub, uintptr_t *start, size_t *size)
{
	static int once;
	uintptr_t sub_base;
	size_t sub_size;
	const size_t cache_size = CONFIG_SMM_RESERVED_SIZE;

	smm_region(&sub_base, &sub_size);
	assert(sub_size > CONFIG_SMM_RESERVED_SIZE);

	if (!once) {
		clear_tvalid();
		once = 1;
	}

	switch (sub) {
	case SMM_SUBREGION_HANDLER:
		/* Handler starts at the base of TSEG. */
		sub_size -= cache_size;
		break;
	case SMM_SUBREGION_CACHE:
		/* External cache is in the middle of TSEG. */
		sub_base += sub_size - cache_size;
		sub_size = cache_size;
		break;
	default:
		*start = 0;
		*size = 0;
		return -1;
	}

	*start = (void *)sub_base;
	*size = sub_size;

	return 0;
}