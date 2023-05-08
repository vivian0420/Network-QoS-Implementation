#ifndef DESTINATION_PORT_NUMBER_H
#define DESTINATION_PORT_NUMBER_H

#include "FilterElement.h"
#include "ns3/log.h"


using namespace ns3;
class DestinationPortNumber: public FilterElement {
    public:
        
        static TypeId GetTypeId(void);

        DestinationPortNumber(uint32_t);
        ~DestinationPortNumber();

        bool match(Ptr<Packet> packet);


    private:
        uint32_t port_number;

};

#endif /*DESTINATION_PORT_NUMBER_H */