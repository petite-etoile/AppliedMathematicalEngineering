#include <iostream>
#include <vector>
#include <string>
using namespace std;
int main(){
    string three = "33";
    cout << "\033["+three+"mAAA\033[m" << endl;
}