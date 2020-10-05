#include <iostream>
#include <vector>
#include <bitset>

using namespace std;

const int h = 10;
const int w = 6;
const int grid_size = h*w;

bitset<grid_size> grid;

vector<bitset<grid_size>> mino = {
    ("110" + "0"*w + "011" + "0"*w + "010"), //F
    ("")
};

int main(){
    return 0;
}
//aaaaaaaaaaa