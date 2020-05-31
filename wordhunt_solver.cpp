/*
 *
 * Author: Nathan Ang (nathanan@andrew.cmu.edu)
 *
 * A lightweight solver for the popular iMessage game WordHunt.
 *
 * WordHunt is a game where players are given a 4x4 board of letters 
 * and drag their finger along the board to make as many words as 
 * possible in under a minute and 30 seconds.
 * The more words that a player makes, the more points they get, 
 * and the longer the words, the higher their score is as well. 
 * The player with the highest score at the end of the time limit wins the game.
 * 
 * It implements DFS and a Trie data structure to compute a list of all
 * the optimum and possible valid words (and paths), given a board in the game.
 * It is able to solve any board very efficiently in less than a second.
 *
 * To use, simply run the program and input in the board from left to right
 * i.e. Board:         Input:
 *      
 *      a b c d        abcdefghijklmnop
 *      e f g h
 *      i j k l
 *      m n o p
 *
*/

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

struct TrieNode {
    map<string, TrieNode*> children; //children are stored as (key= next letter, item=TrieNode)
    bool full_word = false;
};

struct TrieNode *root = new TrieNode;

/*
 * make_trie is a helper function that is used at the
 * start of main in order to fill our Trie data structure
 * with all the list of valid words from our dictionary text file
 *
*/

void make_trie()
{   
    ifstream dictionary_file;
    dictionary_file.open("./dictionary.txt");
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
/*
 * recurse is a helper functions that traverses through our board.
 * It utilizes DFS, and checks if we find any valid words, and adds them
 * to our list of solutions. It also stops traversing
 * when there are no more possible words to be made by checking the 
 * children of our current TrieNode.
 *
 * Parameters: current row, current column, current combination of letters,
 * current path, visited array for DFS, and current TrieNode
 *
*/
void recurse(int row, int col, string word, string path, bool visited[4][4], 
             struct TrieNode *node)
{
	if(row < 0 || row >= 4 || col < 0 || col >= 4)
		return;
	if(visited[row][col])
		return;

    string letter = string(1,board[row][col]);

    //if no children, we are at the end of possible valid combination of letters
    //for the current traversal
    if((node->children).find(letter) == (node->children).end())
        return;
    word += letter;

	visited[row][col] = true;

    //check if word is in trie
    if(word.length() > 3 && (node->children)[letter]->full_word) //if the word is valid
    {
        ans.push_back(make_pair(word, path));
    }

    vector <pair <pair <int, int>, string >> directions =  
        { make_pair(make_pair(1, 0), ", D"),
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

/*
 * main inputs the boards from the user and prints out all the optimum and
 * possible solutions to be used
 *
*/

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