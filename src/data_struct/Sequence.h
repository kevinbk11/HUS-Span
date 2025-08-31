/* --- Sequence.h --- */

/* ------------------------------------------
Author: LoveNyabi
Date: 2025/8/27
------------------------------------------ */

#ifndef SEQUENCE_H
#define SEQUENCE_H
#include "Itemset.h"
#include <vector>
class Sequence {
public:
    Sequence();
    ~Sequence();
    void print_seq();
    vector<Itemset> tid;
private:

};

#endif // SEQUENCE_H
