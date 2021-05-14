//
// Created by Broderick Westrope on 13/05/2021.
//

#ifndef DSA_INDEXING_ASSIGNMENT2_FILEOPERATIONS_H
#define DSA_INDEXING_ASSIGNMENT2_FILEOPERATIONS_H

int FindArticle(vector<ARTICLE> _v, const string &_path)
{
    for (int i = 0; i < _v.size(); i++)
        if (_v[i].path == _path)
            return i;

    return -1;
}

string FormatKey(string _k)
{
    transform(_k.begin(), _k.end(), _k.begin(), [](unsigned char c) { return tolower(c); }); //Remove any uppercase first letters

    for (int i = 0; i < _k.size(); i++)
    {
        if ((_k[i] < 'a' || _k[i] > 'z') && (_k[i] != ' '))// && ((_k[i] != '-') && i <= 1 && i >= _k.size() - 3))
        {
            _k.erase(i, 1);
            i--;
        }
    }
    return _k;
}

string FormatFileName(string _fn)
{
    for (int i = 0; i < _fn.size(); i++)
    {
        if (_fn[i] == '/' || _fn[i] == '\\' || _fn[i] == '?' || _fn[i] == '%' || _fn[i] == '*' || _fn[i] == ':' || _fn[i] == '|' || _fn[i] == '"' || _fn[i] == '<' || _fn[i] == '>' || _fn[i] == '.' ||
            _fn[i] == ',' || _fn[i] == ';' || _fn[i] == '=' || _fn[i] == ' ')
        {
            _fn.erase(i, 1);
            i--;
        }
    }

    return _fn;
}

void AddWord(AvlTree<DATA, string> *_tree, DATA _newData, int _wordPos, string _file)
{
    bool exists = _tree->AVL_Retrieve(_newData.key, _newData); //Check to see if the word already exists in our dictionary

    int articleIndex = FindArticle(_newData.data, _file); //Find the article that corresponds to this file/chapter (if it exits)
    if (_newData.data.empty() || articleIndex == -1) //If there are no articles or our one doesn't exist
    {
        ARTICLE tempArt(_file); //Create the article with the appropriate data
        tempArt.pos.push_back(_wordPos); //Mark the position
        _newData.data.push_back(tempArt); //Add the data
    }
    else //Else, if it already exists
    {
        _newData.data[articleIndex].pos.push_back(_wordPos); //Mark the position
    }

    if (exists) //If the data already exists in our dictionary
    {
        if (!_tree->AVL_Update(_newData.key, _newData)) //Update it
            cout << "***ERROR: Failed to update the retrieved value!***" << endl; //Output an error log if the updating is unsuccessful
    }
    else //Else, if it doesn't exist yet
    {
        _tree->AVL_Insert(_newData); //Insert it as a new dictionary item
    }
}

int ScanArticle(const string &_file, AvlTree<DATA, string> *_tree)
{
    ifstream file;

    //Open File
    file.open(_file.c_str());
    if (file.fail())
    {
        cout << "ERROR: Failed to open the file " << _file << " for scanning." << endl;
    }

    int wordPos = 1;
    DATA newData;
    string word1, word2, word3; //Word1 = current word, word2 = 1 word back, word3 = 2 words back

    while (file >> word1)
    {
        word1 = FormatKey(word1); //Remove any symbols attached to the word1
        if (word1.empty()) //Skip any words that consisted of only symbols
            continue;

        //Create the base newData
        newData.data = vector<ARTICLE>();

        newData.key = word1;
        newData.wordCount = 1;
        AddWord(_tree, newData, wordPos, _file);

        if (INCL_PHRASES)
        {
            if (!word2.empty())
            {
                newData.key = word2 + " " + word1;
                newData.wordCount = 2;
                AddWord(_tree, newData, wordPos, _file);

                if (!word3.empty())
                {
                    newData.key = word3 + " " + word2 + " " + word1;
                    newData.wordCount = 3;
                    AddWord(_tree, newData, wordPos, _file);
                }
            }

            word3 = word2;
            word2 = word1;
        }
        wordPos++; //Increment the word position/count
    }
    WORD_COUNT += wordPos;
    return wordPos;
}

int ScanHarryPotter(AvlTree<DATA, string> *_tree)
{
    int wordCount = 0;
    for (int i = 1; i <= 17; i++)
    {
        wordCount += ScanArticle("..\\texts\\CH" + to_string(i) + ".txt", _tree);
    }
    return wordCount;
}

#endif //DSA_INDEXING_ASSIGNMENT2_FILEOPERATIONS_H
