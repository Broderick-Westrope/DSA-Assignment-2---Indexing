//
// Created by Broderick Westrope on 7/05/2021.
//

#ifndef DSA_INDEXING_ASSIGNMENT2_SEARCH_H
#define DSA_INDEXING_ASSIGNMENT2_SEARCH_H

class Search
{
    AvlTree<DATA, string> *tree;
    Printing printing;

    //Strict/Starts-with Search
    void StrictSearch(string, string);

    //KMP Pattern Matching (Contains)
    void ContainsSearch(DATA);

    static vector<int> KMP_Matching(string _input, string _candidate);

    static void ComputeLPS_Array(const string &_candidate, int C, vector<int> &_lps);

    static int Compare_KMP(DATA _candidate, DATA _input);

    //Levenshtein Distance
    void LevSearch(DATA);

    static int Compare_LEV(DATA _candidate, DATA _input);

    static int GetLevDist(string, string);


public:
    Search(AvlTree<DATA, string> *_tree, Printing printing) : tree(_tree), printing(printing)
    {};

    //Search Engine
    void LoopSearch(string (*)(string), string);

    //Print Dictionary 'Chapter'/Section
    void SearchWordsStartingWith();
};

void Search::LoopSearch(string (*FormatWord)(string), string _searchType)
{
    string sortType;
    if (_searchType == "Strict")
    {
        system("CLS");
        cout << "Did you want to sort the results in order of frequency? [Y/N] (if not, the results will be in alphabetical order)\n" << endl;
        cin >> sortType;
        sortType = (sortType == "y" || sortType == "Y") ? "Freq" : "Alpha";
        cin.ignore();
    }

    while (true)
    {
        string query;
        cout << "SEARCH THE HARRY POTTER CHAPTERS:       (Type '~' to exit searching)" << endl;
        getline(cin, query);
        system("CLS");

        if (query == "~")
            return;

        query = FormatWord(query);
        if (query.empty())
        {
            cout << "No valid input was entered, be sure to take a look at some of our great articles and only use letters in your search!" << endl;
            continue;
        }
        else
        {
            DATA qData;
            qData.key = FormatWord(query);
            if (_searchType == "Strict")
            {
                StrictSearch(query, sortType);
            }
            else if (_searchType == "Contains")
            {
                ContainsSearch(qData);
            }
            else
            {
                if (_searchType != "Lev")
                    cout << "ERROR: No valid search type was provided." << endl;
                LevSearch(qData);
            }
        }

        cin.ignore();
        cout << endl << "Press Enter To Continue..." << endl;
        getchar();
        system("CLS");
    }
}

void Search::SearchWordsStartingWith()
{
    while (true)
    {
        char query;
        cout << "WHAT LETTER OF THE DICTIONARY WOULD YOU LIKE TO PRINT?       (Type '~' to exit searching)" << endl;
        cin >> query;
        system("CLS");

        if (query == '~')
        {
            cout << "Leaving..." << endl;
            return;
        }
        if (!isalpha(query))
        {
            cout << "No valid input was entered, be sure to only enter a single letter!" << endl;
            continue;
        }
        else
        {
            StrictSearch(string(1, query), "Alpha");
        }

        cin.ignore();
        cout << endl << "Press Enter To Continue..." << endl;
        getchar();
        system("CLS");
    }
}

//region - Strict Search
// (results have to start with the search query)
void Search::StrictSearch(string _query, string _sort)
{
    string upperBound = _query.substr(0, _query.size() - 1);
    char lastChar = static_cast<char>(_query[_query.size() - 1] + 1);
    upperBound += lastChar;

    if (_sort == "Freq")
    {
        priority_queue<DATA> results;
        tree->AVL_GetStrictResults(_query, upperBound, results);

        if (results.empty())
        {
            cout << "There were no results for your search of '" << _query << "'. Make sure you have scanned a dictionary or article and try again." << endl;
        }
        else
        {
            cout << "Top " << results.size() << " results for your search of " << _query << "." << endl;
            printing.Search_PrintTable(results);

            string moreInfo;
            cout << "Enter a result (whole number) you would like to see more of, or enter '~' to do another search." << endl;
            cin >> moreInfo;

            bool isNum = true;
            for (char c:moreInfo)
            {
                if (!isdigit(c))
                {
                    isNum = false;
                    break;
                }
            }

            if (isNum)
            {
                int digit = stoi(moreInfo);
                int i = 1;
                while (i != digit)
                {
                    results.pop();
                    i++;
                }
                printing.Search_PrintNode(results.top());
            }
            else if (moreInfo != "~")
                cout << "Invalid input so I'll do another search for you" << endl;
        }
    }
    else
    {
        vector<DATA> results;
        tree->AVL_GetStrictResults(_query, upperBound, results);

        if (results.empty())
        {
            cout << "There were no results for your search of " << _query << ". Make sure you have scanned a dictionary or article and try again." << endl;
        }
        else
        {
            cout << "Top " << results.size() << " results for your search of " << _query;
            if (results.front().key == _query)
            {
                cout << ", including an exact match!";
            }
            cout << endl;
            printing.Search_PrintTable(results);

            char moreInfo;
            cout << "Enter a result (whole number) you would like to see more of, or enter '~' to do another search?" << endl;
            cin >> moreInfo;

            if (isdigit(moreInfo))
                printing.Search_PrintNode(results[atoi(&moreInfo) - 1]);
            else if (moreInfo != '~')
                cout << "Invalid input so I'll do another search for you" << endl;
        }
    }
}
//endregion

