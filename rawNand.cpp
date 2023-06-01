#include "rawNand.h"
#define SIG_LOW (0)
#define SIG_HIGH (1)

void rawNand::reset(uint8_t cs){
  // wait ready
  while (m_rawNandLlabst.getRbbIo()==0){
    m_rawNandLlabst.nop();
  }

  // CS assert
  if(cs==0) {
    m_rawNandLlabst.setCsIo(SIG_LOW,SIG_HIGH);
  } else if (cs==1) {
    m_rawNandLlabst.setCsIo(SIG_HIGH,SIG_LOW);
  }
  
  // CLE=1,ALE=0,WEB=0
  m_rawNandLlabst.setCmdIo(1,0,0);

  // reset command (0xff)
  m_rawNandLlabst.setDqIo(0xff);
  m_rawNandLlabst.setDqDir(1);

  // wait setup time : max(tCS,tCLS,tALS,tDS,tWP)
  m_rawNandLlabst.waitUs(tCS_US);

  // WEB=1
  m_rawNandLlabst.setCmdIo(1,0,1);

  // wait hold time : max(tCLH,tALH,tDH,tWH,tWB,tCH)
  m_rawNandLlabst.waitUs(tWB_US);

  // check RBB
  while (m_rawNandLlabst.getRbbIo()==0) {
    m_rawNandLlabst.nop();
  }

  // CS deassert
  m_rawNandLlabst.setCsIo(SIG_HIGH,SIG_HIGH);  
}

void rawNand::idRead(uint8_t cs, uint8_t * readData) {
  // CS assert
  if(cs==0) {
    m_rawNandLlabst.setCsIo(SIG_LOW,SIG_HIGH);
  } else if (cs==1) {
    m_rawNandLlabst.setCsIo(SIG_HIGH,SIG_LOW);
  }

  // CLE=1,ALE=0,WEB=0
  m_rawNandLlabst.setCmdIo(1,0,0);

  // id read command (0x90)
  m_rawNandLlabst.setDqIo(0x90);
  m_rawNandLlabst.setDqDir(1);

  // wait setup time : max(tCS,tCLS,tALS,tDS,tWP)
  m_rawNandLlabst.waitUs(tCS_US);

  // WEB=1
  m_rawNandLlabst.setCmdIo(1,0,1);

  // wait hold time : max(tCLH,tALH,tDH,tWH)
  m_rawNandLlabst.waitUs(tWH_US);

  // CLE=0,ALE=1,WEB=0
  m_rawNandLlabst.setCmdIo(0,1,0);
  
  // read address (0x00)
  m_rawNandLlabst.setDqIo(0x00);
  m_rawNandLlabst.setDqDir(1);

  // wait setup time : max(tCLS,tALS,tDS,tWP)
  m_rawNandLlabst.waitUs(tCLS_US);

  // WEB=1
  m_rawNandLlabst.setCmdIo(0,1,1);

  // wait hold time : max(tCLH,tALH,tDH,tWH)
  m_rawNandLlabst.waitUs(tWH_US);

  // ALE=0
  m_rawNandLlabst.setCmdIo(0,0,1);

  // IO direction input
  m_rawNandLlabst.setDqDir(0);
  
  // ALE low to tREB low : tAR
  m_rawNandLlabst.waitUs(tAR_US);

  // IO READ read data
  for (int l=0;l<5;l++) {
    // REB=0
    m_rawNandLlabst.setRebIo(0);

    // wait max(tREA,tRP)
    m_rawNandLlabst.waitUs(tREA_US);

    // read data
    *(readData+l)=m_rawNandLlabst.getDqIo();

    // REB=1
    m_rawNandLlabst.setRebIo(1);

    // wait tREH
    m_rawNandLlabst.waitUs(tREH_US);
  }

  // CS deassert
  m_rawNandLlabst.setCsIo(SIG_HIGH,SIG_HIGH);

  // wait io hiz tCHZ_US
  m_rawNandLlabst.waitUs(tCHZ_US);
}

