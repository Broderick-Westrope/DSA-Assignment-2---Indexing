//
// Created by ascle on 16/05/2021.
//

#ifndef DSA_INDEXING_ASSIGNMENT2_PRINTING_H
#define DSA_INDEXING_ASSIGNMENT2_PRINTING_H

class Printing
{
    AvlTree<DATA, string> *tree;

    void Search_PrintRow(DATA _node, int _index);

public:

    Printing(AvlTree<DATA, string> *_tree) : tree(_tree)
    {};

    //Search
    void Search_PrintTable(vector<DATA> data);

    void Search_PrintTable(vector<pair<DATA, int>> data);

    void Search_PrintTable(priority_queue<DATA> data);

    void Search_PrintNode(DATA _node);

    //Dictionary
    void Dictionary_PrintTable();
};

//region - Search
void Printing::Search_PrintTable(vector<DATA> data)
{
    double n = 1;
    //Headers
    cout << setfill('$') << setw(66) << "$" << endl;
    cout << setfill(' ') << fixed;
    cout << setw(8) << "RESULT" << setw(24) << "WORD" << setw(10) << "FILE" << setw(22) << "FREQUENCY / COUNT" << endl;
    cout << setfill('*') << setw(66) << "*" << endl;
    cout << setfill(' ') << fixed;

    //Data
    for (int i = 0; i < data.size(); i++)
    {
        Search_PrintRow(data[i], i + 1);
    }
}

void Printing::Search_PrintTable(vector<pair<DATA, int>> data)
{
    double n = 1;
    //table header
    cout << setfill('$') << setw(66) << "$" << endl;
    cout << setfill(' ') << fixed;
    cout << setw(8) << "RESULT" << setw(24) << "WORD" << setw(10) << "FILE" << setw(22) << "FREQUENCY / COUNT" << endl;
    cout << setfill('*') << setw(66) << "*" << endl;
    cout << setfill(' ') << fixed;

    //Data
    for (int i = 0; i < data.size(); i++)
    {
        cout << data[i].first.key << " : " << to_string(i) << endl;
        Search_PrintRow(data[i].first, i + 1);
    }
}

void Printing::Search_PrintTable(priority_queue<DATA> data)
{
    double n = 1;
    //table header
    cout << setfill('$') << setw(66) << "$" << endl;
    cout << setfill(' ') << fixed;
    cout << setw(8) << "RESULT" << setw(24) << "WORD" << setw(10) << "FILE" << setw(22) << "FREQUENCY / COUNT" << endl;
    cout << setfill('*') << setw(66) << "*" << endl;
    cout << setfill(' ') << fixed;

    //Data
    int i = 0;
    while (!data.empty())
    {
        Search_PrintRow(data.top(), i + 1);
        i++;
        data.pop();
    }
}

void Printing::Search_PrintRow(DATA _node, int _index)
{
    if (_node.data.empty())
        cout << "ERROR: No data to Search_PrintRow." << endl;
    cout << setprecision(1) << setw(8) << _index << setprecision(4) << setw(24) << _node.key << setw(10) << "~TOTAL~" << setw(22) << _node.GetFrequency() << endl;

    for (int i = 0; i < _node.data.size(); i++)
    {
        string ch = _node.data[i].path.substr(9, 7);
        string result = to_string(_index) + "." + to_string(i + 1);
        cout << setprecision(0) << setw(8) << result << setw(24) << _node.key << setw(10) << ch << setw(22) << _node.data[i].GetInstances() << endl;
    }

    cout << setfill('-') << setw(66) << "-" << endl;
    cout << setfill(' ') << fixed;
}

void Printing::Search_PrintNode(DATA _node)
{
    cout << "WORD: " << _node.key << endl;
    cout << "NUMBER OF INSTANCES: " << _node.GetInstances() << endl;
    cout << "FREQUENCY PER 1000 WORDS: " << setprecision(8) << _node.GetFrequency() << setprecision(0) << endl;
    cout << "LOCATIONS: " << endl;
    for (int i = 0; i < _node.data.size(); i++)
    {
        cout << to_string(i) << ".     |" << _node.data[i].path << "|     ( ";
        for (int p : _node.data[i].pos)
        {
            cout << to_string(p + 1) << " ";
        }
        cout << ")" << endl;
    }
}
//endregion

//region - Dictionary
void Dictionary_PrintRow(DATA *_node)
{
    if (!INCL_PHRASES && _node->wordCount > 1)
        return;

    if (_node->data.empty())
        cout << "ERROR: No data to Search_PrintRow." << endl;
    cout << setprecision(4) << setw(24) << _node->key << setw(10) << "~TOTAL~" << setw(22) << _node->GetFrequency() << endl;

    for (ARTICLE &d : _node->data)
    {
        string ch = d.path.substr(9, 7);
        cout << setprecision(0) << setw(24) << _node->key << setprecision(4) << setw(10) << ch << setw(22) << d.GetInstances() << endl;

        if (PRINT_POS)
        {
            cout << "Positions: (";
            for (int i = 0; i < d.pos.size(); i++)
            {
                cout << to_string(d.pos[i]);
                if (i != d.pos.size() - 1)
                    cout << ", ";
            }
            cout << ") " << endl;
        }
    }

    cout << setfill('-') << setw(55) << "-" << endl;
    cout << setfill(' ') << fixed;
}

void Printing::Dictionary_PrintTable()
{
    double n = 1;
    //table header
    cout << setfill('$') << setw(55) << "$" << endl;
    cout << setfill(' ') << fixed;
    cout << setw(24) << "WORD" << setw(10) << "FILE" << setw(22) << "FREQUENCY / COUNT" << endl;
    cout << setfill('*') << setw(55) << "*" << endl;
    cout << setfill(' ') << fixed;

    //Data
    tree->AVL_TraverseInOrder(Dictionary_PrintRow);

    cout << setfill('$') << setw(55) << "$" << endl;
    cout << setfill(' ') << fixed;

}
//endregion

#endif //DSA_INDEXING_ASSIGNMENT2_PRINTING_H
