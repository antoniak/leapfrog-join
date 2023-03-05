#include <iostream>
#include <fstream>
#include <chrono>

#include "DataHandler.h"
#include "LFTJ.h"

using namespace std;

int triangle_instance(int n) {
    std::vector<std::string> filepaths = {"../data/triangle/R.tbl", "../data/triangle/S.tbl", "../data/triangle/T.tbl"};
    for (auto filepath: filepaths) {
        ofstream file;
        file.open (filepath);
        if(file.fail()){
            cerr << "Error opening " << filepath << endl;
            return 1;
        }
        string line;
        for (int i = 0; i < n; i++) {
            file << "0";
            file << " ";
            file << i;
            file << "\n";
        }
        for (int i = 0; i < n; i++) {
            file << i;
            file << " ";
            file << "0";
            file << "\n";
            if (i != n-1) {
                file << "\n";
            }
        }
        file.close();
    }
}
int main (int argc, char** argv) {
    for (int n = 10; n < 1000001; n = n*10) {
        
        triangle_instance(n);
        auto dh = new DataHandler("../data/triangle/");
        
        LFTJ lftj(dh);
        
        auto start = std::chrono::steady_clock::now();
        lftj.Compute();
        
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed_seconds = end-start;
        std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";

    } 
    
    return 0;
}