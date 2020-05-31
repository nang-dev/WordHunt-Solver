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

struct TrieNode {
    map<string, TrieNode*> children; //children are stored as (key= next letter, item=TrieNode)
    bool full_word = false;
};

struct TrieNode *root = new TrieNode;

struct compare { //compare by length of word
    bool operator()(pair<string,string> first, pair<string,string> second) {
        return first.first.size() < second.first.size();
    }
};

void make_trie()
{   
    ifstream dictionary_file;
    dictionary_file.open("dictionaries/470kwords.txt");
    string word;
    if (dictionary_file.is_open()) {
        while (!dictionary_file.eof()) {
            struct TrieNode *curr = root;
            dictionary_file >> word;
            for(int i = 0; i < word.length(); i++)
            {
                string letter = string(1, word[i]);
                if((curr->children).find(letter) == (curr->children).end())
                    (curr->children)[letter] = new TrieNode;
                curr = (curr->children)[letter];
                if(i == word.length() - 1)
                    curr -> full_word = true;
            }
        }
    }
}

void recurse(int row, int col, string word, string path, bool visited[4][4], struct TrieNode *node)
{
	if(row < 0 || row >= 4 || col < 0 || col >= 4)
		return;
	if(visited[row][col])
		return;

    string letter = string(1,board[row][col]);

    if((node->children).find(letter) == (node->children).end())
        return;
    word += letter;

	visited[row][col] = true;

    if(word.length() > 3 && (node->children)[letter]->full_word) //if the word is valid
    {
        ans.push_back(make_pair(word, path));
    }

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
            if(!visited[row+x][col+y])
                recurse(row+x, col+y, word, path+d, visited, (node->children)[letter]);
    }
    visited[row][col] = false;
}

int main() 
{
    make_trie();
    cout << "Enter the board:";
    string read;
    cin >> read;

    board[0] = read.substr(0,4);
    board[1] = read.substr(4,8);
    board[2] = read.substr(8,12);
    board[3] = read.substr(12,16);

    bool visited[4][4];
    for(int i = 0; i < 4; i++)
    	for(int j = 0; j < 4; j++)
    		visited[i][j] = false;

    auto start = chrono::system_clock::now();

    for(int i = 0; i < 4; i++)
    	for(int j = 0; j < 4; j++)
    		recurse(i, j, "", ("(" + to_string(i) + ", " + to_string(j) + ")"),  visited, root);

    //sort the results
    compare c;
    sort(ans.begin(), ans.end(), c);

    //print out the results
    for(int i = 0; i < ans.size(); i++)
       cout << ans.size() - i << ": " << ans[i].first << "\n   " << ans[i].second << "\n\n";
 
    //print time data
    auto end = chrono::system_clock::now();

    chrono::duration<double> elapsed_seconds = end-start;
    time_t end_time = chrono::system_clock::to_time_t(end);

    cout << "Elapsed Time: " << elapsed_seconds.count() << "s\n";
              
}