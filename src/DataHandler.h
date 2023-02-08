#ifndef DATA_HANDLER
#define DATA_HANDLER

#include "Utils.h"

using namespace types;

template<typename T>
class DataHandler {
private:
    /* data */
    parseScema(std::string schema);
public:
    /* constructors */
    DataHandler(std::string path);
    DataHandler(std::string schema, types::Database<T> db);

    /* destructor*/
    ~DataHandler();
};

#endif // DATA_HANDLER