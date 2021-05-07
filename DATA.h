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
    double freq;
    vector<int> pos;

    ARTICLE(string _path, double _freq) : path(_path), freq(_freq)
    {};
};

bool operator<(const ARTICLE &n1, const ARTICLE &n2)
{
    return n1.freq < n2.freq;
}

class DATA //The Node's data
{
public:
    string key;
    vector<ARTICLE> data;
};

bool operator<(const DATA &p1, const DATA &p2)
{
    int high1 = 0, high2 = 0;
    for (int i = 0; i < p1.data.size(); i++)
        if (p1.data[i].freq > high1)
            high1 = p1.data[i].freq;
    for (int i = 0; i < p1.data.size(); i++)
        if (p2.data[i].freq > high2)
            high2 = p2.data[i].freq;

    return high1 < high2;
}

istream &operator>>(istream &in, DATA &obj) //Loading Save
{
//    string key, path;
//    int freq;
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
//        out << obj.key << ' ' << d.freq << ' ' << fName;
//        for (int i : d.pos)
//            out << ' ' << to_string(i);
//        out << endl;
//    }
    return in;
}

ostream &operator<<(ostream &out, const DATA &obj) //Saving Data
{
    for (ARTICLE d : obj.data)
    {
        string fName = d.path.substr(9, 7);
        out << obj.key << ' ' << d.freq << ' ' << fName << '[';
        for (int i = 0; i < d.pos.size(); i++)
        {
            out << to_string(d.pos[i]);
            if (i != d.pos.size() - 1)
                out << ',';
        }
        out << ']' << endl;
    }
    return out;
}

#endif //DSA_INDEXING_ASSIGNMENT2_DATA_H
