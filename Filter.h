#ifndef FILTER_H
#define FILTER_H

 
#include "ns3/log.h"
#include "FilterElement.h"
#include "destination_ip_address.h"
#include "destination_mask.h"
#include "destination_port_number.h"
#include "protocol_number.h"
#include "source_ip_address.h"
#include "source_mask.h"
#include "source_port_number.h"
#include <vector>

class Filter: public Object {
  public:

  Filter();
  Filter(DestinationIpAddress* destIpAddr, DestinationMask* destMask, DestinationPortNumber* destPortNum, 
         ProtocolNumber* protNum, SourceIpAddress* sourIpAddr, SourceMask* sourMask, SourcePortNumber* sourPortNum);
  ~Filter();

  std::vector<FilterElement*> elements;
  bool match(Ptr<Packet> packet);  

};


#endif /* FILTER_H */