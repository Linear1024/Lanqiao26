#ifndef __I2C_H
#define __I2C_H
#include "main.h"

void MEM_Read(unsigned char* ucBuf, unsigned char ucAddr,
  unsigned char ucNum);
void MEM_Write(unsigned char* ucBuf, unsigned char ucAddr,
  unsigned char ucNum);
void MCP_Write(unsigned char ucVal);
#endif /* __I2C_H */
