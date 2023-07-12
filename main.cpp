/** 
This program uses pre-written templates. 
The templates have been created before the beginning of the contest and can be found at
https://github.com/mshandilya/cptemplates
**/

//required macros and template
#include <bits/stdc++.h>
using namespace std;

//type definitions
#define vc vector
typedef vc<int> vi;
typedef vc<pair<int, int>> vii;
typedef vc<list<int>> adjList;

//global variables
/*
 * n: number of vertices
 * m: number of edges
 * al: adjacency list
 * edges: a set (vector) of edges in the graph
 */
int n, m;
adjList al;
vii edges;

//the PMC Extension Instance referred to in the paper implemented as a class
class PMCExtensionInstance {

private:

    //method to add a node to the partition A
    void addA(int a_index) {
        A[a_index] = 1;
        unsaturatedA[a_index] = 1;
        F[a_index] = 0;
        sigmaF[a_index] = 0;
        for(int child: al[a_index]) {
            if(F[child])
                sigmaF[child]++;
            if(B[child]) {
                unsaturatedA[a_index] = 0;
                unsaturatedB[child] = 0;
            }
        }
    }

    //method to add a node to the partition B
    void addB(int b_index) {
        B[b_index] = 1;
        unsaturatedB[b_index] = 1;
        F[b_index] = 0;
        sigmaF[b_index] = 0;
        for(int child: al[b_index]) {
            if(F[child])
                sigmaF[child]++;
            if(A[child]) {
                unsaturatedB[b_index] = 0;
                unsaturatedA[child] = 0;
            }
        }
    }

    int find_cycle(int root, vi& vis, int cur_length = 0, int orig=-1) {
        vis[root] = 1;
        if(orig==-1)
            orig = root;
        else if(root == orig)
            return cur_length;
        else
            cur_length++;
        for(int child: al[root])
            if(!vis[child] and (sigmaF[root] or sigmaF[child]))
                return find_cycle(child, vis, cur_length, orig);
        return -1;
    }

public:
    //instance variables
    /* A is the boolean marker vector for marking which nodes are in subset A
     * B is the boolean marker vector for marking which nodes are in subset B
     * unsaturatedA marks the nodes which are part of A^o
     * unsaturatedB marks the nodes which are part of B^o
     * sigmaF signifies the number of neighbours in A U B.
     */
    vi A, B, unsaturatedA, unsaturatedB, F, sigmaF;

    explicit PMCExtensionInstance(int a_index = -1, int b_index = -1) {
        A.assign(n, 0);
        B.assign(n, 0);
        unsaturatedA.assign(n, 0);
        unsaturatedB.assign(n, 0);
        F.assign(n, 1);
        sigmaF.assign(n, 0);
        if(a_index != -1) {
            A[a_index] = 1;
            B[b_index] = 1;
            unsaturatedA[a_index] = 1;
            unsaturatedB[b_index] = 1;
            F[a_index] = 0;
            F[b_index] = 0;
            for(int child: al[a_index]) {
                if(F[child])
                    sigmaF[child]++;
                if(B[child])
                    unsaturatedA[a_index] = 0;
            }
            for(int child: al[b_index]) {
                if (F[child])
                    sigmaF[child]++;
                if(A[child])
                    unsaturatedB[b_index] = 0;
            }
        }
    }

    PMCExtensionInstance(const PMCExtensionInstance& copy) {}

    PMCExtensionInstance& operator=(const PMCExtensionInstance& copy) {}

    bool extend(const vi& indicesP, const vi& indicesQ) {
        for(int a_index: indicesP)
            addA(a_index);
        for(int b_index: indicesQ)
            addB(b_index);
        return reduce_exhaustively();
    }

