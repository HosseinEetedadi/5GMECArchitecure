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

#include <omnetpp.h>
#include <set>
#include <string>
#include <vector>
#include "nodes/mec/MECOrchestrator/ApplicationDescriptor/ApplicationDescriptor.h"
#include "nodes/mec/VirtualisationInfrastructureManager/VirtualisationInfrastructureManager.h"

struct ResourceRequirement {
    double cpu = 0.0;
    double ram = 0.0;
    double disk = 0.0;
};

using namespace omnetpp;

class NodePruner : public cSimpleModule {
  protected:
    std::set<std::string> prunedNodes;
    std::vector<std::string> allProcessingNodes;  // e.g., {"mecHost1", "coreServer1"}
    std::map<std::string, VirtualisationInfrastructureManager*> nodeToVIM;

    virtual void initialize() override;
    virtual void handleMessage(cMessage* msg) override;

  public:
    bool isPruned(const std::string& nodeName) const;
    std::vector<std::string> getAvailableNodes() const;
    void pruneNode(const std::string& nodeName);
    void restoreNode(const std::string& nodeName);
    void pruneUnfitNodes(const ResourceDescriptor& required);
    void evaluateAndPrune(const ResourceRequirement& req);


};