//region - KMP Search
// (results have to contain the search query)
void Search::ContainsSearch(DATA _qData)
{
    vector<pair<DATA, int>> results = tree->AVL_GetClosestNodes(_qData, Compare_KMP);
    QuickSort(results, 0, results.size() - 1);

    if (results.empty())
    { cout << "There were no results for your search of " << _qData.key << ". Make sure you have scanned a dictionary or article and try again." << endl; }
    else
    {
        cout << "Top " << results.size() << " results for your search of " << _qData.key;
        if (results.front().first.key == _qData.key)
        {
            cout << ", including an exact match!";
        }
        cout << endl;
        printing.Search_PrintTable(results);
    }
}

vector<int> Search::KMP_Matching(string _input, string _candidate)
{
    vector<int> indices;

    int I = (int) _input.size();
    int C = (int) _candidate.size();

    //Stores our LPS (longest prefix-suffix) values for the pattern (the input)
    vector<int> lps(I);

    // Preprocess the pattern (calculate the LPS values)
    ComputeLPS_Array(_input, I, lps);

    int i = 0;
    int j = 0;
    while (i < C)
    {
        if (_input[j] == _candidate[i])
        {
            j++;
            i++;
        }

        if (j == I)
        {
            indices.push_back(i - j);
            j = lps[j - 1];
        }
        else if (i < C && _input[j] != _candidate[i]) //Mismatch after 'J' number of matches
        {
            //LPS[0..lps[j-1]] characters already match so don't attempt those
            if (j != 0)
                j = lps[j - 1];
            else
                i = i + 1;
        }
    }
    return indices;
}

void Search::ComputeLPS_Array(const string &_candidate, int C, vector<int> &_lps)
{
    //Stores length of the previous LPS
    int length = 0;

    _lps[0] = 0; //Index 0 is always of value 0

    //Calculate LPS[i] for indices i = 1 to i = (M-1)
    int i = 1;
    while (i < C)
    {
        if (_candidate[i] == _candidate[length])
        {
            length++;
            _lps[i] = length;
            i++;
        }
        else //When the value at candidate[i] no longer matches that at candidate[length]
        {
            if (length != 0)
            { length = _lps[length - 1]; }
            else
            {
                _lps[i] = 0;
                i++;
            }
        }
    }
}

int Search::Compare_KMP(DATA _candidate, DATA _input)
{
    //If we don't want to include this node in the search (if it's a phrase and we are only looking for words)
    if (!INCL_PHRASES && _candidate.wordCount > 1)
    { return 0; }

    if (_candidate.key == _input.key) //If this is an exact match
    { return 100; }

    vector<int> indices = KMP_Matching(_input.key, _candidate.key);
    int score = (int) indices.size();

    if (!indices.empty())
    {
        if (indices[0] == 0) //If the candidate's key starts with the input's key it's a bonus (this would also be a result in strict search)
        { score = 10 + ((int) indices.size() - 1); }
    }

    return score;
}
//endregion

//region - Levenshtein Search
// (results include all nodes passed in an attempt to find the search query. These are ordered using the algorithm for Levenshtein Distance)
void Search::LevSearch(DATA _qData)
{
    vector<pair<DATA, int>> results = tree->AVL_GetClosestNodes(_qData, Compare_LEV);
    QuickSort(results, 0, results.size() - 1);

    if (results.empty())
    { cout << "There were no results for your search of " << _qData.key << ". Make sure you have scanned a dictionary or article and try again." << endl; }
    else
    {
        cout << "Top " << results.size() << " results for your search of " << _qData.key;
        if (results.front().first.key == _qData.key)
        { cout << ", including an exact match!"; }
        cout << endl;
        printing.Search_PrintTable(results);
    }
}

int Search::Compare_LEV(DATA _candidate, DATA _input)
{
    if (!INCL_PHRASES && _candidate.wordCount > 1)
    { return 0; }

    if (_candidate.key == _input.key)
    {
        return 100;
    }
    int score = (int) max(_candidate.key.size(), _input.key.size()) + 10;
    score -= GetLevDist(_candidate.key, _input.key);
    return score;
}

int Search::GetLevDist(string _input, string _candidate)
{
    int i, j, l1 = (int) _input.size(), l2 = (int) _candidate.size(), t, track;
    int dist[_input.size() + _candidate.size()][_input.size() + _candidate.size()];

    for (i = 0; i <= l1; i++)
    {
        dist[0][i] = i;
    }
    for (j = 0; j <= l2; j++)
    {
        dist[j][0] = j;
    }
    for (j = 1; j <= l1; j++)
    {
        for (i = 1; i <= l2; i++)
        {
            if (_input[i - 1] == _candidate[j - 1])
            {
                track = 0;
            }
            else
            {
                track = 1;
            }
            t = min((dist[i - 1][j] + 1), (dist[i][j - 1] + 1));
            dist[i][j] = min(t, (dist[i - 1][j - 1] + track));
        }
    }
    return dist[l2][l1];
}
//endregion


#endif //DSA_INDEXING_ASSIGNMENT2_SEARCH_H
