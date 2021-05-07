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
#include "AVL_ADT.h"
#include "Search.h"

using namespace std;

int GetBook(AvlTree<DATA, string> *_tree);

int GetChapters(string _file, AvlTree<DATA, string> *_tree);

int FindArticle(vector<ARTICLE> _v, string _path);

string RemoveSymbols(string _w);

void PrintRow(DATA _node);

void PrintTable(AvlTree<DATA, string> *_tree);

void SaveNode(DATA _node);


int main()
{
    AvlTree <DATA, string> tree;
    if (tree.AVL_Empty())
        cout << "Empty tree." << endl;

    int count = GetBook(&tree);
    cout << "Gathered " << to_string(count) << " words." << endl;

    char yesNo;
    cout << "Did you want to rescan the articles? [Y/N]" << endl;
    cin >> yesNo;
    if (yesNo == 'y' || yesNo == 'Y')
        tree.AVL_Traverse(SaveNode);


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

int GetBook(AvlTree<DATA, string> *_tree)
{
    int count = 0;
    count += GetChapters("..\\texts\\CH1.txt", _tree);
    count += GetChapters("..\\texts\\CH2.txt", _tree);
    count += GetChapters("..\\texts\\CH4.txt", _tree);
    count += GetChapters("..\\texts\\CH5.txt", _tree);
    count += GetChapters("..\\texts\\CH6.txt", _tree);
    count += GetChapters("..\\texts\\CH3.txt", _tree);
    return count;
}

int GetChapters(string _file, AvlTree<DATA, string> *_tree)
{
    fstream file;
    string word;

    // opening file
    file.open(_file.c_str());

    int count = 0;
    DATA newData;

    while (file >> word)
    {
        std::transform(word.begin(), word.end(), word.begin(),
                       [](unsigned char c) { return std::tolower(c); });

        word = RemoveSymbols(word);
        newData.data = vector<ARTICLE>();
        newData.key = word;
        bool exists = _tree->AVL_Retrieve(newData.key, newData);

        int artIndex = FindArticle(newData.data, _file);
        if (newData.data.empty() || artIndex == -1)
        {
            ARTICLE tempArt(_file, 1);
            tempArt.pos.push_back(count);
            newData.data.push_back(tempArt);
        }
        else
        {
            newData.data[artIndex].freq++;
            newData.data[artIndex].pos.push_back(count);
        }

        if (exists)
        {
            if (!_tree->AVL_Update(newData.key, newData))
                cout << "***ERROR: Failed to update the retrieved value!***" << endl;
        }
        else
        {
            _tree->AVL_Insert(newData);
        }
        count++;
    }
    return count;
}

int FindArticle(vector<ARTICLE> _v, string _path)
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
    _tree->AVL_Traverse(PrintRow);

    cout << setfill('$') << setw(3 * colWidth) << "$" << endl;
}

void PrintRow(DATA _node)
{
    int colWidth = 25;

    if (_node.data.empty())
        cout << "ERROR: No data to PrintRow." << endl;
    for (const ARTICLE &d : _node.data)
    {
        string ch = d.path.substr(9, 7);
        cout << setprecision(0) << setw(colWidth) << _node.key << setprecision(4) << setw(colWidth) << ch << setw(colWidth) << d.freq << endl;
    }
    cout << setfill('-') << setw(3 * colWidth) << "-" << endl;
    cout << setfill(' ') << fixed;
}

void ClearSaves()
{
    ofstream ofs;
    ofs.open("..\\texts\\CH1.txt", std::ofstream::out | std::ofstream::trunc);
    ofs.close();
    ofs.open("..\\texts\\CH2.txt", std::ofstream::out | std::ofstream::trunc);
    ofs.close();
    ofs.open("..\\texts\\CH3.txt", std::ofstream::out | std::ofstream::trunc);
    ofs.close();
    ofs.open("..\\texts\\CH4.txt", std::ofstream::out | std::ofstream::trunc);
    ofs.close();
    ofs.open("..\\texts\\CH5.txt", std::ofstream::out | std::ofstream::trunc);
    ofs.close();
    ofs.open("..\\texts\\CH6.txt", std::ofstream::out | std::ofstream::trunc);
    ofs.close();
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
