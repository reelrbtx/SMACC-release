/*****************************************************************************************************************
 * ReelRobotix Inc. - Software License Agreement      Copyright (c) 2018-2020
 * 	 Authors: Pablo Inigo Blasco, Brett Aldrich
 *
 ******************************************************************************************************************/

#pragma once

#include <moveit_z_client/cl_movegroup.h>
#include <smacc/smacc_client_behavior.h>
#include <map>
#include <string>

namespace moveit_z_client
{
  //named targets are configured in the urdf file
class CbMoveNamedTarget : public smacc::SmaccClientBehavior
{
protected:
  ClMoveGroup *movegroupClient_;
  std::string namedTarget_;

public:
  CbMoveNamedTarget(std::string namedtarget);

  virtual void onEntry() override;

  virtual void onExit() override;

  std::map<std::string, double> getNamedTargetValues();
};
}  // namespace moveit_z_client
