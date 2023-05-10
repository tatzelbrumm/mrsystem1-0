#define POLY 0x1021

unsigned short UpdateCRC(unsigned short CRC_acc, unsigned char CRC_input)
{  int i;

   /* Create the CRC "dividend" for polynomial arithmetic
      (binary arithmetic with no carries) */
   CRC_acc = CRC_acc ^ (CRC_input << 8);
   /* "Divide" the poly into the dividend using CRC XOR subtraction
      CRC_acc holds the "remainder" of each divide. Only complete
      this division for 8 bits since input is 1 byte */
   for (i = 0; i < 8; i++)
   {
      /* Check if the MSB is set (if MSB is 1, then the POLY
         CAN "divide" into the "dividend") */
      if ((CRC_acc & 0x8000) == 0x8000)
      {
         /* if so, shift the CRC value, and XOR "subtract" the poly */
         CRC_acc = CRC_acc << 1;
         CRC_acc ^= POLY;
      }
      else
      {
         /* if not, just shift the CRC value */
         CRC_acc = CRC_acc << 1;
      }
   }
   /* Return the final remainder (CRC value) */
   return CRC_acc;
}
