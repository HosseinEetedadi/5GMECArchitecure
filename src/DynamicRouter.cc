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

#include "DynamicRouter.h"

Define_Module(DynamicRouter);

void DynamicRouter::initialize() {
    EV << "DynamicRoute is Working now:)"<< endl;
    cModule *routingModule = getParentModule()->getSubmodule("routingTable");
    if (!routingModule) {
        throw cRuntimeError("IPv4 Routing Table not found! Make sure the router has a routingTable submodule.");
    }
    routingTable = check_and_cast<inet::Ipv4RoutingTable *>(routingModule);

    linkMonitor = check_and_cast<LinkMonitor *>(getParentModule()->getSubmodule("linkMonitor"));
    scheduleAt(simTime() + 1, new cMessage("updateRoutes"));
}

void DynamicRouter::handleMessage(cMessage *msg) {
    if (strcmp(msg->getName(), "updateRoutes") == 0) {
        updateRoutes();
        scheduleAt(simTime() + 1, msg);
    } else {
        delete msg;
    }
}

void DynamicRouter::updateRoutes() {
    std::string bestLink = linkMonitor->selectBestLink();
    if (bestLink != "none") {
        EV << "Updating routing table to use best link: " << bestLink << endl;
        // Implement routing table update logic here
    }
}
