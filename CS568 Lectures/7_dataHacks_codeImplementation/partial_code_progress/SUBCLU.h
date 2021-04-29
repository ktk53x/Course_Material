#ifndef SUBCLU_H
#define SUBCLU_H

#include <iostream>
#include <vector>
#include <cstdlib>
#include <map>

#include "Cluster.h"
#include "Subspace.h"
#include "Relation.h"
#include "DBSCAN.h"

class SUBCLU
{
protected:
    double epsilon;
    int minPnts;
    int minDim;
    Relation<double> dataBase;

    map<Subspace, vector<Cluster>> Clusterings; //Should we use sets instead of vector?
    map<vector<double>, int> dbids;

public:
    SUBCLU(string filename, int minpnts, double epsilon, int mindim = 1);
    map<Subspace, vector<Cluster>> run();
    vector<Cluster> runDBSCAN(Subspace &currSubspace, set<int> &ids);
    vector<Subspace> generateSubspaceCandidates(vector<Subspace> &subspaces);
    Subspace besttSubspace(vector<Subspace> &subspaces, Subspace &candidate);
    bool checkLower(Subspace &candidate, vector<Subspace> &subspaces);
};

#endif
