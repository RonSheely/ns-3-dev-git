/*
 * Copyright (c) 2009 University of Washington
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Author: Leonard Tracy <lentracy@gmail.com>
 */

#include "uan-mac-aloha.h"

#include "uan-header-common.h"
#include "uan-phy.h"
#include "uan-tx-mode.h"

#include "ns3/log.h"

#include <iostream>

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("UanMacAloha");

NS_OBJECT_ENSURE_REGISTERED(UanMacAloha);

UanMacAloha::UanMacAloha()
    : UanMac(),
      m_cleared(false)
{
}

UanMacAloha::~UanMacAloha()
{
}

void
UanMacAloha::Clear()
{
    if (m_cleared)
    {
        return;
    }
    m_cleared = true;
    if (m_phy)
    {
        m_phy->Clear();
        m_phy = nullptr;
    }
}

void
UanMacAloha::DoDispose()
{
    Clear();
    UanMac::DoDispose();
}

TypeId
UanMacAloha::GetTypeId()
{
    static TypeId tid = TypeId("ns3::UanMacAloha")
                            .SetParent<UanMac>()
                            .SetGroupName("Uan")
                            .AddConstructor<UanMacAloha>();
    return tid;
}

bool
UanMacAloha::Enqueue(Ptr<Packet> packet, uint16_t protocolNumber, const Address& dest)
{
    NS_LOG_DEBUG("" << Now().As(Time::S) << " MAC " << Mac8Address::ConvertFrom(GetAddress())
                    << " Queueing packet for " << Mac8Address::ConvertFrom(dest));

    if (!m_phy->IsStateTx())
    {
        Mac8Address src = Mac8Address::ConvertFrom(GetAddress());
        Mac8Address udest = Mac8Address::ConvertFrom(dest);

        UanHeaderCommon header;
        header.SetSrc(src);
        header.SetDest(udest);
        header.SetType(0);
        header.SetProtocolNumber(protocolNumber);

        packet->AddHeader(header);
        m_phy->SendPacket(packet, GetTxModeIndex());
        return true;
    }
    else
    {
        return false;
    }
}

void
UanMacAloha::SetForwardUpCb(Callback<void, Ptr<Packet>, uint16_t, const Mac8Address&> cb)
{
    m_forUpCb = cb;
}

void
UanMacAloha::AttachPhy(Ptr<UanPhy> phy)
{
    m_phy = phy;
    m_phy->SetReceiveOkCallback(MakeCallback(&UanMacAloha::RxPacketGood, this));
    m_phy->SetReceiveErrorCallback(MakeCallback(&UanMacAloha::RxPacketError, this));
}

void
UanMacAloha::RxPacketGood(Ptr<Packet> pkt, double /* sinr */, UanTxMode /* txMode */)
{
    UanHeaderCommon header;
    pkt->RemoveHeader(header);
    NS_LOG_DEBUG("Receiving packet from " << header.GetSrc() << " For " << header.GetDest());

    if (header.GetDest() == GetAddress() || header.GetDest() == Mac8Address::GetBroadcast())
    {
        m_forUpCb(pkt, header.GetProtocolNumber(), header.GetSrc());
    }
}

void
UanMacAloha::RxPacketError(Ptr<Packet> pkt, double sinr)
{
    NS_LOG_DEBUG("" << Simulator::Now() << " MAC " << Mac8Address::ConvertFrom(GetAddress())
                    << " Received packet in error with sinr " << sinr);
}

int64_t
UanMacAloha::AssignStreams(int64_t stream)
{
    NS_LOG_FUNCTION(this << stream);
    return 0;
}

} // namespace ns3
