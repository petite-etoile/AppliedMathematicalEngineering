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


int HEIGHT = 10;
int WIDTH = 6;
const int grid_size = 60;

struct Mino{
    int width, height;
    bitset<grid_size> bits;
    vector<bitset<grid_size>> bits_pattern;
    Mino(vector<string> S){
        height = S.size();
        width = S[0].size();
        bits.reset();

        for(int i=0; i<height; i++){
            assert(width == S[i].size());
            for(int j=0; j<width; j++){
                if(S[i][j]=='1') bits.set(i*WIDTH + j);
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

    //文字列のvector,Sからbit列を構成する
    void build(vector<string> const& S){
        height = S.size();
        width = S[0].size();
        bits.reset();

        for(int i=0; i<height; i++){
            assert(width == S[i].size());
            for(int j=0; j<width; j++){
                if(S[i][j]=='1') bits.set(i*WIDTH + j);
            }
        }
    }


    //この構造体が持つbitsetを書き換える(回転)
    void rotate(){
        bitset<grid_size> res;

        /* 回転 */
        int h,w;
        for(int i=0; i<HEIGHT; i++){
            for(int j=0; j<WIDTH; j++){
                h = WIDTH - j - 1;
                w = i;
                if(bits[i*WIDTH + j]) res.set(h * HEIGHT + w);
            }
        }
        assert(bits.count() == res.count());
        swap(width, height);
        bits = res;
    }

    void reverse(){
        bitset<grid_size> res;

        /*　反転　*/
        int h,w;
        for(int i=0; i<HEIGHT; i++){
            for(int j=0; j<width; j++){
                h = i;
                w = width - j - 1;
                if(bits[i*width + j]) res.set(h * width + w);
            }
        }

        bits = res;
    }


};




bitset<grid_size> mask;
vector<Mino> minos;

void set_grid(int h_, int w_){
    HEIGHT = h_;
    WIDTH = w_;
    if(HEIGHT < WIDTH) swap(HEIGHT,WIDTH);
    assert(HEIGHT*WIDTH == grid_size);
}


void rec(int depth, int& cnt, int now_h, vector<int> const& indices, bitset<grid_size> const& grid){
    // debug(depth);
    if(depth == minos.size()){
        /*敷き詰め完了! grid == 1111...1111になってるはず*/
        cout << "敷き詰め完了" << endl;
        cout << grid << endl << endl;
        return;
    }
    int idx = indices[depth];

    while((grid & mask >> ((HEIGHT - (now_h + 1)) * WIDTH)) ==  (mask >> ((HEIGHT - (now_h + 1)) * WIDTH))){
        now_h++;
    }

    for(bitset<grid_size> bits:minos[idx].bits_pattern){
        bits <<= (WIDTH * now_h);
        if(now_h + minos[idx].height > WIDTH){  /*はみだす*/
            continue;
        }
         
        //ミノ回転反転など
        for(int w=0; w<WIDTH; w++){
            if((grid & (bits << w)).any()) continue; //重なる
            bitset<grid_size> new_grid = grid | (bits << w);
            rec(depth + 1, cnt, now_h, indices, new_grid);
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

    vector<string> I = {
        "1", "1", "1", "1", "1"
    };
    minos.emplace_back(I);

    vector<string> L = {
        "10", "10", "10", "11"
    };
    minos.emplace_back(L);

    vector<string> N = {
        "01", "11", "10", "10"
    };
    minos.emplace_back(N);

    vector<string> P = {
        "11", "11", "10"
    };
    minos.emplace_back(P);

    vector<string> T = {
        "111", "010", "010"
    };
    minos.emplace_back(T);

    vector<string> U = {
        "101", "111"
    };
    minos.emplace_back(U);

    vector<string> V = {
        "100", "100", "111"
    };
    minos.emplace_back(V);

    vector<string> W = {
        "100", "110", "011"
    };
    minos.emplace_back(W);

    vector<string> X = {
        "100", "100", "111"
    };
    minos.emplace_back(X);

    vector<string> Y = {
        "01", "11", "01", "01"
    };
    minos.emplace_back(Y);

    vector<string> Z = {
        "110", "010", "011"
    };
    minos.emplace_back(Z);
    
}

int main(){
    set_grid(6,10);
    mask.set(); //すべてのbitをたてる.


    // mask.reset(0);
    // cout << mask << endl;

    mino_init();

    for(auto& mino:minos){
        debug(mino.bits_pattern.size())
    }

    bitset<grid_size> grid;

    vector<int> indices(minos.size());
    for(int i=0; i<minos.size(); i++) indices[i] = i;

    int cnt = 0;
    do{
        rec(0,cnt,0,indices,grid);
    }while(next_permutation(indices.begin(), indices.end()));
    return 0;
}
//aaaaaaaaaaa