//
// Created by Broderick Westrope on 14/05/2021.
//

#ifndef DSA_INDEXING_ASSIGNMENT2_MENUOPTIONS_H
#define DSA_INDEXING_ASSIGNMENT2_MENUOPTIONS_H

class MenuOptions
{
    AvlTree<DATA, string> *tree;
    Search search;
    SavingLoading saveLoad;
    FileOperations fileOps;
    Printing printing;
public:
    MenuOptions(AvlTree<DATA, string> *_tree, FileOperations _fileOps, SavingLoading saveLoad, Search search, Printing printing)
            : tree(_tree), fileOps(_fileOps), saveLoad(saveLoad), search(search), printing(printing)
    {};

    void ReadSaveFile();

    void ReadArticle();

    void SaveDictionary();

    void PrintDictionary();

    void PrintWordsStartingWith();

    void SearchForWord();

    void RemoveUncommon();

    void DeleteBadEggs(double _bound);
};

//region - Option Functions
//region - Reading
void MenuOptions::ReadSaveFile()
{
    string input;
    system("CLS");
    cout << "What file would you like to load from? (eg. 'saveFile'. Only write the name of the file)" << endl;
    cin >> input;
    input = fileOps.FormatFileName(input);
    system("CLS");
    cout << "Loading from '" << input << ".txt'... (This can take a minute or two with big files)" << endl << endl;
    int count = saveLoad.LoadNodes("..\\" + input + ".txt");
    cout << "Done. Successfully loaded " << to_string(count) << " words/phrases." << endl;
    WORD_COUNT += count;
}

void MenuOptions::ReadArticle()
{
    string input;
    system("CLS");
    cout << "Would you like to read phrases as well as words? [Y/N]" << endl;
    cin >> input;
    INCL_PHRASES = (input == "Y" || input == "y") ? true : false;

    system("CLS");
    cout << "What article would you like to read? (eg. 'harryPotter'. Only write the name of the file)" << endl;
    cin >> input;
    input = fileOps.FormatFileName(input);
    if (input == "harryPotter")
    {
        fileOps.ScanHarryPotter();
    }
    else
    {
        fileOps.ScanArticle("..\\" + input + ".txt");
    }
    if (WORD_COUNT != 0)
    {
        WHAT_TO_REMOVE = 'P';
        DeleteBadEggs((1.0 / static_cast<double>(WORD_COUNT)) * 1000.0 + 0.0001);
    }
}
//endregion

//region - Writing
void MenuOptions::SaveDictionary()
{
    string input;
    if (tree->AVL_Empty())
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
        input = fileOps.FormatFileName(input);
        SAVE_FILE = "..\\" + input + ".txt";
        saveLoad.ClearSaves();
        cout << "Saving to '" << input << ".txt'... (This can take a minute or two with big trees)" << endl;
        tree->AVL_TraverseBreadth(SaveNode);
        cout << "Done. Saving was successful." << endl;
    }
}
//endregion

//region - Printing & Searching
void MenuOptions::PrintDictionary()
{
    string input;
    if (tree->AVL_Empty())
    { cout << "The dictionary is empty. Be sure to scan a dictionary file or article first." << endl; }
    else
    {
        system("CLS");
        cout << "Would you like to print phrases as well as words? [Y/N]" << endl;
        cin >> input;
        INCL_PHRASES = (input == "Y" || input == "y") ? true : false;

        system("CLS");
        cout << "Do you want to print all the positions/locations? [Y/N] (These can be really long and cause table-alignment issues)" << endl;
        cin >> input;
        PRINT_POS = (input == "Y" || input == "y") ? true : false;
        printing.Dictionary_PrintTable();
    }
}

void MenuOptions::PrintWordsStartingWith()
{
    string input;
    if (tree->AVL_Empty())
    { cout << "The dictionary is empty. Be sure to scan a dictionary file or article first." << endl; }
    else
    {
        system("CLS");
        cout << "Would you like to print phrases as well as words? [Y/N]" << endl;
        cin >> input;
        INCL_PHRASES = (input == "Y" || input == "y") ? true : false;

        system("CLS");
        cout << "Do you want to print all the positions/locations? [Y/N] (These can be really long and cause table-alignment issues)" << endl;
        cin >> input;
        PRINT_POS = (input == "Y" || input == "y") ? true : false;
        search.SearchWordsStartingWith();
    }
}

void MenuOptions::SearchForWord()
{
    string input, searchType;
    if (tree->AVL_Empty())
    { cout << "The dictionary is empty. Be sure to scan a dictionary file or article first." << endl; }
    else
    {
        system("CLS");
        cout << "Would you like to search for phrases as well as words? [Y/N]" << endl;
        cin >> input;
        INCL_PHRASES = (input == "Y" || input == "y") ? true : false;

        system("CLS");
        cout << "What kind of search do you want to do? (NOTE: only option 1 allows for ordering based off a words frequency)\n"
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
        search.LoopSearch(FormatKey, searchType);
    }
}
//endregion

//region - Remove Uncommon Words
bool EvaluateUncommon(DATA _node, double _bound)
{
    if ((WHAT_TO_REMOVE == 'W' && _node.wordCount > 1) || (WHAT_TO_REMOVE == 'P' && _node.wordCount == 1))
        return false;

    if (_node.GetFrequency() < _bound)
        return true;

    return false;
}

void MenuOptions::DeleteBadEggs(double _bound)
{
    queue<DATA> badEggs = tree->AVL_RemoveUncommon(EvaluateUncommon, _bound);
    queue<DATA> copy;
    cout << "Deleted " << to_string(badEggs.size());
    while (!badEggs.empty())
    {
        tree->AVL_Delete(badEggs.front().key);
        copy.push(badEggs.front());
        badEggs.pop();
    }
    if (WHAT_TO_REMOVE == 'W')
        cout << " words ";
    else if (WHAT_TO_REMOVE == 'P')
        cout << " phrases ";
    else
        cout << " words & phrases ";
    cout << "with frequency lower than " << to_string(_bound) << ".";
}

void MenuOptions::RemoveUncommon()
{
    double bound;
    system("CLS");
    cout << "What would you like to make the lowest frequency? (anything below this decimal value will be deleted)" << endl;
    cin >> bound;

    char input;
    system("CLS");
    cout << "Would you like to remove [W]ords, [P]hrases, or [B]oth? (Enter one of the three letters. Invalid input will default to phrases)" << endl;
    cin >> input;

    if (input == 'w' || input == 'W')
        WHAT_TO_REMOVE = 'W';
    else if (input == 'b' || input == 'B')
        WHAT_TO_REMOVE = 'B';
    else
        WHAT_TO_REMOVE = 'P';

    DeleteBadEggs(bound);
}
//endregion
//endregion

#endif //DSA_INDEXING_ASSIGNMENT2_MENUOPTIONS_H
