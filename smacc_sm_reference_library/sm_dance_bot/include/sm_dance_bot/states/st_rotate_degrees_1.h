#include <smacc/smacc.h>
namespace sm_dance_bot
{
// STATE DECLARATION
struct StRotateDegrees1 : smacc::SmaccState<StRotateDegrees1, MsDanceBotRunMode>
{
  using SmaccState::SmaccState;

// TRANSITION TABLE
  typedef mpl::list<
  
  Transition<EvActionSucceeded<ClMoveBaseZ, OrNavigation>, StNavigateForward1>,
  Transition<EvActionAborted<ClMoveBaseZ, OrNavigation>, StNavigateToWaypointsX>
  
  >reactions;

// STATE FUNCTIONS
  static void staticConfigure()
  {
    configure_orthogonal<OrNavigation, CbRotate>(/*30*/ 90);
    configure_orthogonal<OrLED, CbLEDOff>();
    configure_orthogonal<OrObstaclePerception, CbLidarSensor>();
  }
};
}