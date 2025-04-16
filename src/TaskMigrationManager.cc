#include "TaskMigrationManager.h"
#include "nodes/mec/MECOrchestrator/MecOrchestrator.h"

Define_Module(TaskMigrationManager);

void TaskMigrationManager::initialize() {
    threshold = par("threshold");
}

void TaskMigrationManager::handleMessage(cMessage *msg) {
    // no internal messages for now
    delete msg;
}

void TaskMigrationManager::registerPacket(const std::string& flowId) {
    flowPacketCounter[flowId]++;
    int count = flowPacketCounter[flowId];
    EV << "[MigrationManager] Flow " << flowId << " sent " << count << " packets.\n";

    if (count == threshold) {
        EV << "Triggering migration for " << flowId << endl;

        // Extract UE id (e.g., "ue[0]" → 0)
        int ueId = -1;
        size_t start = flowId.find("ue[");
        if (start != std::string::npos) {
            ueId = std::stoi(flowId.substr(start + 3));
        }

        // Trigger migration via Orchestrator
        if (ueId >= 0) {
            cModule* orchestratorMod = getSimulation()->getModuleByPath("mecOrchestrator");
            auto* orchestrator = check_and_cast<MecOrchestrator*>(orchestratorMod);
            orchestrator->stopMECAppForUE(ueId);  // You'll implement this
            orchestrator->startMECAppForUE(ueId); // You'll implement this
        }
    }
}



