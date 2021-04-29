#include "DBSCAN.h"

#include <assert.h>

#include <cmath>
#include <iostream>
#include <queue>
#include <vector>
#include <map>
#include <set>

#include "Cluster.h"
#include "Relation.h"
#include "Subspace.h"

DBSCAN::DBSCAN(Relation<double> points, Subspace subspace, double eps, uint minPts, map<vector<double>, int> ids) : m_points(points), m_eps(eps), m_minPts(minPts), m_ids(ids)
{
    m_subspace = subspace;
    m_numPoints = points.size();
    m_clusterIDs.clear();
    m_clusters.clear();
}
DBSCAN::~DBSCAN() {}

Subspace DBSCAN::getSubspace() { return m_subspace; }

double DBSCAN::getEps() { return m_eps; }

uint DBSCAN::getMinPts() { return m_minPts; }

Relation<double> DBSCAN::getPoints() { return m_points; }

uint DBSCAN::getNumPoints() { return m_numPoints; }

vector<Cluster> DBSCAN::getClusters()
{
    if (m_clusters.empty())
    {
        int clusterID = 1;
        m_clusterIDs.resize(m_numPoints, UNCLASSIFIED);
        for (uint i = 0; i < m_numPoints; ++i)
        {
            if (m_clusterIDs[i] == UNCLASSIFIED)
            {
                if (expandCuster(i, clusterID) == SUCCESS)
                {
                    ++clusterID;
                }
            }
        }
        --clusterID;
        int dim = m_points[0].size();
        if (clusterID > 0)
        {
            vector<vector<int>> cluster_ids(clusterID);
            vector<vector<double>> mean(clusterID, vector<double>(dim, 0));
            for (int i = 0; i < (int)m_numPoints; ++i)
            {
		if(m_clusterIDs[i] - 1 >= 0)
		{
			cluster_ids[m_clusterIDs[i] - 1].push_back(m_ids[m_points[i]]);
			for (int j = 0; j < dim; ++j)
			    mean[m_clusterIDs[i] - 1][j] += m_points[i][j];
		}
            }
            for (int i = 0; i < clusterID; ++i)
            {
                set<int> s(cluster_ids[i].begin(), cluster_ids[i].end());
                for (int j = 0; j < dim; ++j)
                    mean[i][j] /= cluster_ids.size();
                Cluster cluster("Cluster" + to_string(i), s, false, m_subspace, mean[i]);
                m_clusters.push_back(cluster);
            }
        }
    }
    return m_clusters;
}

int DBSCAN::expandCuster(int idx, uint clusterID)
{
    vector<int> seeds = rangeQuery(m_points[idx]);
    if (seeds.size() < m_minPts)
    {
        m_clusterIDs[idx] = NOISE;
        return FAILURE;
    }
    else
    {
        queue<int> qseeds;
        for (uint i = 0; i < seeds.size(); ++i)
        {
            m_clusterIDs[seeds[i]] = clusterID;
            if (seeds[i] != idx)
            {
                qseeds.push(seeds[i]);
            }
        }

        while (!qseeds.empty())
        {
            int currentP = qseeds.front();
            vector<int> result = rangeQuery(m_points[currentP]);
            if (result.size() >= m_minPts)
            {
                for (int resultP : result)
                {
                    if (m_clusterIDs[resultP] == UNCLASSIFIED ||
                        m_clusterIDs[resultP] == NOISE)
                    {
                        if (m_clusterIDs[resultP] == UNCLASSIFIED)
                        {
                            qseeds.push(resultP);
                        }
                        m_clusterIDs[resultP] = clusterID;
                    }
                }
            }
            qseeds.pop();
        }
        return SUCCESS;
    }
}

vector<int> DBSCAN::rangeQuery(vector<double> p)
{
    vector<int> result;
    for (int i = 0; i < (int)m_numPoints; ++i)
    {
        if (dist(p, m_points[i]) <= m_eps)
        {
            result.push_back(i);
        }
    }
    return result;
}

double DBSCAN::dist(vector<double> p1, vector<double> p2)
{
    double d = 0;
    for (uint i = 0; i < m_subspace.getDimensions().size(); ++i)
    {
        if (m_subspace.hasDimension(i))
        {
            d += ((long double)p1[i] - p2[i]) * (p1[i] - p2[i]);
        }
    }
    return sqrt(d);
}

vector<double> DBSCAN::getMean(vector<int> &v)
{
    assert(!v.empty());
    vector<double> p = m_points[v[0]];
    for (uint i = 1; i < v.size(); ++i)
    {
        for (uint j = 0; j < p.size(); ++j)
        {
            p[j] += m_points[v[i]][j];
        }
    }
    for (uint i = 0; i < p.size(); ++i)
    {
        p[i] /= v.size();
    }
    return p;
}
