/** @file
  This file is SampleCode of the library for Intel Silicon PEI
  Platform Policy initialization.


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "PeiSiPolicyUpdate.h"
#include <MeChipset.h>
#include <Library/PchInfoLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/SiPolicyLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/ConfigBlockLib.h>

STATIC SVID_SID_INIT_ENTRY mCdfSsidTablePtr[] = {
  //
  // SA Device(s)
  //
  {{{PCI_SVID_OFFSET,    SA_MC_FUN,        SA_MC_DEV,        SA_MC_BUS, 0, SA_SEG_NUM, 0}}, {0, 0},0},
  {{{R_SA_PEG_SS_OFFSET, SA_PEG0_FUN_NUM,  SA_PEG0_DEV_NUM,  SA_MC_BUS, 0, SA_SEG_NUM, 0}}, {0, 0},0},
  {{{R_SA_PEG_SS_OFFSET, SA_PEG1_FUN_NUM,  SA_PEG1_DEV_NUM,  SA_MC_BUS, 0, SA_SEG_NUM, 0}}, {0, 0},0},
  {{{R_SA_PEG_SS_OFFSET, SA_PEG2_FUN_NUM,  SA_PEG2_DEV_NUM,  SA_MC_BUS, 0, SA_SEG_NUM, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,    SA_IGD_FUN_0,     SA_IGD_DEV,       SA_MC_BUS, 0, SA_SEG_NUM, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,    SA_IPU_FUN_NUM,   SA_IPU_DEV_NUM,   SA_MC_BUS, 0, SA_SEG_NUM, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,    SA_GNA_FUN_NUM,   SA_GNA_DEV_NUM,   SA_MC_BUS, 0, SA_SEG_NUM, 0}}, {0, 0},0},
  //
  // PCH Device(s)
  //
  {{{PCI_SVID_OFFSET,    PCI_FUNCTION_NUMBER_PCH_LPC,               PCI_DEVICE_NUMBER_PCH_LPC,           DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,    PCI_FUNCTION_NUMBER_PCH_P2SB,              PCI_DEVICE_NUMBER_PCH_P2SB,          DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,    PCI_FUNCTION_NUMBER_PCH_PMC,               PCI_DEVICE_NUMBER_PCH_PMC,           DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,    PCI_FUNCTION_NUMBER_PCH_HDA,               PCI_DEVICE_NUMBER_PCH_HDA,           DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,    PCI_FUNCTION_NUMBER_CDF_PCH_SATA_1,        PCI_DEVICE_NUMBER_CDF_PCH_SATA_1,    DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,    PCI_FUNCTION_NUMBER_CDF_PCH_SATA_2,        PCI_DEVICE_NUMBER_CDF_PCH_SATA_2,    DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,    PCI_FUNCTION_NUMBER_CDF_PCH_SATA_3,        PCI_DEVICE_NUMBER_CDF_PCH_SATA_3,    DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,    PCI_FUNCTION_NUMBER_PCH_SMBUS,             PCI_DEVICE_NUMBER_PCH_SMBUS,         DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,    PCI_FUNCTION_NUMBER_PCH_SPI,               PCI_DEVICE_NUMBER_PCH_SPI,           DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,    PCI_FUNCTION_NUMBER_PCH_TRACE_HUB,         PCI_DEVICE_NUMBER_PCH_TRACE_HUB,     DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,    PCI_FUNCTION_NUMBER_PCH_XHCI,              PCI_DEVICE_NUMBER_PCH_XHCI,          DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,    PCI_FUNCTION_NUMBER_PCH_XDCI,              PCI_DEVICE_NUMBER_PCH_XDCI,          DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,    PCI_FUNCTION_NUMBER_PCH_THERMAL,           PCI_DEVICE_NUMBER_PCH_THERMAL,       DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SVID,    PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_1,  PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_1, DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SVID,    PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_2,  PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_1, DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SVID,    PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_3,  PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_1, DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SVID,    PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_4,  PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_1, DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SVID,    PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_5,  PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_1, DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SVID,    PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_6,  PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_1, DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SVID,    PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_7,  PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_1, DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SVID,    PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_8,  PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_1, DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SVID,    PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_9,  PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_2, DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SVID,    PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_10, PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_2, DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SVID,    PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_11, PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_2, DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SVID,    PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_12, PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_2, DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
};

STATIC SVID_SID_INIT_ENTRY mSsidTablePtr[] = {
  //
  // SA Device(s)
  //
  {{{PCI_SVID_OFFSET,    SA_MC_FUN,        SA_MC_DEV,        SA_MC_BUS, 0, SA_SEG_NUM, 0}}, {0, 0},0},
  {{{R_SA_PEG_SS_OFFSET, SA_PEG0_FUN_NUM,  SA_PEG0_DEV_NUM,  SA_MC_BUS, 0, SA_SEG_NUM, 0}}, {0, 0},0},
  {{{R_SA_PEG_SS_OFFSET, SA_PEG1_FUN_NUM,  SA_PEG1_DEV_NUM,  SA_MC_BUS, 0, SA_SEG_NUM, 0}}, {0, 0},0},
  {{{R_SA_PEG_SS_OFFSET, SA_PEG2_FUN_NUM,  SA_PEG2_DEV_NUM,  SA_MC_BUS, 0, SA_SEG_NUM, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,    SA_IGD_FUN_0,     SA_IGD_DEV,       SA_MC_BUS, 0, SA_SEG_NUM, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,    SA_IPU_FUN_NUM,   SA_IPU_DEV_NUM,   SA_MC_BUS, 0, SA_SEG_NUM, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,    SA_GNA_FUN_NUM,   SA_GNA_DEV_NUM,   SA_MC_BUS, 0, SA_SEG_NUM, 0}}, {0, 0},0},
  //
  // PCH Device(s)
  //
  {{{PCI_SVID_OFFSET,    PCI_FUNCTION_NUMBER_PCH_LPC,     PCI_DEVICE_NUMBER_PCH_LPC,    DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,    PCI_FUNCTION_NUMBER_PCH_P2SB,    PCI_DEVICE_NUMBER_PCH_P2SB,   DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,    PCI_FUNCTION_NUMBER_PCH_PMC,     PCI_DEVICE_NUMBER_PCH_PMC,    DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,    PCI_FUNCTION_NUMBER_PCH_HDA,     PCI_DEVICE_NUMBER_PCH_HDA,    DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,    PCI_FUNCTION_NUMBER_PCH_SATA,    PCI_DEVICE_NUMBER_PCH_SATA,   DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,    PCI_FUNCTION_NUMBER_PCH_SMBUS,   PCI_DEVICE_NUMBER_PCH_SMBUS,  DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,    PCI_FUNCTION_NUMBER_PCH_SPI,     PCI_DEVICE_NUMBER_PCH_SPI,    DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  //
  // Skip PCH LAN controller
  // PCH LAN SVID/SID may be loaded automatically from the NVM Word 0Ch/0Bh upon power up or reset
  // depending on the "Load Subsystem ID" bit field in NVM word 0Ah
  //
  //{{{PCI_SVID_OFFSET,    PCI_FUNCTION_NUMBER_PCH_LAN,     PCI_DEVICE_NUMBER_PCH_LAN,    DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,  PCI_FUNCTION_NUMBER_PCH_TRACE_HUB,       PCI_DEVICE_NUMBER_PCH_TRACE_HUB,        DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,  PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_UART0, PCI_DEVICE_NUMBER_PCH_SERIAL_IO_UART0,  DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,  PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_UART1, PCI_DEVICE_NUMBER_PCH_SERIAL_IO_UART1,  DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,  PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_SPI0,  PCI_DEVICE_NUMBER_PCH_SERIAL_IO_SPI0,   DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,  PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_SPI1,  PCI_DEVICE_NUMBER_PCH_SERIAL_IO_SPI1,   DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,  PCI_FUNCTION_NUMBER_PCH_CNL_SCS_SDCARD, PCI_DEVICE_NUMBER_PCH_CNL_SCS_SDCARD, DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,  PCI_FUNCTION_NUMBER_PCH_XHCI,       PCI_DEVICE_NUMBER_PCH_XHCI,       DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,  PCI_FUNCTION_NUMBER_PCH_XDCI,       PCI_DEVICE_NUMBER_PCH_XDCI,       DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,  PCI_FUNCTION_NUMBER_PCH_THERMAL,    PCI_DEVICE_NUMBER_PCH_THERMAL,    DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,  PCI_FUNCTION_NUMBER_PCH_ISH,               PCI_DEVICE_NUMBER_PCH_ISH,           DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SVID,  PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_1,  PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_1, DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SVID,  PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_2,  PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_1, DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SVID,  PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_3,  PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_1, DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SVID,  PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_4,  PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_1, DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SVID,  PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_5,  PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_1, DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SVID,  PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_6,  PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_1, DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SVID,  PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_7,  PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_1, DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SVID,  PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_8,  PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_1, DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SVID,  PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_9,  PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_2, DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SVID,  PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_10, PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_2, DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SVID,  PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_11, PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_2, DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SVID,  PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_12, PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_2, DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SVID,  PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_13, PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_2, DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SVID,  PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_14, PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_2, DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SVID,  PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_15, PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_2, DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SVID,  PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_16, PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_2, DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SVID,  PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_17, PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_3, DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SVID,  PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_18, PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_3, DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SVID,  PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_19, PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_3, DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SVID,  PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_20, PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_3, DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SVID,  PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_21, PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_3, DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SVID,  PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_22, PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_3, DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SVID,  PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_23, PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_3, DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SVID,  PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_24, PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_3, DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,  PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_I2C0,  PCI_DEVICE_NUMBER_PCH_SERIAL_IO_I2C0,  DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,  PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_I2C1,  PCI_DEVICE_NUMBER_PCH_SERIAL_IO_I2C1,  DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,  PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_I2C2,  PCI_DEVICE_NUMBER_PCH_SERIAL_IO_I2C2,  DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,  PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_I2C3,  PCI_DEVICE_NUMBER_PCH_SERIAL_IO_I2C3,  DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,  PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_UART2, PCI_DEVICE_NUMBER_PCH_SERIAL_IO_UART2, DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,  PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_I2C5,  PCI_DEVICE_NUMBER_PCH_SERIAL_IO_I2C5,  DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,  PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_I2C4,  PCI_DEVICE_NUMBER_PCH_SERIAL_IO_I2C4,  DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  //
  // ME Device(s)
  //
  {{{PCI_SVID_OFFSET,  HECI_FUNCTION_NUMBER,  ME_DEVICE_NUMBER, DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,  HECI2_FUNCTION_NUMBER, ME_DEVICE_NUMBER, DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,  IDER_FUNCTION_NUMBER,  ME_DEVICE_NUMBER, DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,  SOL_FUNCTION_NUMBER,   ME_DEVICE_NUMBER, DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,  HECI3_FUNCTION_NUMBER, ME_DEVICE_NUMBER, DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,  HECI4_FUNCTION_NUMBER, ME_DEVICE_NUMBER, DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0}
};

/**
  This function performs Silicon PEI Policy initialization.

  @param[in] SiPolicy  The Silicon Policy PPI instance

  @retval EFI_SUCCESS  The function completed successfully
**/
EFI_STATUS
EFIAPI
UpdatePeiSiPolicy (
  IN OUT SI_POLICY_PPI *SiPolicy
  )
{
  EFI_STATUS                         Status;
  SI_CONFIG                          *SiConfig;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gSiConfigGuid, (VOID *) &SiConfig);
  ASSERT_EFI_ERROR (Status);

  SiConfig->CsmFlag       = 0;

  if (IsCdfPch ()) {
    SiConfig->SsidTablePtr = (UINT32*)(UINTN) mCdfSsidTablePtr;
    SiConfig->NumberOfSsidTableEntry = (sizeof (mCdfSsidTablePtr) / sizeof (SVID_SID_INIT_ENTRY));
  } else {
    SiConfig->SsidTablePtr = (UINT32*)(UINTN) mSsidTablePtr;
    SiConfig->NumberOfSsidTableEntry = (sizeof (mSsidTablePtr) / sizeof (SVID_SID_INIT_ENTRY));
  }
  return EFI_SUCCESS;
}


