#include <moveit_z_client/client_behaviors/cb_move_named_target.h>

namespace moveit_z_client
{
CbMoveNamedTarget::CbMoveNamedTarget(std::string namedtarget)
: namedTarget_(namedtarget)
{
}

void CbMoveNamedTarget::onEntry()
{
    this->requiresClient(movegroupClient_);
    movegroupClient_->moveGroupClientInterface.setNamedTarget(this->namedTarget_);
}

void CbMoveNamedTarget::onExit()
{
}

std::map<std::string, double> CbMoveNamedTarget::getNamedTargetValues()
{
    return movegroupClient_->moveGroupClientInterface.getNamedTargetValues(this->namedTarget_);
}
}  // namespace moveit_z_client
