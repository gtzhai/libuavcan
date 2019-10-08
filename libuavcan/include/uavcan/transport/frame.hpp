/*
 * Copyright (C) 2014 Pavel Kirienko <pavel.kirienko@gmail.com>
 */

#ifndef UAVCAN_TRANSPORT_FRAME_HPP_INCLUDED
#define UAVCAN_TRANSPORT_FRAME_HPP_INCLUDED

#include <cassert>
#include <uavcan/transport/transfer.hpp>
#include <uavcan/transport/can_io.hpp>
#include <uavcan/build_config.hpp>
#include <uavcan/data_type.hpp>

namespace uavcan
{

class UAVCAN_EXPORT Frame
{
public:
    enum { PayloadCapacity = 8 };       // Will be redefined when CAN FD is available

    uint8_t payload_[PayloadCapacity];
    TransferPriority transfer_priority_;
    TransferType transfer_type_;
    DataTypeID data_type_id_;
    uint_fast8_t payload_len_;
    NodeID src_node_id_;
    NodeID dst_node_id_;
    TransferID transfer_id_;
    bool start_of_transfer_;
    bool end_of_transfer_;
    bool toggle_;
    TransferID transfer_id_base_;
    uint8_t transfer_id_auto_inc_;
    uint8_t type_;// 0: is uavcan
    uint8_t crc_len_;

    enum { FRAME_TYPE_UAVCAN = 0 };
    Frame() :
        transfer_type_(TransferType(NumTransferTypes)),                // Invalid value
        payload_len_(0),
        start_of_transfer_(false),
        end_of_transfer_(false),
        toggle_(false),
        transfer_id_auto_inc_(0),
        type_(0),
        crc_len_(16)
    { }

    Frame(uint8_t type) :
        transfer_type_(TransferType(NumTransferTypes)),                // Invalid value
        payload_len_(0),
        start_of_transfer_(false),
        end_of_transfer_(false),
        toggle_(false),
        transfer_id_auto_inc_(0),
        type_(type),
        crc_len_(16)
    { }

    Frame(DataTypeID data_type_id,
          TransferType transfer_type,
          NodeID src_node_id,
          NodeID dst_node_id,
          TransferID transfer_id) :
        transfer_priority_(TransferPriority::Default),
        transfer_type_(transfer_type),
        data_type_id_(data_type_id),
        payload_len_(0),
        src_node_id_(src_node_id),
        dst_node_id_(dst_node_id),
        transfer_id_(transfer_id),
        start_of_transfer_(false),
        end_of_transfer_(false),
        toggle_(false),
        transfer_id_auto_inc_(0),
        type_(0),
        crc_len_(16)
    {
        UAVCAN_ASSERT((transfer_type == TransferTypeMessageBroadcast) == dst_node_id.isBroadcast());
        UAVCAN_ASSERT(data_type_id.isValidForDataTypeKind(getDataTypeKindForTransferType(transfer_type)));
        UAVCAN_ASSERT(src_node_id.isUnicast() ? (src_node_id != dst_node_id) : true);
    }

    void setCrcLen(uint8_t len) { crc_len_ = len; }
    uint8_t getCrcLen() const { return crc_len_; }
    int getFrameType() const { return type_; }
    void setTransferIdAutoInc(uint8_t x, TransferID base) 
    { 
            transfer_id_auto_inc_= x; 
            if(x)
            {
                transfer_id_base_ = base;
            }
    }
    bool isTransferIdAutoInc()   { if(transfer_id_auto_inc_>0) return true; else return false;}
    TransferID getBaseAutoTransferID()     const { return transfer_id_base_; }

    void setPriority(TransferPriority priority) { transfer_priority_ = priority; }
    TransferPriority getPriority() const { return transfer_priority_; }

    /**
     * Max payload length depends on the transfer type and frame index.
     */
    uint8_t getPayloadCapacity() const { return PayloadCapacity; }
    uint8_t setPayload(const uint8_t* data, unsigned len);

    unsigned getPayloadLen() const { return payload_len_; }
    const uint8_t* getPayloadPtr() const { return payload_; }

    TransferType getTransferType() const { return transfer_type_; }
    void setTransferType(TransferType type) { transfer_type_ = type; }
    DataTypeID getDataTypeID()     const { return data_type_id_; }
    void setDataTypeID(DataTypeID id)     { data_type_id_ = id; }
    NodeID getSrcNodeID()          const { return src_node_id_; }
    void setSrcNodeID(NodeID id)         { src_node_id_ = id; }
    NodeID getDstNodeID()          const { return dst_node_id_; }
    void setDstNodeID(NodeID id)   { dst_node_id_ = id; }
    TransferID getTransferID()     const { return transfer_id_; }
    void setTransferID(TransferID id)    { transfer_id_ = id; }

    void setStartOfTransfer(bool x) { start_of_transfer_ = x; }
    void setEndOfTransfer(bool x)   { end_of_transfer_ = x; }

    bool isStartOfTransfer() const { return start_of_transfer_; }
    bool isEndOfTransfer()   const { return end_of_transfer_; }



    void flipToggle() { toggle_ = !toggle_; }
    bool getToggle() const { return toggle_; }


    virtual bool parse(const CanFrame& can_frame);
    virtual bool compile(CanFrame& can_frame) const;
    virtual bool isValid() const;

    bool operator!=(const Frame& rhs) const { return !operator==(rhs); }
    bool operator==(const Frame& rhs) const;

#if UAVCAN_TOSTRING
    std::string toString() const;
#endif
};


class UAVCAN_EXPORT RxFrame : public Frame
{
public:
    MonotonicTime ts_mono_;
    UtcTime ts_utc_;
    uint8_t iface_index_;

    RxFrame()
        : iface_index_(0)
    { }

    RxFrame(uint8_t type)
        : Frame(type),
        iface_index_(0)
    { }

    RxFrame(const Frame& frame, MonotonicTime ts_mono, UtcTime ts_utc, uint8_t iface_index)
        : ts_mono_(ts_mono)
        , ts_utc_(ts_utc)
        , iface_index_(iface_index)
    {
        *static_cast<Frame*>(this) = frame;
    }

    virtual bool parse(const CanRxFrame& can_frame);

    /**
     * Can't be zero.
     */
    MonotonicTime getMonotonicTimestamp() const { return ts_mono_; }

    /**
     * Can be zero if not supported by the platform driver.
     */
    UtcTime getUtcTimestamp() const { return ts_utc_; }

    uint8_t getIfaceIndex() const { return iface_index_; }

#if UAVCAN_TOSTRING
    std::string toString() const;
#endif
};

}

#endif // UAVCAN_TRANSPORT_FRAME_HPP_INCLUDED
