#include<iostream>
#include<fstream>
#include<vector>
#include<map>
#include<set>

#define fori(i,n) for(int i = 0; i < n; i++)
#define ll long long
#define pb push_back


#if defined(__unix__) || defined(__APPLE__)
    #define is_OS_unix 1
    #include<unistd.h>
    #include<sys/types.h>
#elif defined(_WIN32) || defined(WIN32)
    #define is_OS_unix 0
    #include<windows.h>
#endif


using namespace std;


// Global Settings
string GROUP_SIZE = "15";

#ifdef is_OS_unix
    string intermediate_data_dir = "../data-intermediate/";
    string input_data_dir = "../";
#else
    string intermediate_data_dir = "..\\data-intermediate\\";
    string input_data_dir = "..\\"; 
#endif

// Global variables
map<pair<ll, ll>, int> edge_map;
map<pair<ll, ll>, int> friend_map;
long max_edge_weight = 0;

// map storing union of friends and edges
map<pair<ll, ll>, int> union_map;

// set storing intersection of friends and edges
set<pair<ll, ll>> intersection_set;
long intersection_set_init = 0;
long intersection_set_atleast_weight = 0;
long long intersection_set_length = 0;

// map storing the edge by weights
map<int,set<pair<ll,ll>>> edge_by_weight_map;

// int storing the number of common friends
long long common_friends = 0;

ll convert_id_to_int(string &str){
    // remove characters from the string
    int strlen = str.size();

    string newstr = "";
    fori(i,strlen){
        if(str[i] >= 48 && str[i] <= 57){
            newstr += str[i];
        }
    }

    ll id_int = stoll(newstr);
    return(id_int);
}

// function to pre-process the file
int friend_processor(string fileSuffix) {

    string GROUP_SIZE = fileSuffix;

    #ifdef is_OS_unix
        string requiredOutputFilename = intermediate_data_dir+"checked-friends" + GROUP_SIZE + ".csv";
        string requiredInputFileName = input_data_dir+"friends" + GROUP_SIZE + ".csv";
    #else
        string requiredOutputFilename = intermediate_data_dir+"checked-friends" + GROUP_SIZE + ".csv";
        string requiredInputFileName = input_data_dir+"friends" + GROUP_SIZE + ".csv";
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
        return -1;
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
        
        ll id1 = convert_id_to_int(str1);
        ll id2 = convert_id_to_int(str2);

        // check if the edge is already present
        if(friend_map.find({id1, id2}) != friend_map.end() or friend_map.find({id2, id1}) != friend_map.end()){
            cout << "Duplicate edge: " << id1 << " " << id2 << endl;
            // write to file
            outfile << id1 << "," << id2 << endl;
            common_friends++;
        }
        else{
            friend_map[{id1, id2}] = 1;
        } 
        
    }

    outfile.close();
    file.close();

    // destroy the char array
    delete[] requiredOutputFilenameChar;
    delete[] requiredInputFileNameChar;

    // remove all elements from the map with value 2
    for(auto it = friend_map.begin(); it != friend_map.end();){
        if(it->second == 2){
            it = friend_map.erase(it);
        }
        else{
            it++;
        }
    }

    return 1;
}



int read_friend(string fileSuffix){
    
    string GROUP_SIZE = fileSuffix;

    #ifdef is_OS_unix
        string requiredInputFileName = intermediate_data_dir+"checked-friends" + GROUP_SIZE + ".csv";
    #else
        string requiredInputFileName = intermediate_data_dir+"checked-friends" + GROUP_SIZE + ".csv";
    #endif

    // convert string to char array
    char *requiredInputFileNameChar = new char[requiredInputFileName.size() + 1];
    copy(requiredInputFileName.begin(), requiredInputFileName.end(), requiredInputFileNameChar);
    requiredInputFileNameChar[requiredInputFileName.size()] = '\0';

    // open file
    ifstream file(requiredInputFileNameChar);

    string str;
    string str1, str2;
    while (getline(file, str)) {
        // split string into 2 by comma
        int pos = str.find(",");
        str1 = str.substr(0, pos);
        str2 = str.substr(pos+1, str.size());
        
        ll id1 = convert_id_to_int(str1);
        ll id2 = convert_id_to_int(str2);

        // as the file is already processed, we can directly add the edge to the map
        friend_map[{id1, id2}] = 1;
        common_friends++;
    }

    file.close();

    // destroy the char array
    delete[] requiredInputFileNameChar;

    return 1;
}

