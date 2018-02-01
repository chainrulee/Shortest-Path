#include <iostream>
#include <list>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <algorithm>
#include <vector>
#include <climits>
#include <fstream>

using namespace std;
int V_max = 100;
int weight_max = 10;
int vnum[] = {100,500,1000};
int vnum_size = 3;
 
// Graph class represents a directed graph
// using adjacency list representation
class Graph
{
public: 
    int V;    // No. of vertices
    int E;    // No. of edges
 
    // Pointer to an array containing
    // adjacency lists
    list<pair<int, int> > *adj;
 
    Graph(int V);   // Constructor
 
    // function to add an edge to graph
    void addEdge(int v, int w, int weight);
    void printEdge();
    int numVertex();
};

int Graph::numVertex()
{
    return this->V;
}

Graph::Graph(int V)
{
    this->V = V;
    adj = new list<pair<int, int> >[V];
}
 
void Graph::addEdge(int v, int w, int weight)
{
    //cout << "construct edge: (" << v << ", " << w << ", " << weight << ")" << endl;
    adj[v].push_back(make_pair(w, weight)); // Add w to v’s list.
}

void Graph::printEdge()
{
    for (int k = 0; k < V; k++){
        for (list<pair<int, int> >::const_iterator i = adj[k].begin(); i != adj[k].end(); i++){
            cout << k << "," << i->first << "," << i->second;
            cout << endl;
        }
    }
}

vector<int> randSample(int E, int E_max)
{
    vector<int> myVector;
    for (int i = 0; i < E_max; i++){
        myVector.push_back(i);
    }
    random_shuffle(myVector.begin(), myVector.end());
    vector<int> randVector(myVector.begin(), myVector.begin() + E);
    return randVector;
}

Graph genGraph(int V, int E, int E_max)
{
    srand(time(NULL));
    //int V = rand() % (V_max-1) + 2;
    cout << "V = " << V << endl;
    
    //generate graph
    Graph g(V);
    //int E_max = V * V - V;
    //int E = rand() % E_max + 1;
    cout << "E = " << E << endl;
    g.E = E;
    
    //random sample edges
    vector<int> Edges = randSample(E, E_max);
    /*cout << "Edges = ";
    for (vector<int>::const_iterator i = Edges.begin(); i != Edges.end(); i++){
        cout << *i << " ";
    }
    cout << endl;*/

    //construct edges
    for (int i = 0; i < Edges.size(); i++){
        int v = Edges[i]/(V-1);
        int w = Edges[i]%(V-1);
        if (w >= v) w++;
        int weight = rand() % weight_max + 1;
        g.addEdge(v, w, weight);
    }
    return g;
}

int minDistance(int dist[], bool sptSet[], int V)
{
    // Initialize min value
    int min = INT_MAX, min_index;
  
    for (int v = 0; v < V; v++)
    {
        if (sptSet[v] == false && dist[v] <= min)
            min = dist[v], min_index = v;
    }
    return min_index;
}

int printSolution(int dist[], int V)
{
    cout<<"Vertex   Distance from Source"<<endl;
    for (int i = 0; i < V; i++)
        cout<<i<<"\t "<<dist[i]<<endl;
        //printf("%d \t\t %d\n", i, dist[i]);
}

float dijkstra(Graph g, int src)
{
    clock_t t1, t2;
    t1 = clock();
    int V = g.numVertex();
    int dist[V];     // The output array.  dist[i] will hold the shortest
                     // distance from src to i
  
    bool sptSet[V]; // sptSet[i] will be true if vertex i is included in shortest
                    // path tree or shortest distance from src to i is finalized
  
    // Initialize all distances as INFINITE and stpSet[] as false
    for (int i = 0; i < V; i++)
        dist[i] = INT_MAX, sptSet[i] = false;
   
    // Distance of source vertex from itself is always 0
    dist[src] = 0;
  
    // Find shortest path for all vertices
    for (int count = 0; count < V-1; count++)
    {
        // Pick the minimum distance vertex from the set of vertices not
        // yet processed. u is always equal to src in first iteration.
        int u = minDistance(dist, sptSet, V);
  
        // Mark the picked vertex as processed
        sptSet[u] = true;
  
        if (dist[u] == INT_MAX) break;
        // Update dist value of the adjacent vertices of the picked vertex.
        for (list<pair<int, int> >::const_iterator i = g.adj[u].begin(); i != g.adj[u].end(); i++)
        {
            int v = i->first;
            // Update dist[v] only if is not in sptSet, there is an edge from 
            // u to v, and total weight of path from src to  v through u is 
            // smaller than current value of dist[v]
            if (!sptSet[v] && dist[u]+ i->second < dist[v])
                dist[v] = dist[u] + i->second;
        }
    }
    t2 = clock();
    // print the constructed distance array
    printSolution(dist, V);
    float diff = ((float)t2-(float)t1);
    return diff / CLOCKS_PER_SEC;
}

