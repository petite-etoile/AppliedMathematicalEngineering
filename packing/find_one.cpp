#include <iostream>
#include <vector>
#include <bitset>
#include <cassert>
#include <algorithm>
#include <utility>
#include <tuple>

using namespace std;
#define debug(x) cerr << #x << ": " << x << "\n";

template<typename T>
ostream& operator<< (ostream& os, vector<T> v){
    for(auto a:v){
        os << a;
        os << ",";
    }
    os << endl;
    return os;
}

template<typename T,typename S>
ostream& operator<< (ostream& os, pair<T,S> const& P){
    os << "(";
    os << P.first;
    os << ",";
    os << P.second;
    os << ")";
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
private:
    int __width__, __height__;
public:
    bitset<grid_size> bits;
    vector<bitset<grid_size>> bits_pattern;
    vector<int> width, height;

    Mino(vector<string> S){
        build(S); // Sからbitsetを
        for(int i=0; i<8; i++){
            if(not exist_in(bits_pattern, bits)){
                bits_pattern.emplace_back(bits);
                width.emplace_back(__width__);
                height.emplace_back(__height__);
            }

            if(i == 3) reverse(S);
            else rotate(S);
        }
        assert(width.size() == bits_pattern.size());
    }

private:

    //文字列のvector,Sからbit列を構成する
    void build(vector<string> const& S){
        __height__ = S.size();
        __width__ = S[0].size();
        bits.reset();
        if(__width__ > 40) {
            for(auto s:S[0]) cout << s << ".";
            exit(0);
        }
        for(int i=0; i<__height__; i++){
            assert(__width__ == S[i].size());
            for(int j=0; j<__width__; j++){
                if(S[i][j]=='1') bits.set(i*WIDTH + j);
            }
        }
    }


    //この構造体が持つbitsetを書き換える(回転)
    void rotate(vector<string>& S){
        vector<string> newS(__width__, string(__height__,'.'));
        /* 回転 */
        for(int i=0; i<__height__; i++){
            for(int j=0; j<__width__; j++){
                newS[__width__-j-1][i] = S[i][j];
            }
        }

        build(newS);
        swap(S, newS);
    }

    void reverse(vector<string>& S){
        vector<string> newS(__width__, string(__height__,'.'));

        /*　反転　*/
        for(int i=0; i<__height__; i++){
            for(int j=0; j<__width__; j++){
                newS[j][i] = S[i][j];
            }
        }

        build(newS);
        swap(S, newS);
    }


};




vector<Mino> minos;
// vector<pair<int,int>> positions; //現在そのピースがgridの(上から何番目、左から何番目)か
vector<bitset<grid_size>> positions; //現在そのピースが位置してる場所を示すbitset

//gridの縦横のサイズを設定
void set_grid(int h_, int w_){
    HEIGHT = h_;
    WIDTH = w_;
    if(HEIGHT < WIDTH) swap(HEIGHT,WIDTH);
    assert(HEIGHT*WIDTH == grid_size);
}

char hex_chr(int x){
    if(x < 10) return '0' + x;
    else return 'a' + x;
}

//gridにハメられてるピースの状況を描画
void screen_grid(int const& depth, vector<int> const& indices){
    vector<string> res(HEIGHT, string(WIDTH, '#')); 
    
    for(int i=0; i<depth; i++){
        char chr = hex_chr(indices[i]);
        bitset<grid_size> bits = positions[indices[i]];
        for(int j=0; j<grid_size; j++){
            int h = j/WIDTH;
            int w = j%WIDTH;
            if(bits[j]){
                assert(res[h][w] == '#');
                res[h][w] = chr;
            }
        }
    }

    cout << "盤面の様子" << endl;
    for(auto row:res) cout << row << endl;
    cout << endl;
}

void rec(int depth, int& cnt, int now_h, int now_w, vector<int> const& indices, bitset<grid_size> const& grid, bitset<grid_size> mask){
    if(depth >= 10) debug(depth)
    if(depth >= 5) screen_grid(depth, indices);
    screen_grid(depth, indices);

    if(depth == minos.size()){
        /*敷き詰め完了! grid == 1111...1111になってるはず*/
        cout << "敷き詰め完了" << endl;
        cout << grid << endl << endl;
        return;
    }
    int idx = indices[depth];

    while(true){
        if( (grid & mask) == mask ){
            now_w++;
            if(now_w >= WIDTH){
                now_w = 0;
                now_h++;
            }
            mask<<=1;
            mask.set(0);
        }else break;
    }

    Mino mino = minos[idx];
    for(int i=0; i<mino.bits_pattern.size(); i++){
        bitset<grid_size> bits = mino.bits_pattern[i];
        int mino_width = mino.width[i];
        int mino_height = mino.height[i];
        bits <<= (WIDTH * now_h + now_w);
        // debug(make_pair(now_h, now_w));
        // debug(make_pair(mino_height, mino_width));
        // debug(now_w + mino_width)
        // debug(WIDTH)
        if((grid & bits).any()) continue; //重なる
        if(now_h + mino_height > HEIGHT) continue; /*はみだす*/
        if(now_w + mino_width  > WIDTH) break; /*はみ出す*/

        bitset<grid_size> new_grid = grid | bits;
        positions[idx] = bits;

        rec(depth + 1, cnt, now_h, now_w, indices, new_grid, mask);
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
        "010", "111", "010"
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

    bitset<grid_size> mask;
    mask.set(0);
    mino_init();
    positions.assign(minos.size(), 0);

    for(auto& mino:minos){
        debug(mino.bits_pattern.size())
    }

    bitset<grid_size> grid;

    int cnt = 0;
    // vector<int> indices(minos.size());
    // for(int i=0; i<minos.size(); i++) indices[i] = i;

    // do{
    //     rec(0,cnt,0,0,indices,grid,mask);
    // }while(next_permutation(indices.begin(), indices.end()));

    vector<int> indices = {7,6,2,11,3,10,0,9,8,4,5,1};
    rec(0, cnt, 0, 0, indices, grid, mask);
    return 0;
}
//aaaaaaaaaaa