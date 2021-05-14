//
// Created by ascle on 13/05/2021.
//

#ifndef DSA_INDEXING_ASSIGNMENT2_FILEOPERATIONS_H
#define DSA_INDEXING_ASSIGNMENT2_FILEOPERATIONS_H

int ScanBook(AvlTree<DATA, string> *_tree);

int ScanChapter(const string &_file, AvlTree<DATA, string> *_tree);

void SaveNode(DATA _node);

void ClearSaves();

int LoadNodes(const string &_path, AvlTree<DATA, string> *_tree);

string FormatWord(string _w);

string FormatFileName(string);

void GetArticle(string str, string &path, vector<int> &positions, char token);

int FindArticle(vector<ARTICLE> _v, const string &_path);

int ScanBook(AvlTree<DATA, string> *_tree)
{
    int wordCount = 0;
    for (int i = 1; i <= 17; i++)
    {
        wordCount += ScanChapter("..\\texts\\CH" + to_string(i) + ".txt", _tree);
    }
    WORD_COUNT = wordCount;
    return wordCount;
}

int ScanChapter(const string &_file, AvlTree<DATA, string> *_tree)
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
    string word1, word2, word3;

    while (file >> word1)
    {
        word1 = FormatWord(word1); //Remove any symbols attached to the word1
        if (word1.empty()) //Skip any words that consisted of only symbols
            continue;

        //Create the base data
        newData.data = vector<ARTICLE>();
        newData.key = word1;

        bool exists = _tree->AVL_Retrieve(newData.key, newData); //Check to see if the word1 already exists in our dictionary

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

        wordPos++; //Increment the word1 position/count
    }
    return wordPos;
}

void SaveNode(DATA _node)
{
    ofstream saveFile(SAVE_FILE, ios_base::app);
    saveFile << _node;
    saveFile.close();
}

int FindArticle(vector<ARTICLE> _v, const string &_path)
{
    for (int i = 0; i < _v.size(); i++)
        if (_v[i].path == _path)
            return i;

    return -1;
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
    ofs.open(SAVE_FILE, std::ofstream::out | std::ofstream::trunc);
    ofs.close();
}

string FormatWord(string _w)
{
    transform(_w.begin(), _w.end(), _w.begin(), [](unsigned char c) { return tolower(c); }); //Remove any uppercase first letters

    for (int i = 0; i < _w.size(); i++)
    {
        if ((_w[i] < 'a' || _w[i] > 'z') && (_w[i] != ' '))// && ((_w[i] != '-') && i <= 1 && i >= _w.size() - 3))
        {
            _w.erase(i, 1);
            i--;
        }
    }
    return _w;
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


#endif //DSA_INDEXING_ASSIGNMENT2_FILEOPERATIONS_H
