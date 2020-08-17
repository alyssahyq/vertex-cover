//
//  rgen.cpp
//  ECE650-A3
//  intersertiong detecting
/*Random input generator. Your random input generator rgen should generate random inputs of street specications for your Python script.
 a typical interaction of the random generator is as follows:
 issue enough r commands to clear any existing street database;
 issue a commands to add new streets satisfying the specication;
 issue g command;
 wait for specied number of seconds and repeat.
 You must use /dev/urandom as the source of your random data.
 */



#include <iostream>
#include <fstream>
#include <stdio.h>
#include <unistd.h>
#include <vector>
#include <string>

using namespace std;

//Note that your random generator could go into an innite loop looking for a valid specication.
//You should disallow this from happening by limiting the number of tries.
//A reasonable value to adopt for N could be 25.
#define N 50
//sig_atomic_t term_flag = 0;

/* -s k
 k is an integer ≥ 2.
 Default: k = 10.
 The number of streets should be a random integer in [2,k].
 that is, the number of streets should be a random integer in [2,10].
 */
int regen_s(unsigned int random_num, int k = 10)
{
    if(k<2)
    {
        //k = 2;
        std::cerr << "Error: -s k | where k should be an integer bigger 2." << std::endl;
        //urandom.close();
        exit(2);
    }
    int s;//[a,b] = (rand() % (b-a+1))+ a;
    s = (random_num %(k-2+1))+2;
    return s;
}

/* -n k
 k is an integer ≥ 1.
 Default: k = 5.
 The number of line-segments in each street should be a random integer in [1, k].*/
int regen_n(unsigned int random_num, int k = 5)
{
    if(k < 1)
    {
        //k = 1;
        std::cerr << "Error: -n k | where k should be an integer bigger 1." << std::endl;
        //urandom.close();
        exit(2);
    }
    int n;//[a,b] = (rand() % (b-a+1))+ a;
    n = (random_num %(k-1+1))+1;
    return n;
}

/* -l k
 k is an integer ≥ 5.
 Default: k = 5.
 Your process should wait a random number w seconds, where w is in [5,k] before generating the next (random) input.*/
int regen_l(unsigned int random_num, int k = 5)
{
    if(k < 5)
    {
        //k = 5;
        std::cerr << "Error: -l k | where k should be an integer bigger 5." << std::endl;
        //urandom.close();
        exit(2);
    }
    int l;//[a,b] = (rand() % (b-a+1))+ a;
    l = (random_num %(k-5+1))+5;//w second
    return l;
}

/* -c k
 k is an integer ≥ 1.
 Default: k = 20.
 Your process should generate (x, y) coordinates such that every x and y value is in the range [-k, k].
 For example, if k = 15, all of your coordinate values should be integers between -15 and 15.
 Your program should generate a specication of streets in the format that your Python script expects (see Assignment 1). You can name the streets whatever you want.
 You should ensure that your input does not have errors. For example, if you generate a line-segment that overlaps with a line-segment (across all streets) generated earlier, you should regenerate that line-segment.
 */
int regen_c(unsigned int random_num, int k = 20)
{
    if(k < 1)
    {
        //k = 5;
        std::cerr << "Error: -c k | where k should be an integer bigger 1." << std::endl;
        //urandom.close();
        exit(2);
    }
    int c;//[a,b] = (rand() % (b-a+1))+ a;
    c = (random_num %(k+k+1))-k;
    return c;
}

string num_to_letter(int num)
{
    string str="";
    num = num+1;//num can't be 0
    while(num > 0){
        int letter = num %26;
        if(letter==0)
        {
            letter=26;
        }
        str+=(char)(letter+64);
        num =(num - letter)/26;
    }
    //reverse(str.begin(),str.end());
    return str;
}

//struct used to store single line-segement, used to detect overlap.
//------------------------------------------------------------------
class line_segment
{
public:
    line_segment();
    ~line_segment();
    int x1;
    int x2;
    int y1;
    int y2;
    int a;
    int b;
    int c;
    int calculate_equation();
};

