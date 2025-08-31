/* --- SequenceInfo.h --- */

/* ------------------------------------------
Author: LoveNyabi
Date: 2025/8/28
------------------------------------------ */

#ifndef SEQUENCEINFO_H
#define SEQUENCEINFO_H
#include <string>
#include <vector>
#include<unordered_map>
struct ChainData{
    int tid;
    int acu;
    int ru;
};
struct UtilityChain{
    int sequence_id;
    int PEU_s;
    int u=0;
    std::unordered_map<int,ChainData> chain;
    void update_PEU_s(ChainData cd){
        if(PEU_s<cd.acu+cd.ru && cd.ru>0){
            PEU_s=cd.acu+cd.ru;
        }
    }
    bool update_u(int u){
        if(this->u<u){
            this->u=u;
            return true;
        }
        return false;
    }
};
class SequenceInfo {
public:
    SequenceInfo();
    ~SequenceInfo();
    void update_PEU_t(ChainData cd);
    void add_U_t(int u);
    std::string seq_name;
    int PEU_t=0;
    int U_t=0;
    std::unordered_map<int,UtilityChain> uc;
private:

};

#endif // SEQUENCEINFO_H
