#pragma once

#include <boost/statechart/state.hpp>
#include <boost/statechart/event.hpp>

#include <smacc/smacc_types.h>

namespace smacc
{
namespace default_events
{
using namespace smacc::introspection;
using namespace smacc::default_transition_tags;

template <typename ActionFeedback, typename TObjectTag>
struct EvActionFeedback : sc::event<EvActionFeedback<ActionFeedback, TObjectTag>>
{
  smacc::client_bases::ISmaccActionClient *client;
  ActionFeedback feedbackMessage;
  //boost::any feedbackMessage;
};

template <typename TSource, typename TObjectTag>
struct EvActionResult : sc::event<EvActionResult<TSource, TObjectTag>>
{
  typename TSource::Result resultMessage;
};

//--------------------------------
template <typename TSource, typename TObjectTag>
struct EvActionSucceeded : sc::event<EvActionSucceeded<TSource, TObjectTag>>
{
  typename TSource::Result resultMessage;

  static std::string getEventLabel()
  {
    // show ros message type
    std::string label;
    EventLabel<TSource>(label);
    return label;
  }

  static std::string getDefaultTransitionTag()
  {
    return demangledTypeName<SUCCESS>();
  }

  static std::string getDefaultTransitionType()
  {
    return demangledTypeName<SUCCESS>();
  }
};

template <typename TSource, typename TObjectTag>
struct EvActionAborted : sc::event<EvActionAborted<TSource, TObjectTag>>
{
  typename TSource::Result resultMessage;

  static std::string getEventLabel()
  {
    // show ros message type
    std::string label;
    EventLabel<TSource>(label);
    return label;
  }

  static std::string getDefaultTransitionTag()
  {
    return demangledTypeName<ABORT>();
  }

  static std::string getDefaultTransitionType()
  {
    return demangledTypeName<ABORT>();
  }
};

template <typename TSource, typename TObjectTag>
struct EvActionPreempted : sc::event<EvActionPreempted<TSource, TObjectTag>>
{
  typename TSource::Result resultMessage;

  static std::string getEventLabel()
  {
    // show ros message type
    std::string label;
    EventLabel<TSource>(label);
    return label;
  }

  static std::string getDefaultTransitionTag()
  {
    return demangledTypeName<PREEMPT>();
  }

  static std::string getDefaultTransitionType()
  {
    return demangledTypeName<PREEMPT>();
  }
};

template <typename TSource, typename TObjectTag>
struct EvActionRejected : sc::event<EvActionRejected<TSource, TObjectTag>>
{
  typename TSource::Result resultMessage;

  static std::string getEventLabel()
  {
    // show ros message type
    std::string label;
    EventLabel<TSource>(label);
    return label;
  }

  static std::string getDefaultTransitionTag()
  {
    return demangledTypeName<REJECT>();
  }

  static std::string getDefaultTransitionType()
  {
    return demangledTypeName<REJECT>();
  }
};

template <typename StateType>
struct EvSequenceFinished : sc::event<EvSequenceFinished<StateType>>
{
  
};

template <typename TSource>
struct EvLoopContinue : sc::event<EvLoopContinue<TSource>>
{
  static std::string getDefaultTransitionTag()
  {
    return demangledTypeName<CONTINUELOOP>();
  }

  static std::string getDefaultTransitionType()
  {
    return demangledTypeName<CONTINUELOOP>();
  }
};

template <typename TSource>
struct EvLoopEnd : sc::event<EvLoopEnd<TSource>>
{
  static std::string getDefaultTransitionTag()
  {
    return demangledTypeName<ENDLOOP>();
  }

  static std::string getDefaultTransitionType()
  {
    return demangledTypeName<ENDLOOP>();
  }
};

template <typename TSource, typename TObjectTag>
struct EvTopicInitialMessage : sc::event<EvTopicInitialMessage<TSource, TObjectTag>>
{
  //typename EvTopicInitialMessage<SensorBehaviorType>::TMessageType msgData;
  static std::string getEventLabel()
  {
    auto typeinfo = TypeInfo::getTypeInfoFromType<typename TSource::TMessageType>();

    std::string label = typeinfo->getNonTemplatedTypeName();
    return label;
  }

  typename TSource::TMessageType msgData;
};

template <typename TSource, typename TObjectTag>
struct EvTopicMessage : sc::event<EvTopicMessage<TSource, TObjectTag>>
{
  static std::string getEventLabel()
  {
    auto typeinfo = TypeInfo::getTypeInfoFromType<typename TSource::TMessageType>();

    std::string label = typeinfo->getNonTemplatedTypeName();
    return label;
  }

  typename TSource::TMessageType msgData;
};
} // namespace default_events
} // namespace smacc