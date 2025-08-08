/* SPDX-License-Identifier: GPL-2.0 */
/*
* Copyright (C) 2025 Original Author (retain original author information)
* Copyright (C) 2025 Amlogic, Inc. All rights reserved.
*
* Description:
*/
#ifndef __WIFI_PHY_DSSS_REG_H__
#define __WIFI_PHY_DSSS_REG_H__

#define WIFI_PHY_DSSS_REG_BASE          (0x00a0a000)
#define MDMBEARLYEND                 (WIFI_PHY_DSSS_REG_BASE + 0xe4)
typedef union MDMBEARLYEND_OFFSET_SEL_FIELD
{
  unsigned int data;
  struct
  {
    unsigned int reg_tx_early_end_en : 2;
    unsigned int rsvd_0 : 30;
  } b;
} MDMBEARLYEND_OFFSET_SEL_FIELD_T;

#endif

