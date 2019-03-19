#include<cstdio>
#include<algorithm>
#include<iostream>
#include <list>
#include <fstream>
#include <time.h>
#include <stack>
#include <string.h>
#define NIL -1
using namespace std;
#define MAX_VERTEX_SIZE 301
int j=0;//get the numbers of AP
int edge=0;// the sum of edges remain as m
int vertex=0;// the sum of vertices
int large=0;//use to store the largest SCC(s) in the remainder graph at last
// A class that represents a directed graph
int root[100],rankk[100];
//初始化n个元素
int num=0;
int timee=0;//dfs order
int loww[MAX_VERTEX_SIZE];
int dfnn[MAX_VERTEX_SIZE];
int visit[MAX_VERTEX_SIZE];
int inStack[MAX_VERTEX_SIZE];
bool valid[MAX_VERTEX_SIZE];
int belong[MAX_VERTEX_SIZE];//calculate the element belong to what size of SCC
int scc=0;//count for # of strong connected component in graph
stack<int> st;
stack<int> big;
ofstream out;
class Graph
{
    int V;    // No. of vertices from 0 to V(actually V+1 as 0 is always a dummy one)
    int deletenum; //No. of deleted vertices
    list<int> *adj;    // A dynamic array of adjacency lists
    void APUtil(int v, bool visited[], int disc[], int low[],
                int parent[], bool ap[]);
public:
    Graph(int V);   // Constructor
    void addEdge(int v, int w);   // function to add an edge to graph
    void deleteVertex(int v,bool* valid); //function to delete an edge in graph
    void printGraph(int V,bool* valid);         // prints the remainder giant graph
    void tarjan(int u,int really);
    Graph(const Graph &graph);
    ~Graph();//free me
    //void UnionFind(bool* valid);    // get the maximum connected subgraph(s)
};

Graph::Graph(int V)//default
{
    this->V = V;
    this->deletenum=0;
    adj = new list<int>[V+1];
}

Graph::Graph(const Graph &graph)//deep copy
{
    this->V=graph.V;
    this->deletenum=graph.deletenum;
    //cout<<"V is "<<V<<endl;
    adj = new list<int>[V+1];
    for (int i = 0; i <= V; i++)
    {
        if(valid[i]==true)
        {
            list<int>::iterator k;
            for (k = graph.adj[i].begin(); k != graph.adj[i].end(); ++k)
            {
                addEdge(i,*k);
            }
        }
    }
}

Graph::~Graph()
{
    //cout<<"delete!"<<endl;
    free(adj);
}

void Graph::addEdge(int v, int w)
{
    adj[v].push_back(w);
    //adj[w].push_back(v);  // Note: the graph is directed
}

void Graph::deleteVertex(int v,bool* valid)
{
    if(valid[v]==false)
        cout<<"delete again!"<<endl;
    list<int>::iterator i;
    int k;
    for (k=0; k<=num; k++)
    {
        adj[k].remove(v);
    }
    adj[v].clear();//clear itself with other vertices
    valid[v]=false;
    deletenum++;
    // Note: the graph is directed
}

void Graph::printGraph(int V,bool* valid)
{
    edge=0;
    vertex=0;
    out<<"This is the output of the remainder graph"<<endl;
    for (int i = 0; i <= V; i++)
    {
        if(valid[i]==true)
        {
            //cout<<"vertex "<<i<<" belongs to a SCC with "<<belong[i]<<" elements"<<endl;
            vertex++;
            //out<<"This is the point "<<i<<" along with its edges"<<endl;
            list<int>::iterator k;
            for (k = adj[i].begin(); k != adj[i].end(); ++k)
            {
                int w=*k;
                edge++;
                //out<<i<<" to "<<w<<endl;
                out<<i<<' '<<w<<endl;
            }
        }
    }
    out<<"largest SCC has "<<large<<" vertices"<<endl;
    out<<"delete "<<deletenum<<" vertices"<<endl;
}

int min(int a,int b)
{
    if(a>b)
        return b;
    else
        return a;
}

void Graph::tarjan(int u,int really)//really 0->test; 1->regular; 2->keep largest SCCs
{
    //cout<<"exploring "<<u<<endl;
    int index=0;
    dfnn[u] = loww[u] = timee++;
    st.push(u);
    visit[u] = 1;
    inStack[u] = 1;
    list<int>::iterator v;
    if(!adj[u].empty())//no vertex out
    {
//        //cout<<"this node "<<u<<" doesn't has any edge out"<<endl;
//        if(really)
//            deleteVertex(u,valid);
//    }
//    else
//    {
        for(v = adj[u].begin(); v != adj[u].end(); ++v)
        {
            if(visit[*v] == 0)
            {
                tarjan(*v,really);
                //cout<<"we're comparing"<<loww[u]<<" and "<<loww[*v]<<endl;
                loww[u] = min(loww[u],loww[*v]);
            }
            else
            {
                if(inStack[*v]==1)
                    loww[u] = min(loww[u],dfnn[*v]);
            }
        }
    }

    if(dfnn[u] == loww[u])
    {
        int vtx;
        index=0;//refresh?
        int temp[num+1];//store scc elements
        //cout<<"set is: ";
        do
        {
            vtx = st.top();
            st.pop();
            inStack[vtx] = 0;//表示已经出栈
            temp[index]=vtx;
            index++;
        }
        while(vtx !=u );
        //cout<<"total is "<<index<<endl;
        if(index==1)//we don't need one point CC in real tarjan
        {
            //cout<<temp[0]<<' ';
            if(really)//it's not attempting!
            {
                cout<<"isolated point "<<temp[0]<<endl;
                deleteVertex(temp[0],valid);
                //valid[temp[0]]=false;
            }
            //deleteVertex(temp[0],valid);
            //graph.valid[temp[0]]=false;
        }
        else
        {
            if(really==2)//print SCC>=2 in the final graph
            {
                if(large<index)large=index;
                for(int indexx=0; indexx<index; indexx++)
                {
                    out<<temp[indexx]<<' ';
                    belong[temp[indexx]]=index;//very tricky!
                }
                out<<"end"<<endl;
            }
            scc++;//only take scc>=2 into account as good scc
        }
        //cout<<"end"<<endl;
    }
    if(really==0&&index==1)
        scc++;//accept in attempting to detect any possible APs
    //cout<<"vertex "<<u<<" dfn "<<dfn[u]<<' '<<" low "<<low[u]<<endl;
}

