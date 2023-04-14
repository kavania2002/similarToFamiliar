#include<iostream>
#include<fstream>
#include<vector>
#include<map>
#include<unistd.h>


#if defined(__unix__) || defined(__APPLE__)
    #define is_OS_unix 1
#elif defined(_WIN32) || defined(WIN32)
    #define is_OS_unix 0
#endif


#define fori(i,n) for(int i = 0; i < n; i++)

using namespace std;

// #define GROUP_SIZE 5


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
    }


#ifdef is_OS_unix
    string requiredOutputFilename = "../data-intermediate/checked-friends" + GROUP_SIZE + ".csv";
    string requiredInputFileName = "../friends" + GROUP_SIZE + ".csv";
#else
    string requiredOutputFilename = "..\\data-intermediate\\checked-friends" + GROUP_SIZE + ".csv";
    string requiredInputFileName = "..\\friends" + GROUP_SIZE + ".csv";
#endif

    // convert string to char array
    char *requiredOutputFilenameChar = new char[requiredOutputFilename.size() + 1];
    copy(requiredOutputFilename.begin(), requiredOutputFilename.end(), requiredOutputFilenameChar);
    requiredOutputFilenameChar[requiredOutputFilename.size()] = '\0';

    char *requiredInputFileNameChar = new char[requiredInputFileName.size() + 1];
    copy(requiredInputFileName.begin(), requiredInputFileName.end(), requiredInputFileNameChar);
    requiredInputFileNameChar[requiredInputFileName.size()] = '\0';


    // check if the required input file exists
    if(access(requiredInputFileNameChar, F_OK) == -1){
        cout << "Required input file does not exist" << endl;
        cout << "Filename: '" << requiredInputFileNameChar << "'" << endl;
        return 0;
    }

    
    // read file edges15.csv
    ifstream file(requiredInputFileNameChar);


    // write output to file data-intermediate/edges5.csv
    ofstream outfile(requiredOutputFilenameChar);

    string str;
    string str1, str2;
    while (getline(file, str))
    {
        // detect and ignore header
        if(str[0] == 'S' or str[0] == 's'){
            continue;
        }
        // split string into 2 by comma
        int pos = str.find(",");
        str1 = str.substr(0, pos);
        str2 = str.substr(pos+1, str.size());
        
        long long id1 = convert_id_to_int(str1);
        long long id2 = convert_id_to_int(str2);

        // check if the edge is already present
        if(edge_map.find({id1, id2}) != edge_map.end() or edge_map.find({id2, id1}) != edge_map.end()){
            cout << "Duplicate edge: " << id1 << " " << id2 << endl;
            // write to file
            outfile << id1 << "," << id2 << endl;
        }
        else{
            edge_map[{id1, id2}] = 1;
        } 

        // write to file
    }

    return 0;
}