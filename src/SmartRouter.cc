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

#include "SmartRouter.h"
//#include "InterfaceEntry.h"
#include "inet/networklayer/common/L3AddressResolver.h"
#include "inet/networklayer/ipv4/Ipv4RoutingTable.h"
#include "inet/networklayer/ipv4/Ipv4Route.h"
#include "inet/networklayer/common/L3AddressResolver.h"
#include "inet/networklayer/common/InterfaceTable.h"

using namespace inet;
Define_Module(SmartRouter);

void SmartRouter::initialize() {
    updateInterval = par("updateInterval");
    updateMsg = new cMessage("updateMsg");
    checkLinkLoads(); // pretend we are monitoring something
    scheduleAt(simTime() + updateInterval, updateMsg);
}

void SmartRouter::handleMessage(cMessage *msg) {
    if (msg == updateMsg) {
        checkLinkLoads(); // pretend we are monitoring something
        selectBestPath("router1", "router8"); //Just for Test:))
        scheduleAt(simTime() + updateInterval, updateMsg);
    }
}

void SmartRouter::checkLinkLoads() {
    // Simulate dummy loads for now
    linkLoads["router1-router2"] = uniform(0, 1);
    linkLoads["router1-router3"] = uniform(0, 1);
    linkLoads["router2-router3"] = uniform(0, 1);
    linkLoads["router2-router4"] = uniform(0, 1);
    linkLoads["router3-router5"] = uniform(0, 1);
    linkLoads["router4-router5"] = uniform(0, 1);
    linkLoads["router4-router6"] = uniform(0, 1);
    linkLoads["router4-router7"] = uniform(0, 1);
    linkLoads["router5-router6"] = uniform(0, 1);
    linkLoads["router5-router7"] = uniform(0, 1);
    linkLoads["router6-router8"] = uniform(0, 1);
    linkLoads["router7-router8"] = uniform(0, 1);

    //Added
//    linkLoads["router3-router2"] = uniform(0, 1);
//    linkLoads["router3-router7"] = uniform(0, 1);
//    linkLoads["router7-router4"] = uniform(0, 1);

    // Add more if needed
    EV << "Checked link loads.\n";
}

std::string SmartRouter::selectBestLink() {
    std::string bestLink;
    double minLoad = 9999;

    for (auto& pair : linkLoads) {
        if (pair.second < minLoad) {
            minLoad = pair.second;
            bestLink = pair.first;
        }
    }

    EV << "Best link is: " << bestLink << " with load " << minLoad << "\n";
    return bestLink;
}

void SmartRouter::registerFlow(std::string flowId, std::string destModuleName) {
    EV << "registerFlow is started ... "<<endl;
    if (flowPaths.find(flowId) == flowPaths.end()) {
        auto path = selectBestPath("router1", "router8");
        flowPaths[flowId] = path;

        EV << "Registered flow " << flowId << " with path:\n";
        for (auto& link : path) EV << " " << link;
        EV << endl;

        inet::L3AddressResolver resolver;
        inet::L3Address destAddr = resolver.resolve(destModuleName.c_str());
        inet::Ipv4Address destinationIP = destAddr.toIpv4();

        EV << "Destination module: " << destModuleName << ", resolved IP: " << destinationIP << endl;

        EV << "installRouteForFlow params: "<< "flowId: " << flowId << "destinationIP" << endl;
        installRouteForFlow(flowId, path, destinationIP);
    }
}



std::vector<std::string> SmartRouter::selectBestPath(std::string src, std::string dst) {

    std::vector<std::vector<std::string>> candidatePaths = {
        {"router1-router2", "router2-router4", "router4-router6", "router6-router8"},
        {"router1-router2", "router2-router4", "router4-router5", "router5-router6", "router6-router8"},
        {"router1-router3", "router3-router5", "router5-router6", "router6-router8"},
        {"router1-router3", "router3-router5", "router5-router7", "router7-router8"},
        {"router1-router3", "router3-router2", "router2-router4", "router4-route7", "router7-router8"},
        {"router1-router3", "router3-router2", "router2-router4", "router4-router6", "router6-router8"}
    };

//    std::vector<std::vector<std::string>> candidatePaths = {
//    {"router1-router2", "router2-router4", "router4-router6", "router6-router8"},
//    {"router1-router3", "router3-router7", "router7-router8"},
//    {"router1-router3", "router3-router2", "router2-router4", "router4-route7", "router7-router8"},
//    {"router1-router3", "router3-router2", "router2-router4", "router4-router6", "router6-router8"},
//    {"router1-router2", "router2-router3", "router3-router7", "router7-router8"},
//    {"router1-router3", "router3-router7", "router7-router4", "router4-router6", "router6-router8"},
//
//    };
    double minLoad = 1e9;
    std::vector<std::string> bestPath;

    for(auto& path: candidatePaths){
        double totalLoad = 0;
        bool valid = true;

        for(auto& link: path){
            if(linkLoads.find(link) != linkLoads.end()){
                totalLoad += linkLoads[link];
            }
            else{
                valid = false;
                break;
            }

        }
        if(valid && totalLoad < minLoad){
            minLoad = totalLoad;
            bestPath = path;
        }

    }
    EV << "Selected Best Path: ";
    for(auto& link: bestPath){
        EV << link << " ";
    }

    EV << "\nThe total load of the path is: " << minLoad << endl;

    return bestPath;
}

