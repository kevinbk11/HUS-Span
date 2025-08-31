/* --- ItemSet.cpp --- */

/* ------------------------------------------
author: LoveNyabi
date: 2025/8/27
------------------------------------------ */

#include "Itemset.h"
#include <iostream>
Itemset::Itemset() {
    // Constructor
}

Itemset::~Itemset() {
    // Destructor
}
void Itemset::print_item() {
    for(int i=0;i<item.size();i++) {
        cout << "(" << item[i].first << "," << item[i].second << ") ";
    }
}
