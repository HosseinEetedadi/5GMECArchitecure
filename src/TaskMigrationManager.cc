////
//// This program is free software: you can redistribute it and/or modify
//// it under the terms of the GNU Lesser General Public License as published by
//// the Free Software Foundation, either version 3 of the License, or
//// (at your option) any later version.
////
//// This program is distributed in the hope that it will be useful,
//// but WITHOUT ANY WARRANTY; without even the implied warranty of
//// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//// GNU Lesser General Public License for more details.
////
//// You should have received a copy of the GNU Lesser General Public License
//// along with this program.  If not, see http://www.gnu.org/licenses/.
////
//
#include "TaskMigrationManager.h"
#include "nodes/mec/MECOrchestrator/MecOrchestrator.h"

Define_Module(TaskMigrationManager);

void TaskMigrationManager::initialize() {
    scheduleAt(simTime() + 2, new cMessage("triggerMigration"));
}

void TaskMigrationManager::handleMessage(cMessage *msg) {
    if (strcmp(msg->getName(), "triggerMigration") == 0) {
        EV << "⚙️ TaskMigrationManager: Triggering migration for UE[0]...\n";

        // Access the MecOrchestrator
        cModule* orchestratorMod = getSimulation()->getModuleByPath("mecOrchestrator");
        auto* orchestrator = check_and_cast<MecOrchestrator*>(orchestratorMod);

        // Simulate migration logic — future versions will use priority and policies
        orchestrator->migrateTask(0, "coreServer2");  // 👈 you’ll implement this method

        delete msg;
    }
}
