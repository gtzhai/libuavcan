/*
 * Copyright (C) 2014 Pavel Kirienko <pavel.kirienko@gmail.com>
 */

#ifndef UAVCAN_TRANSPORT_CRC_HPP_INCLUDED
#define UAVCAN_TRANSPORT_CRC_HPP_INCLUDED

#include <cassert>
#include <uavcan/std.hpp>
#include <uavcan/build_config.hpp>

namespace uavcan
{

/**
 * CRC-16-CCITT
 * Initial value: 0xFFFF
 * Poly: 0x1021
 * Reverse: no
 * Output xor: 0
 *
 * import crcmod
 * crc = crcmod.predefined.Crc('crc-ccitt-false')
 * crc.update('123456789')
 * crc.hexdigest()
 * '29B1'
 */
class UAVCAN_EXPORT TransferCRC
{
#if !UAVCAN_TINY
    static const uint16_t Table[256];
#endif

    uint16_t value_;

public:
    enum { NumBytes = 2 };

    TransferCRC()
        : value_(0xFFFFU)
    { }

#if UAVCAN_TINY
    void add(uint8_t byte)
    {
        value_ ^= uint16_t(uint16_t(byte) << 8);
        for (uint8_t bit = 8; bit > 0; --bit)
        {
            if (value_ & 0x8000U)
            {
                value_ = uint16_t(uint16_t(value_ << 1) ^ 0x1021U);
            }
            else
            {
                value_ = uint16_t(value_ << 1);
            }
        }
    }
#else
    void add(uint8_t byte)
    {
        value_ = uint16_t(uint16_t((value_ << 8) ^ Table[uint16_t((value_ >> 8) ^ byte) & 0xFFU]) & 0xFFFFU);
    }
#endif

    void add(const uint8_t* bytes, unsigned len)
    {
        UAVCAN_ASSERT(bytes);
        while (len--)
        {
            add(*bytes++);
        }
    }

    uint16_t get() const { return value_; }
};

class UAVCAN_EXPORT TransferCRC32
{
    static const uint32_t Table[256];

    uint32_t value_;

public:
    enum { NumBytes = 4 };

    TransferCRC32()
        : value_(0xFFFFFFFFU)
    { }

    void add(uint8_t byte)
    {
        value_ = uint32_t(uint32_t((value_ << 8) ^ Table[uint32_t((value_ >> 8) ^ byte) & 0xFFU]) & 0xFFFFFFFFU);
    }

    void add(const uint8_t* bytes, unsigned len)
    {
        UAVCAN_ASSERT(bytes);
        while (len--)
        {
            add(*bytes++);
        }
    }

    uint32_t get() const { return value_; }
};

class UAVCAN_EXPORT TransferCRC48
{
    uint64_t value_;

public:
    enum { NumBytes = 6 };

    TransferCRC48()
        : value_(0x000000000000UL)
    { }

    void add(uint8_t byte)
    {
        value_ ^= uint64_t(byte);
        for (uint8_t bits = 0; bits < 8; bits++)
        {
            int bit = value_&0x1;
            value_ >>= 1;
            if (bit)
            {
          	    value_ ^= 0xeadb71093528L;
            }
        }
    }

    void add(const uint8_t* bytes, unsigned len)
    {
        UAVCAN_ASSERT(bytes);
        while (len--)
        {
            add(*bytes++);
        }
        value_ ^= 0x130edf575accL;
    }

    uint64_t get() const { return value_; }
};
}
#endif // UAVCAN_TRANSPORT_CRC_HPP_INCLUDED

