/*
 * This file is part of the coreboot project.
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

#include <bootstate.h>
#include <soc/ramstage.h>
#include "gpio.h"

void mainboard_silicon_init_params(FSP_SIL_UPD *params)
{
	/* Configure pads prior to SiliconInit() in case there's any
	 * dependencies during hardware initialization. */
	gpio_configure_pads(gpio_table, ARRAY_SIZE(gpio_table));

	/* This must be one, otherwise FSP crashes ... */
	params->PchHdaVcType = 0x1;
}