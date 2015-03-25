/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <project.h>

#define TABLE_LENGTH 254


volatile uint8 ramtable[TABLE_LENGTH];

CYCODE const uint8 sinus_table[TABLE_LENGTH] = {0x80,0x83,0x86,0x89,0x8c,0x8f,0x92,0x95,
0x99,0x9c,0x9f,0xa2,0xa5,0xa8,0xab,0xae,
0xb1,0xb4,0xb6,0xb9,0xbc,0xbf,0xc2,0xc4,
0xc7,0xc9,0xcc,0xce,0xd1,0xd3,0xd6,0xd8,
0xda,0xdc,0xdf,0xe1,0xe3,0xe5,0xe6,0xe8,
0xea,0xec,0xed,0xef,0xf0,0xf2,0xf3,0xf5,
0xf6,0xf7,0xf8,0xf9,0xfa,0xfb,0xfb,0xfc,
0xfd,0xfd,0xfe,0xfe,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xfe,0xfe,0xfd,0xfd,
0xfc,0xfb,0xfb,0xfa,0xf9,0xf8,0xf7,0xf6,
0xf5,0xf3,0xf2,0xf0,0xef,0xed,0xec,0xea,
0xe8,0xe6,0xe5,0xe3,0xe1,0xdf,0xdc,0xda,
0xd8,0xd6,0xd3,0xd1,0xce,0xcc,0xc9,0xc7,
0xc4,0xc2,0xbf,0xbc,0xb9,0xb6,0xb4,0xb1,
0xae,0xab,0xa8,0xa5,0xa2,0x9f,0x9c,0x99,
0x95,0x92,0x8f,0x8c,0x89,0x86,0x83,0x80,
0x7c,0x79,0x76,0x73,0x70,0x6d,0x6a,0x66,
0x63,0x60,0x5d,0x5a,0x57,0x54,0x51,0x4e,
0x4b,0x49,0x46,0x43,0x40,0x3d,0x3b,0x38,
0x36,0x33,0x31,0x2e,0x2c,0x29,0x27,0x25,
0x23,0x20,0x1e,0x1c,0x1a,0x19,0x17,0x15,
0x13,0x12,0x10,0xf,0xd,0xc,0xa,0x9,
0x8,0x7,0x6,0x5,0x4,0x4,0x3,0x2,
0x2,0x1,0x1,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x1,0x1,0x2,0x2,0x3,
0x4,0x4,0x5,0x6,0x7,0x8,0x9,0xa,
0xc,0xd,0xf,0x10,0x12,0x13,0x15,0x17,
0x19,0x1a,0x1c,0x1e,0x20,0x23,0x25,0x27,
0x29,0x2c,0x2e,0x31,0x33,0x36,0x38,0x3b,
0x3d,0x40,0x43,0x46,0x49,0x4b,0x4e,0x51,
0x54,0x57,0x5a,0x5d,0x60,0x63,0x66,0x6a,
0x6d,0x70,0x73,0x76,0x79,0x7c
};

int main()
{
    uint32 i;
    
    //capture_buffer -> PWM
    uint8 PWM_DMA_Chan;
    uint8 PWM_DMA_TD[1];

    for(i=0;i<TABLE_LENGTH;i++)
        ramtable[i] = sinus_table[i];
    
    PWM_Start();

    PWM_DMA_Chan = PWM_DMA_DmaInitialize(1, 1, HI16(&ramtable), HI16(CYDEV_PERIPH_BASE) );
    PWM_DMA_TD[0] = CyDmaTdAllocate();
	CyDmaTdSetConfiguration( PWM_DMA_TD[0], TABLE_LENGTH, PWM_DMA_TD[0], TD_INC_SRC_ADR); 	 
    CyDmaTdSetAddress(PWM_DMA_TD[0], LO16((uint32)&ramtable), LO16((uint32)PWM_COMPARE1_LSB_PTR) );
    CyDmaChSetInitialTd(PWM_DMA_Chan, PWM_DMA_TD[0]);
    CyDmaChEnable(PWM_DMA_Chan, 1); 							

    
    
    /* CyGlobalIntEnable; */ /* Uncomment this line to enable global interrupts. */
    uint32 alpha = 50;
    for(;;)
    {
        SYNC_Write(!SYNC_Read());
        for(i=0;i<TABLE_LENGTH;i++) {
            uint32 tmp = (((uint32)sinus_table[i])*alpha)/100;
            ramtable[i] = tmp;
        }

       
    }
}

/* [] END OF FILE */
