#include <iostream>
#include <utility>
#include <algorithm>
#include <time.h>
#include <array>
#include <list>

// Determine if there is a path from one specified node to another
// Each node is made up of a pair of numbers that are the edges to other nodes

using std::cout;
using std::endl;

const int ARRAY_SIZE=5;
const int EDGE_RANGE=100;

typedef std::pair<int,int> intPair;
typedef std::list< intPair > adjacencyList;

// pair output 
std::ostream& operator<<(std::ostream &os, const intPair &p)
{
    os << "<" << p.first << "," << p.second << ">";
    return os;
}

// adjacency list output
std::ostream& operator<<(std::ostream &os, const adjacencyList &aList)
{
    os << "[ ";
    if (!aList.empty())
        for (intPair p : aList)
        {
            os << p << " ";
        }
    os << "]";
    return os;
}

// adjacency array output
std::ostream& operator<<( std::ostream &os, 
                          const std::array< adjacencyList, EDGE_RANGE > &adjacent)
{
    for (int i=0; i<EDGE_RANGE; ++i)
    {
        os << i << ": " << adjacent[i] << endl;
    }
    return os;
}

// creates an input array of values 
void 
initRandomPairArray( 
        std::array< intPair, ARRAY_SIZE > &nodeArray  // IN: array to be set
        )
{
     // Create random array of pairs
     for (int i = 0; i< ARRAY_SIZE; ++i)
     {
         int first = rand() % EDGE_RANGE;
         int second = rand() % EDGE_RANGE;
         while (first == second) // lets make sure the first and second values are different
             second = rand() % EDGE_RANGE;
 
         nodeArray[i].first = first;
         nodeArray[i].second = second;
     }
}

void    
createEdgeList( std::array< intPair, ARRAY_SIZE > &nodeArray,       // IN: array of nodes
                std::array< adjacencyList, EDGE_RANGE > &adjacent ) // OUT: resulting list
{

    // Create an array which maps a value to a node or nodes in a list of
    // pair <value, node-number>
    // (using array and possibly wasting space but its a fast way to create this list, alternative is a map)
    for (int i = 0; i< ARRAY_SIZE; ++i)
    {
        adjacent[nodeArray[i].first].emplace_back(i, nodeArray[i].second);
        adjacent[nodeArray[i].second].emplace_back(i, nodeArray[i].first);
    }
}

bool
findPath(   std::array< adjacencyList, EDGE_RANGE> &adjacent,   // IN adjacency array 
            std::array< intPair, ARRAY_SIZE > &nodeArray,       // IN node array
            int startNodeNumber,                                // IN where to start
            int endNodeNumber)                                  // IN where to end
{
    // start a list of edges to scan
    std::list<int> toScan;
    toScan.emplace_back(nodeArray[startNodeNumber].first);
    toScan.emplace_back(nodeArray[startNodeNumber].second);

    // use this array a way to avoid cycles
    std::array<bool,ARRAY_SIZE> seen;
    seen.fill(false);
    seen[startNodeNumber]=true;

    // walk through the graph of nodes and edges
    // for this edge, look at its list of adjacent nodes
    auto scan = toScan.begin();
    while (scan != toScan.end())
    {
        int edge = *scan;
        adjacencyList &current = adjacent[edge];

        // check the pair to see if we have found the endNode and if not
        // add the next edge into the list to scan if we haven't visited it before
        for (auto p : current)
        {
            // we can get to this node
            int nodeNumber = p.first; 
            if (nodeNumber == endNodeNumber)
            {   
                return true;
            }
            if (!seen[nodeNumber]) // avoid cycles
            {
                // push both its values on the toScan list
                toScan.emplace_back(nodeArray[nodeNumber].first);
                toScan.emplace_back(nodeArray[nodeNumber].second);
                // don't visit this node again
                seen[nodeNumber] = true;
            }
        }
        ++scan;
    }

    return false;
}

// NOTE quick driver for this problem; I wouldn't consider this a great unit test
int 
main(int argc, const char * argv[]) 
{
    // .TODO. add cmdline parsing
  
    // arrays we will use for this problem
    std::array <intPair, ARRAY_SIZE> nodeArray;
    std::array< adjacencyList, EDGE_RANGE > adjacent;
    
    // init rand 
    srand (time(NULL));

    // remember, this is a driver, not a good unit test!
    // repeat this a number of times
    for (int j; j<20; ++j)
    {
        // initialize the values
        initRandomPairArray( nodeArray );

        // create edge/adjacency list (an array of lists of pairs) 
        createEdgeList( nodeArray, adjacent );

        // choose the start, end nodes
        int startNode = rand() % ARRAY_SIZE;
        int endNode = rand() % ARRAY_SIZE;
        while (startNode==endNode) 
        {
            endNode = rand() % ARRAY_SIZE;
        }

        // output what we have
        auto print = [](const intPair p) {cout << " " << p;};
        cout << "Array: [ ";
        for_each (nodeArray.begin(), nodeArray.end(), print);
        cout << " ]" << endl;
        cout << "finding path from " << startNode << " to " << endNode << endl;

        cout << adjacent << endl;

        // find the path
        bool found = findPath( adjacent, nodeArray, startNode, endNode);
        if (found) 
            cout << "** Path is found **" << endl << endl; 
        else
            cout << "Path is not found" << endl << endl;

        // reset adjacent

        for (int i=0; i<EDGE_RANGE;++i)
        {
            adjacent[i].clear();
        }
//        cout << "***"  << adjacent << "***" << endl;        
    }
    return 1;
}
