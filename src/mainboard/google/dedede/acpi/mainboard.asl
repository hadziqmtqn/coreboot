/*
 * This file is part of the coreboot project.
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include <intelblocks/gpio.h>
#include <soc/gpio_soc_defs.h>

Method (PGPM, 1, Serialized)
{
	For (Local0 = 0, Local0 < TOTAL_GPIO_COMM, Local0++)
	{
		\_SB.PCI0.CGPM (Local0, Arg0)
	}
}

/*
 * Method called from _PTS prior to system sleep state entry
 * Enables dynamic clock gating for all 5 GPIO communities
 */
Method (MPTS, 1, Serialized)
{
	PGPM (MISCCFG_ENABLE_GPIO_PM_CONFIG)
}

/*
 * Method called from _WAK prior to system sleep state wakeup
 * Disables dynamic clock gating for all 5 GPIO communities
 */
Method (MWAK, 1, Serialized)
{
	PGPM (0)
}

/*
 * S0ix Entry/Exit Notifications
 * Called from \_SB.LPID._DSM
 */
Method (MS0X, 1, Serialized)
{
	If (Arg0 == 1) {
		/* S0ix Entry */
		PGPM (MISCCFG_ENABLE_GPIO_PM_CONFIG)
	} Else {
		/* S0ix Exit */
		PGPM (0)
	}
}
