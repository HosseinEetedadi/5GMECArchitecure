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

package multimechost;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.networklayer.ipv4.RoutingTableRecorder;
import inet.node.ethernet.Eth10G;
import simu5g.world.radio.LteChannelControl;
import simu5g.common.binder.Binder;
import simu5g.common.carrierAggregation.CarrierAggregation;
import simu5g.nodes.Upf;
import simu5g.nodes.NR.gNodeB;
import simu5g.nodes.NR.NRUe;
import simu5g.nodes.mec.MECHost;
import simu5g.nodes.mec.MECOrchestrator.MecOrchestrator;
import simu5g.nodes.mec.UALCMP.UALCMP;
import inet.visualizer.contract.IIntegratedVisualizer;
import inet.node.inet.Router;
import multimechost.LinkMonitor;
import multimechost.DynamicRouter;
import inet.networklayer.ipv4.Ipv4RoutingTable;
import inet.networklayer.common.InterfaceTable;
import ned.DatarateChannel;

channel Eth1M extends DatarateChannel
{
    datarate = 0.001bps;
    delay = 1ms;
}

network singleMecHost
{
    parameters:
        @display("i=block/network2;bgb=1334.9751,953.075;bgi=background/pisa");
        int numUes = default(0);
        double playgroundSizeX @unit(m);
        double playgroundSizeY @unit(m);
        double playgroundSizeZ @unit(m);

        @networkNode(true);  // ✅ Add this line
    submodules:

        routingRecorder: RoutingTableRecorder {
            @display("p=1242.7887,386.00623;is=s");
        }
        configurator: Ipv4NetworkConfigurator {
            @display("p=1242.7887,287.6125;is=s");
        }
        channelControl: LteChannelControl {
            @display("p=1241.2749,121.1;is=vs");
        }
        binder: Binder {
            @display("p=1242.7887,205.87;is=s");
        }
        carrierAggregation: CarrierAggregation {
            @display("p=1235.22,37.84375;is=s");
        }
        routingTable: Ipv4RoutingTable {
            parameters:
                interfaceTableModule = default("^.interfaceTable");
                @display("p=1033.5599,36.365997;is=s");

        }
        interfaceTable: InterfaceTable {
            @display("p=830.67596,36.365997;is=s");
        }
        upf: Upf {
            @display("p=953.17194,687.126");
        }

        iUpf: Upf {
            @display("p=170.346,287.09998");
        }

        gnb1: gNodeB {
            @display("p=171.5625,122.38125;is=vl");
        }

        ue[numUes]: NRUe {
            @display("p=94.023994,218.708");
        }

        mecHost1: MECHost {
            @display("p=266.046,250.734;is=s");
            @display("i=device/server");
        }
        router1: Router {
            @display("p=170.346,694.782");

        }
        router2: Router {

            @display("p=352.176,606.738");
        }
        router3: Router {
            @display("p=352.176,765.6");
        }
        router4: Router {
            @display("p=495.72598,606.738");
        }
        router5: Router {

            @display("p=495.72598,765.6");
        }
        router6: Router {
            @display("p=650.75995,606.738");
        }
        router7: Router {
            @display("p=660.32996,765.6");
        }
        router8: Router {
            @display("p=725.40594,681.384");
        }
        mecHost2: MECHost {
            @display("p=264.132,321.55197;is=s");
            @display("i=device/server");
        }

        mecOrchestrator: MecOrchestrator {
            @display("p=951.25793,430.65;is=l");
        }

        ualcmp: UALCMP {
            @display("p=953.17194,566.54395;i=device/smallrouter");
        }

        // ð¹ New: Adding Core Serve
        coreServer1: MECHost {
            @display("p=1117.776,648.84595;is=l");

        }

        coreServer2: MECHost {
            @display("p=1117.776,780.912;is=l");
        }
        coreServer3: MECHost {
            @display("p=1270.896,648.84595;is=l");

        }

        coreServer4: MECHost {
            @display("p=1270.896,780.912;is=l");
        }

        smartRouter: SmartRouter {
            parameters:
                updateInterval = 2s;  // or any value you want
                @display("i=block/network;p=786.654,400.02597");
        }
        nodePruner: NodePruner {

            @display("p=623.964,400.02597;i=block/process");
        }
        taskMigrationManager: TaskMigrationManager {
            @display("p=514.86597,250.734;i=block/gear");
        }

    connections allowunconnected:

        iUpf.pppg++ <--> Eth10G <--> gnb1.ppp;
        iUpf.pppg++ <--> Eth10G <--> router1.pppg++;



        router1.pppg++ <--> Eth10G <--> router2.pppg++;
        router1.pppg++ <--> Eth10G <--> router3.pppg++;
        router2.pppg++ <--> Eth10G <--> router3.pppg++;
        router2.pppg++ <--> Eth10G <--> router4.pppg++;
        router3.pppg++ <--> Eth10G <--> router5.pppg++;
        router4.pppg++ <--> Eth10G <--> router5.pppg++;
        router4.pppg++ <--> Eth10G <--> router6.pppg++;
        router4.pppg++ <--> Eth10G <--> router7.pppg++;
        router5.pppg++ <--> Eth10G <--> router6.pppg++;
        router5.pppg++ <--> Eth10G <--> router7.pppg++;
        router6.pppg++ <--> Eth10G <--> router8.pppg++;
        router7.pppg++ <--> Eth10G <--> router8.pppg++;


        upf.pppg++ <--> Eth10G <--> router8.pppg++;

        ualcmp.ppp++ <--> Eth10G <--> upf.filterGate;

        ualcmp.toMecOrchestrator --> mecOrchestrator.fromUALCMP;
        ualcmp.fromMecOrchestrator <-- mecOrchestrator.toUALCMP;

        mecHost1.ppp++ <--> Eth10G <--> iUpf.pppg++;
        mecHost2.ppp++ <--> Eth10G <--> iUpf.pppg++;

        //  New: Connecting Core Servers
        upf.pppg++ <--> Eth10G <--> coreServer1.ppp++;
        upf.pppg++ <--> Eth10G <--> coreServer2.ppp++;
        upf.pppg++ <--> Eth10G <--> coreServer3.ppp++;
        upf.pppg++ <--> Eth10G <--> coreServer4.ppp++;

}

