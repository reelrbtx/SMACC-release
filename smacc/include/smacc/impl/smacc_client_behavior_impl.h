/*****************************************************************************************************************
 * ReelRobotix Inc. - Software License Agreement      Copyright (c) 2018
 * 	 Authors: Pablo Inigo Blasco, Brett Aldrich
 *
 ******************************************************************************************************************/
#pragma once

#include <smacc/smacc_client_behavior.h>
#include <smacc/smacc_state_machine.h>

namespace smacc
{

template <typename EventType>
void SmaccClientBehavior::postEvent(const EventType &ev)
{
    if (stateMachine_ == nullptr)
    {
        ROS_ERROR("The client behavior cannot post events before being assigned to an orthogonal. Ignoring post event call.");
    }
    else
    {
        stateMachine_->postEvent(ev);
    }
}

template <typename EventType>
void SmaccClientBehavior::postEvent()
{
    if (stateMachine_ == nullptr)
    {
        ROS_ERROR("The client behavior cannot post events before being assigned to an orthogonal. Ignoring post event call.");
    }
    else
    {
        stateMachine_->template postEvent<EventType>();
    }
}

//inline
ISmaccStateMachine *SmaccClientBehavior::getStateMachine()
{
    return this->stateMachine_;
}

//inline
ISmaccState *SmaccClientBehavior::getCurrentState()
{
    return this->currentState;
}

template <typename SmaccClientType>
void SmaccClientBehavior::requiresClient(SmaccClientType *&storage)
{
    currentOrthogonal->requiresClient(storage);
}

template <typename SmaccComponentType>
void SmaccClientBehavior::requiresComponent(SmaccComponentType *&storage)
{
    if (stateMachine_ == nullptr)
    {
        ROS_ERROR("Cannot use the requiresComponent funcionality before asigning the client behavior to an orthogonal. Try using the OnEntry method to capture required components.");
    }
    else
    {
        stateMachine_->requiresComponent(storage);
    }
}

template <typename TObjectTag, typename TDerived>
void SmaccClientBehavior::configureEventSourceTypes() {}

} // namespace smacc