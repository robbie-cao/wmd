#ifndef __PMU_H__
#define __PMU_H__

#define PMU_CHIP_ADDR           (0b110 << 4)

#define PMU_REG_ADDR_PDCFG      0x0
#define PMU_REG_ADDR_TRIMCFG    0x1
#define PMU_REG_ADDR_CVG1       0x2
#define PMU_REG_ADDR_CVG2       0x3
#define PMU_REG_ADDR_DCT1       0x4
#define PMU_REG_ADDR_DCT2       0x5
#define PMU_REG_ADDR_WNF        0x6
#define PMU_REG_ADDR_DCI        0x7
#define PMU_REG_ADDR_CLKDIV     0x8
#define PMU_REG_ADDR_OOL6       0x9
#define PMU_REG_ADDR_OOL0       0xA
#define PMU_REG_ADDR_OOL1       0xB
#define PMU_REG_ADDR_OOL2       0xC
#define PMU_REG_ADDR_OOL3       0xD
#define PMU_REG_ADDR_OOL4       0xE
#define PMU_REG_ADDR_OOL5       0xF

#define PMU_I2C_ADDR(reg)       (PMU_CHIP_ADDR | (PMU_REG_ADDR_##reg))

#define PMU_INT_NONE            0x00
#define PMU_INT_POWON           0x01
#define PMU_INT_POWOFF          0x02
#define PMU_INT_LVDSTATE        0x04
#define PMU_INT_OPTSTATE        0x08
#define PMU_INT_TALKON          0x10
#define PMU_INT_TALKOFF         0x20

#define PMU_IRQ_MODE_LEVEL      0x00
#define PMU_IRQ_MODE_PULSE      0x01
#define PMU_SW_POWON            0x02
#define PMU_SW_POWOFF           0x04


#define PMU_PRON_ENABLE         0
#define PMU_PRON_DISABLE        1

/* PMU module initialization */
void    PMU_Init(void);
/* PMU interrupt service routine */
void    PMU_ISR(void);
/* PMU task entry */
void    PMU_IndHandler(void);

#endif /* __PMU_H__ */
