/* --- ItemSet.h --- */

/* ------------------------------------------
Author: LoveNyabi
Date: 2025/8/27
------------------------------------------ */

#ifndef ITEMSET_H
#define ITEMSET_H
#include <vector>
using namespace std;
class Itemset {
public:
    Itemset();
    ~Itemset();
    void print_item();
    vector<pair<int,int>> item;
private:

};

#endif // ITEMSET_H
