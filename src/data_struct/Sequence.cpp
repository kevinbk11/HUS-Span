/* --- Sequence.cpp --- */

/* ------------------------------------------
author: LoveNyabi
date: 2025/8/27
------------------------------------------ */

#include "Sequence.h"
#include <iostream>
using namespace std;
Sequence::Sequence() {
    // Constructor
}

Sequence::~Sequence() {
    // Destructor
}
void Sequence::print_seq() {
    for (auto it = tid.begin(); it != tid.end(); ++it) {
        cout<<"[";
        it->print_item();
        cout<<"]";
    }
    cout<<endl;
}
