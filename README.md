# cs621-project2

Collaborators: Li Liu, Jiayun Zhang, Haoyu Yan

## Installation ##
(1) Clone this repository under the "scratch" directory of your ns3 installation. \
(2) Navigate to the ns3 root directory. \
(3) Run command ``` ./ns3 ```configure to build the project. \
(3) For SPQ, run ``` ./ns3 run "scratch/cs621-project2/Spq --spq=path of config_spq.txt" ``` \
(4) For DRR, run ``` ./ns3 run "scratch/cs621-project2/Drr --spq=path of config_drr.txt" ``` 

## Incomplete features ##
We encountered an unexpected error when we attempted to separate DiffServ.cc from Spq.cc and Drr.cc. 

```undefined symbols for architecture x86_64: [build] "spq<ns3::packet>::spq()", referenced from: [build] ns3::ptr<spq<ns3::packet> > ns3::createobject<spq<ns3::packet> >(). ``` 

Despite our best efforts, we were unable to resolve the error. As a result, we decided to keep the DiffServ.cc implementation within Spq.cc and Drr.cc in order to avoid this issue.
