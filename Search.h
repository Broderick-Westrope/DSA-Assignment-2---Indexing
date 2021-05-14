//
// Created by ascle on 7/05/2021.
//

#ifndef DSA_INDEXING_ASSIGNMENT2_SEARCH_H
#define DSA_INDEXING_ASSIGNMENT2_SEARCH_H

class Search
{
    void PrintRow(DATA, int);


    static int GetLevDist(string, string);

public:
    void LoopSearch(AvlTree<DATA, string> *, string (*)(string), string);

    static int Compare_Lev(DATA, DATA);

    void PrintTable(vector<DATA>);

    void StrictSearch(string _query, AvlTree<DATA, string> *_tree);

    static void ComputeLPSArray(const string &_candidate, int C, vector<int> &_lps);

    static int Compare_Contains(DATA _candidate, DATA _input);

    static vector<int> KMPSearch(string _input, string _candidate);

    void PrintTable(vector<pair<DATA, int>> data);

    void ContainsSearch(DATA _qData, AvlTree<DATA, string> *_tree);

    void LevSearch(DATA _qData, AvlTree<DATA, string> *_tree);

    void DictionaryStartingWith(AvlTree<DATA, string> *_tree);
};


void Search::LoopSearch(AvlTree<DATA, string> *_tree, string (*FormatWord)(string), string _searchType)
{
    while (true)
    {
        string query;
        cout << "SEARCH THE HARRY POTTER CHAPTERS:       (Type '~E' to exit searching)" << endl;
        getline(cin, query);
        system("CLS");

        if (query == "~E")
        {
            cout << "Leaving..." << endl;
            return;
        }
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
                StrictSearch(query, _tree);
            }
            else if (_searchType == "Contains")
            {
                ContainsSearch(qData, _tree);
            }
            else
            {
                if (_searchType != "Lev")
                    cout << "ERROR: No valid search type was provided." << endl;
                LevSearch(qData, _tree);
            }
        }

        cin.ignore();
        cout << endl << "Press Enter To Continue..." << endl;
        getchar();
        system("CLS");
    }
}

void Search::DictionaryStartingWith(AvlTree<DATA, string> *_tree)
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
            StrictSearch(string(1, query), _tree);
        }

        cin.ignore();
        cout << endl << "Press Enter To Continue..." << endl;
        getchar();
        system("CLS");
    }
}

void Search::StrictSearch(string _query, AvlTree<DATA, string> *_tree) //strict
{
    string upperBound;
    char lastChar = static_cast<char>(_query[_query.size() - 1] + 1);
    if (_query.size() > 1)
    {
        for (int i = 0; i < _query.size() - 1; i++)
        {
            upperBound += _query[i];
        }
    }
    upperBound += lastChar;
    vector<DATA> results = _tree->AVL_GetStrictResults(_query, upperBound);

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
        PrintTable(results);
    }
}

void Search::ContainsSearch(DATA _qData, AvlTree<DATA, string> *_tree) //contains
{
    vector<pair<DATA, int>> results = _tree->AVL_GetClosestNodes(_qData, Compare_Contains);
    quickSort(results, 0, results.size() - 1);

    if (results.empty())
    {
        cout << "There were no results for your search of " << _qData.key << ". Make sure you have scanned a dictionary or article and try again." << endl;
    }
    else
    {
        cout << "Top " << results.size() << " results for your search of " << _qData.key;
        if (results.front().first.key == _qData.key)
        {
            cout << ", including an exact match!";
        }
        cout << endl;
        PrintTable(results);
    }
}

void Search::LevSearch(DATA _qData, AvlTree<DATA, string> *_tree) //levenshtein
{
    vector<pair<DATA, int>> results = _tree->AVL_GetClosestNodes(_qData, Compare_Lev);
    quickSort(results, 0, results.size() - 1);

    if (results.empty())
    {
        cout << "There were no results for your search of " << _qData.key << ". Make sure you have scanned a dictionary or article and try again." << endl;
    }
    else
    {
        cout << "Top " << results.size() << " results for your search of " << _qData.key;
        if (results.front().first.key == _qData.key)
        {
            cout << ", including an exact match!";
        }
        cout << endl;
        PrintTable(results);
    }
}

