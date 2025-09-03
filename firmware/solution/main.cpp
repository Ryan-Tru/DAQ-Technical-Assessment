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

int main() {
    // Load can frames
    auto [controlNet, controlMessages] =
        loadCanFrames("/app/dbc-files/ControlBus.dbc");
    auto [sensorNet, sensorMessages] =
        loadCanFrames("/app/dbc-files/SensorBus.dbc");
    auto [tracktiveNet, tracktiveMessages] =
        loadCanFrames("/app/dbc-files/TractiveBus.dbc");

    // load files
    std::ifstream logFile("/app/dump.log");

    std::ofstream outputFile("/app/output.txt");

    if (!logFile) {
        std::cout << "could not open log file\n";
        exit(1);
    } else if (!outputFile) {
        std::cout << "could not open output file for writing\n";
        exit(1);
    }

    // can0 ControlBus.dbc
    // can1 SensorBus.dbc
    // can2 TracktiveBus.dbc
    std::string line{};
    std::string timeStamp{};
    std::string interface{};
    std::string idAndPayloadStr{};

    while (std::getline(logFile, line)) {
        std::istringstream strStream(line);
        strStream >> timeStamp >> interface >> idAndPayloadStr;
        std::vector<std::string> idAndPayloadVec;
        std::string token;

        std::istringstream idAndPayloadStream(idAndPayloadStr);
        while (std::getline(idAndPayloadStream, token, '#')) {
            idAndPayloadVec.push_back(token);
        }

        if (interface == "can0") {
            parseData(controlMessages, idAndPayloadVec, timeStamp, outputFile);
        } else if (interface == "can1") {
            parseData(sensorMessages, idAndPayloadVec, timeStamp, outputFile);
        } else if (interface == "can2") {
            parseData(tracktiveMessages, idAndPayloadVec, timeStamp, outputFile);
        }
        std::cout << std::flush;
    }

    logFile.close();
    outputFile.close();

    return 0;
}

// This function loads the can frame structure for the specified DBC file,
// using an unordered map to store it. I basically just ripped this code
// straight out of the DBCPPP example.
std::pair<
    std::unique_ptr<dbcppp::INetwork>,
    std::unordered_map<uint64_t, const dbcppp::IMessage *>
> loadCanFrames(
        const std::string &path
) {
    std::unique_ptr<dbcppp::INetwork> net;
    {
        std::ifstream idbc(path);
        net = dbcppp::INetwork::LoadDBCFromIs(idbc);
    }

    if (net.get() == nullptr) {
        std::cerr << "failed to parse!\n";
        exit(-1);
    }

    std::unordered_map<uint64_t, const dbcppp::IMessage *> messages;
    for (const dbcppp::IMessage& msg : net->Messages()) {
        messages.insert(std::make_pair(msg.Id(), &msg));
    }

    return {std::move(net), std::move(messages)};
}

// This function parses the data and writes it to the output file.
void parseData(
        std::unordered_map<uint64_t, const dbcppp::IMessage *> &messageMap,
        std::vector<std::string> idAndPayloadVec,
        const std::string &timeStamp,
        std::ofstream &outputFile
) {
    uint64_t id{};
    id = std::stoi(idAndPayloadVec[0], nullptr, 16);

    uint8_t data[8]{};

    std::string payload{};
    if (idAndPayloadVec.size() > 1) {
         payload = idAndPayloadVec[1];
        for (int i = 0; i < payload.length(); i += 2) {
            uint8_t byte = (convertToHex(payload[i]) << 4)
                | (convertToHex(payload[i + 1]));

            data[i / 2] = byte;
        }
    }

    auto findMsg = messageMap.find(id);

    if (findMsg != messageMap.end()) {
        const dbcppp::IMessage* msg = findMsg->second;
        std::string extractedData{};
        std::ostringstream valStream{};
        double value{};

        for (const dbcppp::ISignal& sig : msg->Signals()) {
            value = sig.RawToPhys(sig.Decode(data));
            // get formatted val
            if (value == 0) {
                valStream << "0";
            } else if (value == std::floor(value)) {
                valStream << static_cast<int>(value);
            } else {
                valStream << std::fixed << std::setprecision(1)
                    << sig.RawToPhys(sig.Decode(data));
            }

            extractedData = timeStamp + ": "
                + sig.Name() + ": "
                + valStream.str() + "\n";

            outputFile << extractedData;

            valStream.str("");
            valStream.clear();
        }
    }
}

// converts the char to its specified hex val using the ascii representation.
uint8_t convertToHex(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return 10 + (c - 'A');
    if (c >= 'a' && c <= 'f') return 10 + (c - 'a');
    throw std::invalid_argument("Invalid hex character");
}
