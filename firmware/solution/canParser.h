#include <iostream>
#include <fstream>
#include <memory>
#include <unordered_map>
#include <string>
#include <utility>
#include <sstream>
#include <vector>
#include <cmath>

#include "dbcppp/CApi.h"
#include "dbcppp/Network.h"

std::pair<
    std::unique_ptr<dbcppp::INetwork>,
    std::unordered_map<uint64_t, const dbcppp::IMessage *>
> loadCanFrames(
        const std::string &path
);

void parseData(
        std::unordered_map<uint64_t, const dbcppp::IMessage *> &messageMap,
        std::vector<std::string> idAndPayloadVec,
        const std::string &timeStamp,
        std::ofstream &outputFile
);

uint8_t convertToHex(char c);
