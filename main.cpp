#include<cstdio>
#include<algorithm>
#include<iostream>
#include <list>
#include <fstream>
#include <time.h>
#define NIL -1
using namespace std;
int j=0;//get the numbers of AP
int edge=0;// the sum of edges remain as m
int vertex=0;// the sum of vertices
// A class that represents an undirected graph
int root[100],rankk[100];
//初始化n个元素

class Graph
{
    int V;    // No. of vertices
    list<int> *adj;    // A dynamic array of adjacency lists
    void APUtil(int v, bool visited[], int disc[], int low[],
                int parent[], bool ap[]);
public:
    Graph(int V);   // Constructor
    void addEdge(int v, int w);   // function to add an edge to graph
    void deleteVertex(int v,bool* valid); //function to delete an edge in graph
    void AP(int* A,bool* valid);    // prints articulation points
    void printGraph(int V,bool* valid);         // prints the remainder giant graph
    //void UnionFind(bool* valid);    // get the maximum connected subgraph(s)
};

Graph::Graph(int V)//default
{
    this->V = V;
    adj = new list<int>[V];
}

void Graph::addEdge(int v, int w)
{
    adj[v].push_back(w);
    adj[w].push_back(v);  // Note: the graph is undirected
}

void Graph::deleteVertex(int v,bool* valid)
{
    list<int>::iterator i;
    for (i = adj[v].begin(); i != adj[v].end(); ++i)
    {
        int w = *i;
        //cout<<v<<' '<<w<<endl;
        adj[w].remove(v);//let other vertex forget it
    }
    adj[v].clear();//clear itself with other vertices
    valid[v]=false;
    // Note: the graph is undirected
}

// A recursive function that find articulation points using DFS traversal
// u --> The vertex to be visited next
// visited[] --> keeps tract of visited vertices
// disc[] --> Stores discovery times of visited vertices
// parent[] --> Stores parent vertices in DFS tree
// ap[] --> Store articulation points
void Graph::APUtil(int u, bool visited[], int disc[],
                   int low[], int parent[], bool ap[])
{
    // A static variable is used for simplicity, we can avoid use of static
    // variable by passing a pointer.
    static int time = 0;

    // Count of children in DFS Tree
    int children = 0;

    // Mark the current node as visited
    visited[u] = true;

    // Initialize discovery time and low value
    disc[u] = low[u] = ++time;

    // Go through all vertices aadjacent to this
    list<int>::iterator i;
    for (i = adj[u].begin(); i != adj[u].end(); ++i)
    {
        int v = *i;  // v is current adjacent of u

        // If v is not visited yet, then make it a child of u
        // in DFS tree and recur for it
        if (!visited[v])
        {
            children++;
            parent[v] = u;
            APUtil(v, visited, disc, low, parent, ap);

            // Check if the subtree rooted with v has a connection to
            // one of the ancestors of u
            low[u]  = min(low[u], low[v]);

            // u is an articulation point in following cases

            // (1) u is root of DFS tree and has two or more chilren.
            if (parent[u] == NIL && children > 1)
            {
                ap[u] = true;
                //cout<<"root is "<<u<<" it has "<<children<<" children"<<endl;
            }
            // (2) If u is not root and low value of one of its child is more
            // than discovery value of u.
            if (parent[u] != NIL && low[v] >= disc[u])
            {
                ap[u] = true;
                //cout<<"leaf is "<<u<<" it has "<<children<<" children"<<endl;
            }
        }

        // Update low value of u for parent function calls.
        else if (v != parent[u])
            low[u]  = min(low[u], disc[v]);
    }
    //cout<<"point is "<<u<<" it has "<<children<<" children"<<endl;
}

// The function to do DFS traversal. It uses recursive function APUtil()
void Graph::AP(int* A,bool* valid)
{
    j=0;
    // Mark all the vertices as not visited
    bool *visited = new bool[V];
    int *disc = new int[V];
    int *low = new int[V];
    int *parent = new int[V];
    bool *ap = new bool[V]; // To store articulation points

    // Initialize parent and visited, and ap(articulation point) arrays
    for (int i = 0; i < V; i++)
    {
        parent[i] = NIL;
        visited[i] = false;
        ap[i] = false;
    }

    // Call the recursive helper function to find articulation points
    // in DFS tree rooted with vertex 'i'
    for (int i = 0; i < V; i++)
        if (visited[i] == false)
            APUtil(i, visited, disc, low, parent, ap);

    // Now ap[] contains articulation points, print them
    for (int i = 0; i < V; i++)
        if (ap[i] == true&&valid[i]==true)
        {
            //cout << i << " ";
            A[j++]=i;
        }
}

