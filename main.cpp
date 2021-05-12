int WORD_COUNT;

#include<stdlib.h>
#include<iomanip>
#include <string>
#include <fstream>
#include <iostream>
#include "vector"
#include "algorithm"
#include <bits/stdc++.h>
#include <filesystem>
#include "queue"
#include "DATA.h"
#include "Quicksort.h"
#include "AVL_ADT.h"
#include "Search.h"


using namespace std;

int ScanBook(AvlTree<DATA, string> *_tree);

int ScanChapter(const string &_file, AvlTree<DATA, string> *_tree);

int FindArticle(vector<ARTICLE> _v, const string &_path);

string FormatWord(string _w);

void PrintRow(DATA *_node);

void PrintTable(AvlTree<DATA, string> *_tree);

void SaveNode(DATA _node);

void ClearSaves();

int LoadNodes(const string &_path, AvlTree<DATA, string> *_tree);


int main()
{
    AvlTree<DATA, string> tree;
    if (!tree.AVL_Empty())
        cout << "ERROR: New tree wasn't empty." << endl;

//    int count = LoadNodes("..\\save.txt", &tree);
    int count = ScanBook(&tree);
    cout << "Gathered " << to_string(count) << " words." << endl;
    cout << "AVL Tree has " << tree.AVL_Count() << " words." << endl;

    char yesNo;
    cout << "Did you want to rebuild the dictionary? [Y/N]" << endl;
    cin >> yesNo;
    system("CLS");
    if (yesNo == 'y' || yesNo == 'Y')
    {
        ClearSaves();
        tree.AVL_TraverseBreadth(SaveNode);
    }

    cin.ignore();

    Search search;
    search.LoopSearch(&tree, FormatWord);

//    PrintTable(&tree);
//    cout << endl;
    printf("BYE!\n");
    return 0;
}

string FormatWord(string _w)
{
    std::transform(_w.begin(), _w.end(), _w.begin(), [](unsigned char c) { return std::tolower(c); }); //Remove any uppercase first letters

    for (int i = 0; i < _w.size(); i++)
    {
        // Finding the character whose
        // ASCII value fall under this
        // range
        if ((_w[i] < 'A' || _w[i] > 'Z' && _w[i] < 'a' || _w[i] > 'z') && (_w[i] != '\'') && (_w[i] != ' '))
        {
            // erase function to erase
            // the character
            _w.erase(i, 1);
            i--;
        }
    }
    return _w;
}

int ScanBook(AvlTree<DATA, string> *_tree)
{
    int wordCount = 0;
    for (int i = 1; i <= 6; i++)
    {
        WORD_COUNT = ScanChapter("..\\texts\\CH" + to_string(i) + ".txt", _tree);
        wordCount += WORD_COUNT;
    }
    return wordCount;
}

int ScanChapter(const string &_file, AvlTree<DATA, string> *_tree)
{
    ifstream file;
    string word;

    //Open File
    file.open(_file.c_str());
    if (file.fail())
    {
        cout << "ERROR: Failed to open the file " << _file << " for scanning." << endl;
    }

    int wordPos = 0;
    int totalWordCount = 0;
    DATA newData;

    while (file >> word)
    {
        word = FormatWord(word); //Remove any symbols attached to the word
        if (word.empty()) //Skip any words that consisted of only symbols
            continue;

        //Create the base data
        newData.data = vector<ARTICLE>();
        newData.key = word;

        bool exists = _tree->AVL_Retrieve(newData.key, newData); //Check to see if the word already exists in our dictionary

        int articleIndex = FindArticle(newData.data, _file); //Find the article that corresponds to this file/chapter (if it exits)
        if (newData.data.empty() || articleIndex == -1) //If there are no articles or our one doesn't exist
        {
            ARTICLE tempArt(_file); //Create the article with the appropriate data
            tempArt.pos.push_back(wordPos); //Mark the position
            newData.data.push_back(tempArt); //Add the data
        }
        else //Else, if it already exists
        {
            newData.data[articleIndex].pos.push_back(wordPos); //Mark the position
        }

        if (exists) //If the data already exists in our dictionary
        {
            if (!_tree->AVL_Update(newData.key, newData)) //Update it
                cout << "***ERROR: Failed to update the retrieved value!***" << endl; //Output an error log if the updating is unsuccessful
        }
        else //Else, if it doesn't exist yet
        {
            _tree->AVL_Insert(newData); //Insert it as a new dictionary item
        }

        wordPos++; //Increment the word position/count
    }
    return wordPos;
}

int FindArticle(vector<ARTICLE> _v, const string &_path)
{
    for (int i = 0; i < _v.size(); i++)
        if (_v[i].path == _path)
            return i;

    return -1;
}

