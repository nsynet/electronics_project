#ifndef _ETHDRV_ENC28J60_H
#define _ETHDRV_ENC28J60_H


//
// Control Registers in Bank 0
//

#define CTL_REG_ERDPTL   0x00
#define CTL_REG_ERDPTH   0x01
#define CTL_REG_EWRPTL   0x02
#define CTL_REG_EWRPTH   0x03
#define CTL_REG_ETXSTL   0x04
#define CTL_REG_ETXSTH   0x05
#define CTL_REG_ETXNDL   0x06
#define CTL_REG_ETXNDH   0x07
#define CTL_REG_ERXSTL   0x08
#define CTL_REG_ERXSTH   0x09
#define CTL_REG_ERXNDL   0x0A
#define CTL_REG_ERXNDA   0x0B
#define CTL_REG_ERXRDPTL 0x0C
#define CTL_REG_ERXRDPTH 0x0D
#define CTL_REG_ERXWRPTL 0x0E
#define CTL_REG_ERXWRPTH 0x0F
#define CTL_REG_EDMASTL  0x10
#define CTL_REG_EDMASTH  0x11
#define CTL_REG_EDMANDL  0x12
#define CTL_REG_EDMANDH  0x13
#define CTL_REG_EDMADSTL 0x14
#define CTL_REG_EDMADSTH 0x15
#define CTL_REG_EDMACSL  0x16
#define CTL_REG_EDMACSH  0x17
// these are common in all banks
#define CTL_REG_EIE      0x1B
#define CTL_REG_EIR      0x1C
#define CTL_REG_ESTAT    0x1D
#define CTL_REG_ECON2    0x1E
#define CTL_REG_ECON1    0x1F

//
// Control Registers in Bank 1
//

#define CTL_REG_EHT0    0x00
#define CTL_REG_EHT1    0x01
#define CTL_REG_EHT2    0x02
#define CTL_REG_EHT3    0x03
#define CTL_REG_EHT4    0x04
#define CTL_REG_EHT5    0x05
#define CTL_REG_EHT6    0x06
#define CTL_REG_EHT7    0x07
#define CTL_REG_EPMM0   0x08
#define CTL_REG_EPMM1   0x09
#define CTL_REG_EPMM2   0x0A
#define CTL_REG_EPMM3   0x0B
#define CTL_REG_EPMM4   0x0C
#define CTL_REG_EPMM5   0x0D
#define CTL_REG_EPMM6   0x0E
#define CTL_REG_EPMM7   0x0F
#define CTL_REG_EPMCSL  0x10
#define CTL_REG_EPMCSH  0x11
#define CTL_REG_EPMOL   0x14
#define CTL_REG_EPMOH   0x15
#define CTL_REG_EWOLIE  0x16
#define CTL_REG_EWOLIR  0x17
#define CTL_REG_ERXFCON 0x18
#define CTL_REG_EPKTCNT 0x19

//
// Control Registers in Bank 2
//

#define CTL_REG_MACON1   0x00
#define CTL_REG_MACON2   0x01
#define CTL_REG_MACON3   0x02
#define CTL_REG_MACON4   0x03
#define CTL_REG_MABBIPG  0x04
#define CTL_REG_MAIPGL   0x06
#define CTL_REG_MAIPGH   0x07
#define CTL_REG_MACLCON1 0x08
#define CTL_REG_MACLCON2 0x09
#define CTL_REG_MAMXFLL  0x0A
#define CTL_REG_MAMXFLH  0x0B
#define CTL_REG_MAPHSUP  0x0D
#define CTL_REG_MICON    0x11
#define CTL_REG_MICMD    0x12
#define CTL_REG_MIREGADR 0x14
#define CTL_REG_MIWRL    0x16
#define CTL_REG_MIWRH    0x17
#define CTL_REG_MIRDL    0x18
#define CTL_REG_MIRDH    0x19

//
// Control Registers in Bank 3
//

