
#pragma once

#include <omnetpp.h>
#include <map>
#include <string>

using namespace omnetpp;

class TaskMigrationManager : public cSimpleModule {
  private:
    int threshold = 3;
    std::map<std::string, int> flowPacketCounter;

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

  public:
    void registerPacket(const std::string& flowId); // called externally
};

