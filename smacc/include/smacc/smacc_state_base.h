/*****************************************************************************************************************
 * ReelRobotix Inc. - Software License Agreement      Copyright (c) 2018
 * 	 Authors: Pablo Inigo Blasco, Brett Aldrich
 *
 ******************************************************************************************************************/

#pragma once
#include <smacc/smacc_state.h>
#include <smacc/smacc_state_reactor.h>

namespace smacc
{
using namespace smacc::introspection;
using namespace smacc::default_events;

template <class MostDerived,
          class Context,
          class InnerInitial = mpl::list<>,
          sc::history_mode historyMode = sc::has_deep_history>
class SmaccState : public sc::simple_state<
                       MostDerived, Context, InnerInitial, historyMode>,
                   public ISmaccState
{
  typedef sc::simple_state<MostDerived, Context, InnerInitial, historyMode>
      base_type;

public:
  typedef Context TContext;
  typedef typename Context::inner_context_type context_type;
  typedef typename context_type::state_iterator state_iterator;

  typedef InnerInitial LastDeepState;

  bool finishStateThrown;
  InnerInitial *smacc_inner_type;

  //////////////////////////////////////////////////////////////////////////
  struct my_context
  {
    my_context(typename base_type::context_ptr_type pContext) : pContext_(pContext)
    {
    }

    typename base_type::context_ptr_type pContext_;
  };

  SmaccState() = delete;

  #define STATE_NAME (demangleSymbol(typeid(MostDerived).name()).c_str())
  // Constructor that initializes the state ros node handle
  SmaccState(my_context ctx)
  {
    static_assert(std::is_base_of<ISmaccState, Context>::value || std::is_base_of<ISmaccStateMachine, Context>::value, "The context class must be a SmaccState or a SmaccStateMachine");

    static_assert(!std::is_same<MostDerived, Context>::value, "The context must be a different state or state machine than the current state");

    ROS_WARN("[%s] creating ", STATE_NAME);
    this->set_context(ctx.pContext_);

    this->stateInfo_ = getStateInfo();

    // storing a reference to the parent state
    auto &ps = this->template context<Context>();
    parentState_ = dynamic_cast<ISmaccState *>(&ps);
    finishStateThrown = false;

    this->contextNh = optionalNodeHandle(ctx.pContext_);
    ROS_DEBUG("[%s] Ros node handle namespace for this state: %s", STATE_NAME, contextNh.getNamespace().c_str());
    if (contextNh.getNamespace() == "/")
    {
      auto nhname = smacc::utils::cleanShortTypeName(typeid(Context));
      ROS_INFO("[%s] Creating ros NodeHandle for this state: %s", STATE_NAME, nhname.c_str());
      contextNh = ros::NodeHandle(nhname);
    }
  }

  virtual ~SmaccState()
  {
  }

  const smacc::introspection::SmaccStateInfo *getStateInfo()
  {
    auto smInfo = this->getStateMachine().getStateMachineInfo();

    auto key = typeid(MostDerived).name();
    if (smInfo.states.count(key))
    {
      return smInfo.states[key].get();
    }
    else
    {
      return nullptr;
    }
  }

  std::string getFullName()
  {
    return demangleSymbol(typeid(MostDerived).name());
  }

  std::string getShortName()
  {
    return smacc::utils::cleanShortTypeName(typeid(MostDerived));
  }

  virtual ISmaccState *getParentState()
  {
    //auto* ctx = dynamic_cast<ISmaccState*>(this->template context<Context *>());

    return parentState_;
  }

  // this function is called by boot statechart before the destructor call
  void exit()
  {
    try
    {
      this->requestLockStateMachine("state exit");
      auto fullname = demangleSymbol(typeid(MostDerived).name());
      ROS_WARN_STREAM("exiting state: " << fullname);
      //this->setParam("destroyed", true);

      // first process orthogonals onexits
      this->getStateMachine().notifyOnStateExit(static_cast<MostDerived *>(this));

      // then call exit state
      ROS_WARN_STREAM("state exit: " << fullname);
      static_cast<MostDerived *>(this)->onExit();
    }
    catch (...)
    {
    }
    this->requestUnlockStateMachine("state exit");
  }

public:
  // This method is static-polymorphic because of the curiously recurring template pattern. It
  // calls to the most derived class onEntry method if declared on smacc state construction
  void runtimeConfigure()
  {
  }

  // This method is static-polymorphic because of the curiously recurring template pattern. It
  // calls to the most derived class onEntry method if declared on smacc state construction
  void onEntry()
  {
  }

  // this method is static-polimorphic because of the curiously recurreing pattern. It
  // calls to the most derived class onExit method if declared on smacc state destruction
  void onExit()
  {
  }

  template <typename T>
  bool getGlobalSMData(std::string name, T &ret)
  {
    return base_type::outermost_context().getGlobalSMData(name, ret);
  }

  // Store globally in this state machine. (By value parameter )
  template <typename T>
  void setGlobalSMData(std::string name, T value)
  {
    base_type::outermost_context().setGlobalSMData(name, value);
  }

  template <typename SmaccComponentType>
  void requiresComponent(SmaccComponentType *&storage)
  {
    base_type::outermost_context().requiresComponent(storage);
  }

  virtual ISmaccStateMachine &getStateMachine()
  {
    return base_type::outermost_context();
  }

  template <typename TOrthogonal, typename TBehavior, typename... Args>
  static void configure_orthogonal(Args &&... args)
  {

    ROS_INFO("[%s] Runtime configure orthogonal %s -> %s", STATE_NAME, demangleSymbol(typeid(TOrthogonal).name()).c_str(), demangleSymbol(typeid(TBehavior).name()).c_str());

    ClientBehaviorInfoEntry bhinfo;
    bhinfo.factoryFunction = [=](ISmaccState *state) {
      //auto bh = std::make_shared<TBehavior>(args...);
      state->configure<TOrthogonal, TBehavior>(args...);
    };

    bhinfo.behaviorType = &(typeid(TBehavior));
    bhinfo.orthogonalType = &(typeid(TOrthogonal));

    const std::type_info *tindex = &(typeid(MostDerived));
    if (!SmaccStateInfo::staticBehaviorInfo.count(tindex))
      SmaccStateInfo::staticBehaviorInfo[tindex] = std::vector<ClientBehaviorInfoEntry>();

    SmaccStateInfo::staticBehaviorInfo[tindex].push_back(bhinfo);
  }

  template <typename TStateReactor, typename... TUArgs>
  static std::shared_ptr<smacc::introspection::StateReactorHandler> static_createStateReactor(TUArgs... args)
  {
    auto srh = std::make_shared<smacc::introspection::StateReactorHandler>();
    auto srinfo = std::make_shared<SmaccStateReactorInfo>();

    srinfo->stateReactorType = &typeid(TStateReactor);
    srinfo->srh = srh;
    srh->srInfo_ = srinfo;

    const std::type_info *tindex = &(typeid(MostDerived)); // get identifier of the current state

    if (!SmaccStateInfo::stateReactorsInfo.count(tindex))
      SmaccStateInfo::stateReactorsInfo[tindex] = std::vector<std::shared_ptr<SmaccStateReactorInfo>>();

    srinfo->factoryFunction = [&, srh, args...](ISmaccState *state) {
      auto sr = state->createStateReactor<TStateReactor>(args...);
      srh->configureStateReactor(sr);
      sr->initialize(state);
      return sr;
    };

    SmaccStateInfo::stateReactorsInfo[tindex].push_back(srinfo);

    return srh;
  }

  void checkWhileLoopConditionAndThrowEvent(bool (MostDerived::*conditionFn)())
  {
    auto *thisobject = static_cast<MostDerived *>(this);
    auto condition = boost::bind(conditionFn, thisobject);
    bool conditionResult = condition();

    //ROS_INFO("LOOP EVENT CONDITION: %d", conditionResult);
    if (conditionResult)
    {
      this->postEvent<EvLoopContinue<MostDerived>>();
    }
    else
    {
      this->postEvent<EvLoopEnd<MostDerived>>();
    }
    ROS_INFO("[]%s POST THROW CONDITION", STATE_NAME);
  }

  void throwSequenceFinishedEvent()
  {
    this->postEvent<EvSequenceFinished<MostDerived>>();
  }

  //////////////////////////////////////////////////////////////////////////
  // The following declarations should be private.
  // They are only public because many compilers lack template friends.
  //////////////////////////////////////////////////////////////////////////
  // See base class for documentation
  typedef typename base_type::outermost_context_base_type
      outermost_context_base_type;
  typedef typename base_type::inner_context_ptr_type inner_context_ptr_type;
  typedef typename base_type::context_ptr_type context_ptr_type;
  typedef typename base_type::inner_initial_list inner_initial_list;

  static void initial_deep_construct(
      outermost_context_base_type &outermostContextBase)
  {
    deep_construct(&outermostContextBase, outermostContextBase);
  }

  // See base class for documentation
  static void deep_construct(
      const context_ptr_type &pContext,
      outermost_context_base_type &outermostContextBase)
  {
    const inner_context_ptr_type pInnerContext(
        shallow_construct(pContext, outermostContextBase));
    base_type::template deep_construct_inner<inner_initial_list>(
        pInnerContext, outermostContextBase);
  }

  static inner_context_ptr_type shallow_construct(
      const context_ptr_type &pContext,
      outermost_context_base_type &outermostContextBase)
  {    
    // allocating in memory
    auto state = new MostDerived(SmaccState<MostDerived, Context, InnerInitial, historyMode>::my_context(pContext));
    const inner_context_ptr_type pInnerContext(state);

    ROS_INFO("[%s] State object created. Initializating...", STATE_NAME);
    state->entryStateInternal();

    outermostContextBase.add(pInnerContext);
    return pInnerContext;
  }

private:
  void entryStateInternal()
  {
    this->getStateMachine().notifyOnStateEntryStart(static_cast<MostDerived *>(this));

    // TODO: make this static to build the parameter tree at startup
    this->nh = ros::NodeHandle(contextNh.getNamespace() + std::string("/") + smacc::utils::cleanShortTypeName(typeid(MostDerived)).c_str());

    ROS_DEBUG("[%s] nodehandle namespace: %s", STATE_NAME, nh.getNamespace().c_str());

    this->setParam("created", true);

    // before dynamic runtimeConfigure, we execute the staticConfigure behavior configurations
    {
      ROS_INFO("[%s] -- STATIC STATE DESCRIPTION --", STATE_NAME);

      for (const auto &stateReactorsVector : SmaccStateInfo::staticBehaviorInfo)
      {
        ROS_DEBUG("[%s] state info: %s", STATE_NAME,  demangleSymbol(stateReactorsVector.first->name()).c_str());
        for (auto &bhinfo : stateReactorsVector.second)
        {
          ROS_DEBUG("[%s] client behavior: %s", STATE_NAME,  demangleSymbol(bhinfo.behaviorType->name()).c_str());
        }
      }

      const std::type_info *tindex = &(typeid(MostDerived));
      auto &staticDefinedBehaviors = SmaccStateInfo::staticBehaviorInfo[tindex];
      auto &staticDefinedStateReactors = SmaccStateInfo::stateReactorsInfo[tindex];

      for (auto &bhinfo : staticDefinedBehaviors)
      {
        ROS_INFO("[%s] Creating static client behavior: %s", STATE_NAME, demangleSymbol(bhinfo.behaviorType->name()).c_str());
        bhinfo.factoryFunction(this);
      }

      for (auto &sr : staticDefinedStateReactors)
      {
        ROS_INFO("[%s] Creating static state reactor: %s", STATE_NAME,  demangleSymbol(sr->stateReactorType->name()).c_str());
        sr->factoryFunction(this);
      }

      ROS_INFO("[%s] ---- END STATIC DESCRIPTION", STATE_NAME);
    }

    ROS_INFO("[%s] State runtime configuration", STATE_NAME);

    // first we runtime configure the state, where we create client behaviors
    static_cast<MostDerived *>(this)->runtimeConfigure();

    // second the orthogonals are internally configured
    this->getStateMachine().notifyOnRuntimeConfigured(static_cast<MostDerived *>(this));

    ROS_INFO("[%s] State OnEntry", STATE_NAME);

    // finally we go to the derived state onEntry Function
    static_cast<MostDerived *>(this)->onEntry();
    ROS_INFO("[%s] State OnEntry code finished", STATE_NAME);

    // here orthogonals and client behaviors are entered OnEntry
    this->getStateMachine().notifyOnStateEntryEnd(static_cast<MostDerived *>(this));
  }
};
} // namespace smacc