// The main function that finds shortest distances from src to
// all other vertices using Bellman-Ford algorithm.  The function
// also detects negative weight cycle
float BellmanFord(Graph g, int src)
{
    clock_t t1, t2;
    t1 = clock();
    int V = g.numVertex();
    int E = g.E;
    int dist[V];
 
    // Step 1: Initialize distances from src to all other vertices
    // as INFINITE
    for (int i = 0; i < V; i++)
        dist[i] = INT_MAX;
    dist[src] = 0;
 
    // Step 2: Relax all edges |V| - 1 times. A simple shortest 
    // path from src to any other vertex can have at-most |V| - 1 
    // edges
    for (int i = 1; i <= V-1; i++)
    {
        int change = 0;
        for (int u = 0; u < V; u++)
        {
            if (dist[u] == INT_MAX) continue;
            for (list<pair<int, int> >::const_iterator i = g.adj[u].begin(); i != g.adj[u].end(); i++)
            {
                int v = i->first;
                int weight = i->second;
                if (dist[u] + weight < dist[v]){
                    dist[v] = dist[u] + weight;
                    change = 1;
                }
            }
        }
        if (!change) break;
    }
 
    // Step 3: check for negative-weight cycles.  The above step 
    // guarantees shortest distances if graph doesn't contain 
    // negative weight cycle.  If we get a shorter path, then there
    // is a cycle.
    /*for (int i = 0; i < E; i++)
    {
        int u = graph->edge[i].src;
        int v = graph->edge[i].dest;
        int weight = graph->edge[i].weight;
        if (dist[u] != INT_MAX && dist[u] + weight < dist[v])
            printf("Graph contains negative weight cycle");
    }*/
    
    t2 = clock();
    printSolution(dist, V);
    float diff = ((float)t2-(float)t1);
    return diff / CLOCKS_PER_SEC;
}

int selectAlgo(int V, int E)
{
    if (V < 250){
        if (E > 0.0239*V*V + 0.0101*V + 29.381) return -1;
        else return 1;
    }
    else {
        if (E > 0.0101*V*V + 22.618*V - 2471.4) return -1;
        else return 1;
    }
}
 
int main()
{
    float seconds, seconds2;
    int correct = 0, wrong = 0;
    ofstream outputFile;
    outputFile.open("output.csv");
    outputFile << "V,E,Dijkstra,Bellman-Ford,Which is faster"<<endl; 
    for (int i = 0; i < vnum_size; i++){
        int V = vnum[i];
        int E_max = V * V - V;
        for (int E = 50; E <= 50*V; E+=5*V){
            int predict = selectAlgo(V,E);
            outputFile << V << "," << E << ",";
            Graph g = genGraph(V, E, E_max);
            //g.printEdge();
            seconds = dijkstra(g, 0);
            cout<<"Run time : "<<seconds<<endl;
            seconds2 = BellmanFord(g, 0);
            cout<<"Run time : "<<seconds2<<endl;
            outputFile << seconds << "," << seconds2 << ",";
            if (seconds < seconds2){
                //cout<<"Dijkstra's Algorithm runs faster"<<endl;
                outputFile << "Dijkstra";
                if (predict == -1) correct++;
                else wrong++;
            }
            else if (seconds > seconds2){
                //cout<<"Bellman-Ford Algorithm runs faster"<<endl;
                outputFile << "Bellman-Ford";
                if (predict == 1) correct++;
                else wrong++;
            }
            else{
                //cout<<"The two algorithms use similar time!"<<endl;
                outputFile << "Even";
                correct++;
            }
            if (predict == -1){
                outputFile << ",Dijkstra" << endl;
            }
            else{
                outputFile << ",Bellman-Ford" << endl;
            }
        }
    }
    outputFile.close();
    float precision = float(correct)/(correct+wrong);
    cout<<"precision: "<<precision<<endl;
    return 0;
}