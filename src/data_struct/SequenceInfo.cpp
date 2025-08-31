/* --- SequenceInfo.cpp --- */

/* ------------------------------------------
author: LoveNyabi
date: 2025/8/28
------------------------------------------ */

#include "SequenceInfo.h"
#include <iostream>
using namespace std;
SequenceInfo::SequenceInfo() {
}

SequenceInfo::~SequenceInfo() {
    // Destructor
}
void SequenceInfo::add_PEU_t(int peu) {
    PEU_t+=peu;
}
void SequenceInfo::add_U_t(int u) {
    U_t+=u;
}