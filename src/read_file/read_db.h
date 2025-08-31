#ifndef READ_DB_H
#define READ_DB_H
#include <iostream>
#include <fstream>
#include <sstream>
#include "data_struct/Sequence.h"
#include "data_struct/Item_pos_info.h"
#include <regex>
#include <vector>
#include <unordered_map>
using namespace std;
// 函式宣告
void read_database(const string& filename,vector<Sequence>& db,vector <int>& seq_util,unordered_map<int,unordered_map<int,unordered_map<int,ItemPosInfo>>>& item_pos_map);

void read_ex_util(const string& filename,unordered_map<int,int>& ex_util_map){
    ifstream infile(filename);
    if (!infile.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }
    string line;
    regex re(R"((\d+):(\d+))");
    while (getline(infile, line)) {
        stringstream ss(line);
        smatch match;
        if (regex_match(line, match, re)) {
            int item= stoi(match[1]);
            int item_external_utility= stoi(match[2]);
            ex_util_map[item]=item_external_utility;
        }

    }
    infile.close();
}

void read_database(const string& filename,vector<Sequence>& db,vector<int>& seq_util,unordered_map<int,unordered_map<int,unordered_map<int,ItemPosInfo>>>& item_pos_map){
    ifstream infile(filename);
    if (!infile.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }
    string line;
    vector<string> lines;
    regex re(R"((\d+)\[(\d+)\])");
    regex re_swu(R"(SUtility:(\d+))");
    int sequence_id=0;
    while (getline(infile, line)) {
        db.push_back(Sequence());
        db.back().tid.push_back(Itemset());
        stringstream ss(line);
        string token;
        int itemset_id=0;
        int item_index=0;
        while (ss>>token)
        {
            smatch match;
            if(token.compare("-1")!=0){
                if (regex_match(token, match, re)) {
                    int item= stoi(match[1]);
                    int item_internal_utility= stoi(match[2]);
                    db.back().tid.back().item.push_back(make_pair(item,item_internal_utility));
                    item_pos_map[item][sequence_id][itemset_id]=(ItemPosInfo{sequence_id,itemset_id,item_index});
                    item_index++;
                }
            }else{
                db.back().tid.push_back(Itemset());
                itemset_id++;
                item_index=0;
            }
            if(regex_match(token, match, re_swu)) {   
                int sequence_utility = stoi(match[1]);
                seq_util.push_back(sequence_utility);
            }
        }
        db.back().tid.pop_back();
        sequence_id++;
    }
    infile.close();
}

#endif // READ_DB_H