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

network singleMecHost
{
    parameters:
        @display("i=block/network2;bgb=911.62396,506.912;bgi=background/pisa");
        int numUes = default(0);
        double playgroundSizeX @unit(m);
        double playgroundSizeY @unit(m);
        double playgroundSizeZ @unit(m);

    submodules:

        routingRecorder: RoutingTableRecorder {
            @display("p=843.14996,189.06999");
        }
        configurator: Ipv4NetworkConfigurator {
            @display("p=843.14996,268.78598");
        }
        channelControl: LteChannelControl {
            @display("p=843.14996,109.354");
        }
        binder: Binder {
            @display("p=843.14996,349.524");
        }
        carrierAggregation: CarrierAggregation {
            @display("p=844.172,426.17398");
        }

        upf: Upf {
            @display("p=545.0638,249.77875");
        }

        iUpf: Upf {
            @display("p=352.92624,249.77875");
        }

        gnb1: gNodeB {
            @display("p=140,250;is=vl");
        }

        ue[numUes]: NRUe {
            @display("p=94.023994,218.708");
        }

        mecHost1: MECHost {
            @display("p=352.92624,145.62;is=l");
        }

        mecHost2: MECHost {
            @display("p=352.59,341.348;is=l");
        }

        mecOrchestrator: MecOrchestrator {
            @display("p=544.0525,63.70875;is=l");
        }

        ualcmp: UALCMP {
            @display("p=545.0638,158.76625;i=device/smallrouter");
        }

        // 🔹 New: Adding Core Servers
        coreServer1: MECHost {
            @display("p=638.75,210.532;is=s");
        }

        coreServer2: MECHost {
            @display("p=638.75,281.05;is=s");
        }

    connections allowunconnected:

        upf.pppg++ <--> Eth10G <--> iUpf.pppg++;
        iUpf.pppg++ <--> Eth10G <--> gnb1.ppp;
        ualcmp.ppp++ <--> Eth10G <--> upf.filterGate;

        ualcmp.toMecOrchestrator --> mecOrchestrator.fromUALCMP;
        ualcmp.fromMecOrchestrator <-- mecOrchestrator.toUALCMP;

        mecHost1.ppp++ <--> Eth10G <--> iUpf.pppg++;
        mecHost2.ppp++ <--> Eth10G <--> iUpf.pppg++;

        //  New: Connecting Core Servers
        upf.pppg++ <--> Eth10G <--> coreServer1.ppp++;
        upf.pppg++ <--> Eth10G <--> coreServer2.ppp++;
}




