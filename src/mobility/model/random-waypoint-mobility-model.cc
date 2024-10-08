/*
 * Copyright (c) 2007 INRIA
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#include "random-waypoint-mobility-model.h"

#include "position-allocator.h"

#include "ns3/pointer.h"
#include "ns3/random-variable-stream.h"
#include "ns3/simulator.h"
#include "ns3/string.h"

#include <cmath>

namespace ns3
{

NS_OBJECT_ENSURE_REGISTERED(RandomWaypointMobilityModel);

TypeId
RandomWaypointMobilityModel::GetTypeId()
{
    static TypeId tid =
        TypeId("ns3::RandomWaypointMobilityModel")
            .SetParent<MobilityModel>()
            .SetGroupName("Mobility")
            .AddConstructor<RandomWaypointMobilityModel>()
            .AddAttribute("Speed",
                          "A random variable used to pick the speed of a random waypoint model.",
                          StringValue("ns3::UniformRandomVariable[Min=0.3|Max=0.7]"),
                          MakePointerAccessor(&RandomWaypointMobilityModel::m_speed),
                          MakePointerChecker<RandomVariableStream>())
            .AddAttribute("Pause",
                          "A random variable used to pick the pause of a random waypoint model.",
                          StringValue("ns3::ConstantRandomVariable[Constant=2.0]"),
                          MakePointerAccessor(&RandomWaypointMobilityModel::m_pause),
                          MakePointerChecker<RandomVariableStream>())
            .AddAttribute("PositionAllocator",
                          "The position model used to pick a destination point.",
                          PointerValue(),
                          MakePointerAccessor(&RandomWaypointMobilityModel::m_position),
                          MakePointerChecker<PositionAllocator>());

    return tid;
}

RandomWaypointMobilityModel::~RandomWaypointMobilityModel()
{
    m_event.Cancel();
}

void
RandomWaypointMobilityModel::BeginWalk()
{
    m_helper.Update();
    Vector m_current = m_helper.GetCurrentPosition();
    NS_ASSERT_MSG(m_position, "No position allocator added before using this model");
    Vector destination = m_position->GetNext();
    Vector delta = destination - m_current;
    double distance = delta.GetLength();
    double speed = m_speed->GetValue();

    NS_ASSERT_MSG(speed > 0, "Speed must be strictly positive.");

    // Note: the following two lines are needed to prevent corner cases where
    // the distance is null (and the Velocity is undefined).
    double k = distance ? speed / distance : 0;
    Time travelDelay = distance ? Seconds(distance / speed) : Time(0);

    m_helper.SetVelocity(k * delta);
    m_helper.Unpause();
    m_event.Cancel();
    m_event =
        Simulator::Schedule(travelDelay, &RandomWaypointMobilityModel::DoInitializePrivate, this);
    NotifyCourseChange();
}

void
RandomWaypointMobilityModel::DoInitialize()
{
    DoInitializePrivate();
    MobilityModel::DoInitialize();
}

void
RandomWaypointMobilityModel::DoInitializePrivate()
{
    m_helper.Update();
    m_helper.Pause();
    Time pause = Seconds(m_pause->GetValue());
    m_event = Simulator::Schedule(pause, &RandomWaypointMobilityModel::BeginWalk, this);
    NotifyCourseChange();
}

Vector
RandomWaypointMobilityModel::DoGetPosition() const
{
    m_helper.Update();
    return m_helper.GetCurrentPosition();
}

void
RandomWaypointMobilityModel::DoSetPosition(const Vector& position)
{
    m_helper.SetPosition(position);
    m_event.Cancel();
    m_event = Simulator::ScheduleNow(&RandomWaypointMobilityModel::DoInitializePrivate, this);
}

Vector
RandomWaypointMobilityModel::DoGetVelocity() const
{
    return m_helper.GetVelocity();
}

int64_t
RandomWaypointMobilityModel::DoAssignStreams(int64_t stream)
{
    int64_t positionStreamsAllocated;
    m_speed->SetStream(stream);
    m_pause->SetStream(stream + 1);
    NS_ASSERT_MSG(m_position, "No position allocator added before using this model");
    positionStreamsAllocated = m_position->AssignStreams(stream + 2);
    return (2 + positionStreamsAllocated);
}

} // namespace ns3
