#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
class Planet{
public:
    int boss;
    int base_gen=0;
    int gen;
    int cur;
    void init(int b, int c, int g){
        boss=b;
        gen =g;
        if(gen>=base_gen)base_gen=gen;
        cur =c;
    }
};
 
class Troop{
public:
    int boss;
    int from;
    int to;
    int cur;
    int rem;
    void init(int b, int f, int t, int c, int r){
        boss=b;
        from=f;
        to  =t;
        cur =c;
        rem =r;
    }
};
 
int main()
{
    int factoryCount; // the number of factories
    cin >> factoryCount; cin.ignore();
    int linkCount; // the number of links between factories
    cin >> linkCount; cin.ignore();
    int ds[factoryCount][factoryCount];
    Planet pl[factoryCount];
    for(int i=0;i<factoryCount;i++)
        for(int j=0;j<factoryCount;j++)
        {
            ds[i][j]=0;
            ds[j][i]=0;
        }
    for (int i = 0; i < linkCount; i++) {
        int factory1;
        int factory2;
        int distance;
        cin >> factory1 >> factory2 >> distance; cin.ignore();
        ds[factory1][factory2]=distance;
        ds[factory2][factory1]=distance;
    }
    int turn=0;
    // game loop
    int cfree=5;
    while (1) {
        int entityCount; // the number of entities (e.g. factories and troops)
        
        cin >> entityCount; cin.ignore();
        int id=0;
        Troop tr[entityCount-factoryCount];
        vector<int> my;
        int td=0;
        for (int i = 0; i < entityCount; i++) {
            int entityId;
            string entityType;
            int arg1;
            int arg2;
            int arg3;
            int arg4;
            int arg5;
            cin >> entityId >> entityType >> arg1 >> arg2 >> arg3 >> arg4 >> arg5; cin.ignore();
            //cerr<< entityId << entityType << arg1 << arg2 << arg3 << arg4 << arg5<<endl;
            if(entityType=="FACTORY"){
                pl[id++].init(arg1, arg2, arg3);
                if(arg1==1) my.push_back(id-1);
            } else {
                if(entityType=="BOMB") cerr<<entityType<<" "<<arg1 << " "<<arg2<<" " << arg3 <<" "<< arg4<<" " << arg5<<endl;
                tr[td++].init(arg1, arg2, arg3, arg4, arg5);
            }
        }
        int mn=999,mt=-1,mf,snd;
        int cr,ma=0;
        if(turn==7||turn==20){
            for(auto it : my){
                for(int i=0;i<factoryCount;i++){
                    if(pl[i].boss==-1){
                        cr=pl[i].gen*8+pl[i].cur+30-ds[it][i];
                        if(cr>ma){
                            ma=cr;
                            mf=it;
                            mt=i;
                        }
                    }
                }
            }
            cout<<"BOMB "<<mf<<" "<<mt<<";";
        }
        ma=0;
        mt=-1;
        for(auto it : my){
            for(int i=0;i<factoryCount;i++){
                if(pl[i].boss!=1){
                    cr=pl[i].cur;
                    if(pl[i].boss!=0) cr+=(pl[i].gen*(ds[it][i]+1));
                    if((ds[it][i]-(pl[i].gen*2)<mn)&&(pl[it].cur>cr)&&(pl[i].base_gen>0)){
                        mn=ds[it][i]-(pl[i].gen*2);
                        mf=it;
                        mt=i;
                        snd=pl[mf].cur;
                    }
                }
            }
        }
        if(mt>=0){
            cfree=5;
            cout<<"MOVE "<<mf<<" "<<mt<<" "<<snd<<endl;
        }else{
            int ap=1;
            ma=0;
            for(int it=0; it<factoryCount; it++){
                if(pl[it].boss==-1){
                    if(pl[it].gen>0)ap=0;
                    ma=1;
                    break;
                }
            }
            if((ma==1)&&(ap==1)){
                
                mn=999;
                mt=-1;
                for(auto it : my){
                    if(pl[it].gen>0)
                        for(int i=0;i<factoryCount;i++){
                            if(pl[i].boss==-1){
                                cr=pl[i].cur+(pl[i].gen*(ds[it][i]+1));
                                if((ds[it][i]<mn)&&(pl[it].cur>cr*2)){
                                    mn=ds[it][i]-(pl[i].gen*2);
                                    mf=it;
                                    mt=i;
                                    snd=pl[it].cur/2+1;
                                }
                            }
                        }
                }
            }
            else cfree=5;
            if(mt>=0){
                cfree--;
                if(cfree==0){
                    cout<<"MOVE "<<mf<<" "<<mt<<" "<<snd<<endl;
                    cfree=5;
                }else{
                    cout << "WAIT" << endl;
                }
            }else{
                int ct=-1;
                int mks=-1,cks;
                for(auto it : my){
                    cks=0;
                    if(pl[it].base_gen<3&&pl[it].cur>=10){
                        for(int i=0;i<factoryCount;i++){
                            if(pl[i].boss==-1)cks+=ds[it][i];
                        }
                        if(cks>mks){
                            mks=cks;
                            ct=it;
                        }
                    }
                }
                if(ct>0)cout<<"INC "<<ct<<endl;
                else cout << "WAIT" << endl;
            }
        }
        turn++;
    }
}

