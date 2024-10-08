/*
 *  Copyright (c) 2007,2008, 2009 INRIA, UDcast
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Author: Mohamed Amine Ismail <amine.ismail@sophia.inria.fr>
 *                              <amine.ismail@udcast.com>
 */

#include "simple-ofdm-send-param.h"

#include "simple-ofdm-wimax-channel.h"
#include "simple-ofdm-wimax-phy.h"

namespace ns3
{
SimpleOfdmSendParam::SimpleOfdmSendParam()
{
    // m_fecBlock = 0;
    m_burstSize = 0;
    m_isFirstBlock = false;
    m_frequency = 0;
    m_modulationType = WimaxPhy::MODULATION_TYPE_QPSK_12;
    m_direction = 0;
    m_rxPowerDbm = 0;
}

SimpleOfdmSendParam::SimpleOfdmSendParam(const Bvec& fecBlock,
                                         uint32_t burstSize,
                                         bool isFirstBlock,
                                         uint64_t Frequency,
                                         WimaxPhy::ModulationType modulationType,
                                         uint8_t direction,
                                         double rxPowerDbm)
{
    m_fecBlock = fecBlock;
    m_burstSize = burstSize;
    m_isFirstBlock = isFirstBlock;
    m_frequency = Frequency;
    m_modulationType = modulationType;
    m_direction = direction;
    m_rxPowerDbm = rxPowerDbm;
}

SimpleOfdmSendParam::SimpleOfdmSendParam(uint32_t burstSize,
                                         bool isFirstBlock,
                                         uint64_t Frequency,
                                         WimaxPhy::ModulationType modulationType,
                                         uint8_t direction,
                                         double rxPowerDbm,
                                         Ptr<PacketBurst> burst)
{
    m_burstSize = burstSize;
    m_isFirstBlock = isFirstBlock;
    m_frequency = Frequency;
    m_modulationType = modulationType;
    m_direction = direction;
    m_rxPowerDbm = rxPowerDbm;
    m_burst = burst;
}

SimpleOfdmSendParam::~SimpleOfdmSendParam()
{
}

void
SimpleOfdmSendParam::SetFecBlock(const Bvec& fecBlock)
{
    m_fecBlock = fecBlock;
}

void
SimpleOfdmSendParam::SetBurstSize(uint32_t burstSize)
{
    m_burstSize = burstSize;
}

void
SimpleOfdmSendParam::SetIsFirstBlock(bool isFirstBlock)
{
    m_isFirstBlock = isFirstBlock;
}

void
SimpleOfdmSendParam::SetFrequency(uint64_t Frequency)
{
    m_frequency = Frequency;
}

void
SimpleOfdmSendParam::SetModulationType(WimaxPhy::ModulationType modulationType)
{
    m_modulationType = modulationType;
}

void
SimpleOfdmSendParam::SetDirection(uint8_t direction)
{
    m_direction = direction;
}

void
SimpleOfdmSendParam::SetRxPowerDbm(double rxPowerDbm)
{
    m_rxPowerDbm = rxPowerDbm;
}

Bvec
SimpleOfdmSendParam::GetFecBlock()
{
    return m_fecBlock;
}

uint32_t
SimpleOfdmSendParam::GetBurstSize() const
{
    return m_burstSize;
}

bool
SimpleOfdmSendParam::GetIsFirstBlock() const
{
    return m_isFirstBlock;
}

uint64_t
SimpleOfdmSendParam::GetFrequency() const
{
    return m_frequency;
}

WimaxPhy::ModulationType
SimpleOfdmSendParam::GetModulationType()
{
    return m_modulationType;
}

uint8_t
SimpleOfdmSendParam::GetDirection() const
{
    return m_direction;
}

double
SimpleOfdmSendParam::GetRxPowerDbm() const
{
    return m_rxPowerDbm;
}

Ptr<PacketBurst>
SimpleOfdmSendParam::GetBurst()
{
    return m_burst;
}

} // namespace ns3
