//
// Created by ascle on 7/05/2021.
//

#ifndef DSA_INDEXING_ASSIGNMENT2_DATA_H
#define DSA_INDEXING_ASSIGNMENT2_DATA_H

using namespace std;

class ARTICLE //The articles's data
{
public:
    string path;
    vector<int> pos;

    ARTICLE(string _path) : path(_path)
    {};

    int GetInstances()
    {
        return pos.size();
    }
};

class DATA //The Node's data
{
    double freq;
public:
    string key;
    int wordCount;
    vector<ARTICLE> data;

    double GetFrequency(int _wordCount)
    {
        auto total = static_cast<double>(GetInstances());
        if (total == 0.0)
            return 0.0;
        return (total / static_cast<double>(_wordCount)) * 1000.0;
    }

    int GetInstances()
    {
        int total = 0;
        if (!data.empty())
            for (auto d:data)
            {
                total += d.GetInstances();
            }
        return total;
    }
};

bool operator<(DATA p1, DATA p2)
{
    return p1.GetFrequency(WORD_COUNT) < p2.GetFrequency(WORD_COUNT);
}

istream &operator>>(istream &in, DATA &obj) //Loading Save
{
//    string key, path;
//    int occurences;
//    vector<int> pos;
//
//    //Line
//    string sLine;
//    getline(in, sLine);
////    cout << sLine << endl;
//
//    int wordCount = 0;
//    string tempWord;
//    for (char c : sLine)
//    {
//        if (c == ' ')
//        {
//            if (wordCount == 0)
//                key = tempWord;
//            wordCount++;
//            continue;
//        }
//
//        if (wordCount == 1 || wordCount > 2)
//        {
//            if (c >= '0' && c <= '9')
//            {
//
//            }
//        }
//        if ((c >= 'A' && c <= 'Z') || (c >= 'a' || c <= 'z'))
//    }
//
//    for (ARTICLE d : obj.data)
//    {
//        string fName = d.path.substr(9, 7);
//        out << obj.key << ' ' << d.occurences << ' ' << fName;
//        for (int i : d.pos)
//            out << ' ' << to_string(i);
//        out << endl;
//    }
    return in;
}

ostream &operator<<(ostream &out, DATA &obj) //Saving Data
{
    out << obj.wordCount << ' ' << obj.key << endl;

    for (ARTICLE d : obj.data)
    {
        out << d.path << ' ';

        for (int i = 0; i < d.pos.size(); i++)
        {
            out << to_string(d.pos[i]) << ' ';
//            if (i == d.pos.size() - 1)
//                out << '|';
        }
        out << endl;
    }

    out << "# " << endl;
    return out;
}

#endif //DSA_INDEXING_ASSIGNMENT2_DATA_H