uint8_t rawNand::statusRead(uint8_t cs) {
  uint8_t status;
  // wait ready
  while (m_rawNandLlabst.getRbbIo()==0){
    m_rawNandLlabst.nop();
  }

  if(cs==0) {
    m_rawNandLlabst.setCsIo(SIG_LOW,SIG_HIGH);
  } else if (cs==1) {
    m_rawNandLlabst.setCsIo(SIG_HIGH,SIG_LOW);
  }

  // CLE=1,ALE=0,WEB=0
  m_rawNandLlabst.setCmdIo(1,0,0);

  // status read command (0x70)
  m_rawNandLlabst.setDqIo(0x70);
  m_rawNandLlabst.setDqDir(1);

  // wait setup time : max(tCS,tCLS,tALS,tDS,tWP)
  m_rawNandLlabst.waitUs(tCS_US);

  // WEB=1
  m_rawNandLlabst.setCmdIo(1,0,1);

  // wait hold time : max(tCLH,tDH,tWH)
  m_rawNandLlabst.waitUs(tWH_US);

  // CLE=0
  m_rawNandLlabst.setCmdIo(0,0,1);

  // IO direction input
  m_rawNandLlabst.setDqDir(0);

  // wait max(tWHR-tWH,tCLR-tWH)
  m_rawNandLlabst.waitUs(tWHR_US-tWH_US);

  // REB=0
  m_rawNandLlabst.setRebIo(0);
  
  // wait max(tREA,tRP)
  m_rawNandLlabst.waitUs(tREA_US);

  // read data
  status = m_rawNandLlabst.getDqIo();

  // REB=1
  m_rawNandLlabst.setRebIo(1);

  // wait tREH
  m_rawNandLlabst.waitUs(tREH_US);

  // CS deassert
  m_rawNandLlabst.setCsIo(SIG_HIGH,SIG_HIGH);
  
  // wait tCHZ_US
  m_rawNandLlabst.waitUs(tCHZ_US);

  return status;
}

void rawNand::setWriteProtect(uint8_t writeProtect) {
  m_rawNandLlabst.setWpbIo(writeProtect);

  // wait tWW
  m_rawNandLlabst.waitUs(tWW_US);
}

