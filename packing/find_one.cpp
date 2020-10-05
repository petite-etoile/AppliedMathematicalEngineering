#include <iostream>
#include <vector>
#include <bitset>
#include <cassert>
#include <algorithm>
using namespace std;
#define debug(x) cerr << #x << ": " << x << "\n";

int height = 10;
int width = 6;
const int grid_size = 60;

struct Mino{
    int width, height;
    bitset<grid_size> bits;
    Mino(vector<string> S){
        height = S.size();
        width = S[0].size();
        for(int i=0; i<height; i++){
            assert(width == S[i].size());
            for(int j=0; j<width; j++){
                bits
            }
        }
    }

    //この構造体が持つbitsetを書き換える(回転)
    void rotate(){
        bitset<grid_size> res;

        /*
        回転
        */

        swap(width, height);
        bits = res;
    }

    void reverse(){
        bitset<grid_size> res;

        /*
        反転
        */

        bits = res;
    }

    bitset<grid_size> get_bits(){
        return bits;
    }

};




bitset<grid_size> grid,mask;
vector<Mino> minos;

void set_grid(int h_, int w_){
    height = h_;
    width = w_;
    if(height < width) swap(height,width);
    assert(height*width == grid_size);
}


void rec(int depth, int& cnt, int now_h, vector<int> const& indices){
    debug(depth);
    debug(minos.size());
    int idx = indices[depth];
    if(depth == minos.size()){
        /*敷き詰め完了! grid == 1111...1111になってるはず*/
        cout << "敷き詰め完了" << endl;
        cout << grid << endl;
        return;
    }

    while((grid & mask >> ((height - (now_h + 1)) * width)) ==  (mask >> ((height - (now_h + 1)) * width))){
        now_h++;
    }

    for(int i = 0; i < 8; i++){
        bitset<grid_size> bits = minos[idx].get_bits() << (width * now_h);
        if(now_h + minos[idx].height > height){  /*はみだす*/
            continue;
        }
        
        
        for(int w=0; w<width; w++){
            if((grid & (bits >> w)).any()) continue;
            rec(depth + 1, cnt, now_h, indices);
            break;
        }
        if(i==3) minos[idx].reverse();
        minos[idx].rotate();
    }

    return;

}


int main(){
    set_grid(6,10);
    mask.set(); //すべてのbitをたてる.


    mask.reset(0);
    cout << mask << endl;

    vector<int> indices(minos.size());
    for(int i=0; i<minos.size(); i++) indices[i] = i;

    int cnt = 0;
    do{
        rec(0,cnt,0,indices);
    }while(next_permutation(indices.begin(), indices.end()));
    return 0;
}
//aaaaaaaaaaa