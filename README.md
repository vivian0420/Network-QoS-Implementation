# Simulating Strict Priority Queueing, Deficit Round Robin with NS-3

Collaborators: Li Liu, Haoyu Yan, Jiayun Zhang

## Overview
The ns-3 network simulator is a powerful tool for evaluating and developing networking protocols and algorithms. In this project, we explored the capabilities of ns-3 by implement- ing modifications to the base class design to better suit our project’s needs. Our project focused on designing the Diff- Serv and TrafficClass classes to implement the Strict Priority Queue (SPQ) and Deficit Round Robin (DRR) scheduling algorithms, respectively. We also developed a customized UdpApplication to allow for packet customization, including packet size, amount, data rate, and packet UDP header.

## Design
We did not modify any ns-3 classes directly. However, we made some modifications to the base class design to better suit our project’s needs. For example, (1) We made changes to the Source Mask class and Destination Mask class by combining the packet’s address with its Ipv4Mask before comparing it with the packet’s source and destination IP to ensure that the packet is from the corresponding subnet. (2) Additionally, we made some adjustments to the Traffic- Class and DiffServ class to make them more suitable for our project. For instance, we removed irrelevant variables such bytes, maxBytes, and weight from TrafficClass and added quantum, DeficitCounter, and a standard queue to store packets for dequeue for DRR implementation. (3) Since the general Application does not offer a "packet customized" service, we developed our UdpApplication for the client to include our customized packet size, packet amount, data rate, and packet udpHeader.



## Installation ##
1. Clone this repository under the "scratch" directory of your ns3 installation. 
2. Navigate to the ns3 root directory. 
3. SPQ validation: \
   (1)Rename ```Drr.cc``` to ```Drr.txt``` and ```Drr.h``` to ```Drr1.txt``` to avoid "duplicate symbol for architecture x86_64" error.\
   (2)Run ``` ./ns3 ``` configure to build the project. \
   (3)Run ``` ./ns3 run "scratch/cs621-project2/Spq --spq=path of config_spq.txt" ``` 
4. DRR validation: \
   (1)Rename ```Drr.txt``` to ```Drr.cc``` and ```Drr1.txt``` to ```Drr.h``` to recover Drr class and header.\
   (2)Rename ```Spq.cc``` to ```Spq.txt``` and ```Spq.h``` to ```Spq1.txt``` to avoid "duplicate symbol for architecture x86_64" error.\
   (3)Run ``` ./ns3 ``` configure to build the project. \
   (4)Run ``` ./ns3 run "scratch/cs621-project2/Drr --spq=path of config_drr.txt" ``` 

<!-- ## Incomplete features ##
We encountered an unexpected error when we attempted to separate DiffServ.cc from Spq.cc and Drr.cc. 

```undefined symbols for architecture x86_64: [build] "spq<ns3::packet>::spq()", referenced from: [build] ns3::ptr<spq<ns3::packet> > ns3::createobject<spq<ns3::packet> >(). ``` 

Despite our best efforts, we were unable to resolve the error. As a result, we decided to keep the DiffServ.cc implementation within Spq.cc and Drr.cc in order to avoid this issue. -->

 
