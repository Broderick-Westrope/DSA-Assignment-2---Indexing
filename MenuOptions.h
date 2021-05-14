//
// Created by Broderick Westrope on 14/05/2021.
//

#ifndef DSA_INDEXING_ASSIGNMENT2_MENUOPTIONS_H
#define DSA_INDEXING_ASSIGNMENT2_MENUOPTIONS_H

//region - Function Declarations
int ReadSaveFile(AvlTree<DATA, string> *_tree);

int ReadArticle(AvlTree<DATA, string> *_tree);

void SaveDictionary(AvlTree<DATA, string> *_tree);

void PrintDictionary(AvlTree<DATA, string> *_tree);

void PrintWordsStartingWith(AvlTree<DATA, string> *_tree, Search _search);

void SearchForWord(AvlTree<DATA, string> *_tree, Search _search);

void PrintRow(DATA *_node);

void PrintTable(AvlTree<DATA, string> *_tree);
//endregion

//region - Option Functions
int ReadSaveFile(AvlTree<DATA, string> *_tree)
{
    string input;
    system("CLS");
    cout << "What file would you like to load from? (eg. 'saveFile'. Only write the name of the file)" << endl;
    cin >> input;
    input = FormatFileName(input);
    cout << "Loading from '" << input << ".txt'... (This can take a minute or two with big files)" << endl;
    int count = LoadNodes("..\\" + input + ".txt", _tree);
    cout << "Done. Loading was successful." << endl;
    return count;
}

int ReadArticle(AvlTree<DATA, string> *_tree)
{
    string input;
    system("CLS");
    cout << "Would you like to read phrases as well as words? [Y/N]" << endl;
    cin >> input;
    INCL_PHRASES = (input == "Y" || input == "y") ? true : false;

    system("CLS");
    cout << "What article would you like to read? (eg. 'harryPotter'. Only write the name of the file)" << endl;
    cin >> input;
    input = FormatFileName(input);
    if (input == "harryPotter")
        return ScanHarryPotter(_tree);

    return ScanArticle("..\\" + input + ".txt", _tree);
}

void SaveDictionary(AvlTree<DATA, string> *_tree)
{
    string input;
    if (_tree->AVL_Empty())
    { cout << "The dictionary is empty. Be sure to scan a dictionary file or article first." << endl; }
    else
    {
        system("CLS");
        cout << "Would you like to save phrases as well as words? [Y/N]" << endl;
        cin >> input;
        INCL_PHRASES = (input == "Y" || input == "y") ? true : false;

        system("CLS");
        cout << "What file would you like to save to? (eg. 'saveFile'. Only write the name of the file)" << endl;
        cin >> input;
        input = FormatFileName(input);
        SAVE_FILE = "..\\" + input + ".txt";
        ClearSaves();
        cout << "Saving to '" << input << ".txt'... (This can take a minute or two with big trees)" << endl;
        _tree->AVL_TraverseBreadth(SaveNode);
        cout << "Done. Saving was successful." << endl;
    }
}

void PrintDictionary(AvlTree<DATA, string> *_tree)
{
    string input;
    if (_tree->AVL_Empty())
    { cout << "The dictionary is empty. Be sure to scan a dictionary file or article first." << endl; }
    else
    {
        system("CLS");
        cout << "Would you like to print phrases as well as words? [Y/N]" << endl;
        cin >> input;
        INCL_PHRASES = (input == "Y" || input == "y") ? true : false;

        system("CLS");
        cout << "Do you want to print all the positions? [Y/N] (These can be really long and cause table-alignment issues)" << endl;
        cin >> input;
        PRINT_POS = (input == "Y" || input == "y") ? true : false;
        PrintTable(_tree);
    }
}

void PrintWordsStartingWith(AvlTree<DATA, string> *_tree, Search _search)
{
    string input;
    if (_tree->AVL_Empty())
    { cout << "The dictionary is empty. Be sure to scan a dictionary file or article first." << endl; }
    else
    {
        system("CLS");
        cout << "Would you like to print phrases as well as words? [Y/N]" << endl;
        cin >> input;
        INCL_PHRASES = (input == "Y" || input == "y") ? true : false;
        _search.SearchWordsStartingWith(_tree);
    }
}

void SearchForWord(AvlTree<DATA, string> *_tree, Search _search)
{
    string input, searchType;
    if (_tree->AVL_Empty())
    { cout << "The dictionary is empty. Be sure to scan a dictionary file or article first." << endl; }
    else
    {
        system("CLS");
        cout << "Would you like to search for phrases as well as words? [Y/N]" << endl;
        cin >> input;
        INCL_PHRASES = (input == "Y" || input == "y") ? true : false;

        system("CLS");
        cout << "What kind of search do you want to do?\n"
                " -[1]- Strict (Results are words/phrases that start with your search query)\n"
                " -[2]- Contains (Results are words/phrases that contain your search query)\n"
                " -[3]- Levenshtein (Results are all words passed to find the closest to your\n"
                "       query. Sorted based on their Levenshtein distance from the query)" << endl;
        cin >> input;

        if (input == "1")
            searchType = "Strict";
        else if (input == "2")
            searchType = "Contains";
        else if (input != "3")
            cout << "Invalid input. I'll do a strict search for you" << endl;

        cin.ignore();
        _search.LoopSearch(_tree, FormatKey, searchType);
    }
}

void PrintTable(AvlTree<DATA, string> *_tree)
{
    double n = 1;
    //table header
    cout << setfill('$') << setw(55) << "$" << endl;
    cout << setfill(' ') << fixed;
    cout << setw(24) << "WORD" << setw(10) << "FILE" << setw(22) << "FREQUENCY / COUNT" << endl;
    cout << setfill('*') << setw(55) << "*" << endl;
    cout << setfill(' ') << fixed;

    //Data
    _tree->AVL_TraverseInOrder(PrintRow);

    cout << setfill('$') << setw(55) << "$" << endl;
    cout << setfill(' ') << fixed;

}

void PrintRow(DATA *_node)
{
    if (!INCL_PHRASES && _node->wordCount > 1)
        return;

    if (_node->data.empty())
        cout << "ERROR: No data to PrintRow." << endl;
    cout << setprecision(4) << setw(24) << _node->key << setw(10) << "~TOTAL~" << setw(22) << _node->GetFrequency(WORD_COUNT) << endl;

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

bool EvaluateUncommon(DATA _node, double _bound)
{
    if (_node.GetFrequency(WORD_COUNT) < _bound)
        return true;
    return false;
}

void RemoveUncommon(AvlTree<DATA, string> *_tree, double _bound)
{
    queue<DATA> badEggs = _tree->AVL_RemoveUncommon(EvaluateUncommon, _bound);
    queue<DATA> copy;
    cout << "Deleted " << to_string(badEggs.size()) << " nodes with frequency lower than " << to_string(_bound) << ".";
    while (!badEggs.empty())
    {
        _tree->AVL_Delete(badEggs.front().key);
        copy.push(badEggs.front());
        badEggs.pop();
    }
}
//endregion

#endif //DSA_INDEXING_ASSIGNMENT2_MENUOPTIONS_H
