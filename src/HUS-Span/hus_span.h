#ifndef HUS_SPAN_H
#define HUS_SPAN_H
#include <vector>
#include <map>
#include "data_struct/Sequence.h"
#include "data_struct/SequenceInfo.h"
#include "data_struct/Item_pos_info.h"
#include <unordered_set>
#include <unordered_map>
#include <set>
// 函式宣告
int threshold=0;
vector<Sequence> db;
vector<int> seq_util;
unordered_map<int,int> ex_util_map;
unordered_map<int,unordered_map<int,unordered_map<int,ItemPosInfo>>> item_pos_map; //item, <sequence_id, itemset_id>
vector<SequenceInfo> one_seq_info;
void set_threshold(int th){
    threshold=th;
}

void hus_span(SequenceInfo seq_info);
void build_1_seq_uc();
void build_1_seq_uc(){
    for(int seq_index=0;seq_index<db.size();seq_index++){
        int seq_utility = seq_util[seq_index];
        if(seq_utility<threshold) continue;
        Sequence seq = db[seq_index];
        vector<Itemset> transactions = seq.tid;
        for(int tid=0;tid<transactions.size();tid++){
            Itemset itemset = transactions[tid];
            for(int item_index=0;item_index<itemset.item.size();item_index++){

                pair<int,int> item = itemset.item[item_index];
                int item_utility = item.second * ex_util_map[item.first];
                seq_utility-=item_utility;
                SequenceInfo* item_seq_info=&one_seq_info[item.first-1];
                item_seq_info->seq_name=to_string(item.first);
                if(item_seq_info->uc.count(seq_index)==0){
                    UtilityChain uc=UtilityChain({seq_index,0,0,{}});//sequence_id,PEU_s,u,chain
                    item_seq_info->uc.insert({seq_index,uc});
                }

                ChainData cd=ChainData({tid,item_utility,seq_utility});
                //cout<<cd.tid<<", "<<cd.acu<<", "<<cd.ru<<endl;
                item_seq_info->uc[seq_index].chain.insert({tid,cd});
                item_seq_info->update_PEU_t(cd);
                item_seq_info->uc[seq_index].update_PEU_s(cd);
                int u = item_seq_info->uc[seq_index].u;

                if(item_seq_info->uc[seq_index].update_u(cd.acu)){
                    item_seq_info->add_U_t(cd.acu-u);
                }
            }
        }
    }
}
void RSU_pruning(unordered_set<int>* list,unordered_map<int,vector<pair<int,int>>> list_instance_pos){
    vector<int> removed;
    for(auto item:*list){
        int rsu_sum=0;
        unordered_map<int,int> max_rsu;
        for(auto it=list_instance_pos[item].begin();it!=list_instance_pos[item].end();++it){
            ChainData *cd =&one_seq_info[item-1].uc[it->first].chain[it->second]; //曾經最大最訝異BUG item-1填成last_item-1
            if(cd->acu+cd->ru>max_rsu[it->first] && cd->ru>0){
                max_rsu[it->first]=cd->acu+cd->ru;
            }
        }
                
        for(auto it=max_rsu.begin();it!=max_rsu.end();++it){
            rsu_sum+=it->second;
        }
        
        if(rsu_sum<threshold) removed.push_back(item);
    }

    for(auto item:removed){
        list->erase(item);
    }
    return;
}
pair<unordered_set<int>,unordered_map<int,vector<pair<int,int>>>> generate_ilist(SequenceInfo seq_info){
    /*if(!seq_info.seq_name.compare("2")){
        for(auto cc:seq_info.uc){
            cout<<cc.second.sequence_id<<","<<cc.second.PEU_s<<"L1"<<endl;
            for(auto bb:cc.second.chain){
                cout<<bb.second.acu<<","<<bb.second.ru<<","<<bb.second.tid<<"L2"<<endl;
            }
        }
    }*/
    unordered_set<int> ilist;
    unordered_map<int,vector<pair<int,int>>> ilist_instance_pos;
    char last_item=seq_info.seq_name[seq_info.seq_name.size()-1];
    for(auto it=seq_info.uc.begin();it!=seq_info.uc.end();++it){
        
        UtilityChain uc=it->second;
        
        for(auto cd_it=uc.chain.begin();cd_it!=uc.chain.end();++cd_it){
            ChainData* cd=&cd_it->second;
            int item_index=item_pos_map[last_item-'0'][uc.sequence_id][cd->tid].index;
            for(int i=item_index+1;i<db[uc.sequence_id].tid[cd->tid].item.size();i++){
                int item=db[uc.sequence_id].tid[cd->tid].item[i].first;
                ilist.insert(item);
                ilist_instance_pos[item].push_back(make_pair(uc.sequence_id,cd->tid));
            }
        }

    }

    RSU_pruning(&ilist,ilist_instance_pos);
    return pair(ilist,ilist_instance_pos);
}
struct PairHash {
    size_t operator()(const std::pair<int,int>& p) const {
        // 簡單 hash: 把兩個 int 混合
        return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
    }
};
pair<unordered_set<int>,unordered_map<int,vector<pair<int,int>>>> generate_llist(SequenceInfo seq_info){
    unordered_set<int> llist;
    unordered_map<int,vector<pair<int,int>>>llist_instance_pos;
    unordered_set<pair<int,int>,PairHash> visited;
    char last_item=seq_info.seq_name[seq_info.seq_name.size()-1];

    for(auto it=seq_info.uc.begin();it!=seq_info.uc.end();++it){
        UtilityChain uc=it->second;
        for(auto cd_it=uc.chain.begin();cd_it!=uc.chain.end();++cd_it){
            ChainData* cd=&cd_it->second;
            int tid_index=item_pos_map[last_item-'0'][uc.sequence_id][cd->tid].itemset_id;
            if(!seq_info.seq_name.compare("2")){
                /*printf("%d,%d,%d,%d\n"
                    ,last_item-'0'
                    ,uc.sequence_id
                    ,cd->tid
                    ,item_pos_map[last_item-'0'][uc.sequence_id][cd->tid].itemset_id);*/
            }
           
            for(int tid=1+tid_index;tid<db[uc.sequence_id].tid.size();tid++){
                for(int item_index=0;item_index<db[uc.sequence_id].tid[tid].item.size();item_index++){
                    if(visited.count(make_pair(uc.sequence_id,tid))!=0)continue;
                    int item=db[uc.sequence_id].tid[tid].item[item_index].first;
                    llist_instance_pos[item].push_back(make_pair(uc.sequence_id,tid));
                    llist.insert(item);
                }
            }
        }
    }
    /*for(auto vec:llist_instance_pos){
        for(auto item:vec.second){
            printf("%c,%d(%d,%d)",last_item,vec.first,item.first,item.second);
        }
        cout<<endl;
    }*/
    RSU_pruning(&llist,llist_instance_pos);
    return pair(llist,llist_instance_pos);
}
/*SequenceInfo build_seq_info(string seq_name){


}*/
set<string> s;
void hus_span(SequenceInfo seq_info){
    
    if(seq_info.PEU_t<threshold) return;
    s.insert(seq_info.seq_name);
    unordered_set<int> ilist;
    unordered_set<int> llist;
    unordered_map<int,vector<pair<int,int>>> ilist_instance_pos;
    unordered_map<int,vector<pair<int,int>>>llist_instance_pos;
    auto p1=generate_ilist(seq_info);
    auto p2=generate_llist(seq_info);
    ilist=p1.first;
    ilist_instance_pos=p1.second;
    llist=p2.first;
    llist_instance_pos=p2.second;
    //cout<<seq_info.seq_name[seq_info.seq_name.size()-1]<<":"<<endl;
    for(int item:ilist){
        SequenceInfo new_seq;
        new_seq.seq_name=seq_info.seq_name+","+to_string(item);
        for(auto pos:ilist_instance_pos[item]){
            int* seq_num=&pos.first;
            int* tid=&pos.second;
            int* index=&item_pos_map[item][*seq_num][*tid].index;
            if(!new_seq.seq_name.compare("2,4")){
                printf("(%d,%d,%d)",*seq_num,*tid,*index);
            }
            ChainData* cd=&one_seq_info[item-1].uc[*seq_num].chain[*tid];
            ChainData* new_seq_cd = &new_seq.uc[*seq_num].chain[*tid];
            ChainData* seq_cd = &seq_info.uc[*seq_num].chain[*tid];
            ChainData new_chain_data={*tid,seq_cd->acu+cd->acu,cd->ru};

            if(new_seq_cd->acu<new_chain_data.acu){
                new_seq_cd->acu=new_chain_data.acu;
                new_seq_cd->ru=new_chain_data.ru;
            }

            new_seq.update_PEU_t(new_chain_data);
            new_seq.uc[*seq_num].update_PEU_s(new_chain_data);
            int u = new_seq.uc[*seq_num].u;
            if(new_seq.uc[*seq_num].update_u(new_chain_data.acu)){
                new_seq.add_U_t(new_chain_data.acu-u);
            }
            new_seq_cd->tid=*tid;
            new_seq.uc[*seq_num].sequence_id=*seq_num;



        }
        /*for(auto uc:new_seq.uc){
            for(auto cd:uc.second.chain){
                ChainData* c_d=&cd.second;
                cout<<new_seq.seq_name;
                printf(":(seq_num:%d,tid:%d,acu:%d,ru:%d)",uc.second.sequence_id,c_d->tid,c_d->acu,c_d->ru);
            }
            cout<<endl;
        }*/
        /*if(!new_seq.seq_name.compare("2,4")){
            /*cout<<"seq:"<<new_seq.seq_name<<",";
            printf("peu:%d,ut:%d\n",new_seq.PEU_t,new_seq.U_t);
            for(auto cc:new_seq.uc){
                cout<<cc.second.sequence_id<<","<<cc.second.PEU_s<<endl;
                for(auto bb:cc.second.chain){
                    cout<<bb.second.acu<<","<<bb.second.ru<<","<<bb.second.tid<<endl;
                }
            }
        }*/
        hus_span(new_seq);
    }


    for(int item:llist){
        SequenceInfo new_seq;
        new_seq.seq_name=seq_info.seq_name+"|"+to_string(item);
        for(auto uc:seq_info.uc){
            for(auto cd:uc.second.chain){
                ChainData c_d=cd.second;
                 for(auto pos:llist_instance_pos[item]){
                    int* seq_num=&pos.first;
                    int* tid=&pos.second;
                    int* index=&item_pos_map[item][*seq_num][*tid].index;
                    if(c_d.tid>=*tid)continue;
                    
                    ChainData* cd=&one_seq_info[item-1].uc[*seq_num].chain[*tid];
                    ChainData* new_seq_cd = &new_seq.uc[*seq_num].chain[*tid];
                    ChainData* seq_cd = &seq_info.uc[*seq_num].chain[c_d.tid];
                    ChainData new_chain_data={*tid,seq_cd->acu+cd->acu,cd->ru};
                    
                    if(new_seq_cd->acu<new_chain_data.acu){
                        new_seq_cd->acu=new_chain_data.acu;
                        new_seq_cd->ru=new_chain_data.ru;
                    }

                    new_seq.update_PEU_t(new_chain_data);
                    new_seq.uc[*seq_num].update_PEU_s(new_chain_data);
                    int u = new_seq.uc[*seq_num].u;
                    if(new_seq.uc[*seq_num].update_u(new_chain_data.acu)){
                        new_seq.add_U_t(new_chain_data.acu-u);
                    }
                    new_seq_cd->tid=*tid;
                    new_seq.uc[*seq_num].sequence_id=*seq_num;
                    
                }
            }   
        }
        /*for(auto uc:new_seq.uc){
            for(auto cd:uc.second.chain){
                ChainData* c_d=&cd.second;
                cout<<new_seq.seq_name<<endl;
                if(new_seq.seq_name=="5|1"){
                    cout<<new_seq.seq_name;
                    printf(":(seq_num:%d,tid:%d,acu:%d,ru:%d)",uc.second.sequence_id,c_d->tid,c_d->acu,c_d->ru);
                }

            }
            //cout<<endl;
        }*/
        if(!new_seq.seq_name.compare("2,4|1,2|19")){
            /*cout<<"seq:"<<new_seq.seq_name<<",";
            printf("peu:%d,ut:%d\n",new_seq.PEU_t,new_seq.U_t);*/
            for(auto cc:new_seq.uc){
                cout<<cc.second.sequence_id<<","<<cc.second.PEU_s<<endl;
                for(auto bb:cc.second.chain){
                    cout<<bb.second.acu<<","<<bb.second.ru<<endl;
                    cout<<endl<<bb.second.tid;
                }
            }
        }
        hus_span(new_seq);
    }
    /*for(auto item=ilist.begin();item!=ilist.end();++item){
        cout<<*item<<" ";
    }
    cout<<endl;
    for(auto item=llist.begin();item!=llist.end();++item){
        cout<<*item<<" ";
    }
    cout<<endl;*/


}


#endif // HUS_SPAN_H