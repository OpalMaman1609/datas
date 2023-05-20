// 
// 234218 Data Structures 1.
// Semester: 2023B (spring).
// Wet Exercise #1.
// 
// Recommended TAB size to view this file: 8.
// 
// The following main file is necessary to link and run your code.
// This file is READ ONLY: even if you submit something else, the compiler ..
// .. WILL use our file.
// 



#include "StreamingDBa1.h"
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

void print(string cmd, StatusType res);
void print(string cmd, output_t<int> res);
void query_get_all_movies(string cmd, streaming_database *obj, Genre genre);

int main()
{
	
    int d1, d2, d3, g1;
    string b1;
    bool b;

    // Init
    streaming_database *obj = new streaming_database();
	
    // Execute all commands in file
	string op;
    std::ifstream infile("./inFiles/test3.in");
	while (infile >> op)
    // while (cin >> op)
    {
        if (!op.compare("add_movie")) {
            infile >> d1 >> g1 >> d2 >> b1;
            if (!b1.compare("True")) {
                b = true;
            }
            else if (!b1.compare("False")) {
                b = false;
            }
            else {
                cout << "Invalid input format" << endl;
                return -1;
            }
            print(op, obj->add_movie(d1, (Genre) g1, d2, b));
        } else if (!op.compare("remove_movie")) {
            infile >> d1;
            print(op, obj->remove_movie(d1));
        } else if (!op.compare("add_user")) {
            infile >> d1 >> b1;
            if (!b1.compare("True")) {
                b = true;
            }
            else if (!b1.compare("False")) {
                b = false;
            }
            else {
                cout << "Invalid input format" << endl;
                return -1;
            }
            print(op, obj->add_user(d1, b));
        } else if (!op.compare("remove_user")) {
            infile >> d1;
            print(op, obj->remove_user(d1));
        } else if (!op.compare("add_group")) {
            infile >> d1;
            print(op, obj->add_group(d1));
        } else if (!op.compare("remove_group")) {
            infile >> d1;
            print(op, obj->remove_group(d1));
        } else if (!op.compare("add_user_to_group")) {
            infile >> d1 >> d2;
            print(op, obj->add_user_to_group(d1, d2));
        } else if (!op.compare("get_all_movies_count")) {
            infile >> g1;
            print(op, obj->get_all_movies_count((Genre) g1));
        } else if (!op.compare("get_all_movies")) {
            infile >> g1;
        
            query_get_all_movies(op, obj, (Genre) g1);
        } else if (!op.compare("user_watch")) {
            infile >> d1 >> d2;
            print(op, obj->user_watch(d1, d2));
        } else if (!op.compare("group_watch")) {
            infile >> d1 >> d2;
            print(op, obj->group_watch(d1, d2));
        } else if (!op.compare("get_num_views")) {
            infile >> d1 >> g1;
            print(op, obj->get_num_views(d1, (Genre) g1));
        } else if (!op.compare("rate_movie")) {
            infile >> d1 >> d2 >> d3;
            print(op, obj->rate_movie(d1, d2, d3));
        } else if (!op.compare("get_group_recommendation")) {
            infile >> d1;
            print(op, obj->get_group_recommendation(d1));
        } else {
            cout << "Unknown command: " << op << endl;
            return -1;
        }
        // Verify no faults
        if (cin.fail()){
            cout << "Invalid input format" << endl;
            return -1;
        }
    }

    // Quit 
	delete obj;
	return 0;
}

// Helpers
static const char *StatusTypeStr[] =
{
   	"SUCCESS",
	"ALLOCATION_ERROR",
	"INVALID_INPUT",
	"FAILURE"
};

void print(string cmd, StatusType res) 
{
	cout << cmd << ": " << StatusTypeStr[(int) res] << endl;
}

void print(string cmd, output_t<int> res)
{
    if (res.status() == StatusType::SUCCESS) {
	    cout << cmd << ": " << StatusTypeStr[(int) res.status()] << ", " << res.ans() << endl;
    } else {
	    cout << cmd << ": " << StatusTypeStr[(int) res.status()] << endl;
    }
}

void query_get_all_movies(string cmd, streaming_database *obj, Genre genre)
{
    output_t<int> count = obj->get_all_movies_count(genre);
    int to_alloc = count.ans();
    if (to_alloc == 0)
    {
        //if there are no movies we will allocate 1 so that we will get failure instead of invalid input
        ++to_alloc;
    }
    // Allocate if okay
    int *out_mem = nullptr;
    if (count.status() == StatusType::SUCCESS)
    {
        out_mem = new int[to_alloc];
        for (int i = 0; i < to_alloc; ++i) out_mem[i] = -1;
    }
    // Call function
    StatusType status = obj->get_all_movies(genre, out_mem);
    print(cmd, status);
    if (status == StatusType::SUCCESS) {
	    for (int i = 0; i < to_alloc; ++i)
        {
		    cout << out_mem[i] << endl;
	    }
    }
    delete[] out_mem;
}


// int main(){
//    int a = 1;
//     streaming_database *obj = new streaming_database();
   
//     // StatusType status = obj->add_user(123,true);
//     // StatusType status1 = obj->add_user(1,true);
//     // StatusType status2 = obj->add_user(2,true);
//     // StatusType status3 = obj->add_group(2);
//     // StatusType status7 = obj->add_group(3);
//     // StatusType status4 = obj->add_user_to_group(1,2);
//     // StatusType status9 = obj->add_user_to_group(1,3);
//     // StatusType status6 = obj->add_user_to_group(123,2);
//     // StatusType status8 = obj->add_user_to_group(2,3);
//     // StatusType status5 = obj->remove_user(2);
  
//     //StatusType status = obj->add_group(1);
//     StatusType status6 = obj->add_user(1,true);
//     StatusType status2 = obj->add_user(2,true);
//     StatusType status4 = obj->add_user_to_group(1,1);
//     //StatusType status5 = obj->add_user_to_group(2,1);
//    StatusType status1 = obj->remove_group(1);
//     //StatusType status2 = obj->add_user(5257,true);

//     StatusType status7 = obj->add_movie(2772,Genre::ACTION, 14, false);
//     output_t<int> status8 = obj->get_group_recommendation(1);
//     //StatusType status5 = obj->remove_user(5257);

//     delete obj;
// }
