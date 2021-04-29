#include "Subspace.h"

#include <algorithm>
#include <iostream>
#include <vector>

#include "BitsUtil.h"

using namespace std;

Subspace::Subspace(int dimension)
{
    (this->dimensions).assign(dimension + 1, 0);
    (this->dimensions)[dimension] = 1;
    this->dimensionality = 1;
}

Subspace::Subspace()
{
    this->dimensions = vector<int>();
    this->dimensionality = 0;
}

Subspace::Subspace(vector<int> &dimensions)
{
    this->dimensions = dimensions;
    this->dimensionality = count(dimensions.begin(), dimensions.end(), 1);
}

vector<int> &Subspace::getDimensions()
{
    return this->dimensions;
}

int& Subspace::getDimensionality()
{
    return this->dimensionality;
}



Subspace Subspace::join(Subspace &other)
{
    vector<int> newDimensions = joinLastDimensions(other);
    return Subspace(newDimensions);
}

vector<int> Subspace::joinLastDimensions(Subspace &other)
{
    vector<int> result;
    if ((this->dimensionality) != other.dimensionality)
        return vector<int>();


    if ((this->dimensions).size() >= (other.dimensions).size())
        return vector<int>();

    if ((this->dimensionality) == 1)
        return BitsUtil::orVectors(this->dimensions, other.dimensions);
    result = BitsUtil::orVectors(this->dimensions, other.dimensions);

    if (count(result.begin(), result.end(), 1) != this->dimensionality + 1)
        return vector<int>();

    return result;
}

bool Subspace::isSubspace(Subspace &subspace)
{
    if ((this->dimensionality) <= subspace.dimensionality)
    {
        if ((this->dimensionality) == BitsUtil::intersection(this->dimensions, subspace.dimensions))
            return true;
        return false;
    }
    return false;
}

bool Subspace::hasDimension(int i)
{
    if (i < (int)((this->dimensions).size()))
    {
        return (this->dimensions)[i];
    }
    return false;
}

void Subspace::print()
{
    cout << "dimensionality = " << this->dimensionality << '\n';
    cout << "dimensions = ";
    for (auto x : this->dimensions)
        cout << x;
    cout << '\n';
}

bool Subspace::isValid()
{
    return ((this->dimensionality) != 0 && !(this->dimensions).empty());
}

bool Subspace::operator<(const Subspace &s2) const
{
    if (dimensionality < s2.dimensionality)
    {
        return true;
    }
    else if (dimensions.size() < s2.dimensions.size())
    {
        return true;
    }
    else
    {
        for (int i = 0; i < (int)(dimensions.size()); i++)
        {
            if ((dimensions[i] + s2.dimensions[i]) == 1)
            {
                if (dimensions[i] == 1)
                    return false;
                else
                    return true;
            }
        }
    }
    return false;
}
