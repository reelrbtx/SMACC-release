/*****************************************************************************************************************
 * ReelRobotix Inc. - Software License Agreement      Copyright (c) 2018
 * 	 Authors: Pablo Inigo Blasco, Brett Aldrich
 *
 ******************************************************************************************************************/

#pragma once
#include <smacc/common.h>
#include <utility>

namespace smacc
{
class ISmaccOrthogonal
{
public:
    void setStateMachine(ISmaccStateMachine *value);

    inline ISmaccStateMachine *getStateMachine();

    void addClientBehavior(std::shared_ptr<smacc::SmaccClientBehavior> clientBehavior);

    void runtimeConfigure();

    void onEntry();

    void onExit();

    virtual std::string getName() const;

    template <typename SmaccComponentType>
    void requiresComponent(SmaccComponentType *&storage);

    template <typename SmaccClientType>
    bool requiresClient(SmaccClientType *&storage);

    inline const std::vector<std::shared_ptr<smacc::ISmaccClient>> &getClients();

    inline const std::vector<std::shared_ptr<smacc::SmaccClientBehavior>> &getClientBehaviors() const;

    template <typename T>
    void setGlobalSMData(std::string name, T value);

    template <typename T>
    bool getGlobalSMData(std::string name, T &ret);

    template <typename TClientBehavior>
    TClientBehavior *getClientBehavior();


protected:
    virtual void onInitialize();

    void assignClientToOrthogonal(smacc::ISmaccClient* client);

    std::vector<std::shared_ptr<smacc::ISmaccClient>> clients_;

private:
    ISmaccStateMachine *stateMachine_;

    std::vector<std::shared_ptr<smacc::SmaccClientBehavior>> clientBehaviors_;
};

} // namespace smacc