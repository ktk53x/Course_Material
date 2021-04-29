#include <iostream>
#include <set>
#include <vector>

#include "DBSCAN.h"
#include "Cluster.h"
#include "ReadInput.h"
#include "Subspace.h"
#include "SUBCLU.h"

using namespace std;

int main() {

    SUBCLU subclu("data.csv", 1, 70, 1);

    map<Subspace, vector<Cluster>> res = subclu.run();
    
    for(auto x : res)
    {
	    cout << "MAP()____________\n";
	    for(auto y : x.second)
	    {
		    y.print();
		    cout << "-------------------------------------\n";
	    }
	    cout << "////////////////////////////////////////////\n";
    }
  

    

    return 0;
}
