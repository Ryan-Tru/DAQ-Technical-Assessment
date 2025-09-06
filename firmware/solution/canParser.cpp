#include "canParser.h"

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
        throw std::runtime_error("Failed to parse\n");
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
            if (static_cast<int>(value) == 0) {
                valStream << "0";
            } else if (value == std::floor(value)) {
                valStream << static_cast<int>(value);
            } else {
                valStream << std::fixed << std::setprecision(1)
                    << value;
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
    throw std::invalid_argument("Invalid HEX character\n");
}
