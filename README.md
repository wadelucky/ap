# Amateur concept of deleting articulation points in both undirected and directed graphs.
***
Definition of *articulation point(s)*: It is often written as ap, and usually defined in undirected graph, which has an attribute that once you remove this point from the graph, this remainder graph is no longer connected.
***
Note: in directed graphs, we can also define the ap in a similar way.
***
The key method I use to detect ap points is [Tarjan's algorithm](https://en.wikipedia.org/wiki/Tarjan%27s_strongly_connected_components_algorithm).
***
In an undirected graph, I use Tarjan's algorithm and [union-find algorithm](https://en.wikipedia.org/wiki/Disjoint-set_data_structure) to detect ap and delete them to form a remainder graph, terminate when there's no ap in the remainder graph. Finally, output the final graph into a text file called "undirected.txt". 

In a directed graph, I still use Tarjan's algorithm but in a slightly different format. This Tarjan's algorithm gives me the total number of *strong connected component(scc)* in a graph, so I run it in every remainder graph(just simply attempt to delete every node in the current graph) to check. Once the total amount of scc changes, then this removed point will be marked as an ap. Run this algorithm until there's no ap left and print the largest scc in the final graph into a text file called "rslt.txt".
***
Edited by Wei Le, wadelucky@berkeley.edu
