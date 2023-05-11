#include "ns3/ipv4-address.h"
#include "ns3/core-module.h"
using namespace ns3;


struct MyConfig {
    uint32_t maxPackets;
    uint32_t priority_level;
    uint32_t quantum_size;
    bool isDefault;
    Ipv4Address destIpAddr;
    Ipv4Mask destMask;
    uint32_t destPortNum;
    uint32_t protNum;
    Ipv4Address sourIpAddr;
    Ipv4Mask sourMask;
    uint32_t sourPortNum;
    Ipv4Address sourceIp;
    Ipv4Address destIp;
};

bool
startsWith(const std::string& str, const std::string& prefix)
{
    return (str.size() >= prefix.size()) && (str.substr(0, prefix.size()) == prefix);
}

void
setConfigValue(std::string& line, MyConfig& config)
{
    if (startsWith(line, "\"maxPackets\"")) {
        std::string maxPacketsStr = line.substr(line.find(":") + 1);
        config.maxPackets = atoi(maxPacketsStr.c_str());
    } else if (startsWith(line, "\"priorityLevel\"")) {
        std::string priorityLevelStr = line.substr(line.find(":") + 1);
        config.priority_level = atoi(priorityLevelStr .c_str());
    } else if (startsWith(line, "\"quantumSize\"")) {
        std::string qutumSizeStr = line.substr(line.find(":") + 1);
        config.quantum_size = atoi(qutumSizeStr .c_str());
    } else if (startsWith(line, "\"isDefault\"")) {
        std::string isDefaultStr = line.substr(line.find(":") + 1);
        config.isDefault = atoi(isDefaultStr .c_str());
    } else if (startsWith(line, "\"sourceIpAddress\"")) {
        std::string sourceIpAddressStr = line.substr(line.find(":") + 2);
        Ipv4Address addr(sourceIpAddressStr.c_str());
        config.sourIpAddr = addr;
    } else if (startsWith(line, "\"sourceMask\"")) {
        std::string sourceMaskStr = line.substr(line.find(":") + 2);
        Ipv4Mask mask(sourceMaskStr.c_str());
        config.sourMask = mask;
    } else if (startsWith(line, "\"sourcePortNumber\"")) {
        std::string sourcePortNumberStr = line.substr(line.find(":") + 1);
        config.sourPortNum = atoi(sourcePortNumberStr .c_str());
    } else if (startsWith(line, "\"destinatoinIpAddress\"")) {
        std::string destIpAddressStr = line.substr(line.find(":") + 2);
        config.destIpAddr.Set(destIpAddressStr.c_str());
    } else if (startsWith(line, "\"destinationMask\"")) {
        std::string destMaskStr = line.substr(line.find(":") + 2);
        Ipv4Mask mask(destMaskStr.c_str());
        config.destMask = mask;
    } else if (startsWith(line, "\"destinationPortNumber\"")) {
        std::string destPortNumberStr = line.substr(line.find(":") + 1);
        config.destPortNum = atoi(destPortNumberStr.c_str());
    } else if (startsWith(line, "\"protocolNumber\"")) {
        std::string protocolNumberStr = line.substr(line.find(":") + 1);
        config.protNum = atoi(protocolNumberStr.c_str());
    } else if (startsWith(line, "\"sourceIP\"")) {
        std::string sourceIPStr = line.substr(line.find(":") + 2);
        config.sourceIp.Set(sourceIPStr.c_str());
    } else if (startsWith(line, "\"destIP\"")) {
        std::string destIPStr = line.substr(line.find(":") + 2);
        Ipv4Address addr(destIPStr.c_str());
        config.destIp = addr;
    }
}