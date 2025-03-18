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

//#ifndef DYNAMICROUTER_H_
//#define DYNAMICROUTER_H_
#ifndef __MULTIMECHOST_DYNAMICROUTER_H_
#define __MULTIMECHOST_DYNAMICROUTER_H_

#include <omnetpp.h>
#include "inet/networklayer/ipv4/Ipv4RoutingTable.h"  // Correct header
#include "LinkMonitor.h"

using namespace omnetpp;
using namespace inet;  // Ensure you use the inet namespace

class DynamicRouter : public cSimpleModule {
  private:
    Ipv4RoutingTable *routingTable;  // Use inet::Ipv4RoutingTable instead of just Ipv4RoutingTable
    LinkMonitor *linkMonitor;

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    void updateRoutes();
};

#endif /* __MULTIMECHOST_DYNAMICROUTER_H_ */
