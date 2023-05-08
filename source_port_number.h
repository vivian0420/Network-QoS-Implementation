#ifndef SOURCE_PORT_NUMBER_H
#define SOURCE_PORT_NUMBER_H

#include "FilterElement.h"
#include "ns3/log.h"



using namespace ns3;

class SourcePortNumber: public FilterElement {
    public:
        
        static TypeId GetTypeId(void);

        SourcePortNumber(uint32_t);
        ~SourcePortNumber();


        bool match(Ptr<ns3::Packet> packet);


    private:

        uint32_t src_port;

};

#endif /*SOURCE_PORT_NUMBER_H */