    bool reduce_exhaustively() {
        int matchedNeighbours, undeterminedNeighbours, lastNeighbour, ANeighbours, BNeighbours;
        //reduction 0
        for(int i = 0; i<n; i++) {
            matchedNeighbours = 0;
            if(A[i]) {
                for(int child: al[i]) {
                    if(B[child])
                        matchedNeighbours++;
                }
                if(matchedNeighbours > 1)
                    return false;
            }
            else if(B[i]) {
                for(int child: al[i]) {
                    if(A[child])
                        matchedNeighbours++;
                }
                if(matchedNeighbours > 1)
                    return false;
            }
        }
        //reduction 1
        for(int i = 0; i<n; i++) {
            if(A[i]-unsaturatedA[i]) {
                for(int child: al[i]) {
                    if(F[child]) {
                        addA(child);
                        return reduce_exhaustively();
                    }
                }
            }
            else if(B[i]-unsaturatedB[i]) {
                for(int child: al[i]) {
                    if(F[child]) {
                        addB(child);
                        return reduce_exhaustively();
                    }
                }
            }
        }
        //reduction 2
        for(int i = 0; i<n; i++) {
            undeterminedNeighbours = 0;
            lastNeighbour = -1;
            if(unsaturatedA[i]) {
                for(int child: al[i]) {
                    if(F[child]) {
                        undeterminedNeighbours++;
                        lastNeighbour = child;
                    }
                }
                if(undeterminedNeighbours == 1) {
                    addB(lastNeighbour);
                    return reduce_exhaustively();
                }
            }
            else if(unsaturatedB[i]) {
                for(int child: al[i]) {
                    if(F[child]) {
                        undeterminedNeighbours++;
                        lastNeighbour = child;
                    }
                }
                if(undeterminedNeighbours == 1) {
                    addA(lastNeighbour);
                    return reduce_exhaustively();
                }
            }
        }
        //reduction 3
        for(int i = 0; i<n; i++) {
            undeterminedNeighbours = 0;
            if(unsaturatedA[i] or unsaturatedB[i]) {
                for(int child: al[i]) {
                    if(F[child])
                        undeterminedNeighbours++;
                }
                if(!undeterminedNeighbours)
                    return false;
            }
        }
        //reduction 4
        for(int i = 0; i<n; i++) {
            ANeighbours = 0;
            BNeighbours = 0;
            if(F[i]) {
                for(int child: al[i]) {
                    if(A[child])
                        ANeighbours++;
                    else if(B[child])
                        BNeighbours++;
                }
                if(ANeighbours >= 2) {
                    addA(i);
                    return reduce_exhaustively();
                }
                else if(BNeighbours >= 2) {
                    addB(i);
                    return reduce_exhaustively();
                }
            }
        }
        //reduction 5
        for(int i = 0; i<n; i++) {
            if(sigmaF[i] and al[i].size() == 2) {
                int neighbourA = -1, neighbourB = -1, neighbourF = -1;
                for(int child: al[i]) {
                    if(F[child])
                        neighbourF = child;
                    else if(A[child])
                        neighbourA = child;
                    else if(B[child])
                        neighbourB = child;
                }
                if(neighbourF!=-1 and sigmaF[neighbourF]==1) {
                    if(neighbourA!=-1) {
                        addB(neighbourF);
                        return reduce_exhaustively();
                    }
                    else if(neighbourB!=-1) {
                        addA(neighbourF);
                        return reduce_exhaustively();
                    }
                }
            }
        }
        //reduction 6
        for(int i = 0; i<n; i++) {
            if(sigmaF[i] and al[i].size() == 2) {
                int neighbourA = -1, neighbourB = -1, neighbourF = -1;
                for(int child: al[i]) {
                    if(F[child])
                        neighbourF = child;
                    else if(A[child])
                        neighbourA = child;
                    else if(B[child])
                        neighbourB = child;
                }
                if(neighbourF!=-1 and al[neighbourF].size()==1) {
                    if(neighbourA!=-1) {
                        addA(i);
                        addB(neighbourF);
                        return reduce_exhaustively();
                    }
                    else if(neighbourB!=-1) {
                        addA(neighbourF);
                        addB(i);
                        return reduce_exhaustively();
                    }
                }
            }
        }
        //reduction 7
        for(int i = 0; i<n; i++) {
            if(sigmaF[i] and (al[i].size() == 1 or al[i].size() == 3)) {
                int neighboursA = 0, neighboursB = 0, neighboursF = 0;
                for(int child: al[i]) {
                    if(F[child])
                        neighboursF++;
                    else if(A[child])
                        neighboursA++;
                    else if(B[child])
                        neighboursB++;
                }
                if(!neighboursF) {
                    if(neighboursA==1) {
                        addB(i);
                        return reduce_exhaustively();
                    }
                    else if(neighboursB==1) {
                        addA(i);
                        return reduce_exhaustively();
                    }
                }
            }
        }
        //checking for isolation
        vi dynamicDegree(n, 0);
        for(int i = 0; i < n; i++) {
            if((!dynamicDegree[i]) and (A[i] or B[i])) {
                for(int child: al[i]) {
                    if(A[child] or B[child]) {
                        dynamicDegree[i] = 1;
                        dynamicDegree[child] = 1;
                        break;
                    }
                }
                if(!dynamicDegree[i])
                    return false;
            }
        }
        return true;
    }

