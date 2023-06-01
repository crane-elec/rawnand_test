#ifndef RAWNAND_H
#define RAWNAND_H

#include "rawNandLlabst.h"

// KIOXIA 1Gbit SLC NAND, 3.3V, x8, 24nm, TSOP
// TC58NVG0S3HTA00 datasheet 2019-10-01C 

class rawNand{
 public:
  void     reset(uint8_t cs);
  void     idRead(uint8_t cs, uint8_t * readData);
  uint8_t  statusRead(uint8_t cs);
  void     setWriteProtect(uint8_t writeProtect);
  void     pageRead(uint8_t cs, uint8_t * readData, uint16_t blockAddress, uint8_t pageAddress, uint16_t columnAddress, uint16_t beats);
  uint8_t  pageProgram(uint8_t cs, const uint8_t *writeData, uint16_t blockAddress, uint8_t pageAddress, uint16_t columnAddress, uint16_t beats);
  uint8_t  erase(uint8_t cs, uint16_t blockAddress);
 private:
  //
  // Timing parameter (const)
  //
  const static uint64_t tCLS_US        =12;
  const static uint64_t tCLH_US        =5;
  const static uint64_t tCS_US         =20;
  const static uint64_t tCH_US         =5;
  const static uint64_t tWP_US         =12;
  const static uint64_t tALS_US        =12;
  const static uint64_t tALH_US        =5;
  const static uint64_t tDS_US         =12;
  const static uint64_t tDH_US         =5;
  const static uint64_t tWC_US         =25;
  const static uint64_t tWH_US         =10;
  const static uint64_t tWW_US         =100;
  const static uint64_t tRR_US         =20;
  const static uint64_t tRW_US         =20;
  const static uint64_t tRP_US         =12;
  const static uint64_t tRC_US         =25;
  const static uint64_t tREA_US        =20;
  const static uint64_t tCEA_US        =25;
  const static uint64_t tCLR_US        =10;
  const static uint64_t tAR_US         =10;
  const static uint64_t tRHOH_US       =25;
  const static uint64_t tRLOH_US       =5;
  const static uint64_t tRHZ_US        =60;
  const static uint64_t tCHZ_US        =20;
  const static uint64_t tCSD_US        =0;
  const static uint64_t tREH_US        =10;
  const static uint64_t tIR_US         =0;
  const static uint64_t tRHW_US        =30;
  const static uint64_t tWHC_US        =30;
  const static uint64_t tWHR_US        =60;
  const static uint64_t tWB_US         =100;
  const static uint64_t tRST_RDY_US    =5000;
  const static uint64_t tRST_READ_US   =5000;
  const static uint64_t tRST_PROGRAM_US=10000;
  const static uint64_t tRST_ERASE_US  =500000;
  const static uint64_t tPROG_US       =700000;
  const static uint64_t tDCBAYW2_US    =700000;
  const static int NCYCLES_SAME_PAGE   =4;
  const static uint64_t tBERASE_US     =5000000;
  const static uint64_t MC2SA_US       =25000;
  const static uint64_t tDCBSYR1_US    =25000;
  const static uint64_t tDCBSYR2_US    =30000;

  // low level abstruction layer
  rawNandLlabst m_rawNandLlabst;
};
#endif
