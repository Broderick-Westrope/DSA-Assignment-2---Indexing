//
// Created by ascle on 12/05/2021.
//

#ifndef DSA_INDEXING_ASSIGNMENT2_QUICKSORT_H
#define DSA_INDEXING_ASSIGNMENT2_QUICKSORT_H

//region - pair<DATA, int>
void swap(pair<DATA, int> *a, pair<DATA, int> *b)
{
    pair<DATA, int> t = *a;
    *a = *b;
    *b = t;
}

int partition(vector<pair<DATA, int>> &_v, int low, int high)
{
    pair<DATA, int> pivot = _v[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++)
    {
        if (_v[j].second <= pivot.second)
        {
            i++;
            swap(&_v[i], &_v[j]);
        }
    }
    swap(&_v[i + 1], &_v[high]);

    return (i + 1);
}

vector<pair<DATA, int>> quickSort(vector<pair<DATA, int>> _v, int low, int high)
{
    if (low < high)
    {
        /* pi is partitioning index, _v[p] is now
           at right place */
//        cout << "BEFORE: ";
//        for (int i = 0; i < _v.size(); i++)
//        {
//            cout << ", " << _v[i].first.key;
//        }
//        cout << endl;
        int pi = partition(_v, low, high);
//        cout << "AFTER: ";
//        for (int i = 0; i < _v.size(); i++)
//        {
//            cout << ", " << _v[i].first.key;
//        }
//        cout << endl;
        // Separately sort elements before
        // partition and after partition
        _v = quickSort(_v, low, pi - 1);
        _v = quickSort(_v, pi + 1, high);
    }
    else
        cout << "low = high" << endl;

    return _v;
}
//endregion

#endif //DSA_INDEXING_ASSIGNMENT2_QUICKSORT_H
