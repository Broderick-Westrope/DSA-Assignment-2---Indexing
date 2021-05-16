//
// Created by ascle on 7/05/2021.
//

#ifndef DSA_INDEXING_ASSIGNMENT2_DATA_H
#define DSA_INDEXING_ASSIGNMENT2_DATA_H

using namespace std;

class ARTICLE //The article's data
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

ostream &operator<<(ostream &out, DATA &obj) //Saving Data
{
    out << obj.wordCount << ' ' << obj.key << endl;

    for (ARTICLE d : obj.data)
    {
        out << d.path << ' ';

        for (int i = 0; i < d.pos.size(); i++)
        {
            out << to_string(d.pos[i]) << ' ';
        }
        out << endl;
    }

    out << '#' << endl;
    return out;
}

#endif //DSA_INDEXING_ASSIGNMENT2_DATA_H