#define CTL_REG_MAADR1  0x00
#define CTL_REG_MAADR0  0x01
#define CTL_REG_MAADR3  0x02
#define CTL_REG_MAADR2  0x03
#define CTL_REG_MAADR5  0x04
#define CTL_REG_MAADR4  0x05
#define CTL_REG_EBSTSD  0x06
#define CTL_REG_EBSTCON 0x07
#define CTL_REG_EBSTCSL 0x08
#define CTL_REG_EBSTCSH 0x09
#define CTL_REG_MISTAT  0x0A
#define CTL_REG_EREVID  0x12
#define CTL_REG_ECOCON  0x15
#define CTL_REG_EFLOCON 0x17
#define CTL_REG_EPAUSL  0x18
#define CTL_REG_EPAUSH  0x19


//
// PHY Register
//

#define PHY_REG_PHID1 0x02
#define PHY_REG_PHID2 0x03


//
// Receive Filter Register (ERXFCON) bits
//

#define ENC_RFR_UCEN  0x80
#define ENC_RFR_ANDOR 0x40
#define ENC_RFR_CRCEN 0x20
#define ENC_RFR_PMEN  0x10
#define ENC_RFR_MPEN  0x08
#define ENC_RFR_HTEN  0x04
#define ENC_RFR_MCEN  0x02
#define ENC_RFR_BCEN  0x01

//
// ECON1 Register Bits
//

#define ENC_ECON1_TXRST  0x80
#define ENC_ECON1_RXRST  0x40
#define ENC_ECON1_DMAST  0x20
#define ENC_ECON1_CSUMEN 0x10
#define ENC_ECON1_TXRTS  0x08
#define ENC_ECON1_RXEN   0x04
#define ENC_ECON1_BSEL1  0x02
#define ENC_ECON1_BSEL0  0x01

//
// ECON2 Register Bits
//
#define ENC_ECON2_AUTOINC 0x80
#define ENC_ECON2_PKTDEC  0x40
#define ENC_ECON2_PWRSV   0x20
#define ENC_ECON2_VRPS    0x08

//
// EIR Register Bits
//
#define ENC_EIR_PKTIF  0x40
#define ENC_EIR_DMAIF  0x20
#define ENC_EIR_LINKIF 0x10
#define ENC_EIR_TXIF   0x08
#define ENC_EIR_WOLIF  0x04
#define ENC_EIR_TXERIF 0x02
#define ENC_EIR_RXERIF 0x01

//
// ESTAT Register Bits
//

#define ENC_ESTAT_INT     0x80
#define ENC_ESTAT_LATECOL 0x10
#define ENC_ESTAT_RXBUSY  0x04
#define ENC_ESTAT_TXABRT  0x02
#define ENC_ESTAT_CLKRDY  0x01


//
// MACON1 Register Bits
//
#define ENC_MACON1_LOOPBK  0x10
#define ENC_MACON1_TXPAUS  0x08
#define ENC_MACON1_RXPAUS  0x04
#define ENC_MACON1_PASSALL 0x02
#define ENC_MACON1_MARXEN  0x01


//
// MACON2 Register Bits
//
#define ENC_MACON2_MARST   0x80
#define ENC_MACON2_RNDRST  0x40
#define ENC_MACON2_MARXRST 0x08
#define ENC_MACON2_RFUNRST 0x04
#define ENC_MACON2_MATXRST 0x02
#define ENC_MACON2_TFUNRST 0x01

//
// MACON3 Register Bits
//
#define ENC_MACON3_PADCFG2 0x80
#define ENC_MACON3_PADCFG1 0x40
#define ENC_MACON3_PADCFG0 0x20
#define ENC_MACON3_TXCRCEN 0x10
#define ENC_MACON3_PHDRLEN 0x08
#define ENC_MACON3_HFRMEN  0x04
#define ENC_MACON3_FRMLNEN 0x02
#define ENC_MACON3_FULDPX  0x01

//
// MICMD Register Bits
//
#define ENC_MICMD_MIISCAN 0x02
#define ENC_MICMD_MIIRD   0x01

//
// MISTAT Register Bits
//
#define ENC_MISTAT_NVALID 0x04
#define ENC_MISTAT_SCAN   0x02
#define ENC_MISTAT_BUSY   0x01


//
// PHID1 and PHID2 values
//
#define ENC_PHID1_VALUE 0x0083
#define ENC_PHID2_VALUE 0x1400
#define ENC_PHID2_MASK  0xFC00



#endif