void rawNand::pageRead(uint8_t cs, uint8_t * readData, uint16_t blockAddress, uint8_t pageAddress, uint16_t columnAddress, uint16_t beats) {
  // wait ready
  while (m_rawNandLlabst.getRbbIo()==0){
    m_rawNandLlabst.nop();
  }

  // CS assert
  if(cs==0) {
    m_rawNandLlabst.setCsIo(SIG_LOW,SIG_HIGH);
  } else if (cs==1) {
    m_rawNandLlabst.setCsIo(SIG_HIGH,SIG_LOW);
  }

  // CLE=1,ALE=0,WEB=0
  m_rawNandLlabst.setCmdIo(1,0,0);

  // 1st read command (0x00)
  m_rawNandLlabst.setDqIo(0x00);
  m_rawNandLlabst.setDqDir(1);

  // wait setup time : max(tCS,tCLS,tALS,tDS,tWP)
  m_rawNandLlabst.waitUs(tCS_US);
  
  // WEB=1
  m_rawNandLlabst.setCmdIo(1,0,1);

  // wait hold time : max(tCLH,tDH,tWH)
  m_rawNandLlabst.waitUs(tWH_US);

  // 1st address column [7:0]
  // CLE=0,ALE=1,WEB=0
  m_rawNandLlabst.setCmdIo(0,1,0);

  // address column [7:0]
  m_rawNandLlabst.setDqIo( columnAddress & 0xff );

  // wait setup time : max(tALS,tCLS,tDS,tWP)
  m_rawNandLlabst.waitUs(tWP_US);

  // WEB=1
  m_rawNandLlabst.setCmdIo(0,1,1);

  // wait hold time : max(tCLH,tDH,tWH)
  m_rawNandLlabst.waitUs(tWH_US);

  // 2nd address column [11:8]
  // CLE=0,ALE=1,WEB=0
  m_rawNandLlabst.setCmdIo(0,1,0);

  // address column [11:8]
  m_rawNandLlabst.setDqIo( (columnAddress>>8) & 0x0f );

  // wait setup time : max(tALS,tCLS,tDS,tWP)
  m_rawNandLlabst.waitUs(tWP_US);

  // WEB=1
  m_rawNandLlabst.setCmdIo(0,1,1);
  
  // wait hold time : max(tCLH,tDH,tWH)
  m_rawNandLlabst.waitUs(tWH_US);

  // 3rd address  {blockAddress[1:0],pageAddress[5:0]}
  // CLE=0,ALE=1,WEB=0
  m_rawNandLlabst.setCmdIo(0,1,0);

  // {blockAddress[1:0],pageAddress[5:0]}
  m_rawNandLlabst.setDqIo((blockAddress<<6) | pageAddress) ;

  // wait setup time : max(tALS,tDS,tWP)
  m_rawNandLlabst.waitUs(tWP_US);

  // WEB=1
  m_rawNandLlabst.setCmdIo(0,1,1);
  
  // wait hold time : max(tALH,tDH,tWH)
  m_rawNandLlabst.waitUs(tWH_US);

  // 4th address  blockAddress[9:2]
  // CLE=0,ALE=1,WEB=0
  m_rawNandLlabst.setCmdIo(0,1,0);

  // blockAddress[9:2]
  m_rawNandLlabst.setDqIo (blockAddress>>2) ;

  // wait setup time : max(tALS,tDS,tWP)
  m_rawNandLlabst.waitUs(tWP_US);

  // WEB=1
  m_rawNandLlabst.setCmdIo(0,1,1);

  // wait hold time : max(tALH,tDH,tWH)
  m_rawNandLlabst.waitUs(tWH_US);

  // 2ND READ COMMAND (0x30)
  // CLE=1,ALE=0,WEB=0
  m_rawNandLlabst.setCmdIo(1,0,0);

  // command 0x30
  m_rawNandLlabst.setDqIo (0x30);

  // wait setup time : max(tALS,tCLS,tDS,tWP)
  m_rawNandLlabst.waitUs(tWP_US);

  // WEB=1
  m_rawNandLlabst.setCmdIo(1,0,1);
  
  // wait hold time : max(tALH,tDH,tWH,tWB)
  m_rawNandLlabst.waitUs(tWB_US);

  // CLE=0,ALE=0,WEB=1
  m_rawNandLlabst.setCmdIo(0,0,1);

  // IO direction input
  m_rawNandLlabst.setDqDir(0);
  
  // wait ready
  while (m_rawNandLlabst.getRbbIo()==0){
    m_rawNandLlabst.nop();
  }
  
  // wait tRR  ( RBB to REB Low )
  m_rawNandLlabst.waitUs(tRR_US);

  // read sequence
  for (int b=0;b<beats;b++){
    // REB=0;
    m_rawNandLlabst.setRebIo(0);

    // wait max(tREA,tRP)
    m_rawNandLlabst.waitUs(tREA_US);

    // read data
    *(readData + b)= m_rawNandLlabst.getDqIo();

    // REB=1
    m_rawNandLlabst.setRebIo(1);

    // wait tREH
    m_rawNandLlabst.waitUs(tREH_US);
  }

  // CS deassert
  m_rawNandLlabst.setCsIo(SIG_HIGH,SIG_HIGH);
  
  // wait io hiz
  m_rawNandLlabst.waitUs(tCHZ_US);
}

