/*****************************************************************************************************************
 * ReelRobotix Inc. - Software License Agreement      Copyright (c) 2018
 * 	 Authors: Pablo Inigo Blasco, Brett Aldrich
 *
 ******************************************************************************************************************/
#pragma once

#include <move_base_z_client_plugin/move_base_z_client_plugin.h>
#include <tf/transform_listener.h>

#include   <move_base_z_client_plugin/components/odom_tracker/odom_tracker.h>
#include   <move_base_z_client_plugin/components/planner_switcher/planner_switcher.h>

namespace cl_move_base_z
{
class CbNavigateBackwards : public smacc::SmaccClientBehavior
{
public:
    boost::optional<float> backwardDistance;

    // just a stub to show how to use parameterless constructor
    boost::optional<float> backwardSpeed;

    tf::TransformListener listener;

    cl_move_base_z::ClMoveBaseZ *moveBaseClient_;
    cl_move_base_z::odom_tracker::OdomTracker *odomTracker_;

    CbNavigateBackwards(float backwardDistance);

    CbNavigateBackwards();

    virtual void onEntry() override;

    virtual void onExit() override;
};
} // namespace cl_move_base_z
