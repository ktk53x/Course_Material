#ifndef BITSUTIL_H
#define BITSUTIL_H

#include <iostream>
#include <vector>

using namespace std;

class BitsUtil {
   public:
    // Returns OR of two vectors
    static vector<int> orVectors(vector<int>& a, vector<int>& b);

    // Returns the intersection count
    static int intersection(vector<int>& a, vector<int>& b);
};

#endif
