#include "SUBCLU.h"

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <iterator>
#include <algorithm>
#include <string>

#include "Cluster.h"
#include "Subspace.h"
#include "Relation.h"
#include "DBSCAN.h"
#include "ReadInput.h"

using namespace std;

SUBCLU::SUBCLU(string filename, int minPts, double epsilon, int minDim)
{
	this->minPnts = minPts;
	this->epsilon = epsilon;
	this->minDim = minDim;

	ReadInput reader(filename);
	this->dataBase = reader.read();

	for (int i = 0; i < (int)(dataBase.size()); i++)
	{
		this->dbids[dataBase[i]] = i;
	}

	(this -> Clusterings).clear();
}

map<Subspace, vector<Cluster>> SUBCLU::run()
{
	int size = dataBase[0].size();
	vector<Subspace> subspaces;
	if (size <= 1)
	{
		cout << "Error: SUBCLU needs Multivariate Data";
		return (this -> Clusterings);
	}
	for (int dimension = 0; dimension < size; dimension++)
	{
		Subspace currSubspace(dimension);
		DBSCAN dbscan(dataBase, currSubspace, epsilon, minPnts, dbids);
		vector<Cluster> clusters = dbscan.getClusters();
		if (!clusters.empty())
		{
			(this -> Clusterings).insert({currSubspace, clusters});
			subspaces.push_back(currSubspace);
		}
	}
	
	for (int dimensionality = 2; dimensionality <= size; dimensionality++)
	{
		vector<Subspace> candidates = generateSubspaceCandidates(subspaces);
		vector<Subspace> sub_d;

		for (Subspace candidate : candidates)
		{
			Subspace bestSubspace = besttSubspace(subspaces, candidate);
			vector<Cluster> clusters;

			for (auto cluster : Clusterings[bestSubspace])
			{
				if (cluster.size() >= this->minPnts)
				{
					vector<Cluster> candidateClusters = runDBSCAN(candidate, cluster.getIds());
					if (!candidateClusters.empty())
					{
						for (auto currCluster : candidateClusters)
							clusters.push_back(currCluster);
					}
				}
			}

			if (!clusters.empty())
			{
				sub_d.push_back(candidate);
				Clusterings.insert({candidate, clusters});
			}
		}

		subspaces = sub_d;
	}

	// int numClusters = 0;
	// set<int> noise;

	// map<Subspace, vector<Cluster>> filtered;
	// for (auto it = Clusterings.rbegin(); it != Clusterings.rend(); it++)
	// {
	//     Subspace subspace = it->first;
	//     if (subspace.getDimensionality() < minDim)
	//         continue;
	// }
	return Clusterings;
}

vector<Subspace> SUBCLU::generateSubspaceCandidates(vector<Subspace> &subspaces)
{
	if (subspaces.empty())
		return vector<Subspace>();

	set<Subspace> candidates;
	int dimensionality = subspaces[0].getDimensionality() + 1;

	for (int i = 0; i < (int)(subspaces.size()); i++)
	{
		for (int j = 0; j < (int)(subspaces.size()); j++)
		{
			if(i != j)
			{
				Subspace candidate = subspaces[i].join(subspaces[j]);

				if (!candidate.isValid())
					continue;

				if (dimensionality == 2 || checkLower(candidate, subspaces))
					candidates.insert(candidate);
			}
		}
	}

	return vector<Subspace>(candidates.begin(), candidates.end());
}

bool SUBCLU::checkLower(Subspace &candidate, vector<Subspace> &subspaces)
{
	int size = candidate.getDimensions().size();

	Subspace candidateCopy = candidate;
	candidateCopy.getDimensionality()--;

	for (int dimension = 0; dimension < size; dimension++)
	{
		if (candidate.hasDimension(dimension))
		{
			vector<int>& ref = candidateCopy.getDimensions();

			if(dimension == size - 1)
				ref.pop_back();
			else
				ref[dimension] = 0;

			if(!candidateCopy.isSubspace(candidate))
				return false;

			if(dimension == size - 1)
				ref.push_back(1);
			else
				ref[dimension] = 1;
		}
	}
	return true;
}

Subspace SUBCLU::besttSubspace(vector<Subspace> &subspaces, Subspace &candidate)
{
	Subspace theBest;
	long min = (long)INT64_MAX;
	for (Subspace subspace : subspaces)
	{
		if (subspace.isSubspace(candidate))
		{
			int sum = 0;
			for (Cluster cluster : Clusterings[subspace])
				sum += cluster.size();
			if (sum < min)
			{
				min = sum;
				theBest = subspace;
			}
		}
	}

	return theBest;
}

vector<Cluster> SUBCLU::runDBSCAN(Subspace &currSubspace, set<int> &ids)
{
	Relation<double> points;
	for (auto i : ids)
	{
		points.push_back(dataBase[i]);
	}
	DBSCAN dbscan(points, currSubspace, epsilon, minPnts, dbids);
	return dbscan.getClusters();
}