uint8_t  rawNand::pageProgram(uint8_t cs, const uint8_t *writeData, uint16_t blockAddress, uint8_t pageAddress, uint16_t columnAddress, uint16_t beats) {
  // wait ready
  while (m_rawNandLlabst.getRbbIo()==0){
    m_rawNandLlabst.nop();
  }

  // CS assert
  if(cs==0) {
    m_rawNandLlabst.setCsIo(SIG_LOW,SIG_HIGH);
  } else if (cs==1) {
    m_rawNandLlabst.setCsIo(SIG_HIGH,SIG_LOW);
  }

  // CLE=1,ALE=0,WEB=0
  m_rawNandLlabst.setCmdIo(1,0,0);

  // 1st program command (0x80)
  m_rawNandLlabst.setDqIo(0x80);
  m_rawNandLlabst.setDqDir(1);

  // wait setup time : max(tCS,tCLS,tALS,tDS,tWP)
  m_rawNandLlabst.waitUs(tCS_US);

  // WEB=1
  m_rawNandLlabst.setCmdIo(1,0,1);

  // wait hold time : max(tCLH,tDH,tWH)
  m_rawNandLlabst.waitUs(tWH_US);

  // 1st address column [7:0]
  // CLE=0,ALE=1,WEB=0
  m_rawNandLlabst.setCmdIo(0,1,0);

  // 1st address column [7:0]
  m_rawNandLlabst.setDqIo(columnAddress & 0xff);

  // wait setup time : max(tALS,tCLS,tDS,tWP)
  m_rawNandLlabst.waitUs(tWP_US);

  // WEB=1
  m_rawNandLlabst.setCmdIo(0,1,1);
  
  // wait hold time : max(tCLH,tDH,tWH)
  m_rawNandLlabst.waitUs(tWH_US);

  // 2nd address column [11:8]
  // CLE=0,ALE=1,WEB=0
  m_rawNandLlabst.setCmdIo(0,1,0);

  // 2nd address column [11:8]
  m_rawNandLlabst.setDqIo((columnAddress>>8) & 0x0f);
  
  // wait setup time : max(tALS,tCLS,tDS,tWP)
  m_rawNandLlabst.waitUs(tWP_US);

  // WEB=1
  m_rawNandLlabst.setCmdIo(0,1,1);

  // wait hold time : max(tCLH,tDH,tWH)
  m_rawNandLlabst.waitUs(tWH_US);

  // 3rd address  {blockAddress[1:0],pageAddress[5:0]}
  // CLE=0,ALE=1,WEB=0
  m_rawNandLlabst.setCmdIo(0,1,0);

  // 3rd address  {blockAddress[1:0],pageAddress[5:0]}
  m_rawNandLlabst.setDqIo((blockAddress<<6) | pageAddress) ;

  // wait setup time : max(tALS,tDS,tWP)
  m_rawNandLlabst.waitUs(tWP_US);

  // WEB=1
  m_rawNandLlabst.setCmdIo(0,1,1);

  // wait hold time : max(tALH,tDH,tWH)
  m_rawNandLlabst.waitUs(tWH_US);

  // 4th address  blockAddress[9:2]
  // CLE=0,ALE=1,WEB=0
  m_rawNandLlabst.setCmdIo(0,1,0);

  // 4th address  blockAddress[9:2]
  m_rawNandLlabst.setDqIo(blockAddress>>2) ;

  // wait setup time : max(tALS,tDS,tWP)
  m_rawNandLlabst.waitUs(tWP_US);

  // WEB=1
  m_rawNandLlabst.setCmdIo(0,1,1);
  
  // wait hold time : max(tALH,tDH,tWH)
  m_rawNandLlabst.waitUs(tWH_US);

  // CLE=0,ALE=0,WEB=1
  m_rawNandLlabst.setCmdIo(0,0,1);

  // datain
  for (int b=0;b<beats;b++) {
    m_rawNandLlabst.setDqIo( *(writeData+b));

    // CLE=0,ALE=0,WEB=0
    m_rawNandLlabst.setCmdIo(0,0,0);
    
    // wait setup time : max(tALS,tCLS,tDS,tWP)
    m_rawNandLlabst.waitUs(tWP_US);

    // WEB=1
    m_rawNandLlabst.setCmdIo(0,0,1);
    
    // wait hold time : max(tALH,tDH,tWH)
    m_rawNandLlabst.waitUs(tWH_US);
  }

  // 2ND PROGRAM COMMAND (0x10)
  // CLE=1,ALE=0,WEB=1
  m_rawNandLlabst.setCmdIo(1,0,1);

  // 2ND PROGRAM COMMAND (0x10)
  m_rawNandLlabst.setDqIo(0x10);

  // CLE=1,ALE=0,WEB=0
  m_rawNandLlabst.setCmdIo(1,0,0);

  // wait setup time : max(tALS,tCLS,tDS,tWP)
  m_rawNandLlabst.waitUs(tWP_US);

  // WEB=1
  m_rawNandLlabst.setCmdIo(1,0,1);
  
  // wait hold time : max(tCLH,tDH,tWH,tWB)
  m_rawNandLlabst.waitUs(tWB_US);

  // CLE=0,ALE=0,WEB=1
  m_rawNandLlabst.setCmdIo(0,0,1);

  // IO direction input
  m_rawNandLlabst.setDqDir(0);
  
  // wait ready
  while (m_rawNandLlabst.getRbbIo()==0){
    m_rawNandLlabst.nop();
  }

  // return status read result
  // bit0 == 0 : pass, bit0 == 1 : fail
  return statusRead(cs);
}

