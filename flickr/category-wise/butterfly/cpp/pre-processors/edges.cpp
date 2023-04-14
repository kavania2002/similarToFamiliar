
#define fori(i,n) for(int i = 0; i < n; i++)

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
    }else{
        cout << "To define input files, use: ./edges ${Group Count}" << endl;
        cout << "Example: ./edges 15" << endl;
        cout << "will use files 'edges15.csv' and output to 'weighted-edges15.csv'" << endl;
    }

// for windows file support
#ifdef is_OS_unix
    string requiredOutputFilename = "../data-intermediate/weighted-edges" + GROUP_SIZE + ".csv";
    string outputMaxWeightFilename = "../data-intermediate/max-edge-weight"+ GROUP_SIZE + ".csv";
    string requiredInputFileName = "../edges" + GROUP_SIZE + ".csv";
#else
    string requiredOutputFilename = "..\\data-intermediate\\weighted-edges" + GROUP_SIZE + ".csv";
    string outputMaxWeightFilename = "..\\data-intermediate\\max-edge-weight"+ GROUP_SIZE + ".csv";
    string requiredInputFileName = "..\\edges" + GROUP_SIZE + ".csv";
#endif

    // convert string to char array
    char *requiredOutputFilenameChar = new char[requiredOutputFilename.size() + 1];
    copy(requiredOutputFilename.begin(), requiredOutputFilename.end(), requiredOutputFilenameChar);
    requiredOutputFilenameChar[requiredOutputFilename.size()] = '\0';

    char *outputMaxWeightFilenameChar = new char[outputMaxWeightFilename.size() + 1];
    copy(outputMaxWeightFilename.begin(), outputMaxWeightFilename.end(), outputMaxWeightFilenameChar);
    outputMaxWeightFilenameChar[outputMaxWeightFilename.size()] = '\0';

    char *requiredInputFileNameChar = new char[requiredInputFileName.size() + 1];
    copy(requiredInputFileName.begin(), requiredInputFileName.end(), requiredInputFileNameChar);
    requiredInputFileNameChar[requiredInputFileName.size()] = '\0';


    // check if the required input file exists
    if(access(requiredInputFileNameChar, F_OK) == -1){
        cout << "Required input file does not exist" << endl;
        cout << "Filename: '" << requiredInputFileNameChar << "'" << endl;
        return 0;
    }
    cout << "Pre-processing file..." << endl;

    

    // read file edges15.csv
    ifstream file(requiredInputFileNameChar);


    // write output to file data-intermediate/edges5.csv
    ofstream outfile(requiredOutputFilenameChar);

    ofstream maxCount_file(outputMaxWeightFilenameChar);

    long maxWeight = 0;

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

        // convert string to int
        long long id1 = convert_id_to_int(str1);
        long long id2 = convert_id_to_int(str2);

        // search pair in map
        auto it = edge_map.find(make_pair(id1, id2));

        // find inverse edge
        if(it == edge_map.end()){
            // pair not found
            it = edge_map.find(make_pair(id2, id1));
        }

        // add weight
        if(it != edge_map.end()){
            // pair found
            it->second += 1;
            if(it->second > maxWeight){
                maxWeight = it->second;
            }
        }else{
            // pair not found
            edge_map.insert(make_pair(make_pair(id1, id2), 1));
        }

        // // write to file
        // outfile << convert_id_to_int(str1) << "," << convert_id_to_int(str2) << "," <<  << endl;
    }

    cout << "Edges Weight Calculated." << endl;
    cout << "Writing to file..." << endl;

    // write the map pair with weights
    for(auto it = edge_map.begin(); it != edge_map.end(); it++){
        outfile << it->first.first << "," << it->first.second << "," << it->second << endl;
    } 

    // write maxWeight to file
    maxCount_file << maxWeight << endl;

    cout << "Written to Edges with weights to file" << endl;

    return 0;
}