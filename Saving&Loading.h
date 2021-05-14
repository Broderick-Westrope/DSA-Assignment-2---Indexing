//
// Created by Broderick Westrope on 14/05/2021.
//

#ifndef DSA_INDEXING_ASSIGNMENT2_SAVING_LOADING_H
#define DSA_INDEXING_ASSIGNMENT2_SAVING_LOADING_H

void ClearSaves();

void SaveNode(DATA _node);

int LoadNodes(const string &_path, AvlTree<DATA, string> *_tree);

void ClearSaves()
{
    ofstream ofs;
    ofs.open(SAVE_FILE, std::ofstream::out | std::ofstream::trunc);
    ofs.close();
}

void SaveNode(DATA _node)
{
    if (INCL_PHRASES || _node.wordCount == 1)
    {
        ofstream saveFile(SAVE_FILE, ios_base::app);
        saveFile << _node;
        saveFile.close();
    }
}

void GetArticle(string str, vector<int> &positions)
{
    char token = ' ';
    //Skips the delimiters at the beginning of the string
    int lastPosition = str.find_first_not_of(token, 0);
    //Find the first non delimiter
    int position = str.find_first_of(token, lastPosition);

    //While loop which iterates through a string to subract tokens
    while (string::npos != position || string::npos != lastPosition)
    {
        //Adds found token to the vector
        positions.push_back(stoi(str.substr(lastPosition, position - lastPosition)));
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
        cout << "Failed to open the save file " << _path << ". Check that it exists, then try again." << endl;

    int wordCount;
    string word, tempWord, articleData;
    vector<string> pathsList;
    vector<vector<int>> positionsList;
    DATA tempData;
    int wordPos = 0;

    string sLine;

    while (!loadFile.eof()) //Go through whole file
    {
        loadFile >> tempWord;

        if (tempWord == "") //todo figure out how to change the eof to remove this statement and the use of tempWord
        {
            break;
        }

        wordCount = stoi(tempWord);

        for (int i = 0; i < wordCount; i++)
        {
            loadFile >> tempWord;
            word += tempWord;
            if (i != wordCount - 1)
                word += ' ';
        }

        while (true) //Loop through the lines of chapters
        {
            getline(loadFile, articleData);

            if (articleData == "")
                continue;
            else if (articleData.rfind('#', 3) != string::npos)
                break;


            string path;
            for (int i = 0; i < articleData.size(); i++)
            {
                if (articleData[i] == ' ')
                {
                    articleData.erase(0, i + 1);
                    break;
                }
                path += articleData[i];
            }

            pathsList.push_back(path);

            vector<int> positions;
            GetArticle(articleData, positions);
            positionsList.push_back(positions);
            wordPos += positions.size();
        }

        if (positionsList.empty())
            cout << "ERROR: The article positions were empty." << endl;
        else if (positionsList.size() != pathsList.size())
            cout << "ERROR: The number of paths and position lists is different." << endl;

        tempData.key = word;
        tempData.wordCount = wordCount;
        for (int i = 0; i < pathsList.size(); i++)
        {
//            if (positionsList[i].empty())
//                continue;
            ARTICLE tempArticle(pathsList[i]);
//            cout << pathsList[i] << " : " << positionsList[i].size() << " : " << positionsList[i][0] << endl;
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

//        wordPos += tempData.GetInstances();

        word = "", tempWord = "", articleData = "";
        pathsList.clear();
        positionsList.clear();
    }

    if (wordPos == 0)
        cout << "No words were loaded from the save file " << _path << ". Make sure it has contents." << endl;

    return wordPos;
}


#endif //DSA_INDEXING_ASSIGNMENT2_SAVING_LOADING_H
