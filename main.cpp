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

int WORD_COUNT;

using namespace std;

int ScanBook(AvlTree<DATA, string> *_tree);

int ScanChapter(const string &_file, AvlTree<DATA, string> *_tree);

int FindArticle(vector<ARTICLE> _v, const string &_path);

string RemoveSymbols(string _w);

void PrintRow(DATA *_node);

void PrintTable(AvlTree<DATA, string> *_tree);

void SaveNode(DATA _node);

void ClearSaves();

void LoadNodes(const string &_path);


int main()
{
    AvlTree<DATA, string> tree;
    if (!tree.AVL_Empty())
        cout << "ERROR: New tree wasn't empty." << endl;

    int count = ScanBook(&tree);
    cout << "Gathered " << to_string(count) << " words." << endl;

    char yesNo;
    cout << "Did you want to rebuild the dictionary? [Y/N]" << endl;
    cin >> yesNo;
    if (yesNo == 'y' || yesNo == 'Y')
    {
        ClearSaves();
        tree.AVL_TraverseBreadth(SaveNode);
    }

    LoadNodes("..\\saves\\SAVE_CH1.txt");

    Search search;
    search.LoopSearch(&tree, RemoveSymbols);

//    PrintTable(&tree);
//    cout << endl;
    printf("BYE!\n");
    return 0;
}

string RemoveSymbols(string _w)
{
    for (int i = 0; i < _w.size(); i++)
    {
        // Finding the character whose
        // ASCII value fall under this
        // range
        if ((_w[i] < 'A' || _w[i] > 'Z' && _w[i] < 'a' || _w[i] > 'z') && (_w[i] != '\''))
        {
            // erase function to erase
            // the character
            _w.erase(i, 1);
            i--;
        }
    }
    return _w;
}

void OccurenceToFrequency(DATA *_node)
{
    for (ARTICLE a : _node->data)
    {
//        cout << (a.freq / static_cast<double>(WORD_COUNT)) * 1000.00 << endl;
//        cout << a.freq << endl;
        a.freq = (a.freq / static_cast<double>(WORD_COUNT)) * 1000.00;
//        cout << a.freq << endl;
    }

//    cout << endl;
}

int ScanBook(AvlTree<DATA, string> *_tree)
{
    int wordCount = 0;
    for (int i = 1; i <= 6; i++)
    {
        WORD_COUNT = ScanChapter("..\\texts\\CH" + to_string(i) + ".txt", _tree);
        _tree->AVL_TraverseInOrder(OccurenceToFrequency);
        wordCount += WORD_COUNT;
    }
//    int ch1 = ScanChapter("..\\texts\\CH1.txt", _tree);
//    int ch2 = ScanChapter("..\\texts\\CH2.txt", _tree);
//    int ch3 = ScanChapter("..\\texts\\CH3.txt", _tree);
//    int ch4 = ScanChapter("..\\texts\\CH4.txt", _tree);
//    int ch5 = ScanChapter("..\\texts\\CH5.txt", _tree);
//    int ch6 = ScanChapter("..\\texts\\CH6.txt", _tree);
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

    int wordIndex = 0;
    int totalWordCount = 0;
    DATA newData;

    while (file >> word)
    {
        totalWordCount++;
    }
    cout << totalWordCount << " words in total." << endl;
    file.close();
    file.open(_file.c_str());

    while (file >> word)
    {
        std::transform(word.begin(), word.end(), word.begin(), [](unsigned char c) { return std::tolower(c); }); //Remove any uppercase first letters
        word = RemoveSymbols(word); //Remove any symbols attached to the word
        if (word.empty()) //Skip any words that consisted of only symbols
            continue;

        //Create the base data
        newData.data = vector<ARTICLE>();
        newData.key = word;
        bool exists = _tree->AVL_Retrieve(newData.key, newData); //Check to see if the word already exists in our dictionary

        int articleIndex = FindArticle(newData.data, _file); //Find the article that corresponds to this file/chapter (if it exits)
        if (newData.data.empty() || articleIndex == -1) //If there are no articles or our one doesn't exist
        {
            ARTICLE tempArt(_file, 1); //Create the article with the appropriate data
            tempArt.pos.push_back(wordIndex); //Mark the position
            newData.data.push_back(tempArt); //Add the data
        }
        else //Else, if it already exists
        {
            newData.data[articleIndex].freq++; //Increment the word's frequency
            newData.data[articleIndex].pos.push_back(wordIndex); //Mark the position
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
        wordIndex++; //Increment the word count
    }
    return wordIndex;
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
    cout << setw(colWidth) << "WORD" << setw(colWidth) << "PATH/FILE" << setw(colWidth) << "FREQUENCY" << endl;
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
    for (const ARTICLE &d : _node->data)
    {
        string ch = d.path.substr(9, 7);
        cout << setprecision(0) << setw(colWidth) << _node->key << setprecision(4) << setw(colWidth) << ch << setw(colWidth) << d.freq << endl;
    }
    cout << setfill('-') << setw(3 * colWidth) << "-" << endl;
    cout << setfill(' ') << fixed;
}

void SaveNode(DATA _node)
{
    for (ARTICLE &d : _node.data)
    {
        string fName = d.path.substr(9, 7);
        ofstream saveFile("..\\saves\\SAVE_" + fName, std::ios_base::app);
        saveFile << _node;
        saveFile.close();
    }
}

void LoadNodes(const string &_path)
{
    ifstream loadFile(_path);
    string word;

    string sLine;
    int line_no = 0;
    while (getline(loadFile, sLine))
    {
//        sLine >> word;


        line_no++;
    }
    cout << line_no << " lines!" << endl;

    while (!loadFile.eof())
    {
        loadFile >> word;
        DATA newData;
        loadFile >> newData;

        for (char c : word)
        {
            if (c == '\n')
            {   // detects the end of the line
                cout << "This is end of the line" << endl;
            }
        }
    }
}

void ClearSaves()
{
    ofstream ofs;
    ofs.open("..\\saves\\SAVE_CH1.txt", std::ofstream::out | std::ofstream::trunc);
    ofs.close();
    ofs.open("..\\saves\\SAVE_CH2.txt", std::ofstream::out | std::ofstream::trunc);
    ofs.close();
    ofs.open("..\\saves\\SAVE_CH3.txt", std::ofstream::out | std::ofstream::trunc);
    ofs.close();
    ofs.open("..\\saves\\SAVE_CH4.txt", std::ofstream::out | std::ofstream::trunc);
    ofs.close();
    ofs.open("..\\saves\\SAVE_CH5.txt", std::ofstream::out | std::ofstream::trunc);
    ofs.close();
    ofs.open("..\\saves\\SAVE_CH6.txt", std::ofstream::out | std::ofstream::trunc);
    ofs.close();
}