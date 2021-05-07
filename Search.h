//
// Created by ascle on 7/05/2021.
//

#ifndef DSA_INDEXING_ASSIGNMENT2_SEARCH_H
#define DSA_INDEXING_ASSIGNMENT2_SEARCH_H

class Search
{
    void PrintRow(DATA _node, int _index);

    void PrintTable(vector<DATA> data);

    static int GetLevDist(string input, string candidate);

public:
    static int Comparison(DATA _current, DATA _old, int diff);

    void LoopSearch(AvlTree<DATA, string> *_tree, string (*RemoveSymbols)(string));
};

void Search::LoopSearch(AvlTree<DATA, string> *_tree, string (*RemoveSymbols)(string))
{
    while (true)
    {
        string query;
        cout << "SEARCH THE HOGWARTS LIBRARY:       (Type '~E' to exit)" << endl;
        cin >> query;
        system("CLS");

        if (query == "~E")
        {
            cout << "Leaving the library..." << endl;
            return;
        }
        query = RemoveSymbols(query);
        if (query.empty())
        {
            cout << "No valid input was entered, be sure to take a look at some of our great articles and only use letters in your search!" << endl;
            continue;
        }
        else
        {
            DATA qData;
            qData.key = RemoveSymbols(query);
            vector<pair<DATA, int>> results = _tree->AVL_GetClosestNodes(qData, Comparison);
            vector<DATA> sorted;
            for (int i = 0; i < results.size(); i++)
            {
                pair<DATA, int> best = make_pair(results[i].first, i);
                for (int j = i + 1; j < results.size(); j++)
                {
                    if (results[j].first.key.empty())
                    {
                        continue;
                    }
                    if (results[j].second < best.second)
                    {
                        best = make_pair(results[j].first, j);
                    }
                }
                results[best.second].first.key = "";
                if (!best.first.key.empty())
                    sorted.push_back(best.first);
            }
            cout << "Top " << sorted.size() << " results for your search of " << query << endl;
            PrintTable(sorted);
//            for (int i = 0; i < sorted.size(); i++)
//            {
//                cout << i + 1 << ". |" << sorted[i].key << "|" << endl;
//            }
        }
    }
}


int Search::GetLevDist(string input, string candidate) //Gets the Levenshtein Distance between two strings to find the closest result
{
    int i, j, l1, l2, t, track;
    int dist[50][50];
    //take the strings as input
    char s1[] = "tutorials";
    char s2[] = "point";
    //stores the lenght of strings s1 and s2
    l1 = input.size();
    l2 = candidate.size();
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
            if (s1[i - 1] == s2[j - 1])
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
//    cout << "The Levinstein distance is:" << dist[l2][l1] << endl;
    return dist[l2][l1];
}


int Search::Comparison(DATA _current, DATA _old, int diff)
{
    if (_current.key == _old.key)
    {
        return diff;
    }

    diff = GetLevDist(_current.key, _old.key);
    return diff;
}

void Search::PrintTable(vector<DATA> data)
{
    double n = 1;
    //table header
    cout << setfill('$') << setw(52) << "$" << endl;
    cout << setfill(' ') << fixed;
    cout << setw(8) << "RESULT" << setw(16) << "WORD" << setw(12) << "FILE" << setw(12) << "FREQUENCY" << endl;
    cout << setfill('*') << setw(52) << "*" << endl;
    cout << setfill(' ') << fixed;

    //Data
    for (int i = 0; i < data.size(); i++)
    {
        PrintRow(data[i], i + 1);
    }
}

void Search::PrintRow(DATA _node, int _index)
{
    if (_node.data.empty())
        cout << "ERROR: No data to PrintRow." << endl;
    for (int i = 0; i < _node.data.size(); i++)
    {
        string ch = _node.data[i].path.substr(9, 7);
        string result = to_string(_index) + "." + to_string(i + 1);
        cout << setprecision(0) << setw(8) << result << setw(16) << _node.key << setprecision(4) << setw(12) << ch << setw(12) << _node.data[i].freq << endl;
    }
    cout << setfill('-') << setw(52) << "-" << endl;
    cout << setfill(' ') << fixed;
}


#endif //DSA_INDEXING_ASSIGNMENT2_SEARCH_H
