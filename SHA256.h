#ifndef SHA256_H
#define SHA256_H

#include <string>
#include <sstream>
#include <iomanip>
#include <cstdint>
using namespace std;

class SHA256 {
public:
    static string hash(const string& input);
private:
    static uint32_t _rotr(uint32_t x,int n){return (x >> n)|(x << (32-n));}
};

#endif
