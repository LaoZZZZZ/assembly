#ifndef GENERATETABLE_H
#define GENERATETABLE_H
#include <unordered_map>
#include <math.h>
#include <algorithm>
#include <vector>
#include <cassert>
#include <iostream>
using std::unordered_map;
using std::vector;
/**
  * Generate the mapping between prefix group and rank
  * Currently it has two modes. One is deteministic, the other is random
*/
class generateTable
{
public:
    generateTable();
    /**
      * give a deteministic mapping,
      * Basically, it map the prefix from smaller to larger rank nodes
    */
    static std::unordered_map<int,int>  fixedMapping(int np,int prefLen){
        assert(np > 1);
        std::unordered_map<int,int> table;
        int totalPrefix = std::pow(4,prefLen);
        int groupza = totalPrefix/(np-1);
        if(groupza == 0)
            groupza = 1;
        // the rank 0 process is the master node,which will not be involved to the
        for(int i = 0; i < totalPrefix; i++){
            int mygroup = i/groupza;
            table[i] = mygroup % (np -1) + 1;

        }
        return table;

    }
    /**
      * random assign the prefix to the nodes
      * it take effects only when number of nodes is less than the number of possible prefiex groups
    */
    static std::unordered_map<int,int> randomMapping(int np,int prefLen,int seed){
        assert(np > 1);
        std::unordered_map<int,int> table;
        int totalPrefix = pow(4,prefLen);
        int groupza = totalPrefix/(np-1);
        if(groupza == 0)
            groupza = 1;
        vector<int> groups;
        for(int i = 1; i < np; i++)
                groups.push_back(i);
        std::shuffle(groups.begin(),groups.end(),std::default_random_engine(seed));
        for(int i = 0;i < totalPrefix; i++){
            int mygroup = i/groupza;
            table[i] = groups[mygroup %(np-1)];
        }
        return table;
    }
    static int myseed(int seed){return seed;}
};

#endif // GENERATETABLE_H
