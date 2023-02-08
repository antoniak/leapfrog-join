#include "DataHandler.h"
#include "Utils.h"

using namespace utils;

DataHandler::DataHandler(std::string path) {
    if(utils::isDirectory(path.c_str())) { throw "Not a valid directory!"; } 

    std::ifstream input(path + "schema.json", std::ifstream::in);
    if (input) {
        parseSchema();
    } else {
        throw "Schema not found!"
    }
}

DataHandler::parseSchema() {

}

DataHandler::~DataHandler() {
}