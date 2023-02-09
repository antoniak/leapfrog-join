#ifndef DATA_HANDLER
#define DATA_HANDLER

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
// #include <boost/foreach.hpp>

#include "Utils.h"

using namespace types;
using namespace boost;

class DataHandler {

public:
    /* data */
    std::vector<std::string> variables;
    std::vector<std::string> relations;
    std::unordered_map<int, std::vector<int>> relationsToVariables;
    std::unordered_map<int, std::vector<int>> variablesToRelations;
    std::vector<int>  variableOrder;
    Database<size_t> database;

    /* constructors */
    DataHandler(std::string path);
    DataHandler(std::string schema, types::Database<size_t> db);

    /* destructor */
    ~DataHandler();

private:

    void parseSchema(std::string path);
    void readDatabase();

};

#endif // DATA_HANDLER