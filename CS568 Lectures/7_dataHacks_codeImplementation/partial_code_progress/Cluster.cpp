#include "Cluster.h"

#include <iostream>
#include <set>
#include <vector>

#include "Subspace.h"

using namespace std;

Cluster::Cluster(string name, set<int>& ids, bool noise, Subspace& subspace, vector<double>& mean) {
    this->name = name;
    this->ids = ids;
    this->noise = noise;
    this->subspace = subspace;
    this->mean = mean;
}

int Cluster::size() {
    return (this->ids).size();
}

string& Cluster::getName() {
    return this->name;
}

set<int>& Cluster::getIds() {
    return this->ids;
}

bool Cluster::isNoise() {
    return this->noise;
}

Subspace& Cluster::getSubspace() {
    return this->subspace;
}

void Cluster::setName(string& name) {
    this->name = name;
}

void Cluster::setIds(set<int>& ids) {
    this->ids = ids;
}

void Cluster::setNoise(bool noise) {
    this->noise = noise;
}

void Cluster::setSubspace(Subspace& subspace) {
    this->subspace = subspace;
}

void Cluster::print() {
    cout << "name = " << this->name << '\n';
    cout << "ids = ";
    for (auto x : this->ids)
        cout << x << ' ';
    cout << '\n';
    cout << "noise = " << this->noise << '\n';
    (this->subspace).print();
    cout << "mean = ";
    for (auto x : this->mean)
        cout << x << ' ';
    cout << '\n';
}

bool Cluster::isValid() {
    return (this->subspace).isValid() && !(this->mean).empty();
}