std::vector<std::string> extractRoutersFromLinks(const std::vector<std::string>& linkPath) {
    std::vector<std::string> routerSequence;
    for (const auto& link : linkPath) {
        size_t dash = link.find("-");
        std::string from = link.substr(0, dash);
        std::string to = link.substr(dash + 1);

        if (routerSequence.empty())
            routerSequence.push_back(from);
        routerSequence.push_back(to);
    }
    return routerSequence;
}

#include "inet/networklayer/ipv4/Ipv4RoutingTable.h"
#include "inet/networklayer/ipv4/Ipv4Route.h"
#include "inet/networklayer/common/InterfaceTable.h"
#include "inet/networklayer/common/L3AddressResolver.h"
#include "inet/networklayer/common/NetworkInterface.h"

using namespace inet;

void SmartRouter::installRouteForFlow(const std::string& flowId, const std::vector<std::string>& path, const inet::Ipv4Address& destinationIP) {
    auto routerList = extractRoutersFromLinks(path);
    std::string destStr = destinationIP.str();

//    // 🔒 Only override routes to CoreServer IP range (10.0.0.12X)
//    if (destStr.rfind("10.0.0.12", 0) != 0) {
//        EV << "ℹ️ Skipping SmartRouter route injection for flow " << flowId
//           << " (non-core destination: " << destinationIP << ")" << endl;
//        return;
//    }

    EV << ">>> installRouteForFlow() started for flowId: " << flowId
       << " | Destination: " << destinationIP << endl;

    for (size_t i = 0; i < routerList.size() - 1; ++i) {
        const std::string& currentRouter = routerList[i];
        const std::string& nextRouter = routerList[i + 1];

        EV << "🔄 Processing hop: " << currentRouter << " → " << nextRouter << endl;

        // 🔍 Locate router module
        cModule* routerMod = getSimulation()->getModuleByPath(currentRouter.c_str());
        if (!routerMod) {
            EV << "❌ Router module not found: " << currentRouter << endl;
            continue;
        }

        // 📦 Get routingTable
        auto* ipv4 = routerMod->getSubmodule("ipv4");
        auto* rtMod = ipv4 ? ipv4->getSubmodule("routingTable") : nullptr;
        if (!rtMod) {
            EV << "❌ No routingTable found in " << currentRouter << ".ipv4 — skipping\n";
            continue;
        }
        auto* rt = check_and_cast<inet::Ipv4RoutingTable*>(rtMod);

        // 📦 Get interfaceTable
        auto* itMod = routerMod->getSubmodule("interfaceTable");
        if (!itMod) {
            EV << "❌ No interfaceTable in " << currentRouter << " — skipping\n";
            continue;
        }
        auto* ift = check_and_cast<inet::InterfaceTable*>(itMod);

        // 🚪 Choose the first usable (non-loopback) interface
        inet::NetworkInterface* outInterface = nullptr;
        for (int j = 0; j < ift->getNumInterfaces(); ++j) {
            auto* intf = ift->getInterface(j);
            if (intf && !intf->isLoopback()) {
                outInterface = intf;
                break;
            }
        }

        if (!outInterface) {
            EV << "❌ No usable interface found in " << currentRouter << " — skipping\n";
            continue;
        }

        // 🧹 Remove any existing route to the same destination
        for (int r = rt->getNumRoutes() - 1; r >= 0; --r) {
            auto* route = rt->getRoute(r);
            const auto& dest = route->getDestination();
            const auto& mask = route->getNetmask();

            int prefixLength = mask.getNetmaskLength();

            if (dest == destinationIP || dest.prefixMatches(destinationIP, prefixLength)) {
                EV << "🗑 Removing route in " << currentRouter
                   << " to " << dest << "/" << prefixLength << " via "
                   << (route->getInterface() ? route->getInterface()->getInterfaceName() : "null") << endl;
                rt->deleteRoute(route);
            }
        }

        // ➕ Install SmartRouter-selected route
        auto* route = new inet::Ipv4Route();
        route->setDestination(destinationIP);
        route->setNetmask(inet::Ipv4Address::ALLONES_ADDRESS);  // /32
        route->setInterface(outInterface);
        route->setNextHop(inet::Ipv4Address::UNSPECIFIED_ADDRESS);  // direct
        route->setSourceType(inet::IRoute::MANUAL);

        rt->addRoute(route);


        EV << "📋 Final routing table for " << currentRouter << ":\n";
        for (int r = 0; r < rt->getNumRoutes(); ++r) {
            auto route = rt->getRoute(r);
            EV << "  → " << route->getDestination() << " via "
               << (route->getInterface() ? route->getInterface()->getInterfaceName() : "null")
               << " [metric=" << route->getMetric() << "]\n";
        }


        EV << "✅ Route installed in " << currentRouter
           << " to " << destinationIP
           << " via interface " << outInterface->getInterfaceName() << endl;
    }

    EV << "✅ Flow " << flowId << " routing successfully configured.\n";
}















