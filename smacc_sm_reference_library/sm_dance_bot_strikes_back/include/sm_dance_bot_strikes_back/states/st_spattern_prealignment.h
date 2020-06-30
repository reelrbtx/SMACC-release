#include <smacc/smacc.h>
namespace sm_dance_bot_strikes_back
{
// STATE DECLARATION
struct StSpatternPrealignment : smacc::SmaccState<StSpatternPrealignment, MsDanceBotRunMode>
{
  using SmaccState::SmaccState;

// TRANSITION TABLE
  typedef mpl::list<
    
  Transition<EvActionSucceeded<ClMoveBaseZ, OrNavigation>, SS5::SsSPattern1>,
  Transition<EvActionAborted<ClMoveBaseZ, OrNavigation>, StNavigateToWaypointsX>
  
  >reactions;

// STATE FUNCTIONS
  static void staticConfigure()
  {
    configure_orthogonal<OrNavigation, CbAbsoluteRotate>(60.5);
    configure_orthogonal<OrLED, CbLEDOff>();
  }
  
  void runtimeConfigure()
  { 
  }
};
}