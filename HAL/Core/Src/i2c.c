#include "gpio.h"

void delay1(unsigned int n)
{
  unsigned int i;
  for (i=0;i<n;++i);
}

void I2C_Start(void)
{
  SDA_Output(1); delay1(500);
  SCL_Output(1); delay1(500);
  SDA_Output(0); delay1(500);
  SCL_Output(0); delay1(500);
}

void I2C_Stop(void)
{
  SCL_Output(0); delay1(500);
  SDA_Output(0); delay1(500);
  SCL_Output(1); delay1(500);
  SDA_Output(1); delay1(500);
}

unsigned char I2C_WaitAck(void)
{
  unsigned short cErrTime = 5;
  SDA_Input_Mode();	delay1(500);
  SCL_Output(1); delay1(500);
  while (SDA_Input()) {
    cErrTime--;
    delay1(500);
    if (cErrTime == 0) {
      SDA_Output_Mode();
      I2C_Stop();
      return 1;
    }
  }
  SDA_Output_Mode();
  SCL_Output(0); delay1(500); 
  return 0;
}

void I2C_SendAck(void)
{
  SDA_Output(0); delay1(500);
  SCL_Output(1); delay1(500);
  SCL_Output(0); delay1(500);
}

void I2C_SendNotAck(void)
{
  SDA_Output(1); delay1(500);
  SCL_Output(1); delay1(500);
  SCL_Output(0); delay1(500);
}

void I2C_SendByte(unsigned char cSendByte)
{
  unsigned char  i = 8;
  while (i--) {
    SCL_Output(0); delay1(500); 
    SDA_Output(cSendByte & 0x80); delay1(500);
    cSendByte <<= 1;
    SCL_Output(1);delay1(500); 
  }
  SCL_Output(0); delay1(500); 
}

unsigned char I2C_ReceiveByte(void)
{
  unsigned char i = 8;
  unsigned char cR_Byte = 0;
  SDA_Input_Mode(); 
  while (i--) {
    cR_Byte <<= 1;
    SCL_Output(0); delay1(500); 
    SCL_Output(1); delay1(500); 
    cR_Byte |=  SDA_Input(); 
  }
  SCL_Output(0); delay1(500); 
  SDA_Output_Mode();
  return cR_Byte;
}

void MEM_Read(unsigned char* ucBuf, unsigned char ucAddr,
  unsigned char ucNum)
{
  I2C_Start(); 
  I2C_SendByte(0xa0);
  I2C_WaitAck(); 

  I2C_SendByte(ucAddr);
  I2C_WaitAck(); 

  I2C_Start();
  I2C_SendByte(0xa1); 
  I2C_WaitAck();

  while (ucNum--) {
    *ucBuf++ = I2C_ReceiveByte();
    if (ucNum)
      I2C_SendAck();
    else
      I2C_SendNotAck();
  }
  I2C_Stop();
}

void MEM_Write(unsigned char* ucBuf, unsigned char ucAddr,
  unsigned char ucNum)
{
  I2C_Start(); 
  I2C_SendByte(0xa0); 
  I2C_WaitAck(); 

  I2C_SendByte(ucAddr);	
  I2C_WaitAck();

  while (ucNum--) {
    I2C_SendByte(*ucBuf++); 
    I2C_WaitAck(); 
  }
  I2C_Stop();
  delay1(500);
}

void MCP_Write(unsigned char ucVal)
{
  I2C_Start();
  I2C_SendByte(0x5e);
  I2C_WaitAck();

  I2C_SendByte(ucVal);
  I2C_WaitAck();
  I2C_Stop();
}