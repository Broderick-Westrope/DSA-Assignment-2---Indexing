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
        int pi = partition(_v, low, high);

        _v = quickSort(_v, low, pi - 1);
        _v = quickSort(_v, pi + 1, high);
    }

    return _v;
}
//endregion

#endif //DSA_INDEXING_ASSIGNMENT2_QUICKSORT_H
