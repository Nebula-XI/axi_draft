#pragma once

namespace xilinx_iic
{

    enum iic_registers
    {
        GIE = 0x1C,
        ISR = 0x20,
        IER = 0x28,
        SOFTR = 0x40,
        CR = 0x100,
        SR = 0x104,
        TX_FIFO = 0x108,
        RX_FIFO = 0x10C,
        ADR = 0x110,
        TEN_ADR = 0x11C,
        RX_FIFO_PIRQ = 0x120,
        GPO = 0x124,
        TSUSTA = 0x128,
        TSUSTO = 0x12C,
        THDSTA = 0x130,
        TSUSAT = 0x134,
        TBUF = 0x138,
        THIGH = 0x13C,
        TLOW = 0x140,
        THDDAT = 0x144,
    };

    enum iic_irq_register
    {
        IRQ_ARB_Lost = 0x1,
        IRQ_TX_ERR = 0x2,
        IRQ_TX_FIFO_Empty_irq = 0x4,
        IRQ_RX_FIFO_Full_Empty = 0x8,
        IRQ_Bus_Not_Busy = 0x10,
        IRQ_AAS_ieq = 0x20,
        IRQ_NASS = 0x40,
        IRQ_TX_FIFO_HALF_Empty = 0x80,
    };

    enum iic_status_register
    {
        STAT_ABGC = 0x1,
        STAT_AAS = 0x2,
        STAT_BB = 0x4,
        STAT_SRW = 0x8,
        STAT_TX_FIFO_Full = 0x10,
        STAT_RX_FIFO_Full = 0x20,
        STAT_RX_FIFO_Empty = 0x40,
        STAT_TX_FIFO_Empty = 0x80,
    };

    enum iic_irqstat_register
    {
        ISR_ARB_LOST = 0x1,
        ISR_TX_ERROR = 0x2,
        ISR_TX_FIFO_EMPTY = 0x4,
        ISR_RX_FIFO_FULL = 0x8,
        ISR_BUS_NOT_BUSY = 0x10,
        ISR_ADDR_AS_SLAVE = 0x20,
        ISR_NOT_ADDR_AS_SLAVE = 0x40,
        ISR_TX_FIFO_HALF_EMPTY = 0x80,
    };

    enum iic_control_register
    {
        CR_EN = 0x1,
        CR_TX_FIFO_Reset = 0x2,
        CR_MSMS = 0x4,
        CR_TX = 0x8,
        CR_TXAK = 0x10,
        CR_RSTA = 0x20,
        CR_GC_EN = 0x40,
    };

    enum iic_fifo_bits
    {
        IIC_START = 0x100,
        IIC_STOP = 0x200,
        IIC_WRITE = 0x0,
        IIC_READ = 0x1,
    };
}