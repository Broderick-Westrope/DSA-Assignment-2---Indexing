//
// Created by Broderick Westrope on 12/05/2021.
//

#ifndef DSA_INDEXING_ASSIGNMENT2_QUICKSORT_H
#define DSA_INDEXING_ASSIGNMENT2_QUICKSORT_H

//region - pair<DATA, int>
void Swap(pair<DATA, int> *_a, pair<DATA, int> *_b)
{
    pair<DATA, int> t = *_a;
    *_a = *_b;
    *_b = t;
}

int Partition(vector<pair<DATA, int>> &_v, int low, int high)
{
    pair<DATA, int> pivot = _v[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++)
    {
        if (_v[j].second >= pivot.second)
        {
            i++;
            Swap(&_v[i], &_v[j]);
        }
    }
    Swap(&_v[i + 1], &_v[high]);

    return (i + 1);
}

vector<pair<DATA, int>> QuickSort(vector<pair<DATA, int>> _v, int low, int high)
{
    if (low < high)
    {
        int pi = Partition(_v, low, high);

        _v = QuickSort(_v, low, pi - 1);
        _v = QuickSort(_v, pi + 1, high);
    }

    return _v;
}
//endregion

#endif //DSA_INDEXING_ASSIGNMENT2_QUICKSORT_H