int read_edge(string fileSuffix){
    
    string GROUP_SIZE = fileSuffix;

    #ifdef is_OS_unix
        string requiredInputFileName = intermediate_data_dir+"weighted-edges" + GROUP_SIZE + ".csv";
    #else
        string requiredInputFileName = intermediate_data_dir+"weighted-edges" + GROUP_SIZE + ".csv";
    #endif

    // convert string to char array
    char *requiredInputFileNameChar = new char[requiredInputFileName.size() + 1];
    copy(requiredInputFileName.begin(), requiredInputFileName.end(), requiredInputFileNameChar);
    requiredInputFileNameChar[requiredInputFileName.size()] = '\0';

    // open file
    ifstream file(requiredInputFileNameChar);

    string str;
    string str1, str2,str3;
    while (getline(file, str)) {
        // format of edge file = id1,id2,weight
        // split string into 3 by comma
        int pos1 = str.find(",");
        int pos2 = str.find(",", pos1+1);
        str1 = str.substr(0, pos1);
        str2 = str.substr(pos1+1, pos2-pos1-1);
        str3 = str.substr(pos2+1, str.size());

        ll id1 = convert_id_to_int(str1);
        ll id2 = convert_id_to_int(str2);
        ll weight = convert_id_to_int(str3);

        // as the file is already processed, we can directly insert the edge to the map
        // edge_map[{id1, id2}] = weight;
        if(weight > max_edge_weight){
            max_edge_weight = weight;
        }

        edge_by_weight_map[weight].insert({id1, id2});
    }

    file.close();

    // destroy the char array
    delete[] requiredInputFileNameChar;

    return 1;
}


int edge_processor(string fileSuffix){

    string GROUP_SIZE = fileSuffix;

    // for windows file support
#ifdef is_OS_unix
    string requiredOutputFilename = intermediate_data_dir+"weighted-edges" + GROUP_SIZE + ".csv";
    string outputMaxWeightFilename = intermediate_data_dir+"max-edge-weight"+ GROUP_SIZE + ".csv";
    string requiredInputFileName = "../edges" + GROUP_SIZE + ".csv";
#else
    string requiredOutputFilename = intermediate_data_dir+"weighted-edges" + GROUP_SIZE + ".csv";
    string outputMaxWeightFilename = intermediate_data_dir+"max-edge-weight"+ GROUP_SIZE + ".csv";
    string requiredInputFileName = input_data_dir+"edges" + GROUP_SIZE + ".csv";
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
    while (getline(file, str)) {
        // detect and ignore header
        if(str[0] == 'S' or str[0] == 's'){
            continue;
        }
        // split string into 2 by comma
        int pos = str.find(",");
        str1 = str.substr(0, pos);
        str2 = str.substr(pos+1, str.size());

        // convert string to int
        ll id1 = convert_id_to_int(str1);
        ll id2 = convert_id_to_int(str2);

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

        // add to edge_by_weight
        edge_by_weight_map[it->second].insert({it->first.first, it->first.second});
    } 

    // write maxWeight to file
    maxCount_file << maxWeight << endl;
    max_edge_weight = maxWeight;

    cout << "Written to Edges with weights to file" << endl;


    // close file
    outfile.close();
    file.close();

    // destroy the char array
    delete[] requiredOutputFilenameChar;
    delete[] requiredInputFileNameChar;

    return 1;
}