uint8_t  rawNand::erase(uint8_t cs, uint16_t blockAddress) {
  // wait ready
  while (m_rawNandLlabst.getRbbIo()==0){
    m_rawNandLlabst.nop();
  }

  // CS assert
  if(cs==0) {
    m_rawNandLlabst.setCsIo(SIG_LOW,SIG_HIGH);
  } else if (cs==1) {
    m_rawNandLlabst.setCsIo(SIG_HIGH,SIG_LOW);
  }

  // CLE=1,ALE=0,WEB=0
  m_rawNandLlabst.setCmdIo(1,0,0);

  // 1st erase command (0x60)
  m_rawNandLlabst.setDqIo(0x60);
  m_rawNandLlabst.setDqDir(1);

  // wait setup time : max(tCS,tCLS,tALS,tDS,tWP)
  m_rawNandLlabst.waitUs(tCS_US);

  // WEB=1
  m_rawNandLlabst.setCmdIo(1,0,1);

  // wait hold time : max(tCLH,tDH,tWH)
  m_rawNandLlabst.waitUs(tWH_US);

  // 1st page address  {blockAddress[1:0],pageAddress[5:0]}
  // CLE=0,ALE=1,WEB=0
  m_rawNandLlabst.setCmdIo(0,1,0);

  // blockAddress[1:0]
  m_rawNandLlabst.setDqIo (blockAddress<<6) ;

  // wait setup time : max(tALS,tDS,tWP)
  m_rawNandLlabst.waitUs(tWP_US);

  // WEB=1
  m_rawNandLlabst.setCmdIo(0,1,1);

  // wait hold time : max(tALH,tDH,tWH)
  m_rawNandLlabst.waitUs(tWH_US);

  // 2nd page address  blockAddress[9:2]
  // CLE=0,ALE=1,WEB=0
  m_rawNandLlabst.setCmdIo(0,1,0);

  // blockAddress[9:2]
  m_rawNandLlabst.setDqIo (blockAddress>>2) ;

  // wait setup time : max(tALS,tDS,tWP)
  m_rawNandLlabst.waitUs(tWP_US);

  // WEB=1
  m_rawNandLlabst.setCmdIo(0,1,1);

  // wait hold time : max(tALH,tDH,tWH)
  m_rawNandLlabst.waitUs(tWH_US);


  // 2ND ERASE COMMAND (0xD0)
  // CLE=0,ALE=1,WEB=0
  m_rawNandLlabst.setCmdIo(1,0,0);
  
  // 2nd erase command 0xd0
  m_rawNandLlabst.setDqIo(0xD0);

  // wait setup time : max(tALS,tCLS,tDS,tWP)
  m_rawNandLlabst.waitUs(tWP_US);

  // WEB=1
  m_rawNandLlabst.setCmdIo(1,0,1);
  
  // wait hold time : max(tALH,tDH,tWH,tWB)
  m_rawNandLlabst.waitUs(tWB_US);

  // CLE=0,ALE=0,WEB=1
  m_rawNandLlabst.setCmdIo(0,0,1);

  // IO direction input
  m_rawNandLlabst.setDqDir(0);

  // wait ready
  while (m_rawNandLlabst.getRbbIo()==0){
    m_rawNandLlabst.nop();
  }

  // return status read result
  // bit0 == 0 : pass, bit0 == 1 : fail
  return statusRead(cs);
}
