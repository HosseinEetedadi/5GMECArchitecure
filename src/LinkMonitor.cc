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

#include "LinkMonitor.h"
#include "inet/common/ModuleAccess.h"

Define_Module(LinkMonitor);

void LinkMonitor::initialize() {
    EV << "LinkMonitor is Working now:)"<< endl;
    updateInterval = par("updateInterval");
    updateMsg = new cMessage("update");
    scheduleAt(simTime() + updateInterval, updateMsg);
}

void LinkMonitor::handleMessage(cMessage *msg) {
    if (msg == updateMsg) {
        checkLinkLoads();
        std::string bestLink = selectBestLink();
        EV << "Best link selected: " << bestLink << endl;
        scheduleAt(simTime() + updateInterval, updateMsg);
    } else {
        delete msg;
    }
}

void LinkMonitor::checkLinkLoads() {

    for (cModule::SubmoduleIterator it(getSimulation()->getSystemModule()); !it.end(); ++it) {
            cModule *mod = *it;

        // Check if module is a router
        if (strstr(mod->getName(), "router") != nullptr) {
            for (int i = 0; i < mod->gateSize("pppg"); ++i) {  // ✅ Loop through both directions
                EV << "The module is: " << mod << endl;
                cGate *gateIn = mod->gate("pppg$i", i);  // ✅ Input gate
                cGate *gateOut = mod->gate("pppg$o", i); // ✅ Output gate
                std::string gatePath = gateIn-> getFullPath();
                if (gateIn->isConnected()) {
                    cChannel *channel = gateIn->getChannel();

                    if (channel) {
                        double load = channel->getTransmissionFinishTime().dbl();
                        linkLoads[gateIn->getFullPath()] = load;  // ✅ Store correct load
                        for(const auto& elem : linkLoads){
                            EV << "first element = " << elem.first << " Second element: " << elem.second << endl;
                        }
                        EV <<"The checkLinkLoads in gateIn is working well:)"<< "This is The gateFullPath: " << gateIn->getFullPath() << endl;
                    }

                }

                if (gateOut->isConnected()) {
                    cChannel *channel = gateOut->getChannel();
                    if (channel) {
                        double load = channel->getTransmissionFinishTime().dbl();
                        linkLoads[gateOut->getFullPath()] = load;  // ✅ Store correct load
                        EV <<"The checkLinkLoads in gateOut is working well:)"<< endl;
                    }

                }
            }
        }
    }
}


std::string LinkMonitor::selectBestLink() {
    //The algorithm for choosing best link
    double minLoad = DBL_MAX;
    std::string bestLink = "none";
    EV <<"The selectBestLink is working well:)"<< endl;
    for (const auto &entry : linkLoads) {
        if (entry.second < minLoad) {
            minLoad = entry.second;
            bestLink = entry.first;
        }
    }
    EV << "The best link is: "<< bestLink<< endl;
    return bestLink;
}
