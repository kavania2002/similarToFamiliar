#include<iostream>
#include<fstream>
#include<vector>
#include<map>
#include<cstring>
#include <sys/types.h>
#include <unistd.h>


#if defined(__unix__) || defined(__APPLE__)
    #define is_OS_unix 1
#elif defined(_WIN32) || defined(WIN32)
    #define is_OS_unix 0
    #include<windows.h>
#endif


#define fori(i,n) for(int i = 0; i < n; i++)

using namespace std;

// #define GROUP_SIZE "5"


long long convert_id_to_int(string &str){
    // remove characters from the string
    int strlen = str.size();

    string newstr = "";
    fori(i,strlen){
        if(str[i] >= 48 && str[i] <= 57){
            newstr += str[i];
        }
    }

    long long id_int = stoll(newstr);
    return(id_int);
}

// map to store the edges
map<pair<long long, long long>, int> edge_map;


int main(int argc, char *argv[]) {

    string GROUP_SIZE = "5";


    // get group size from arguments
    if(argc > 1){
        GROUP_SIZE = argv[1];
    }else{
        cout << "To define input files, use: ./main1 ${Group Count}" << endl;
        cout << "Example: ./main1 15" << endl;
        cout << "will use files 'weighted-edges15.csv'" << endl;
    }


#ifdef is_OS_unix
    string requiredEdgeFilename = "../data-intermediate/edges" + GROUP_SIZE + ".csv";
    string requiredFriendFilename = "../data-intermediate/checked-friends" + GROUP_SIZE + ".csv";
#else
    string requiredEdgeFilename = "..\\data-intermediate\\edges" + GROUP_SIZE + ".csv";
    string requiredFriendFilename = "..\\data-intermediate\\checked-friends" + GROUP_SIZE + ".csv";
#endif

    // convert string to char array
    char *requiredEdgeFilenameChar = new char[requiredEdgeFilename.size() + 1];
    copy(requiredEdgeFilename.begin(), requiredEdgeFilename.end(), requiredEdgeFilenameChar);
    requiredEdgeFilenameChar[requiredEdgeFilename.size()] = '\0';

    char *requiredFriendFilenameChar = new char[requiredFriendFilename.size() + 1];
    copy(requiredFriendFilename.begin(), requiredFriendFilename.end(), requiredFriendFilenameChar);
    requiredFriendFilenameChar[requiredFriendFilename.size()] = '\0';

    // group size to char array
    char *GROUP_SIZE_char = new char[GROUP_SIZE.size() + 1];
    copy(GROUP_SIZE.begin(), GROUP_SIZE.end(), GROUP_SIZE_char);
    GROUP_SIZE_char[GROUP_SIZE.size()] = '\0';


    // check if the following file exists: data-intermediate/edges5.csv or data-intermediate/edges10.csv or data-intermediate/edges15.csv
    if(access(requiredEdgeFilenameChar, F_OK) != -1){
        // file exists
        cout << "Edge weights are processed already." << endl;
    }
    else{
        // file doesn't exist
        cout << "Pre-processed file doesn't exist" << endl;
#ifdef is_OS_unix
        cout << "Running edges pre-processor..." << endl;
        // run the pre-processor
        system("g++ ./pre-processors/edges.cpp -o ./pre-processors/edges");

        // combine the command with Group Size.
        char *edgeExec = new char[30 + GROUP_SIZE.size() + 1];
        bzero(edgeExec, 30 + GROUP_SIZE.size() + 1);
        strcpy(edgeExec, "./pre-processors/edges ");
        strcat(edgeExec, GROUP_SIZE_char);
        
        system(edgeExec);
#else
        cout << "Please run compile & run pre-processors/edges.cpp manually as system is not Unix-like." << endl;
        return 0;
#endif
    }

    // check if the following file exists: data-intermediate/checked-friends5.csv or data-intermediate/checked-friends10.csv or data-intermediate/checked-friends15.csv
    if(access(requiredFriendFilenameChar, F_OK) != -1){
        // file exists
        cout << "Friends is processed" << endl;
    }
    else{
        // file doesn't exist   
        cout << "File doesn't exist" << endl;
#ifdef is_OS_unix
        cout << "Running friend pre-processor..." << endl;
        // run the pre-processor
        system("g++ ./pre-processors/friend-finder.cpp -o ./pre-processors/friend-finder");

        // combine the command with Group Size.
        char *friendExec = new char[30 + GROUP_SIZE.size() + 1];
        bzero(friendExec, 30 + GROUP_SIZE.size() + 1);
        strcpy(friendExec, "./pre-processors/friend-finder ");
        strcat(friendExec, GROUP_SIZE_char);

        system(friendExec);
        // system("./pre-processors/friend-finder " + GROUP_SIZE_char);
#else
        cout << "Please compile & run pre-processors/friend.cpp manually as system is not Unix-like." << endl;
        return 0;   
#endif
    }

    // read file edges15.csv
    ifstream file(requiredEdgeFilename);


    // write output to file data-intermediate/edges5.csv
    ofstream outfile("../data-intermediate/edges15.csv");

    string str;
    string str1, str2;


    return 0;
}