// take in argument of function as pointer to edge_by_weight_map
ll intersection_len_calculator(long common_group_size, map<int, set<pair<ll, ll>>> *edge_by_weight, map<pair<ll, ll>, int> *friend_map){
    ll count = 0;
    // for each pair in edge_by_weight_map[common_group_size], check if they are friends
    // if they are friends, add to count
    for(auto it = edge_by_weight_map[common_group_size].begin(); it != edge_by_weight_map[common_group_size].end(); it++){
        // check if they are friends
        auto it2 = friend_map->find(make_pair(it->first, it->second));
        if(it2 == friend_map->end()){
            // pair not found
            it2 = friend_map->find(make_pair(it->second, it->first));
        }

        if(it2 != friend_map->end()){
            // pair found
            count += 1;
        }
    }
    return count;
}

// ll union_len_calculator(long common_group_size, map<int, set<pair<ll, ll>>> *edge_by_weight){
//     ll count = 0;
//     // for each pair in edge_by_weight_map[common_group_size], add to count
//     for(auto it = edge_by_weight_map[common_group_size].begin(); it != edge_by_weight_map[common_group_size].end(); it++){
//         count += 1;
//     }
//     return count;
// }


long long calculate_intersection(long atleast_group_size){
    // for each pair in edge_by_weight_map[common_group_size], check if they are friends
    // if they are friends, add to intersection_set

    if(intersection_set_atleast_weight == atleast_group_size){
        return intersection_set_length;
    }else if(intersection_set_atleast_weight > atleast_group_size){

    }else{
        intersection_set.clear();
        intersection_set_atleast_weight = max_edge_weight;
    }

    intersection_set_init = false;

    long this_group_size = atleast_group_size;


    // iterating over different weights from max to atleast required
    // if the values for more common groups are cached, then it will not be calculated again

    for(this_group_size = intersection_set_atleast_weight; this_group_size > atleast_group_size;  this_group_size-- ){
    // for(this_group_size = atleast_group_size; this_group_size < max_edge_weight;  this_group_size++ ){
        for(auto it = edge_by_weight_map[this_group_size].begin(); it != edge_by_weight_map[this_group_size].end(); it++){
            // check if they are friends
            auto it2 = friend_map.find(make_pair(it->first, it->second));
            if(it2 == friend_map.end()){
                // pair not found
                it2 = friend_map.find(make_pair(it->second, it->first));
            }

            if(it2 != friend_map.end()){
                // pair found
                intersection_set.insert(make_pair(it->first, it->second));
            }
        }
    }

    intersection_set_length = intersection_set.size();

    intersection_set_init = true;
    intersection_set_atleast_weight = atleast_group_size;

    return intersection_set_length;
}

ll false_positive_len(long common_group_size){
    // for each pair in edge_by_weight_map[common_group_size], check if they are friends
    // if they are friends, add to intersection_set


    calculate_intersection(common_group_size);

    long this_group_size;

    set<pair<ll, ll>> false_positive_set;

    // copy all pairs in edge_by_weight_map[common_group_size] to false_positive_set
    for(this_group_size = common_group_size; this_group_size < max_edge_weight; this_group_size++){
        for(auto it = edge_by_weight_map[this_group_size].begin(); it != edge_by_weight_map[this_group_size].end(); it++){
            false_positive_set.insert(*it);
        }
    }

    // remove all pairs in intersection_set from false_positive_set
    for(auto it = intersection_set.begin(); it != intersection_set.end(); it++){
        false_positive_set.erase(*it);
    }

    return false_positive_set.size(); 

}

int false_negative_len(long common_group_size){

}


long long get_len_edges_in_similar_atleast(long common_group_size){
    long long sum = 0;

    for(long i = common_group_size; i < max_edge_weight; i++){
        sum += edge_by_weight_map[i].size();
    }
    return sum;
}


float calculate_inter_over_Gsk(long common_group_size){

    long long setLen = calculate_intersection(common_group_size);

    long long edges_in_similar_atleast = get_len_edges_in_similar_atleast(common_group_size);

    return setLen / (double)(edges_in_similar_atleast);
}

float calculate_inter_over_Gf(long common_group_size){

    long long setLen = calculate_intersection(common_group_size);

    return setLen / (double)(common_friends);
}   