void print(int* A)
{
    out<<"This is the output of AP points array"<<endl;
    for(int i=0; i<j; i++)
        out<<A[i]<<' ';
    out<<endl;
}

int main()
{
    //int max_scc=0;//store largest number of elements among all SCCs
    clock_t start,off;
    ifstream in;
    start=clock();
    //in.open("first.txt");
    in.open("Edges_threshold.txt");
    out.open("rslt.txt");
    in>>num;
    Graph g(num);
    //initialization for valid, belong, and array
    for(int i=0; i<=num; i++)
        valid[i]=true;
    for(int i=0;i<=num;i++)
    {
        belong[i]=0;
    }
    int* array=new int[num];//an array to store all the AP
    int edge1,edge2;
    int back1,back2=0;
    while(!in.eof())
    {
        in>>edge1;
        in>>edge2;
        if (back1==edge1 &&back2==edge2)
        {
            break;
        }
        back1=edge1;//backup
        back2=edge2;//backup, avoid repeating
        g.addEdge(edge1,edge2);
    }
    int flag=0;// test case, set 1 to ignore
    //g.printGraph(num,valid);//initial graph
    while(j!=0||flag==0)//we want to get SCC first
    {
        cout<<"a new loop!"<<endl;
        if(j==0)
            flag=1;
        timee=0;
        while(!st.empty())//clear
        {
            st.pop();
        }
        for(int i=0; i<=num; i++)
        {
            visit[i]=0;
            dfnn[i]=0;
            loww[i]=0;
            inStack[i]=0;
        }
        for(int i=0; i<j; i++)
        {
            g.deleteVertex(array[i],valid);
        }
        j=0;//renew
        for(int i =0; i<=num; i++)
            if(visit[i] == 0&&valid[i])
            {
                g.tarjan(i,1);
            }
        int current_scc=scc;
        //cout<<"current scc is "<<current_scc<<endl;
        for(int k =0; k<=num; k++)
        {
            scc=0;
            if(valid[k])//test by deleting it first
            {
                //cout<<"We are trying to delete "<<k<<endl;
                Graph gg(g);
                gg.deleteVertex(k,valid);
                //gg.printGraph(num,valid);
                timee=0;
                while(!st.empty())//clear
                {
                    st.pop();
                }
                for(int i=0; i<=num; i++)
                {
                    visit[i]=0;
                    dfnn[i]=0;
                    loww[i]=0;
                    inStack[i]=0;
                }
                for(int i =0; i<=num; i++)
                    if(visit[i] == 0&&valid[i])
                    {
                        gg.tarjan(i,0);
                    }
                int temp_scc=scc;
                cout<<"after trying to delete "<<k<<" we get "<<temp_scc<<" before "<<current_scc<<endl;
                if(temp_scc>current_scc)
                {
                    cout<<"find one "<<k<<endl;
                    array[j++]=k;
                }
                valid[k]=true;//resume it!
            }
            else//this vertex is not valid for some reason
            {
                continue;
            }
        }
        print(array);
    }
    //The output is several SCCs without any articulation point
    timee=0;
    while(!st.empty())//clear
    {
        st.pop();
    }
    for(int i=0; i<=num; i++)
    {
        visit[i]=0;
        dfnn[i]=0;
        loww[i]=0;
        inStack[i]=0;
    }
    for(int i=0; i<j; i++)
    {
        g.deleteVertex(array[i],valid);
    }
    j=0;//renew
    for(int i =0; i<=num; i++)
        if(visit[i] == 0&&valid[i])
        {
            g.tarjan(i,2);//only keep elements at least 2 of SCC(s)
        }
    for(int i=0;i<=num;i++)
        if(valid[i]&&belong[i]<large)
    {
        cout<<"vertex "<<i<<" is not in the biggest SCC(s) because it belongs to a SCC whom has "<<belong[i]<<" elements"<<endl;
        g.deleteVertex(i,valid);
    }
    g.printGraph(num,valid);
    off=clock();
    out<<"Runtime is "<<off-start<<" ms"<<endl;
    out.close();
    return 0;
}
