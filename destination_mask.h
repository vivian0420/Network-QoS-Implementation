#ifndef DESTINATION_MASK_H
#define DESTINATION_MASK_H

#include "FilterElement.h" 
#include "ns3/log.h"
#include "ns3/ipv4-address.h" 


using namespace ns3;

class DestinationMask: public FilterElement {
    public:

    static TypeId GetTypeId(void);

    DestinationMask();
    DestinationMask(Ipv4Mask ipv4Mask, Ipv4Address dest_ip_addr);  
    ~DestinationMask();


    bool match(Ptr<Packet> packet);


    private:
    Ipv4Mask dest_mask;
    Ipv4Address dest_ip_addr;  

};


#endif /* DESTINATION_MASK_H */