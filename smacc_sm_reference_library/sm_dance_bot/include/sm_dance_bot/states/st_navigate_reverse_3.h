#include <smacc/smacc.h>
namespace sm_dance_bot
{
// STATE DECLARATION
struct StNavigateReverse3 : smacc::SmaccState<StNavigateReverse3, MsDanceBotRunMode>
{
   using SmaccState::SmaccState;

// TRANSITION TABLE
   typedef mpl::list<
   
   Transition<EvActionSucceeded<ClMoveBaseZ, OrNavigation>, StNavigateToWaypoint1>,
   Transition<EvActionAborted<ClMoveBaseZ, OrNavigation>, StNavigateToWaypointsX>
   
   >reactions;

// STATE FUNCTIONS
   static void staticConfigure()
   {
      configure_orthogonal<OrNavigation, CbNavigateBackwards>(2);
      configure_orthogonal<OrLED, CbLEDOff>();
      configure_orthogonal<OrObstaclePerception, CbLidarSensor>();
   }
};
}