    bool evaluate() {
        if(reduce_exhaustively()) {
            //branch condition 1
            for (int u = 0; u < n; u++) {
                if (sigmaF[u] == 1 and al[u].size() == 2) {
                    int alpha = (A[*al[u].begin()] or B[*al[u].begin()]) ? (al[u].front()) : (*(++al[u].begin())),
                            v = (A[*al[u].begin()] or B[*al[u].begin()]) ? (*(++al[u].begin())) : (al[u].front());
                    //sub-condition 1
                    if (al[v].size() == 2) {
                        int p = (al[alpha].front() == u) ? (al[alpha].back()) : (al[alpha].front()), q = (al[v].front() == u) ? (al[v].back()) : (al[v].front());
                        PMCExtensionInstance new_pmc = *this;
                        if (A[alpha]) {
                            return ((new_pmc.extend({p, q}, {u, v}) and new_pmc.evaluate()) or
                                    (extend({u}, {p, q, v}) and evaluate()));
                        } else if (B[alpha]) {
                            return ((new_pmc.extend({u, v}, {p, q}) and new_pmc.evaluate()) or
                                    (extend({p, q, v}, {u}) and evaluate()));
                        }
                    }
                        //sub-condition 2
                    else if (al[v].size() == 3) {
                        vi qNr;
                        for (int child: al[v])
                            if (child != u)
                                qNr.push_back(child);
                        int p = (al[alpha].front() == u) ? (al[alpha].back())
                                                         : (al[alpha].front()), q = qNr[0], r = qNr[1];
                        PMCExtensionInstance new_pmc = *this;
                        if (A[alpha]) {
                            return ((new_pmc.extend({u}, {p, q, r, v}) and new_pmc.evaluate()) or
                                    (extend({p}, {u, v}) and evaluate()));
                        } else if (B[alpha]) {
                            return ((new_pmc.extend({p, q, r, v}, {u}) and new_pmc.evaluate()) or
                                    (extend({u, v}, {p}) and evaluate()));
                        }
                    }
                }
            }
            //branching condition 2
            for (int u = 0; u < n; u++) {
                if (sigmaF[u] and al[u].size() == 3) {
                    int a = -1, b = -1, v = -1;
                    for (int child: al[u]) {
                        if (A[child])
                            a = child;
                        else if (B[child])
                            b = child;
                        else if (F[child])
                            v = child;
                    }
                    if (a != -1 and b != -1 and v != -1) {
                        int ad = -1, bd = -1;
                        for (int child: al[a])
                            if (F[child] and child != u) {
                                ad = child;
                                break;
                            }
                        for (int child: al[b])
                            if (F[child] and child != u) {
                                bd = child;
                                break;
                            }
                        PMCExtensionInstance new_pmc = *this;
                        return ((new_pmc.extend({u, v}, {ad, bd}) and new_pmc.evaluate()) or
                                (extend({ad, bd}, {u, v}) and evaluate()));
                    }
                }
            }
            //branching condition 3
            for (int v = 0; v < n; v++) {
                if (sigmaF[v] == 1) {
                    int a, p, q, u;
                    vi pNq;
                    for (int child: al[v]) {
                        if (A[child] or B[child])
                            a = child;
                        else if (F[child])
                            pNq.push_back(child);
                    }
                    p = pNq[0];
                    q = pNq[1];
                    for (int child: al[a]) {
                        if (F[child] and child != v)
                            u = child;
                    }
                    //sub-branching rule 1
                    if (al[u].size() == 3 and sigmaF[u] == 1) {
                        vi rNs;
                        for (int child: al[u])
                            if (F[child])
                                rNs.push_back(child);
                        int r = rNs[0], s = rNs[1];
                        PMCExtensionInstance new_pmc = *this;
                        if (A[a])
                            return ((new_pmc.extend({v}, {u, r, s}) and new_pmc.evaluate()) or
                                    (extend({u}, {v, p, q}) and evaluate()));
                        else if (B[a])
                            return ((new_pmc.extend({u, r, s}, {v}) and new_pmc.evaluate()) or
                                    (extend({v, p, q}, {u}) and evaluate()));
                    }
                        //sub-branching rule 2
                    else if (al[u].size() == 2 and sigmaF[u] == 2) {
                        int b = (al[u].front() == a) ? (al[u].back()) : (al[u].front()), w;
                        for (int child: al[b])
                            if (F[child] and child != u)
                                w = child;
                        PMCExtensionInstance new_pmc = *this;
                        if (A[a])
                            return ((new_pmc.extend({w, v}, {u}) and new_pmc.evaluate()) or
                                    (extend({u}, {v, p, q, w}) and evaluate()));
                        else if (B[a])
                            return ((new_pmc.extend({u}, {w, v}) and new_pmc.evaluate()) or
                                    (extend({v, p, q, w}, {u}) and evaluate()));
                    }
                }
            }
            //base case
            //to check for even cycles
            vi vis(n, 0);
            for (int i = 0; i < n; i++) {
                if ((sigmaF[i] or unsaturatedA[i] or unsaturatedB[i]) and !vis[i]) {
                    if (find_cycle(i, vis) % 2 != 0)
                        return false;
                }
            }
            return true;
        }
        else
            return false;
    }

};

void takeInput() {
    //we take the input here.
    //n denotes the number of vertices
    //m denotes the number of edges
    int x, y;
    cin>>n>>m;
    al.resize(n);
    edges.resize(n);
    for(int i = 0; i<m; i++) {
        cin>>x>>y;
        al[x].push_back(y);
        al[y].push_back(x);
        edges.emplace_back(x, y);
    }
}

//main method
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    takeInput();
    // let reduce_exhaustively return if the cut instance is clean since otherwise it can't work anyway.
    bool pos = false;
    for(int i = 0; i<m; i++) {
        PMCExtensionInstance pmc(edges[i].first, edges[i].second);
        if(pmc.evaluate()) {
            cout << "Yes\n";
            pos = true;
            break;
        }
    }
    if(!pos)
        cout<<"No\n";
    return 0;
}