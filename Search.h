//
// Created by ascle on 7/05/2021.
//

#ifndef DSA_INDEXING_ASSIGNMENT2_SEARCH_H
#define DSA_INDEXING_ASSIGNMENT2_SEARCH_H

class Search
{
    void PrintRow(DATA _node, int _index);

    void PrintTable(vector<pair<DATA, int>> data);

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
        cout << "SEARCH THE HARRY POTTER CHAPTERS:       (Type '~E' to exit)" << endl;
        getline(cin, query);
        system("CLS");

        if (query == "~E")
        {
            cout << "Leaving..." << endl;
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
            for (int i = 0; i < results.size(); i++)
            {
                cout << i + 1 << ". |" << results[i].first.key << "| " << results[i].second << endl;
            }

            results = quickSort(results, 0, (int) results.size() - 1);

            cout << "Top " << results.size() << " results for your search of " << query;
            if (results.front().first.key == query)
            {
                cout << ", including an exact match!";
            }
            cout << endl;
            PrintTable(results);
//            for (int i = 0; i < results.size(); i++)
//            {
//                cout << i + 1 << ". |" << results[i].first.key << "|   " << endl;
//            }
        }

        cout << endl << "Press Enter To Continue..." << endl;
        getchar();
        system("CLS");
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

//Function which parses out a string based on the delimiter of choice. The results are stored back into a vector which is passed in by memory the address
void GetTokens(string str, vector<string> &tokenVector, char token)
{
    //Skips the delimiters at the beginning of the string
    int lastPosition = str.find_first_not_of(token, 0);
    //Find the first non delimiter
    int position = str.find_first_of(token, lastPosition);

    //While loop which iterates through a string to subract tokens
    while (string::npos != position || string::npos != lastPosition)
    {
        //Adds found token to the vector
        tokenVector.push_back(str.substr(lastPosition, position - lastPosition));
        //Finds the next delimiter
        lastPosition = str.find_first_not_of(token, position);
        //Finds the next non delimiter
        position = str.find_first_of(token, lastPosition);
    }
}

//Complex searching algorithm to handle multi word searches
void complexSearch(string _input, string _candidate)
{
    vector<string> searchResults;
    vector<string> formattedText;

    //This uses the strstr function to find if an occurance of our search string has occurred in the original string
    const char *ptr = strstr(_candidate.c_str(), _input.c_str()); //!used to be OriginalNotes
    //A match is found if the pointer returned by the strstr function is not NULL
    if (ptr != NULL)
    {
        //Create a new string by using strdup to duplicate the string starting at the position an occurance was found
        string str = strdup(ptr);
        string searchString = _input;
        int start = 0;
        string final;
        bool finished = false;
        //Round up the last word by finding the next whitespace
        while (start < searchString.size() - 1 || !finished && start < str.size())
        {
            //Make sure we don’t go out of bounds while itterating the string
            if (start > searchString.size() - 1)
            {
                //If we find anything, but a space, add the character to our last word
                if (str[start] != ' ')
                {
                    //Add the character to the final word and increase the position
                    final = final + str[start];
                    start++;
                }
                    //If a space was found, it means that the word has been rounded up
                else
                {
                    //Set a boolean so our loop will break
                    finished = true;
                }
            }
                //This is an edge case for if the word to be rounded is the last word in the original string
            else
            {
                //Add the character to the final word and increase the position
                final = final + str[start];
                start++;
            }
        }
        //Inilize a vector to store tokenized strings into
        vector<string> tokens;
        //Call the tokenizing function and separate the words by space
        GetTokens(final, tokens, ' ');
        int foundPosition;
        //Look for the occurance of the first search word in vector of original words
        for (int j = 0; j < formattedText.size(); j++)
        {
            const char *ptr = strstr(formattedText[j].c_str(), tokens[0].c_str());
            //Found an occurance
            if (ptr != NULL)
            {
                //If we find a match, we need to start at that position and see if the next words in the search vector match the corresponding order of words in the original word vector
                foundPosition = j;
                int count = 0;
                //Loop through the vector of tokens
                for (int i = 0; i < tokens.size(); i++)
                {
                    //If the arrays don’t match, then break from the loop
                    if (tokens[i] != formattedText[foundPosition])
                    {
                        break;
                    }
                    foundPosition++;
                    count++;
                }
                //If all occurance of the search string occurred in correct order, then we are done
                if (count == tokens.size())
                {
                    searchResults.push_back(final);
                }
            }
        }
    }
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

void Search::PrintTable(vector<pair<DATA, int>> data)
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
        PrintRow(data[i].first, i + 1);
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
        cout << setprecision(0) << setw(8) << result << setw(16) << _node.key << setprecision(7) << setw(12) << ch << setw(12) << _node.GetFrequency(WORD_COUNT) << endl;
    }
    cout << setfill('-') << setw(52) << "-" << endl;
    cout << setfill(' ') << fixed;
}


#endif //DSA_INDEXING_ASSIGNMENT2_SEARCH_H
