#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <map>
#include <time.h> 
#include <chrono>
using namespace std;

string board[4];
map<string, int> dict; //map is stored as (word, item)
vector<pair<string, string>> ans; //answer is stored as (word, path)

struct compare { //compare by length of word
    bool operator()(pair<string,string> first, pair<string,string> second) {
        return first.first.size() < second.first.size();
    }
};

void make_dict()
{
    ifstream dictionary_file;
    dictionary_file.open("dictionaries/470kwords.txt");
    string word;
    if (dictionary_file.is_open()) {
        while (!dictionary_file.eof()) {
            dictionary_file >> word;
            dict.insert(make_pair(word, 0));
        }
    }
}

void recurse(int row, int col, string word, string path, bool visited[4][4])
{

	if(row < 0 || row >= 4 || col < 0 || col >= 4)
		return;
	if(visited[row][col])
		return;

	word += board[row][col];
	visited[row][col] = true;
    if(word.length() > 2 && dict.find(word) != dict.end()) //if the word is valid
    {
        ans.push_back(make_pair(word, path));
    }

    //check if word is in dictionary
    vector <pair <pair <int, int>, string >> directions =  { make_pair(make_pair(1, 0), ", D"),
                                                             make_pair(make_pair(0, 1), ", R"), 
                                                             make_pair(make_pair(-1, 0), ", U"), 
                                                             make_pair(make_pair(0, -1), ", L"), 
                                                             make_pair(make_pair(1, -1), ", LD"), 
                                                             make_pair(make_pair(-1, 1), ", RU"), 
                                                             make_pair(make_pair(1, 1), ", RD"), 
                                                             make_pair(make_pair(-1, -1), ", LU") };


    for( pair < pair <int, int>, string > elem : directions) 
    {
        int x = elem.first.first;
        int y = elem.first.second;
        string d = elem.second;
        if(row+x >= 0 && row+x < 4 && col+y >= 0 && col+y < 4)
        {
            if(!visited[row+x][col+y])
                recurse(row+x, col+y, word, path+d, visited);
        }
    }
    visited[row][col] = false;
}

int main() 
{
    make_dict();
    cout << "Enter the board:";
    string read;
    cin >> read;

    board[0] = read.substr(0,4);
    board[1] = read.substr(4,8);
    board[2] = read.substr(8,12);
    board[3] = read.substr(12,16);

    //vistited matrix for our DFS traversal
    bool visited[4][4];
    for(int i = 0; i < 4; i++)
    	for(int j = 0; j < 4; j++)
    		visited[i][j] = false;

    auto start = chrono::system_clock::now();

    for(int i = 0; i < 4; i++)
    {
    	for(int j = 0; j < 4; j++)
        {
    		recurse(i, j, "", ("(" + to_string(i) + ", " + to_string(j) + ")"),  visited);
        }
    }

    //sort the results
    compare c;
    sort(ans.begin(), ans.end(), c);

    //print out the results
    for(int i = 0; i < ans.size(); i++) 
    {
       cout << ans.size() - i << ": " << ans[i].first << "\n   " << ans[i].second << "\n\n";
    }
 
    auto end = chrono::system_clock::now();

    chrono::duration<double> elapsed_seconds = end-start;
    time_t end_time = chrono::system_clock::to_time_t(end);

    cout << "finished computation at " << ctime(&end_time)
              << "elapsed time: " << elapsed_seconds.count() << "s\n";
              
}