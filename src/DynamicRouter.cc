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
#include "inet/networklayer/ipv4/Ipv4Route.h"
#include "inet/networklayer/common/InterfaceTable.h"
//#include "inet/networklayer/common/InterfaceEntry.h"
#include "inet/common/ModuleAccess.h"

using namespace std;
//using ::InterfaceEntry;  // or just refer to InterfaceEntry directly

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


    string bestLink = linkMonitor->selectBestLink();

    if (bestLink != "none") {
           EV << "Updating routing table to use best link: " << bestLink << endl;
           EV << "The Router name is: "<< bestLink << endl;
           EV << "routingTable Count = " << routingTable->getNumRoutes() << endl;
           for(int i =0; i < routingTable->getNumRoutes(); i++){
               const Ipv4Route *route = routingTable->getRoute(i);
               EV << "Destination: " << route->getDestination()
                  << ", NetMask: "<<route->getNetmask()
                  << ", GW: " << route->getGateway()
                  << ", Interface: "<< route->getInterface() << endl;
           }
          ///// Implement routing table update logic here/////

//           cGate *gate = getSimulation()->getModuleByPath(bestLink.c_str())->gate("$o");
//           EV << "This is the getModuleByPah: "<< gate << endl;

//
//           // Get the gate from the bestLink path
//           cGate *gate = getSimulation()->getModuleByPath(bestLink.c_str())->gate("$o");
//
//           EV << "This is the gate: "<<gate << endl;
//           EV << "This is the getModuleByPah: "<< getSimulation()->getModuleByPath(bestLink.c_str()) << endl;
//
//           if (!gate) {
//               EV << "Best link gate not found: " << bestLink << endl;
//               return;
//           }
//
//           // Get the parent module (router) and its interface table
//           cModule *routerMod = gate->getOwnerModule();
//           inet::IInterfaceTable *ifaceTable = check_and_cast<inet::IInterfaceTable *>(routerMod->getSubmodule("interfaceTable"));
////           InterfaceEntry *iface = ifaceTable->findInterfaceByNodeOutputGateId(gate->getId());
//
////           if (!iface) {
////               EV << "No interface found for gate: " << bestLink << endl;
////               return;
////           }
//       // Create a default route using this interface
//              inet::Ipv4Route *route = new inet::Ipv4Route();
//              route->setDestination(inet::Ipv4Address("0.0.0.0")); // Default route
//              route->setNetmask(inet::Ipv4Address("0.0.0.0"));
//              route->setGateway(inet::Ipv4Address::UNSPECIFIED_ADDRESS);
////              route->setInterface(iface);
//
//              routingTable->addRoute(route);
//              EV << "Dynamic default route installed via: " << bestLink << endl;
    }
}