int Search::GetLevDist(string input, string candidate) //Gets the Levenshtein Distance between two strings to find the closest result
{
    int i, j, l1 = (int) input.size(), l2 = (int) candidate.size(), t, track;
    int dist[input.size() + candidate.size()][input.size() + candidate.size()];

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
            if (input[i - 1] == candidate[j - 1])
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

// Prints occurrences of _candidate[] in _input[]
vector<int> Search::KMPSearch(string _input, string _candidate)
{
    vector<int> indices;

    int I = (int) _input.size();
    int C = (int) _candidate.size();

    // create lps[] that will hold the longest prefix suffix
    // values for pattern
    vector<int> lps(I);

    // Preprocess the pattern (calculate lps[] array)
    ComputeLPSArray(_input, I, lps);

    int i = 0; // index for _candidate[]
    int j = 0; // index for _input[]
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
//            printf("Found pattern at index %d ", i - j);
            j = lps[j - 1];
        }
        else if (i < C && _input[j] != _candidate[i]) // mismatch after j matches
        {
            // Do not match lps[0..lps[j-1]] characters,
            // they will match anyway
            if (j != 0)
                j = lps[j - 1];
            else
                i = i + 1;
        }
    }

    return indices;
}

// Fills lps[] for given patttern pat[0..M-1]
void Search::ComputeLPSArray(const string &_candidate, int C, vector<int> &_lps)
{
    // length of the previous longest prefix suffix
    int len = 0;

    _lps[0] = 0; // lps[0] is always 0

    // the loop calculates lps[i] for i = 1 to M-1
    int i = 1;
    while (i < C)
    {
        if (_candidate[i] == _candidate[len])
        {
            len++;
            _lps[i] = len;
            i++;
        }
        else // (pat[i] != pat[len])
        {
            // This is tricky. Consider the example.
            // AAACAAAA and i = 7. The idea is similar
            // to search step.
            if (len != 0)
            {
                len = _lps[len - 1];

                // Also, note that we do not increment
                // i here
            }
            else // if (len == 0)
            {
                _lps[i] = 0;
                i++;
            }
        }
    }
}

int Search::Compare_Contains(DATA _candidate, DATA _input)
{
    int score;

    if (_candidate.key == _input.key)
    {
        return 100;
    }

    vector<int> indices = KMPSearch(_input.key, _candidate.key);

    score = (int) indices.size();

    if (!indices.empty())
    {
        if (indices[0] == 0) //If the candidate's key starts with the input's key
        {
            score = 10 + ((int) indices.size() - 1);
        }
    }

    return score;
}

int Search::Compare_Lev(DATA _candidate, DATA _input)
{
    int score;

    if (_candidate.key == _input.key)
    {
        return 100;
    }

    score = (int) max(_candidate.key.size(), _input.key.size()) + 10;
    score -= GetLevDist(_candidate.key, _input.key);
    return score;
}

void Search::PrintTable(vector<DATA> data)
{
    double n = 1;
    //table header
    cout << setfill('$') << setw(60) << "$" << endl;
    cout << setfill(' ') << fixed;
    cout << setw(8) << "RESULT" << setw(18) << "WORD" << setw(10) << "FILE" << setw(22) << "FREQUENCY / COUNT" << endl;
    cout << setfill('*') << setw(60) << "*" << endl;
    cout << setfill(' ') << fixed;

    //Data
    for (int i = 0; i < data.size(); i++)
    {
        PrintRow(data[i], i + 1);
    }
}

void Search::PrintTable(vector<pair<DATA, int>> data)
{
    double n = 1;
    //table header
    cout << setfill('$') << setw(60) << "$" << endl;
    cout << setfill(' ') << fixed;
    cout << setw(8) << "RESULT" << setw(18) << "WORD" << setw(10) << "FILE" << setw(22) << "FREQUENCY / COUNT" << endl;
    cout << setfill('*') << setw(60) << "*" << endl;
    cout << setfill(' ') << fixed;

    //Data
    for (int i = 0; i < data.size(); i++)
    {
        PrintRow(data[i].first, i + 1);
    }
}

void Search::PrintRow(DATA _node, int _index)
{
    if (_node.data.empty())
        cout << "ERROR: No data to PrintRow." << endl;
    cout << setprecision(1) << setw(8) << _index << setprecision(4) << setw(18) << _node.key << setw(10) << "~TOTAL~" << setw(22) << _node.GetFrequency(WORD_COUNT) << endl;

    for (int i = 0; i < _node.data.size(); i++)
    {
        string ch = _node.data[i].path.substr(9, 7);
        string result = to_string(_index) + "." + to_string(i + 1);
        cout << setprecision(0) << setw(8) << result << setw(18) << _node.key << setw(10) << ch << setw(22) << _node.data[i].GetInstances() << endl;
    }

    cout << setfill('-') << setw(60) << "-" << endl;
    cout << setfill(' ') << fixed;
}


#endif //DSA_INDEXING_ASSIGNMENT2_SEARCH_H
