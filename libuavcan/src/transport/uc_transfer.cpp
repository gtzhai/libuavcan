/*
 * Copyright (C) 2014 Pavel Kirienko <pavel.kirienko@gmail.com>
 */

#include <uavcan/transport/transfer.hpp>
#include <uavcan/transport/frame.hpp>
#include <uavcan/transport/can_io.hpp>

namespace uavcan
{
/**
 * TransferPriority
 */
uint8_t TransferPriority::BitLen = 5U;
uint8_t TransferPriority::NumericallyMax = (1U << BitLen) - 1;
uint8_t TransferPriority::NumericallyMin = 0;

TransferPriority TransferPriority::Default((1U << BitLen) / 2);
TransferPriority TransferPriority::MiddleLower((1U << BitLen) / 2 + (1U << BitLen) / 4);
TransferPriority TransferPriority::OneHigherThanLowest(NumericallyMax - 1);
TransferPriority TransferPriority::OneLowerThanHighest(NumericallyMin + 1);
TransferPriority TransferPriority::Lowest(NumericallyMax);

/**
 * TransferID
 */
uint8_t TransferID::BitLen = 5U;
uint8_t TransferID::Max = (1U << BitLen) - 1U;
uint8_t TransferID::Half = (1U << BitLen) / 2U;


/**
 * NodeID
 */
const uint8_t NodeID::ValueBroadcast;
const uint8_t NodeID::ValueInvalid;
uint8_t NodeID::BitLen = 7U;
uint8_t NodeID::Max = (1U << BitLen) - 1U;
uint8_t NodeID::MaxRecommendedForRegularNodes = Max - 2;
NodeID NodeID::Broadcast(ValueBroadcast);

/**
 * TransferID
 */
int TransferID::computeForwardDistance(TransferID rhs) const
{
    int d = int(rhs.get()) - int(get());
    if (d < 0)
    {
        d += 1 << BitLen;
    }

    UAVCAN_ASSERT(((get() + d) & Max) == rhs.get());
    return d;
}

}
