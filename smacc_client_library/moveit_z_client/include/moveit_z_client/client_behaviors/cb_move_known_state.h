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
#include <moveit_z_client/client_behaviors/cb_move_joints.h>

namespace moveit_z_client
{
class CbMoveKnownState : public CbMoveJoints
{
public:
  CbMoveKnownState(std::string pkg, std::string config_path);

  private:
  static std::map<std::string, double> loadJointStatesFromFile(std::string pkg, std::string filepath);
};
}  // namespace moveit_z_client
