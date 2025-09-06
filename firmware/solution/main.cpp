#include <canParser.h>

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