int check_pre_process_cache_exists(string fileSuffix){
    string GROUP_SIZE = fileSuffix;

    int edges_processed = 0;
    int friends_processed = 0;

#ifdef is_OS_unix
    string requiredEdgeFilename = intermediate_data_dir+"weighted-edges" + GROUP_SIZE + ".csv";
    string requiredFriendFilename = intermediate_data_dir+"checked-friends" + GROUP_SIZE + ".csv";
#else
    string requiredEdgeFilename = intermediate_data_dir+"weighted-edges" + GROUP_SIZE + ".csv";
    string requiredFriendFilename = intermediate_data_dir+"checked-friends" + GROUP_SIZE + ".csv";
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
        cout << "Edge intermediate is processed." << endl;
        edges_processed = 1;
        read_edge(GROUP_SIZE_char);
    }
    else{
        // file doesn't exist
        cout << "Pre-processed edge file doesn't exist" << endl;
        edges_processed = edge_processor(GROUP_SIZE);
    }

    // check if the following file exists: data-intermediate/checked-friends5.csv or data-intermediate/checked-friends10.csv or data-intermediate/checked-friends15.csv
    if(access(requiredFriendFilenameChar, F_OK) != -1){
        // file exists
        cout << "Friends intermediate is processed. " << endl;
        friends_processed = 1;
        read_friend(GROUP_SIZE_char);
    }
    else{
        // file doesn't exist   
        cout << "File doesn't exist" << endl;
        friends_processed = friend_processor(GROUP_SIZE);
    }

    if(friends_processed && edges_processed){
        return 1;
    }

}


int store_vector_float_to_filename(vector<float> &vec, string filename){
    ofstream myfile;

    // convert to char array
    char *filename_char = new char[filename.size() + 1];
    copy(filename.begin(), filename.end(), filename_char);
    filename_char[filename.size()] = '\0';

    myfile.open(filename);
    for(auto it = vec.begin(); it != vec.end(); it++){
        myfile << *it << endl;
    }
    myfile.close();
    return 0;
}

int main(int argc, char *argv[]) {
    string GROUP_SIZE = "15";

    // get group size from arguments
    if(argc > 1){
        GROUP_SIZE = argv[1];
    }else{
        cout << "To define input files, use: ./main1 ${Group Count}" << endl;
        cout << "Example: ./main 15" << endl;
        cout << "will use files 'edges15.csv' as input" << endl;
    }

    // check if the pre-processed files exist
    if(check_pre_process_cache_exists(GROUP_SIZE)){
        cout << "Pre-processed files exist." << endl;
        cout << "Reading from pre-processed files..." << endl;
    }else{
        return -1;
    }

    // print the size of the edgemap
    cout << "Size of edge map: " << edge_map.size() << endl;
    cout << "Size of friend map: " << friend_map.size() << endl;

    vector<float> outputs_inter_over_Gsk;

    float temp = 0;

    // print value of intersection over count of Gsk
    // cout << "Intersection over union: " << calculate_inter_over_Gsk(5) << endl;
    for(int i = max_edge_weight; i > 0; i--){
        temp = calculate_inter_over_Gsk(i);
        cout << "Intersection over GsK for common groups (" << i << "): Value = " << temp << endl;
        outputs_inter_over_Gsk.push_back(temp);
    }
    
    // store to file
    store_vector_float_to_filename(outputs_inter_over_Gsk, "outputs_inter_over_Gsk.csv");

    vector<float> outputs_inter_over_Gf;

    // print value of intersection over count of Gf
    // cout << "Intersection over union: " << calculate_inter_over_Gf(5) << endl;
    for(int i = max_edge_weight; i > 0; i--){
        temp = calculate_inter_over_Gf(i);
        cout << "Intersection over Gf for common groups (" << i << "): Value = " << temp << endl;
        outputs_inter_over_Gf.push_back(temp);
    }

    // store to file
    store_vector_float_to_filename(outputs_inter_over_Gf, "outputs_inter_over_Gf.csv");


    return 0;

}