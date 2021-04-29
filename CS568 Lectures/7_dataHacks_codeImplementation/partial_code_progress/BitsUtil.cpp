#include "BitsUtil.h"

#include <iostream>
#include <vector>

using namespace std;

vector<int> BitsUtil::orVectors(vector<int>& a, vector<int>& b) {
    vector<int> result;
    int n = min(a.size(), b.size());
    if (n == (int)a.size())
        result = b;
    else
        result = a;

    for (int i = 0; i < n; i++)
        result[i] = a[i] | b[i];
    return result;
}

int BitsUtil::intersection(vector<int>& a, vector<int>& b) {
    int n = min(a.size(), b.size());
    int cnt = 0;
    for (int i = 0; i < n; i++)
        if (a[i] & b[i])
            cnt++;
    return cnt;
}
