//
// Created by Broderick Westrope on 14/05/2021.
//

#ifndef DSA_INDEXING_ASSIGNMENT2_SAVING_LOADING_H
#define DSA_INDEXING_ASSIGNMENT2_SAVING_LOADING_H

class SavingLoading
{
    AvlTree<DATA, string> *tree;

    void GetArticle(string str, vector<int> &positions);

public:
    SavingLoading(AvlTree<DATA, string> *_tree) : tree(_tree)
    {};

    void ClearSaves();

    int LoadNodes(const string &_path);
};


void SavingLoading::ClearSaves()
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

void SavingLoading::GetArticle(string str, vector<int> &positions)
{
    char delim = ' ';
    //Skips the delimiters at the beginning of the string
    int lastPos = str.find_first_not_of(delim, 0);
    //Find the first non delimiter
    int pos = str.find_first_of(delim, lastPos);

    //While loop which iterates through a string to subract tokens
    while (string::npos != pos || string::npos != lastPos)
    {
        //Adds found delim to the vector
        positions.push_back(stoi(str.substr(lastPos, pos - lastPos)));
        //Finds the next delimiter
        lastPos = str.find_first_not_of(delim, pos);
        //Finds the next non delimiter
        pos = str.find_first_of(delim, lastPos);
    }
}

int SavingLoading::LoadNodes(const string &_path)
{
    ifstream loadFile(_path);

    if (loadFile.fail())
    {
        cout << "Failed to open the save file " << _path << ". Check that it exists, then try again." << endl;
        return 0;
    }

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

        if (tempWord == "" || tempWord == "\n")
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

        for (int i = 0; i <= 50; i++) //Loop through the lines of chapters (max of 50 iterations)
        {
            if (i >= 50)
            {
                cout << "ERROR: The word/phrase '" << word << "' was found to be in more than 50 different articles (this is the max)." << endl;
                break;
            }

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

        if (word == "ha")
            cout << word << " has " << pathsList.size() << endl;

        if (positionsList.empty())
            cout << "ERROR: The article positions were empty." << endl;
        else if (positionsList.size() != pathsList.size())
            cout << "ERROR: The number of paths and position lists is different." << endl;

        tempData.key = word;
        tempData.wordCount = wordCount;
        for (int i = 0; i < positionsList.size(); i++)
        {
            if (positionsList[i].empty() || positionsList[i].empty())
                continue;
            ARTICLE tempArticle(pathsList[i]);
            tempArticle.pos = positionsList[i];
            tempData.data.push_back(tempArticle);
        }

        bool exists = tree->AVL_Retrieve(tempData.key, tempData); //Check to see if the word already exists in our dictionary

        if (exists) //If the data already exists in our dictionary
        {
            if (!tree->AVL_Update(tempData.key, tempData)) //Update it
                cout << "***ERROR: Failed to update the retrieved value!***" << endl; //Output an error log if the updating is unsuccessful
        }
        else //Else, if it doesn't exist yet
        {
            tree->AVL_Insert(tempData); //Insert it as a new dictionary item
        }

        word = "", tempWord = "", articleData = "";
        pathsList.clear();
        positionsList.clear();
    }

    if (wordPos == 0)
        cout << "No words were loaded from the save file " << _path << ". Make sure it has contents." << endl;

    return wordPos;
}


#endif //DSA_INDEXING_ASSIGNMENT2_SAVING_LOADING_H
