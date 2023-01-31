#include <iostream>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>
#include <unordered_set>
#include <iomanip>



typedef long long int ll ;
using namespace std;
using Clock = std::conditional_t<std::chrono::high_resolution_clock::is_steady,
                                 std::chrono::high_resolution_clock ,
                                 std::chrono::steady_clock> ;



class Graph {
public :
    unordered_map<ll,vector<ll> > degreeVector ; //containing <k, v>, where k ranges from 1 to |V|, and v is the vector of vertices of degree k
    ll mini ; // minimum value of i such that degreeVector[mini] is not empty
    ll e ;  // |E|
    ll v ; // |V|
    unordered_map<ll, ll> degree ; // <k, v> where k is a vertex, and v its degree ;
    unordered_map<ll, vector<ll> > neighbors ;
    unordered_map<ll, bool> inducedGraph ;

    Graph(ll n, ll m) {
        mini = 0 ;
        e = m ;
        v = n  ; // we consider that there is also a node 0 ;
        for (ll i = 0 ; i < m ; i++) {
            ll aux1 , aux2 ;
            cin >> aux1 >> aux2 ;
            if (neighbors.find(aux1) == neighbors.end()) {
                neighbors[aux1].clear();
                neighbors[aux1].push_back(aux2) ;
            }
            else {
                neighbors[aux1].push_back(aux2) ;
            }
            if (neighbors.find(aux2) == neighbors.end()) {
                neighbors[aux2].clear();
                neighbors[aux2].push_back(aux1) ;
            }
            else {
                neighbors[aux2].push_back(aux1) ;
            }
            // now we considered that they are neighbouring vertices
            if (inducedGraph.find(aux1) == inducedGraph.end()) {
                inducedGraph[aux1] = true ;
            }
            if (inducedGraph.find(aux2) == inducedGraph.end()) {
                inducedGraph[aux2] = true ;
            }

            // we considered that the vertices are in the graph
        }
        //now we handle degree:
        for (auto &x : neighbors) {
            degree[x.first] = (ll)(x.second.size()) ;
        }
        for (ll i = 0 ; i < n ; i++) {
            if (degree.find(i) == degree.end()) {
                degree[i] = 0 ;
            }
            if (inducedGraph.find(i) == inducedGraph.end()) {
                inducedGraph[i] = true ;
            }
        }
        for (ll i = 0 ; i < n ; i++) {
            degreeVector[i].clear() ;
        }
        for (auto &x : degree) {
            degreeVector[x.second].push_back(x.first) ;
        }
    }

    Graph(string s) {
        ifstream file("C:\\Users\\bayou\\CLionProjects\\DensestSubGraph\\Data\\" + s + ".csv") ;
        vector<ll> edges ;
        ll n , m ;
        if (!file.is_open()) {
            throw runtime_error("could not open file, please review format and try again") ;
        }
        string line, colname ;
        ll val;
        while (getline(file,  line)) {
            stringstream ss(line) ;
            while (ss >> val ) {
                edges.push_back(val) ;
                if (ss.peek() == ',') {
                    ss.ignore() ;
                }
            }
        }
        file.close() ;
        n = 0 ;
        for (ll i : edges) {
            n = max(n , i) ;
        }
        m = edges.size()/2 ;
        mini = 0 ;
        e = m ;
        v = n + 1  ; // we consider that there is also a node 0 ;
        for (ll i = 0 ; i < m ; i++) {
            ll aux1 , aux2 ;
            aux1 = edges[2*i] ;
            aux2 = edges[2*i + 1] ;
            if (neighbors.find(aux1) == neighbors.end()) {
                neighbors[aux1].clear();
                neighbors[aux1].push_back(aux2) ;
            }
            else {
                neighbors[aux1].push_back(aux2) ;
            }
            if (neighbors.find(aux2) == neighbors.end()) {
                neighbors[aux2].clear();
                neighbors[aux2].push_back(aux1) ;
            }
            else {
                neighbors[aux2].push_back(aux1) ;
            }
            // now we considered that they are neighbouring vertices
            if (inducedGraph.find(aux1) == inducedGraph.end()) {
                inducedGraph[aux1] = true ;
            }
            if (inducedGraph.find(aux2) == inducedGraph.end()) {
                inducedGraph[aux2] = true ;
            }

            // we considered that the vertices are in the graph
        }
        //now we handle degree:
        for (auto &x : neighbors) {
            degree[x.first] = (ll)(x.second.size()) ;
        }
        for (ll i = 0 ; i < n + 1 ; i++) {
            if (degree.find(i) == degree.end()) {
                degree[i] = 0 ;
            }
            if (inducedGraph.find(i) == inducedGraph.end()) {
                inducedGraph[i] = true ;
            }
        }
        for (ll i = 0 ; i < n + 1 ; i++) {
            degreeVector[i].clear() ;
        }
        for (auto &x : degree) {
            degreeVector[x.second].push_back(x.first) ;
        }
    }

