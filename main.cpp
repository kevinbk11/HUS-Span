#include <iostream>
#include <vector>
#include <map>
#include "read_file/read_db.h"
#include "data_struct/Sequence.h"
#include "data_struct/SequenceInfo.h"
#include "HUS-Span/hus_span.h"
using namespace std;
//1721
//db[0].tid[0]
void db_print(){
    for(auto it=db.begin();it!=db.end();++it){
        it->print_seq();
    }
    for(auto su:seq_util){
        cout<<su<<endl;
    }
    cout<<endl;
}
void info_print(vector<SequenceInfo> seq_info){
        for(int i=0;i<seq_info.size();i++){
        cout<<"Item: "<<seq_info[i].seq_name<<", PEU_t: "<<seq_info[i].PEU_t<<", U_t: "<<seq_info[i].U_t<<endl;
        for(auto it=seq_info[i].uc.begin();it!=seq_info[i].uc.end();++it){
            cout<<"Sequence id: "<<it->first<<", PEU_s: "<<it->second.PEU_s<<", u: "<<it->second.u<<endl;
            for(auto cd_it=it->second.chain.begin();cd_it!=it->second.chain.end();++cd_it){
                cout<<"    tid: "<<cd_it->second.tid<<", acu: "<<cd_it->second.acu<<", ru: "<<cd_it->second.ru<<endl;
            }
        }
    }
}
int main() {
    read_database("src/jzwpaper_db.txt",db,seq_util,item_pos_map);
    read_ex_util("src/jzwpaper_utb.txt",ex_util_map);
    one_seq_info=vector<SequenceInfo>(db.size());
    set_threshold(0);                           
    db_print();
    build_1_seq_uc();
    
    for(int i=0;i<one_seq_info.size();i++){
        hus_span(one_seq_info[i]);
    }
    
}