//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef __SMARTROUTER_H_
#define __SMARTROUTER_H_

#include <omnetpp.h>
#include <map>
#include <vector>
#include <string>
#include "inet/networklayer/contract/ipv4/Ipv4Address.h"
#include "inet/networklayer/ipv4/Ipv4RoutingTable.h"

using namespace omnetpp;

class SmartRouter : public cSimpleModule {
  private:
    simtime_t updateInterval;
    cMessage *updateMsg;

    std::map<std::string, double> linkLoads; // linkId → load or inverse capacity
    std::map<std::string, std::vector<std::string>> flowPaths; // flowId → path
    void installRouteForFlow(const std::string& flowId, const std::vector<std::string>& path, const inet::Ipv4Address& destinationIP);
    void removeConflictingRoutes(inet::Ipv4RoutingTable* rt, const inet::Ipv4Address& destinationIP, const std::string& routerName);


  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

    void checkLinkLoads();

  public:
    std::string selectBestLink(); // simple logic for now
    std::vector<std::string> selectBestPath(std::string src, std::string dst); // placeholder
    void registerFlow(std::string flowId, std::string destModuleName);

};

#endif
