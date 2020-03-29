#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cmath>
#include <ctime>
#include <vector>
#include <list>
#include <algorithm>
#include <cctype>
using namespace std;

// Graph class represents a directed graph
// using adjacency list representation
class Graph
{
    int V;    // No. of vertices

    // Pointer to an array containing
    // adjacency lists
    list<int> *adj;
    list<int> *pred;
    bool *visited;
    //vector<vector<int>> connexes;

    // A recursive function used by DFS
    void DFSUtil(int v, bool visited[]);

public:
    Graph(int V);   // Constructor

    // function to add an edge to graph
    void addEdge(int v, int w);
    void set_adj(int V);
    void afficherAdjacents() const;
    void DFS(int v);
    void BFS(int s);
    void afficherPredecesseurs() const;
    void rechercheEtAffichageComposanteConnexe(int ordre);
};

Graph::Graph(int V)
{
    this->V = V;
    adj = new list<int>[V];
    pred = new list<int>[V];
    visited = new bool[V];
    //connexes = {{0}};
}

void Graph::set_adj(int V)
{
    this->V = V;
    adj = new list<int>[V];
    pred = new list<int>[V];
    visited = new bool[V];
    //connexes = {{0}};
}

void Graph::addEdge(int v, int w)
{
    adj[v].push_back(w); // Add w to v’s list.
}

void Graph::rechercheEtAffichageComposanteConnexe(int ordre)
{
    int cmp = 0;
    list<int> marques;
    list<int>::iterator m;
    list<int> listeVide;
    for (int i = 0; i < ordre; i++)
    {
        m = find(marques.begin(), marques.end(), i);
        if (m == marques.end())
        {
            cmp ++;
            DFS(i);
            BFS(i);

            for (int i = 0; i < V; i++)
                pred[i] = listeVide;

            for (int i = 0; i < ordre; i++)
            {
                if (visited[i])
                    marques.push_back(i);
            }
            cout << "composante connexe " << cmp << " : ";
            for (int i = 0; i < ordre; i++)
            {
                if (visited[i])
                    cout << i << " ";
            }
            cout << endl;
        }
    }
}

void Graph::afficherAdjacents() const
{
    cout << "   liste d'adjacence :" << endl;
    for (int i = 0; i < V; i++)
    {
        cout << "       sommet " << i << " : ";

        list<int>::iterator j;
        for (j = adj[i].begin(); j != adj[i].end(); ++j)
        {
             cout << *j << " ";
        }
        cout << endl;
    }
}

void Graph::afficherPredecesseurs() const
{
    list<int>::iterator j;
    list<int> listeVide;

    for (int i = 0; i < V; i++)
    {
        if (pred[i] != listeVide)
            cout << i;

        for (j = pred[i].begin(); j != pred[i].end(); ++j)
        {
             cout << " <-- " << *j;
        }

        if (pred[i] != listeVide)
            cout << endl;

        pred[i] = listeVide;
    }
}

void Graph::DFSUtil(int v, bool visited[])
{
    // Mark the current node as visited and
    // print it
    visited[v] = true;
    //connexes[x].push_back(v);

    // Recur for all the vertices adjacent
    // to this vertex
    list<int>::iterator i;
    for (i = adj[v].begin(); i != adj[v].end(); ++i)
        if (!visited[*i]){

            pred[*i].push_back(v);

            list<int>::iterator j;
            for (j = pred[v].begin(); j != pred[v].end(); ++j)
                    pred[*i].push_back(*j);

            DFSUtil(*i, visited);
        }
}

// DFS traversal of the vertices reachable from v.
// It uses recursive DFSUtil()
void Graph::DFS(int v)
{
    // Mark all the vertices as not visited
    for (int i = 0; i < V; i++)
        visited[i] = false;

    // Call the recursive helper function
    // to print DFS traversal
    DFSUtil(v, visited);
}

void Graph::BFS(int s)
{
    // Mark all the vertices as not visited
    for(int i = 0; i < V; i++)
        visited[i] = false;

    // Create a queue for BFS
    list<int> queue;

    // Mark the current node as visited and enqueue it
    visited[s] = true;
    queue.push_back(s);

    //connexes[x].push_back(s);

    // 'i' will be used to get all adjacent
    // vertices of a vertex
    list<int>::iterator i;

    while(!queue.empty())
    {
        // Dequeue a vertex from queue and print it

        s = queue.front();
        queue.pop_front();

        // Get all adjacent vertices of the dequeued
        // vertex s. If a adjacent has not been visited,
        // then mark it visited and enqueue it
        for (i = adj[s].begin(); i != adj[s].end(); ++i)
        {
            if (!visited[*i])
            {
                visited[*i] = true;
                //connexes[x].push_back(s);
                queue.push_back(*i);

                pred[*i].push_back(s);

                list<int>::iterator j;
                for (j = pred[s].begin(); j != pred[s].end(); ++j)
                    pred[*i].push_back(*j);
            }
        }
    }
}

// Driver code
int main()
{
    // Create a graph given in the above diagram
    ifstream fichier("graphe.txt");  //Ouverture d'un fichier en lecture

    int sommetIni = 0;
    int cmp = 0;
    double taille = 0;
    double type = 0;
    double ordre = 0;
    double nombre = 0;
    double nombre2 = 0;
    Graph g(ordre);

    if(fichier)
    {
      while(cmp < 3 + taille) //Tant qu'on n'est pas à la fin, on lit
      {
        cmp++;
        if (cmp == 1){
            fichier >> type;
            if (type == 0)
                cout << "graphe non oriente" << endl;
            else
                cout << "graphe oriente" << endl;
        }
        else if (cmp == 2){
            fichier >> ordre;
            cout << "   ordre = " << ordre << endl;
            g.set_adj(ordre);
        }
        else if (cmp == 3){
            fichier >> taille;
        }
        else
        {
            fichier >> nombre;
            fichier >> nombre2;
            g.addEdge(nombre, nombre2);
            if (type == 0)
                g.addEdge(nombre2, nombre);
        }
      }
    }

    else
    {
       cout << "ERREUR: Impossible d'ouvrir le fichier en lecture." << endl;
    }

    g.afficherAdjacents();

    cout << "Depuis quel sommet souhaitez-vous lancer les algorithmes ? " << endl;
    cin >> sommetIni;
    cout << "Parcours BFS a partir du sommet " << sommetIni << " " << endl;
    g.BFS(sommetIni);
    g.afficherPredecesseurs();
    cout << "Parcours DFS a partir du sommet " << sommetIni << " " << endl;
    g.DFS(sommetIni);
    g.afficherPredecesseurs();

    g.rechercheEtAffichageComposanteConnexe(ordre);

    system("PAUSE");

    return 0;
}
