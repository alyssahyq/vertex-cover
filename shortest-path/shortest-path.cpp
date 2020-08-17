//
//  main.cpp
//  ECE650A2
//
//  Created by 黄怡沁 on 2020/1/30.
//  Copyright © 2020年 Alyssa Yiqin Huang. All rights reserved.
//  Feb 9 1:13

#include <iostream>
#include <string>
#include <regex>
#include <cstdlib>
#include <sstream>
#include <queue>
#include <stack>

using namespace std;
static int _V;

struct Vertex
{
    int predecessor;    //-1 means NIL
    int distance;       //inf = _V+2
    int colour;          //0 = white; 1 = gray; 2 = black
    Vertex() :predecessor(), distance(), colour(){}
    Vertex(int a, int b, int c) :predecessor(a), distance(b), colour(c){};
};

int read_V(string v_command) //verified
{
    smatch v_result;
    regex_search(v_command,v_result,regex("[0-9]+"));
    string v_str = v_result.str();
    _V = std::stoi(v_str);
    return 0;
}

int read_E(string e_command,vector<vector<int>>* adj_list)
{
    //clear the vector
    unsigned long v_size = (*adj_list).size();
    for (int i = 0; i < v_size; ++i)
    {
        for (int j = 0; j < v_size; ++j)
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
    }
    return 0;
}

int print_path(int start, int endin, struct Vertex vertices[], stack<int>* path)
{
    if(start == endin)
    {
        //cout<<start<<endl;
        return 0;
    }
    else if (vertices[endin].predecessor == -1)
    {
        std::cerr<<"Error: No path from "<<start<<" to "<<endin<<" exists."<<endl;
        return 0;
    }
    else
    {
        path->push(endin);
        print_path(start, vertices[endin].predecessor,vertices, path);
    }
    return 1;
}

int find_path(int start, int endin, vector<vector<int>> adj_list)
{
    if(start == endin)
    {
        cout<<start<<endl;
        return 0;
    }
    struct Vertex vertices[_V];
    for(int i=0;i<_V+1;i++)
    {
        vertices[i] = Vertex(-1,_V+1,0);
    }
    vertices[start].colour = 1;//grey
    vertices[start].distance = 0;
    vertices[start].predecessor = -1;
    queue<int> Q;
    Q.push(start);
    while(!Q.empty())
    {
        int u = Q.front();
        Q.pop();
        for(int v = 0; v < _V; v++)
        {
            if(adj_list[u][v] == 1)
            {
                if(vertices[v].colour == 0) //if colour is white
                {
                    vertices[v].colour = 1; // colour is now gray
                    vertices[v].distance = vertices[u].distance+1;
                    vertices[v].predecessor = u;
                    Q.push(v);
                }
            }
        }
        vertices[u].colour = 2; // colour is now black
    }
    //print path
    stack<int> path;
    if(print_path(start, endin, vertices, &path))
    {
        cout<<start;
        while(!path.empty())
        {
            cout<<"-"<<path.top();
            path.pop();
        }
        cout<<endl;
    }
    return 1;
}

int read_s(string s_command,vector<vector<int>> adj_list)
{
    vector<int> targe;
    int start;
    int endin;
    smatch s_result;
    string::const_iterator beg = s_command.begin();
    string::const_iterator end = s_command.end();
    while(regex_search(beg,end,s_result,regex("[0-9]+")))
    {
        string s_str = s_result.str();
        beg = s_result[0].second;
        targe.push_back(std::stoi(s_str));
    }
    start = targe[0];
    endin = targe[1];
    if(start > _V-1 or endin> _V-1)
    {
        std:cerr<<"Error: Out of limit."<<endl;
        return 0;
    }
    find_path(start, endin, adj_list);
    return 1;
}


int main(int argc, const char * argv[])
{
    regex pattern_E ("E \\{<\\d+,\\d+>(,<\\d+,\\d+>)+\\}");
    regex pattern_V ("V [0-9]+");
    regex pattern_s ("s [0-9]+ [0-9]+");
    _V = 2;
    //adjacency-matrix
    vector<vector<int>> adj_list(_V,vector<int>(_V,1));
    while (!std::cin.eof())
    {
        std::string command;
        std::getline(std::cin,command);
        std::istringstream input(command);
        if (regex_match(command,pattern_E))
        {
            read_E(command,&adj_list);
        }
        else if(regex_match(command,pattern_V))
        {
            read_V(command);
        }
        else if(regex_match(command,pattern_s))
        {
            read_s(command, adj_list);
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
