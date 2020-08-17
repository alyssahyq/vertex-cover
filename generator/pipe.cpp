
#include <iostream>
#include <vector>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

using namespace std;

sig_atomic_t _child_status;
sig_atomic_t exit_child;

int pid_ren = 0;
int pid_1 = 0;
int pid_2 = 0;

void clean_up_child(int signal_number)
{
    int status;
    wait(&status);
    _child_status = status;
    exit_child++;
}

int main(int argc, char* argv[])
{
    struct sigaction sigchild_action;
    memset(&sigchild_action, 0, sizeof(sigchild_action));
    sigchild_action.sa_handler = &clean_up_child;
    sigaction(SIGCHLD, &sigchild_action, NULL);
    
    pid_t pid_rgen;
    pid_t pid_a1;
    pid_t pid_a2;
    
    int rgen_a1[2];
    pipe(rgen_a1);
    int a1_a2[2];
    pipe(a1_a2);
    
    pid_rgen = fork();
    
    if(pid_rgen == 0)//child0: rgen
    {
        pid_a1 = fork();
        if(pid_a1 == 0)//child1: a1(python)
        {
            pid_a2 = fork();
            if(pid_a2 == 0)
            {
                //child2: a2 -> exec(a2)
                dup2(a1_a2[0], STDIN_FILENO);
                close(a1_a2[0]);
                close(a1_a2[1]);
                execlp("./shortest-path", "./shortest-path", NULL);
                abort();
            }//if(pid_a2 == 0)
            else
            {
                //parent2: a1 -> exec(a1)
                dup2(rgen_a1[0], STDIN_FILENO);
                dup2(a1_a2[1], STDOUT_FILENO);
                close(rgen_a1[1]);
                close(rgen_a1[0]);
                close(a1_a2[0]);
                close(a1_a2[1]);
                execlp("python3", "python3", "./find-vectex.py", NULL);
                abort();
            }//else of if(pid_a2 == 0)
        }//if(pid_a1 == 0)//child1: a1(python)
        else
        {
            //parent1: rgen -> exec(rgen)
            dup2(rgen_a1[1], STDOUT_FILENO);
            close(rgen_a1[0]);
            close(rgen_a1[1]);
            argv[0] = "./rgen";
            execv("./rgen", argv);
            abort();
        }//else of if(pid_a1 == 0)//child1: a1(python)
    }
    else
    {
        //parent0: a3(driver) -> be a driver
        dup2(a1_a2[1], STDOUT_FILENO);
    }
    vector<pid_t> children;
    children.push_back(pid_a1);
    children.push_back(pid_rgen);
    children.push_back(pid_a2);
    string line;
    while (getline(cin, line) and exit_child == (sig_atomic_t) 0)
    {
        cout << line << endl;
    }
    if (exit_child)
    {
        cerr << "Error: child process exit." << endl;
    }
    
    for (auto child: children)
    {
        int stauts;
        if (child == pid_rgen)
        {
            kill(child, SIGTERM);
            waitpid(child, &stauts, WNOHANG);
        }
        else
        {
            kill(child, SIGKILL);
            waitpid(child, &stauts, WNOHANG);
        }
    }
    return 0;
}

