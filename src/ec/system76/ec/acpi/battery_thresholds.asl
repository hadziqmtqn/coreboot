/* SPDX-License-Identifier: GPL-2.0-only */

/*
 * Get battery charging threshold
 *
 * Arg0: 1: Primary battery
 *       2: Secondary battery
 *
 * Arg1: 0: Start threshold
 *       1: Stop threshold
 */
Method (BCTG, 2, NotSerialized)
{
	If (Arg0 == 1 && BAT0) {
		If (Arg1 == 0) {
			Return (BTL0)
		}

		If (Arg1 == 1) {
			Return (BTH0)
		}
	}

	If (Arg0 == 2 && BAT1) {
		If (Arg1 == 0) {
			Return (BTL1)
		}

		If (Arg1 == 1) {
			Return (BTH1)
		}
	}

	Return (0xFF)
}

/*
 * Set battery charging threshold
 *
 * Arg0: 0: Any battery
 *       1: Primary battery
 *       2: Secondary battery
 *
 * Arg1: 0: Start threshold
 *       1: Stop threshold
 *
 * Arg2: Percentage
 */
Method (BCTS, 3, NotSerialized)
{
	If (Arg2 <= 100) {
		If (Arg0 == 0) {
			If (Arg1 == 0) {
				If (BAT0) {
					BTL0 = Arg2
				}
				If (BAT1) {
					BTL1 = Arg2
				}
			}
			If (Arg1 == 1) {
				If (BAT0) {
					BTH0 = Arg2
				}
				If (BAT1) {
					BTH1 = Arg2
				}
			}
		}

		If (Arg0 == 1 && BAT0) {
			If (Arg1 == 0) {
				BTL0 = Arg2
			}
			If (Arg1 == 1) {
				BTH0 = Arg2
			}
		}

		If (Arg0 == 2 && BAT1) {
			If (Arg1 == 0) {
				BTL1 = Arg2
			}
			If (Arg1 == 1) {
				BTH1 = Arg2
			}
		}
	}
}