void PrintTable(AvlTree<DATA, string> *_tree)
{
    double n = 1;
    int colWidth = 25;
    //table header
    cout << setfill('$') << setw(3 * colWidth) << "$" << endl;
    cout << setfill(' ') << fixed;
    cout << setw(colWidth) << "WORD" << setw(colWidth) << "FREQUENCY" << endl;
    cout << setfill('*') << setw(3 * colWidth) << "*" << endl;
    cout << setfill(' ') << fixed;

    //Data
    _tree->AVL_TraverseInOrder(PrintRow);

    cout << setfill('$') << setw(3 * colWidth) << "$" << endl;
}

void PrintRow(DATA *_node)
{
    int colWidth = 25;

    if (_node->data.empty())
        cout << "ERROR: No data to PrintRow." << endl;
    cout << setprecision(4) << setw(colWidth) << _node->key << setw(colWidth) << _node->GetFrequency(WORD_COUNT) << endl;

//    for (const ARTICLE &d : _node->data)
//    {
//        string ch = d.path.substr(9, 7);
//        cout << setprecision(0) << setw(colWidth) << _node->key << setprecision(4) << setw(colWidth) << ch << setw(colWidth) << d.occurences << endl;
//    }
    cout << setfill('-') << setw(3 * colWidth) << "-" << endl;
    cout << setfill(' ') << fixed;
}

void SaveNode(DATA _node)
{
    for (ARTICLE &d : _node.data)
    {
        string fName = d.path.substr(9, 7);
        ofstream saveFile("..\\save.txt", ios_base::app);
        saveFile << _node;
        saveFile.close();
    }
}

//Function which parses out a string based on the delimiter of choice. The results are stored back into a vector which is passed in by memory the address
void GetArticle(string str, string &path, vector<int> &positions, char token)
{
    //Skips the delimiters at the beginning of the string
    int lastPosition = str.find_first_not_of(token, 0);
    //Find the first non delimiter
    int position = str.find_first_of(token, lastPosition);

    //While loop which iterates through a string to subract tokens
    while (string::npos != position || string::npos != lastPosition)
    {
        if (path.empty())
        {
            path = str.substr(lastPosition, position - lastPosition);
        }
        else
        {
            //Adds found token to the vector
            positions.push_back(stoi(str.substr(lastPosition, position - lastPosition)));
        }
        //Finds the next delimiter
        lastPosition = str.find_first_not_of(token, position);
        //Finds the next non delimiter
        position = str.find_first_of(token, lastPosition);
    }
}

int LoadNodes(const string &_path, AvlTree<DATA, string> *_tree)
{
    ifstream loadFile(_path);

    if (loadFile.fail())
    {
        cout << "Failed to open " << _path << endl;
    }

    string word, articleData;
    vector<vector<int>> positionsList;
    vector<string> pathsList;
    DATA tempData;
    int wordCount = 0;

    string sLine;

    while (!loadFile.eof()) //Go through whole file
    {
        loadFile >> word;

        while (true) //Loop through the lines of chapters
        {
//            getline(loadFile, articleData);

            string path;
            loadFile >> path;

            if (path.find('#') != string::npos)
                break;

            vector<int> positions;
            GetArticle(articleData, path, positions, ' ');
            pathsList.push_back(path);
            positionsList.push_back(positions);
            wordCount++;
        }

        if (positionsList.empty())
            cout << "ERROR: The article positions were empty." << endl;
        else if (positionsList.size() != pathsList.size())
            cout << "ERROR: The number of paths and position lists is different." << endl;

        tempData.key = word;
        for (int i = 0; i < pathsList.size(); i++)
        {
            ARTICLE tempArticle(pathsList[i]);
            tempArticle.pos = positionsList[i];
            tempData.data.push_back(tempArticle);
        }


        bool exists = _tree->AVL_Retrieve(tempData.key, tempData); //Check to see if the word already exists in our dictionary

        if (exists) //If the data already exists in our dictionary
        {
            if (!_tree->AVL_Update(tempData.key, tempData)) //Update it
                cout << "***ERROR: Failed to update the retrieved value!***" << endl; //Output an error log if the updating is unsuccessful
        }
        else //Else, if it doesn't exist yet
        {
            _tree->AVL_Insert(tempData); //Insert it as a new dictionary item
        }

        wordCount += tempData.GetInstances();
    }

    return wordCount;
}

void ClearSaves()
{
    ofstream ofs;
    ofs.open("..\\save.txt", std::ofstream::out | std::ofstream::trunc);
    ofs.close();
}