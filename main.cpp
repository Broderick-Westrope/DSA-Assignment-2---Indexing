//
// Created by Broderick Westrope on 5/05/2021.
//

//region - References
#include<stdlib.h>
#include<iomanip>
#include <string>
#include <fstream>
#include <iostream>
#include "vector"
#include "algorithm"
#include "queue"

using namespace std;

int WORD_COUNT = 0;
string SAVE_FILE = "saveFile.txt";
bool PRINT_POS = false;
bool INCL_PHRASES = false;

#include "DATA.h"
#include "Quicksort.h"
#include "AVL_ADT.h"
#include "Search.h"
#include "Saving&Loading.h"
#include "FileOperations.h"
#include "MenuOptions.h"
//endregion

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

        //region - Options
        cout << " \nREAD:\n"
                "  -[1]-   Read a save file\n" //Reads the given file and saves the data to the tree
                "  -[2]-   Read an article\n" //loads from inputted text like article.txt. give option for what file and whether or not to include phrases
                " \nWRITE:\n"
                "  -[3]-   Write the dictionary to a save file\n" //save the dictionary data to a text file, giving the user the action of name
                " \nPRINT:\n"
                "  -[4]-   Print the dictionary\n" //Prints each word, how many files contain it, and then lists each containing file with all the positions
                "  -[5]-   Print all words starting with a given letter\n"
                "  -[6]-   Print the level & balance of each node\n" //prints the AVL tree (using Dongmos original AVL function)
                "  -[7]-   Print the number of nodes in the tree\n"
                " \nOTHER:\n"
                "  -[8]-   Search for a word\n"
                "  -[9]-   Remove uncommon (low frequency) words/phrases from the dictionary\n"
                " \n\n"
                "  -[0]-   Exit the program\n\n"
                " What would you like to do?\n";
        //endregion

        int action;
        cin >> action;
        int count = 0;

        switch (action)
        {
            case 1: //Read a save file
                count = ReadSaveFile(_tree);
                break;

            case 2: //Read an article
                count = ReadArticle(_tree);
                break;

            case 3: //Save the dictionary
                SaveDictionary(_tree);
                break;

            case 4: //Print the dictionary
                PrintDictionary(_tree);
                break;

            case 5: //Print dictionary starting with a letter
                PrintWordsStartingWith(_tree, search);
                break;

            case 6: //Print level & balance of each node in the tree
                if (_tree->AVL_Empty())
                { cout << "The dictionary is empty. Be sure to scan a dictionary file or article first." << endl; }
                else
                {
                    system("CLS");
                    _tree->AVL_Print();
                }
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
                SearchForWord(_tree, search);
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

int main()
{
    AvlTree<DATA, string> tree;
    if (!tree.AVL_Empty())
    {
        cout << "ERROR: New tree wasn't empty." << endl;
        exit(1);
    }

    Menu(&tree);
    cout << "Leaving..." << endl;

    return 0;
}