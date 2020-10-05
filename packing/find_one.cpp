#include <iostream>
#include <vector>
#include <bitset>
#include <cassert>
#include <algorithm>

using namespace std;
#define debug(x) cerr << #x << ": " << x << "\n";

template<typename T>
ostream& operator<< (ostream& os, vector<T> v){
    for(auto a:v){
        os << a;
        os << " ";
    }
    os << endl;
    return os;
}

template<typename T>
bool exist_in(vector<T> V, T a){
    for(auto v:V) if(a==v) return true;
    return false;
}


int height = 10;
int width = 6;
const int grid_size = 60;

struct Mino{
    int width, height;
    bitset<grid_size> bits;
    vector<bitset<grid_size>> bits_pattern;
    Mino(vector<string> S){
        height = S.size();
        width = S[0].size();
        for(int i=0; i<height; i++){
            assert(width == S[i].size());
            for(int j=0; j<width; j++){
                if(S[i][j]=='1') bits.set(i*width + j);
            }
        }

        for(int i=0; i<8; i++){
            debug(bits)
            if(not exist_in(bits_pattern, bits))
                bits_pattern.emplace_back(bits);
            rotate();
            if(i == 3) reverse();
        }
    }

private:
    //この構造体が持つbitsetを書き換える(回転)
    void rotate(){
        bitset<grid_size> res;

        /* 回転 */
        int h,w;
        for(int i=0; i<height; i++){
            for(int j=0; j<width; j++){
                h = width - j - 1;
                w = i;
                debug(i*width + j)
                if(bits[i*width + j]) res.set(h * height + w);
            }
        }

        swap(width, height);
        bits = res;
    }

    void reverse(){
        bitset<grid_size> res;

        /*　反転　*/
        int h,w;
        for(int i=0; i<height; i++){
            for(int j=0; j<width; j++){
                h = i;
                w = width - j - 1;
                if(bits[i*width + j]) res.set(h * height + w);
            }
        }

        bits = res;
    }

public:
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
        cout << grid << endl << endl;
        return;
    }

    while((grid & mask >> ((height - (now_h + 1)) * width)) ==  (mask >> ((height - (now_h + 1)) * width))){
        now_h++;
    }

    debug(minos[idx].bits_pattern)
    for(bitset<grid_size> bits:minos[idx].bits_pattern){
        bits <<= (width * now_h);
        if(now_h + minos[idx].height > height){  /*はみだす*/
            continue;
        }
        

        //ミノ回転反転など
        for(int w=0; w<width; w++){
            if((grid & (bits >> w)).any()) continue; //重なる
            rec(depth + 1, cnt, now_h, indices);
            break;
        }
    }

    return;

}

void mino_init(){
    vector<string> F = {
        "110", "011", "010"
    };
    minos.emplace_back(F);
}

int main(){
    set_grid(6,10);
    mask.set(); //すべてのbitをたてる.


    // mask.reset(0);
    // cout << mask << endl;

    mino_init();


    vector<int> indices(minos.size());
    for(int i=0; i<minos.size(); i++) indices[i] = i;

    int cnt = 0;
    do{
        debug(indices);
        rec(0,cnt,0,indices);
    }while(next_permutation(indices.begin(), indices.end()));
    return 0;
}
//aaaaaaaaaaa