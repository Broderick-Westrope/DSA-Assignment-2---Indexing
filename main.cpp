#include<stdlib.h>
#include<iomanip>
#include <string>
#include <fstream>
#include <iostream>
#include "vector"
#include "algorithm"
#include <bits/stdc++.h>
#include <filesystem>
#include "queue"

using namespace std;

int WORD_COUNT = 0;
string SAVE_FILE = "save.txt";
bool PRINT_POS = false;
bool INCL_PHRASES = false;

#include "DATA.h"
#include "Quicksort.h"
#include "AVL_ADT.h"
#include "Search.h"
#include "FileOperations.h"

void PrintRow(DATA *_node);

void PrintTable(AvlTree<DATA, string> *_tree);

int Menu(AvlTree<DATA, string> *_tree);


int main()
{
    AvlTree<DATA, string> tree;
    if (!tree.AVL_Empty())
    {
        cout << "ERROR: New tree wasn't empty." << endl;
        exit(1);
    }

    Menu(&tree);

    printf("BYE!\n");
    return 0;
}

int Menu(AvlTree<DATA, string> *_tree)
{
    Search search;

    while (true)
    {
        //region - Title
        cout << "                                         _ __\n"
                "         ___                             | '  \\\n"
                "    ___  \\ /  ___         ,'\\_           | .-. \\        /|\n"
                "    \\ /  | |,'__ \\  ,'\\_  |   \\          | | | |      ,' |_   /|\n"
                "  _ | |  | |\\/  \\ \\ |   \\ | |\\_|    _    | |_| |   _ '-. .-',' |_   _\n"
                " // | |  | |____| | | |\\_|| |__    //    |     | ,'_`. | | '-. .-',' `. ,'\\_\n"
                " \\\\_| |_,' .-, _  | | |   | |\\ \\  //    .| |\\_/ | / \\ || |   | | / |\\  \\|   \\\n"
                "  `-. .-'| |/ / | | | |   | | \\ \\//     |  |    | | | || |   | | | |_\\ || |\\_|\n"
                "    | |  | || \\_| | | |   /_\\  \\ /      | |`    | | | || |   | | | .---'| |\n"
                "    | |  | |\\___,_\\ /_\\ _      //       | |     | \\_/ || |   | | | |  /\\| |\n"
                "    /_\\  | |           //_____//       .||`      `._,' | |   | | \\ `-' /| |\n"
                "         /_\\           `------'        \\ |   AND        `.\\  | |  `._,' /_\\\n"
                "                                        \\|       THE          `.\\\n"
                "                                             _  _  _  _  __ _  __ _ /_\n"
                "                                            (_`/ \\|_)/ '|_ |_)|_ |_)(_\n"
                "                                            ._)\\_/| \\\\_,|__| \\|__| \\ _)\n"
                "                                                            _ ___ _      _\n"
                "                                                           (_` | / \\|\\ ||__\n"
                "                                                           ._) | \\_/| \\||___\n";
        //endregion

        cout << "READ:\n"
                " -[1]-   Read a save file\n" //Reads the given file and saves the data to the tree
                " -[2]-   Read an article\n" //loads from inputted text like article.txt. give option for what file and whether or not to include phrases
                "WRITE:\n"
                " -[3]-   Write the dictionary to a save file\n" //save the dictionary data to a text file, giving the user the action of name
                "PRINT:\n"
                " -[4]-   Print the dictionary\n" //Prints each word, how many files contain it, and then lists each containing file with all the positions
                " -[5]-   Print all words starting with a given letter\n"
                " -[6]-   Print the level & balance of each node\n" //prints the AVL tree (using Dongmos original AVL function)
                " -[7]-   Print the number of nodes in the tree\n"
                "OTHER:\n"
                " -[8]-   Search for a word\n"
                " -[9]-   Remove uncommon (low frequency) words/phrases from the dictionary\n"
                "\n"
                " -[0]-   Exit the program\n\n"
                "What would you like to do?\n";

        int action;
        cin >> action;

        int count = 0;
        string input;
        string searchType = "Lev";

        switch (action)
        {
            case 1: //Read a save file
                system("CLS");
                cout << "What file would you like to load from? (eg. 'saveFile'. Only write the name of the file)" << endl;
                cin >> input;
                input = FormatFileName(input);
                count = LoadNodes("..\\" + input + ".txt", _tree);
                break;

            case 2: //Read an article
                system("CLS");
                cout << "Would you like to read phrases as well as words? [Y/N]" << endl;
                cin >> input;
                INCL_PHRASES = (input == "Y" || input == "y") ? true : false;

                system("CLS");
                cout << "What article would you like to read? (eg. 'harryPotter'. Only write the name of the file)" << endl;
                cin >> input;
                input = FormatFileName(input);
                if (input == "harryPotter")
                { count = ScanBook(_tree); }
                else
                { count = ScanChapter("..\\" + input + ".txt", _tree); }
                break;

            case 3: //Save the dictionary
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
                    _tree->AVL_TraverseBreadth(SaveNode);
                }
                break;

            case 4: //Print the dictionary
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
                break;

            case 5: //Print dictionary starting with a letter
                if (_tree->AVL_Empty())
                { cout << "The dictionary is empty. Be sure to scan a dictionary file or article first." << endl; }
                else
                {
                    system("CLS");
                    cout << "Would you like to print phrases as well as words? [Y/N]" << endl;
                    cin >> input;
                    INCL_PHRASES = (input == "Y" || input == "y") ? true : false;
                    search.DictionaryStartingWith(_tree);
                }
                break;

            case 6: //Print level & balance of each node in the tree
                if (_tree->AVL_Empty())
                { cout << "The dictionary is empty. Be sure to scan a dictionary file or article first." << endl; }
                else
                { _tree->AVL_Print(); }
                break;

            case 7:
                if (_tree->AVL_Empty())
                { cout << "The dictionary is empty. Be sure to scan a dictionary file or article first." << endl; }
                else
                {
                    system("CLS");
                    cout << "Node Count: " << _tree->AVL_Count();
                }
                break;

            case 8: //Search for a word
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
                    search.LoopSearch(_tree, FormatWord, searchType);
                }
                break;

            case 9: //Remove uncommon words
                double bound;
                system("CLS");
                cout << "What would you like to make the lowest frequency? (anything below this decimal value will be deleted)" << endl;
                cin >> bound;
                RemoveUncommon(_tree, bound);
                break;

            case 0: //Exit the program
                return 0;
            default: //Invalid input
                cin.ignore();
                cout << "\nInvalid input. Press enter to continue..." << endl;
                getchar();
        }

        if (count > 0)
            cout << "Read " << to_string(count) << " words." << endl;

        cin.ignore();
        cout << "\nPress enter to continue..." << endl;
        getchar();
        system("CLS");
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

