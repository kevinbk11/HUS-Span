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
void SequenceInfo::update_PEU_t(ChainData cd) {
    if(PEU_t<cd.acu+cd.ru && cd.ru>0){
        PEU_t=cd.acu+cd.ru;
    }
}
void SequenceInfo::add_U_t(int u) {
    U_t+=u;
}