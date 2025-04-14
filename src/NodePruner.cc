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

#include "NodePruner.h"
#include "nodes/mec/VirtualisationInfrastructureManager/VirtualisationInfrastructureManager.h"

Define_Module(NodePruner);

void NodePruner::initialize() {
    // Example: initialize known nodes (can be dynamic later)
    allProcessingNodes = {"mecHost1", "mecHost2", "coreServer1", "coreServer2", "coreServer3", "coreServer4"};
}

void NodePruner::handleMessage(cMessage* msg) {
    // optional: respond to probes, triggers, or timers
}

bool NodePruner::isPruned(const std::string& nodeName) const {
    return prunedNodes.count(nodeName) > 0;
}

std::vector<std::string> NodePruner::getAvailableNodes() const {
    std::vector<std::string> available;
    for (const auto& n : allProcessingNodes) {
        if (!isPruned(n))
            available.push_back(n);
    }
    return available;
}

void NodePruner::pruneNode(const std::string& nodeName) {
    prunedNodes.insert(nodeName);
    EV << "🔴 Node pruned: " << nodeName << endl;
}

void NodePruner::restoreNode(const std::string& nodeName) {
    prunedNodes.erase(nodeName);
    EV << "🟢 Node restored: " << nodeName << endl;
}

void NodePruner::evaluateAndPrune(const ResourceRequirement& req) {
    prunedNodes.clear();  // 🧹 reset before evaluation

    for (const auto& nodeName : allProcessingNodes) {
        cModule* node = getSimulation()->getModuleByPath(nodeName.c_str());
        if (!node) continue;

        auto* vim = check_and_cast<VirtualisationInfrastructureManager*>(node->getSubmodule("vim"));
        if (!vim) continue;

        bool canAllocate = vim->isAllocable(req.ram, req.disk, req.cpu);
        if (!canAllocate) {
            prunedNodes.insert(nodeName);
            EV << "❌ NodePruner: Pruned " << nodeName << " due to insufficient resources." << endl;
        } else {
            EV << "✅ NodePruner: " << nodeName << " has enough resources." << endl;
        }
    }
}