    Graph(Graph& g) {
        degreeVector = g.degreeVector ;
        mini = g.mini ;
        e = g.e ;
        v = g.v ;
        degree = g.degree ;
        neighbors = g.neighbors;
        inducedGraph = g.inducedGraph ;
    }

    Graph() {
        mini = 0 ;
    }

    double rho() {
        double ed = (double)(e) ;
        double vd = (double)(v) ;
        return ed/vd ;
    }
};

int main() {

    ofstream of("C:\\Users\\bayou\\CLionProjects\\DensestSubGraph\\output_1.csv") ;

    string s = "";
    for (int p = 0 ; p < 10 ; p++) {
        s = "" ;
        s += (char)((int)('0') + p) ;
        Clock::time_point start = Clock::now() ;
        Graph g =  Graph(s);
        ll n = g.v ;
        ll m = g.e;
        cout << "Here's n : " << g.v << " and here's m : " << g.e << endl ;
        Graph h = Graph(g) ;
        unordered_set<ll> waitListElimination, waitListDegree ;
        while(g.mini <= n) {
            while (g.degreeVector[g.mini].size() == 0 && g.mini <= n) {
                g.mini ++ ;
            }
            bool found = false ;
            ll vmin = 0 ;
            while (!found && g.mini <= n) {
                for (ll v : g.degreeVector[g.mini]) {
                    if (g.inducedGraph[v]) {
                        vmin = v ;
                        found = true ;
                        break ;
                    }
                }
                if (!found) {
                    g.mini ++ ;
                }
            }
            if (found) {
                g.v -- ;
                g.e -= g.degree[vmin] ;
                if (g.rho() > h.rho()) {
                    h.mini = g.mini ;
                    h.v = g.v ;
                    h.e = g.e;
                    for (ll v : g.neighbors[vmin]) {
                        g.degree[v] -- ;
                        h.degree[v]= g.degree[v];
                        g.mini = min(g.mini , g.degree[v]) ;
                        h.mini = min(h.mini , h.degree[v]) ;
                        g.degreeVector[g.degree[v]].push_back(v) ;
                        h.degreeVector[h.degree[v]].push_back(v) ;
                    }
                    for (ll i : waitListDegree) {
                        h.degree[i] = g.degree[i] ;
                    }
                    waitListDegree.clear() ;
                    g.inducedGraph[vmin] = false ;
                    h.inducedGraph[vmin] = false ;
                    for (ll i : waitListElimination) {
                        h.inducedGraph[i] = false ;
                    }
                    waitListElimination.clear() ;
                }
                else {
                    for (ll v : g.neighbors[vmin]) {
                        g.degree[v] -- ;
                        waitListDegree.insert(v) ;
                        g.degreeVector[g.degree[v]].push_back(v) ;
                        g.mini = min(g.mini , g.degree[v]) ;
                    }
                    g.inducedGraph[vmin] = false ;
                    waitListElimination.insert(vmin) ;
                }

            }

        }

        Clock::time_point finish = Clock::now() ;
        cout << " here are the characteristics of h : " << endl ;
        cout << "|E| : " << h.e << endl ;
        cout << "|V| : " << h.v << endl ;
        cout << "nodes : " ;
        for (ll i = 0 ; i < h.inducedGraph.size() ; i++) {
            if (h.inducedGraph[i]) {
                cout << i << " of degree : "<< h.degree[i] << endl << "        " ;
            }
        }
        cout << endl ;
        cout << "and here's rho : " << h.rho() << endl ;

        cout << "Time taken is : " << std::chrono::duration_cast<std::chrono::microseconds>(finish- start).count() << " microseconds." << endl ;


        of << n+m << "," << std::chrono::duration_cast<std::chrono::microseconds>(finish- start).count() << endl ;

    }
    of.close() ;



    return 0;
}