void init(int n, bool* valid)
{
    for(int i=0; i<n; i++)
    {
        if(valid[i]==true)
        {
            root[i]=i;
            rankk[i]=0;
        }
//        else
//        {
//            root[i]=-1;
//            rankk[i]=-1;
//        }
    }
}
//查询树的根
int find(int x)
{
    if(root[x]==x)
    {
        return x;
    }
    else
    {
        return root[x]=find(root[x]);
    }
}
//合并x和y所属的集合
void unite(int x,int y)
{
    x=find(x);
    y=find(y);
    if(x==y)
        return;
    if(rankk[x]<rankk[y])
    {
        root[x]=y;
    }
    else
    {
        root[y]=x;
        if(rankk[x]==rankk[y])
        {
            rankk[x]++;
        }
    }
}

void Graph::printGraph(int V,bool* valid)
{
    edge=0;
    vertex=0;
    cout<<"This is the output of the remainder graph"<<endl;
    for (int i = 0; i < V; i++)
    {
        if(valid[i]==true)
        {
            vertex++;
            cout<<"This is the point "<<i<<" along with its edges"<<endl;
            list<int>::iterator k;
            for (k = adj[i].begin(); k != adj[i].end(); ++k)
            {
                int w=*k;
                //union start
                unite(i,w);
                edge++;
                cout<<i<<" to "<<w<<endl;
            }
        }
    }
}

void print(int* A)
{
    cout<<"This is the output of AP points array"<<endl;
    for(int i=0; i<j; i++)
        cout<<A[i]<<' ';
    cout<<endl;
}

int main()
{
    clock_t start,off;
    // Create graphs given in above diagrams
    ifstream in;
    start=clock();
    in.open("first.txt");
    int num;
    in>>num;
    Graph g(num);
    bool* valid=new bool[num];
    for(int i=0; i<num; i++)
        valid[i]=true;
    int* array=new int[num];//an array to store all the AP
    int edge1,edge2;
    while(!in.eof())
    {
        in>>edge1;
        in>>edge2;
        g.addEdge(edge1,edge2);
    }
    g.AP(array,valid);
    print(array);
    while(j!=0)
    {
        for(int i=0; i<j; i++)
        {
            g.deleteVertex(array[i],valid);
        }
        //find the maximum connected subgraph after deleting those AP points above
        init(num,valid);
        g.printGraph(num,valid);// for union find
        int sum1[100]= {0},sum2[100]= {0};
        int sum=0;
        int p=0;
        for (p = 0; p < num; p++)
        {
            if(valid[p]==true)
            {
                sum1[sum]=root[p];
                break;
            }
        }
        sum++;
        for(int i=0; i<num; i++)
        {
            if(i!=p&&valid[i]==true)
            {
                for(int l=0; l<sum; l++)
                {
                    if(root[i]==sum1[l])
                        break;
                    if(l==sum-1)
                    {
                        sum1[sum]=root[i];
                        sum++;
                    }
                }
            }
        }
        //cout<<"There are "<<sum<<" different clusters"<<endl;
        for(int i=0; i<sum; i++)
        {
            for(int l=0; l<num; l++)
            {
                if (valid[l]==true)
                {
                    if(root[l]==sum1[i])
                    {
                        sum2[i]++;
                    }
                }
            }
        }
        int maxx=0;
        for(int i=0; i<sum; i++)
        {
            if(maxx<=sum2[i])
                maxx=sum2[i];
        }
        cout<<"maxx is "<<maxx<<" sum is "<<sum<<endl;
        /* helper function
        for(int i=0; i<sum; i++)
        {
            cout<<sum1[i]<<' '<<sum2[i]<<endl;
        }
        for(int i=0;i<num;i++)
        {
            if(valid[i]==true)
                cout<<"i is "<<i<<" its parent is "<<root[i]<<endl;
        }
        */
        for(int i=0; i<sum; i++)
        {
            if(maxx>sum2[i])
            {
                for(int l=0; l<num; l++)
                    if(root[l]==sum1[i])
                    {
                        valid[l]=false;
                        g.deleteVertex(l,valid);
                    }
            }
            if(maxx==sum2[i])
                cout<<"This is the subgraph's root whom has "<<maxx<<" vertices: "<<sum1[i]<<endl;
        }
        g.AP(array,valid);
        print(array);
    }
    g.printGraph(num,valid);
    off=clock();
    cout<<"Runtime is "<<off-start<<" ms"<<endl;
    return 0;
}
