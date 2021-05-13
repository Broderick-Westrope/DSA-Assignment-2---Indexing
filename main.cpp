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

int WORD_COUNT;
string SAVE_FILE = "save.txt";
bool PRINT_POS = false;

#include "DATA.h"
#include "Quicksort.h"
#include "AVL_ADT.h"
#include "Search.h"
#include "FileOperations.h"

void PrintRow(DATA *_node);

void PrintTable(AvlTree<DATA, string> *_tree);

int Menu(AvlTree<DATA, string> *_tree);

string FormatFileName(string);

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
                " -[6]-   Print the tree data\n" //prints the AVL tree (using Dongmos original AVL function)
                "OTHER:\n"
                " -[7]-   Search for a word\n"
                " -[8]-   Remove uncommon (low frequency) words/phrases from the dictionary\n"
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
                cout << "What article would you like to read? (eg. 'harryPotter'. Only write the name of the file)" << endl;
                cin >> input;
                input = FormatFileName(input);
                if (input == "harryPotter")
                    count = ScanBook(_tree);
                else
                {
                    count = ScanChapter("..\\" + input + ".txt", _tree);
                }
                break;

            case 3: //Save the dictionary
                system("CLS");
                cout << "What file would you like to save to? (eg. 'saveFile'. Only write the name of the file)" << endl;
                cin >> input;
                input = FormatFileName(input);
                SAVE_FILE = "..\\" + input + ".txt";
                ClearSaves();
                _tree->AVL_TraverseBreadth(SaveNode);
                break;

            case 4: //Print the dictionary
                system("CLS");
                cout << "Do you want to print all the positions? [Y/N] (These can be really long and cause table-alignment issues)" << endl;
                cin >> input;
                PRINT_POS = (input == "Y" || input == "y") ? true : false;
                PrintTable(_tree);
                break;

            case 5:

                break;

            case 6: //Print the tree data
                _tree->AVL_Print();
                break;

            case 7: //Search for a word
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
                break;

            case 8: //Remove uncommon words

                break;

            case 0: //Exit the program
                return 0;
            default: //Invalid input
                cout << "Invalid input. \nPress any key to continue..." << endl;
                getchar();
        }

        if (count > 0)
            cout << "Read " << to_string(count) << " words." << endl;

        getchar();
        system("CLS");
    }
}

void PrintTable(AvlTree<DATA, string> *_tree)
{
    double n = 1;
    //table header
    cout << setfill('$') << setw(52) << "$" << endl;
    cout << setfill(' ') << fixed;
    cout << setw(15) << "WORD" << setw(10) << "FILE" << setw(22) << "FREQUENCY / COUNT" << endl;
    cout << setfill('*') << setw(52) << "*" << endl;
    cout << setfill(' ') << fixed;

    //Data
    _tree->AVL_TraverseInOrder(PrintRow);

    cout << setfill('$') << setw(52) << "$" << endl;
    cout << setfill(' ') << fixed;

}

void PrintRow(DATA *_node)
{
    if (_node->data.empty())
        cout << "ERROR: No data to PrintRow." << endl;
    cout << setprecision(4) << setw(15) << _node->key << setw(10) << "~TOTAL~" << setw(22) << _node->GetFrequency(WORD_COUNT) << endl;

    for (ARTICLE &d : _node->data)
    {
        string ch = d.path.substr(9, 7);
        cout << setprecision(0) << setw(15) << _node->key << setprecision(4) << setw(10) << ch << setw(22) << d.GetInstances() << endl;

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

    cout << setfill('-') << setw(52) << "-" << endl;
    cout << setfill(' ') << fixed;
}

string FormatFileName(string _fn)
{
    for (int i = 0; i < _fn.size(); i++)
    {
        // Finding the character whose
        // ASCII value fall under this
        // range
        if (_fn[i] == '/' || _fn[i] == '\\' || _fn[i] == '?' || _fn[i] == '%' || _fn[i] == '*' || _fn[i] == ':' || _fn[i] == '|' || _fn[i] == '"' || _fn[i] == '<' || _fn[i] == '>' || _fn[i] == '.' ||
            _fn[i] == ',' || _fn[i] == ';' || _fn[i] == '=' || _fn[i] == ' ')
        {
            // erase function to erase
            // the character
            _fn.erase(i, 1);
            i--;
        }
    }

    return _fn;
}