
#include <iostream>
#include <string>
#include <regex>
#include <cstdlib>
#include <sstream>
#include <queue>
#include <stack>
#include <vector>
#include <algorithm>

// defined std::unique_ptr
#include <memory>
// defines Var and Lit
#include "minisat/core/SolverTypes.h"
// defines Solver
#include "minisat/core/Solver.h"

using namespace std;
static int _V;

int read_V(string v_command) //verified
{
    smatch v_result;
    regex_search(v_command,v_result,regex("[0-9]+"));
    string v_str = v_result.str();
    _V = std::stoi(v_str);
    return 0;
}

//denote |V| = _V = n in pdf
vector<int> sat_solver(int num_vertex, int k, vector<vector<int>> adj_list, vector<vector<int>> edge_list)
{
    // -- allocate on the heap so that we can reset later if needed
    std::unique_ptr<Minisat::Solver> solver(new Minisat::Solver());
    vector<vector<Minisat::Var>> variable(_V,vector<Minisat::Var>(_V));
    for(int i = 0; i<num_vertex; i++)
    {
        for(int j = 0; j<k; j++)
        {
            variable[i][j] = solver->newVar();
        }
    }
    
    //At least one vertex is the ith vertex in the vertex cover:
    for(int i=0; i<k; i++)
    {
        Minisat::vec<Minisat::Lit> literal;
        for(int n=0; n<num_vertex; n++)
        {
            literal.push(Minisat::mkLit(variable[n][i]));
        }
        solver->addClause(literal);
    }
    
    //No one vertex can appear twice in a vertex cover.
    //In other words, it is not the case that vertex m appears both in positions p and q of the vertex cover.
    for (unsigned int m=0; m<num_vertex; m++)
    {
        for (int p=0; p<k; p++)
        {
            for (int q=p+1; q<k; q++)
            {
                Minisat::vec<Minisat::Lit> literal;
                literal.push(~Minisat::mkLit(variable[m][p]));
                literal.push(~Minisat::mkLit(variable[m][q]));
                solver->addClause(literal);
            }
        }
    }
    
    //No more than one vertex appears in the mth position of the vertex cover.
    for (int m=0; m<k; m++)
    {
        for (int p=0; p<_V; p++)
        {
            for (int q=p+1; q<_V; q++) {
                Minisat::vec<Minisat::Lit> literal;
                literal.push(~Minisat::mkLit(variable[p][m]));
                literal.push(~Minisat::mkLit(variable[q][m]));
                solver->addClause(literal);
            }
        }
    }
    
    //Every edge is incident to at least one vertex in the vertex cover.
    for (unsigned int i = 0; i < edge_list.size(); ++i)
    {
        Minisat::vec<Minisat::Lit> literal;
        for (int j = 0; j < k; ++j)
        {
            literal.push(Minisat::mkLit(variable[edge_list[i][0]][j]));
            literal.push(Minisat::mkLit(variable[edge_list[i][1]][j]));
        }
        solver->addClause(literal);
    }
    
    vector<int> result;
    auto sat = solver->solve();
    if (sat) {
        for (unsigned int i=0; i<num_vertex; i++) {
            for (int j=0; j<k; j++) {
                if (solver->modelValue(variable[i][j]) == Minisat::l_True) {
                    result.push_back(i);
                }
            }
        }
    }
    return result;
}

void print_result(vector<vector<int>> adj_list, vector<vector<int>> edge_list,vector<int> vertex_list)
{
    vector<int> result;
    int low = 1;
    int high = vertex_list.size();
    int k = ((high - low)/2) + low;
    while (k>=1 and k <= vertex_list.size() and low <= high)
    {
        //vector<int> sat_solver(int num_vertex, int k, vector<vector<int>> adj_list, vector<vector<int>> edge_list)
        std::vector<int> tmp_result = sat_solver(vertex_list.size(),k,adj_list,edge_list);
        if (tmp_result.size()>0) {
            result = tmp_result;
            high = k-1;
            k = ((high-low)/2) + low;
        } else {
            low = k+1;
            k = ((high-low)/2) + low;
            
        }
    }
    std::sort(result.begin(), result.end());
    for(unsigned int i=0; i<result.size(); i++) {
        if (i == (result.size()-1)) {
            std::cout << vertex_list[result[i]] << std::endl;
        } else {
            std::cout << vertex_list[result[i]] << " ";
        }
    }
}

int read_E(string e_command,vector<vector<int>>* adj_list)
{
    vector<vector<int>> edge_list;
    vector<int> vertex_list;
    //clear the vector
    unsigned long v_size = (*adj_list).size();
    for (unsigned long i = 0; i < v_size; ++i)
    {
        for (unsigned long j = 0; j < v_size; ++j)
        {
            (*adj_list)[i][j]=0;
        }
    }
    //resize the vector
    (*adj_list).resize(_V);//resize into V*V
    for (int k = 0; k < _V; ++k)
    {
        (*adj_list)[k].resize(_V);
    }
    //read adjacency-matrix
    smatch e_result;
    string::const_iterator beg = e_command.begin();
    string::const_iterator end = e_command.end();
    while(regex_search(beg,end,e_result,regex("<[0-9]+,[0-9]+>")))
    {
        string e_str = e_result.str();
        beg = e_result[0].second;
        unsigned long left = e_str.find("<");
        unsigned long right = e_str.find(">");
        unsigned long comma= e_str.find(",");
        int num1 = std::stoi(e_str.substr(left+1,comma-left-1));
        int num2 = std::stoi(e_str.substr(comma+1,right-comma-1));
        if(num1 > _V-1 or num2 > _V-1)
        {
            std::cerr<<"Error: Over V limit."<<endl;
            return 0;
        }
        if(num1 == num2 )
        {
            std::cout<<"Error: Loop."<<endl;
        }
        (*adj_list)[num1][num2] = 1;
        (*adj_list)[num2][num1] = 1;
        vector<int> edge;
        edge.push_back(num1);
        edge.push_back(num2);
        edge_list.push_back(edge);
        bool vertex_flag_1 = false;
        bool vertex_flag_2 = false;
        for(unsigned i =0; i<vertex_list.size();i++)
        {
            if(num1 == vertex_list[i])
            {
                vertex_flag_1 = true;
            }
            if(num2 == vertex_list[i])
            {
                vertex_flag_2 = true;
            }
        }
        if(!vertex_flag_1)
        {
            vertex_list.push_back(num1);
        }
        if(!vertex_flag_2)
        {
            vertex_list.push_back(num2);
        }
    }
    std::sort(vertex_list.begin(), vertex_list.end());
    
    print_result(*adj_list, edge_list, vertex_list);
    return 0;
}

int main(int argc, const char * argv[])
{
    regex pattern_E ("E \\{<[0-9]+,[0-9]+>(,<[0-9]+,[0-9]+>)*\\}");
    regex pattern_V ("V [0-9]+");
    regex pattern_blank_E ("E \\{\\}");
    _V = 2;
    //adjacency-matrix
    vector<vector<int>> adj_list(_V,vector<int>(_V,1));
    
    while (!std::cin.eof())
    {
        std::string command;
        std::getline(std::cin,command);
        std::istringstream input(command);
        if (regex_match(command,pattern_blank_E))
        {
            cout<<endl;
        }
        else if (regex_match(command,pattern_E))
        {
            read_E(command,&adj_list);
        }
        else if(regex_match(command,pattern_V))
        {
            read_V(command);
        }
        else
        {
            if(std::cin.eof())
            {
                break;
            }
            else
            {
                cout<<"Error: Wrong input."<<endl;
            }
        }
    }
    return 0;
}
