#ifndef CLUSTER_H
#define CLUSTER_H

#include <iostream>
#include <set>
#include <cstdlib>
#include <vector>

#include "Subspace.h"

using namespace std;

class Cluster
{
private:
    string name = "";
    set<int> ids;
    bool noise = false;
    Subspace subspace;
    vector<double> mean;

public:
    // Constructor
    Cluster(string name, set<int> &ids, bool noise, Subspace &subspace, vector<double> &mean);

    // Returns the number of ids inside the cluster
    int size();

    // Getters
    string &getName();
    set<int> &getIds();
    bool isNoise();
    Subspace &getSubspace();

    // Setters
    void setName(string &name);
    void setIds(set<int> &ids);
    void setNoise(bool noise);
    void setSubspace(Subspace &subspace);

    // Displays the Cluster details
    void print();
    bool isValid();
};

#endif
