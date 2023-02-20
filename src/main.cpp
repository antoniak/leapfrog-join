#include <iostream>
#include "DataHandler.h"
#include "LFTJ.h"

int main (int argc, char** argv) {
    auto dh = new DataHandler("../data/triangle/");
    LFTJ lftj(dh);
    std::cout << lftj.Count() << std::endl;
    return 0;
}