line_segment::line_segment(void){}

line_segment::~line_segment(void){}

int line_segment::calculate_equation()
{
    if(x1==x2)
    {
        if(y1==y2)
        {
            return 1;
        }
        else
        {
            a = 1;
            b = 0;
            c = -x1;
            return 0;
        }
    }
    else
    {
        if(y1==y2)
        {
            a = 0;
            b = 1;
            c = -y1;
        }
        else
        {
            a = (y2 - y1)/(x1-x2);
            b = 1;
            c = (x2 * y1 - x1 * y2)/(x1-x2);
            return 0;
        }
    }
    return 1;
}
//------------------------------------------------------------------

/* Verifying whether overlap occurs
 - Overlap has occured: return true
 - Overlap hasn't occured: return false*/
bool overlap_occured(line_segment line1, line_segment line2)
{
    if(line1.a == line2.a and line1.b == line2.b and line1.c == line2.c)
    {
        if(line1.b == 0 and line2.b == 0)//vertical -> compare y
        {
            if( ( min(line1.y1,line1.y2) < max(line2.y1,line2.y2) ) or ( ( min(line2.y1,line2.y2) < max(line1.y1,line1.y2) ) ) )
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else //compare x
        {
            if( ( min(line1.x1,line1.x2) < max(line2.x1,line2.x2) ) or ( ( min(line2.x1,line2.x2) < max(line1.x1,line1.x2) ) ) )
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
    return false;
}

bool intersection_occured(line_segment line1, line_segment line2)
{
    if(line1.a != line2.a or line1.b != line2.b)
    {
        if(line1.a == 0 and line2.a == 0)
        {
            if(line1.c == line2.c)
            {
                if(max(line1.x1,line1.x2)<min(line2.x1,line2.x2) or min(line1.x1,line1.x2)>max(line2.x1,line2.x2))
                {
                    return false;
                }
                else
                {
                    return true;
                }
            }
            else
            {
                return false;
            }
        }
        else if(line1.b == 0 and line2.b == 0)
        {
            if(line1.c == line2.c)
            {
                if(max(line1.y1,line1.y2)<min(line2.y1,line2.y2) or min(line1.y1,line1.y2)>max(line2.y1,line2.y2))
                {
                    return false;
                }
                else
                {
                    return true;
                }
            }
            else
            {
                return false;
            }
        }
        else
        {
            //x_i = (b1 * c2 - b2 * c1) / (a1 * b2 - a2 * b1)
            //y_i = -(a1 * c2 - a2 * c1) / (a1 * b2 - a2 * b1)
            //itst = (x_i,y_i)
            //cout<<"line1.a = "<<line1.a<<" line2.b = "<<line2.b<<" line2.a = "<<line2.a<<" line1.b = "<<line1.b<<endl;
            float x_i;
            x_i = ((line1.b * line2.c)-(line2.b * line1.c))/((line1.a * line2.b)-(line2.a * line1.b));
            float y_i;
            y_i = -(((line1.a * line2.c)-(line2.a * line1.c))/((line1.a * line2.b)-(line2.a * line1.b)));
            if(x_i > max(line1.x1,line1.x2) or x_i < min(line1.x1,line1.x2) or x_i > max(line2.x1,line2.x2) or x_i < min(line2.x1,line2.x2) or y_i > max(line1.y1,line1.y2) or y_i < min(line1.y1,line1.y2) or y_i > max(line2.y1,line2.y2) or y_i < min(line2.y1,line2.y2))
            {
                return false;
            }
            else
            {
                return true;
            }
        }
    }
    else
    {
        return false;
    }
    return true;
}


int main(int argc, char* argv[])
{
    int s_k = 10;
    int n_k = 5;
    int l_k = 5;
    int c_k = 20;
    int option;
    
    // open /dev/urandom to read
    std::ifstream urandom("/dev/urandom");
    
    // check that it did not fail
    if (urandom.fail()) {
        std::cerr << "Error: cannot open /dev/urandom\n";
        return 1;
    }
    
    //It takes four command-line arguments. All are optional.
    while ((option = getopt(argc, argv, "s:n:l:c:")) != -1)
    {
        switch (option)
        {
            case 's':
                s_k = std::stoi(optarg);
                //s_k =
                break;
            case 'n':
                n_k = std::stoi(optarg);
                break;
            case 'l':
                l_k = std::stoi(optarg);
                break;
            case 'c':
                c_k = std::stoi(optarg);
                break;
            case '?':
                if (optopt == 's' or optopt == 'n' or optopt == 'l' or optopt == 'c')
                {
                    std::cerr << "Error: missing parameter." << std::endl;
                }
                else
                {
                    std::cerr << "Error: unknown option: " << optopt << std::endl;
                }
                urandom.close();
                exit(2);
            default:
                break;
        }
    }
    while(true)
    {
        // read a random unsigned int
        unsigned int random_num = 42;
        urandom.read((char*)&random_num, sizeof(int));
        
        int num_of_str;//number of streets
        num_of_str = regen_s(random_num,s_k);
        
        //vector used to store all line-segments.
        std::vector<line_segment> all_line_segment;
        
        
        int x = 0;
        int y = 0;
        
        int last_x=0;
        int last_y=0;
        
        for(int i = 0; i < num_of_str; i++)//generate <num_of_str> streets
        {

            
            urandom.read((char*)&random_num, sizeof(int));//update random num
            int num_of_seg;// The number of line-segments in each street
            num_of_seg = regen_n(random_num,n_k);
            
            unsigned long sum_seg;//sum of line-segment already generated.
            sum_seg = all_line_segment.size();
            for(int s = 0; s < num_of_seg; s++)
            {
                line_segment this_line_segment;
                all_line_segment.push_back(this_line_segment);
            }
            
            cout<<"a \"str"<<num_to_letter(i)<<"\" ";
            
            //vertices of this street, used to measure the length
            vector<vector<int>> vertices_of_this_street (num_of_seg+1, vector<int>(2));
         
            
            //generate <num_of_seg+1> vertices of <num_of_seg> line-segments for this street
            for(int j = 0; j < num_of_seg+1; j++)
            {
                
                
                if(j == 0)//if j == 0, just roll a vertex and create a new segment
                {
                    if (sum_seg == 0)
                    {
                        urandom.read((char*)&random_num, sizeof(int));//update random num
                        x = regen_c(random_num,c_k);
                        vertices_of_this_street[j][0] = x;
                        all_line_segment[sum_seg].x1 = x;
                        last_x = x;
                        
                        urandom.read((char*)&random_num, sizeof(int));//update random num
                        y = regen_c(random_num,c_k);
                        vertices_of_this_street[j][1] = y;
                        all_line_segment[sum_seg].y1 = y;
                        last_y = y;
                    }
                    else
                    {
                        vertices_of_this_street[j][0] = last_x;
                        all_line_segment[sum_seg].x1 = last_x;

                        vertices_of_this_street[j][1] = last_y;
                        all_line_segment[sum_seg].y1 = last_y;
                    }
                }
                
                if(j > 0)
                {
                    bool flag_overlap = true;
                    bool flag_length = true;
                    bool flag_self_intersect = true;
                    //3 flags're initialized as true, this loop'll executed at least once.
                    int n = 0;
                    while(flag_overlap or flag_length or flag_self_intersect)
                        //while(flag_overlap or flag_length)
                    {
                        n++;
                        //If your random generator fails to generate a valid specication for a continuous N number of attempts, it should exit() with an error message reported on stderr, and the program ends, (i.e., all other processes will be also terminated).
                        //Whatever value N you adopt, your error message should mention it. That is, your error message should be something like, "Error: failed to generate valid input for 25 simultaneous attempts”.
                        if(n > N)
                        {
                            cerr<<endl<<"Error: failed to generate valid input for "<<N<<" simultaneous attempts"<<endl;
                            urandom.close();
                            exit(0);
                        }
                        urandom.read((char*)&random_num, sizeof(int));//update random num
                        x = regen_c(random_num,c_k);
                        vertices_of_this_street[j][0] = x;
                        
                        urandom.read((char*)&random_num, sizeof(int));//update random num
                        y = regen_c(random_num,c_k);
                        vertices_of_this_street[j][1] = y;
                        
                        //cout<<"trying: ("<<x<<","<<y<<")"<<endl;/////////
                        
                        //You should not have any zero-length line segments
                        //verifying the length of the line-segment
                        if(x == vertices_of_this_street[j-1][0] and y == vertices_of_this_street[j-1][1])
                        {
                            flag_length = true;
                            //cout<<"attemp"<<n<<": length = 0"<<endl;
                        }
                        else
                        {
                            flag_length = false;
                        }
                        
                        //verifying whether overlap occur
                        all_line_segment[sum_seg+j-1].x2 = x;
                        all_line_segment[sum_seg+j-1].y2 = y;
                        all_line_segment[sum_seg+j-1].calculate_equation();
                        for(int o = 0; o < j; o++)//j -> j-1
                        {
                            if(overlap_occured(all_line_segment[j],all_line_segment[o]))
                            {
                                flag_overlap = true;
                                //cout<<"attemp"<<n<<": overlapping"<<endl;
                                break;
                            }
                            else
                            {
                                flag_overlap = false;
                            }
                        }//for(int o = 0; o < j; o++)
                        //verifying whether self-intersection occur
                        //cout<<"j = "<<j <<endl;
                        //cout<<"sum_seg = "<<sum_seg<<endl;
                        if(j > 2)
                        {
                            for(unsigned long si = sum_seg; si < sum_seg + j-1; si++)
                            {
                                //cout<<"comparing "<<si<<" and "<<sum_seg + j-1<<endl;
                                if(intersection_occured(all_line_segment[si],all_line_segment[sum_seg+j-1]))
                                {
                                    flag_self_intersect = true;
                                    //cout<<"attemp"<<n<<": self-intesection"<<endl;
                                    break;
                                }
                                else
                                {
                                    flag_self_intersect = false;
                                }
                            }
                        }
                        else
                        {
                            
                            flag_self_intersect = false;
                        }
                        //cout<<"flag_overlap = "<<flag_overlap<<"  flag_length = "<<flag_length<<" flag_self_intersect = "<<flag_self_intersect<<endl;
                    }//while(flag_overlap or flag_length)
                }//if(j > 0)
                
                if(j < num_of_seg-1)//not the last vertex?
                {
                    all_line_segment[sum_seg+j].x1 = x;
                    all_line_segment[sum_seg+j].y1 = y;
                }
                else
                {
                    last_x = x;
                    last_y = y;
                }
                
                if(j>0)
                {
                    cout<<"("<<x<<","<<y<<") ";
                }
                else
                {
                    cout<<"("<<last_x<<","<<last_y<<") ";
                }
                
            }
            cout<<endl;
        }
        
        //V 0???
        bool flag_V_0 = false;
        for(unsigned long v = 0; v < all_line_segment.size(); v++)
        {
            if(flag_V_0)
            {
                break;
            }
            for(unsigned long vv = v+1; vv < all_line_segment.size(); vv++)
            {
                if(intersection_occured(all_line_segment[v],all_line_segment[vv]))
                {
                    flag_V_0 = true;
                    break;
                }
            }
        }
        if(!flag_V_0)
        {
            for(int r = 0; r < num_of_str; r++)
                cout<<"r "<<"\"str"<<num_to_letter(r)<<"\""<<endl;//clean up and continue
            //cerr<<"Error: V 0"<<endl;
            continue;
        }
        
        cout<<"g"<<endl;
        
        //Your process should wait a random number w seconds,
        //where w is in [5,k] before generating the next (random) input.
        int wait_time = regen_l(random_num,l_k);
        sleep(wait_time);
        
        for(int r = 0; r < num_of_str; r++)
            cout<<"r "<<"\"str"<<num_to_letter(r)<<"\""<<endl;
    }
    
    // close random stream
    //urandom.close();
    return 0;
}
