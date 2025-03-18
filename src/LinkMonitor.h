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

//#ifndef LINKMONITOR_H_
//#define LINKMONITOR_H_

#ifndef __MULTIMECHOST_LINKMONITOR_H_
#define __MULTIMECHOST_LINKMONITOR_H_

#include <omnetpp.h>
#include <map>
#include <string>

using namespace omnetpp;

class LinkMonitor : public cSimpleModule {
  private:
    simtime_t updateInterval;
    cMessage *updateMsg;
    std::map<std::string, double> linkLoads; // Stores the load of each link

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    void checkLinkLoads();

  public:
    std::string selectBestLink(); // Returns the best link based on load
};

#endif /* __MULTIMECHOST_LINKMONITOR_H_ */
