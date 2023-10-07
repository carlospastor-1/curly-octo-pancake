#include <iostream>
#include <string>
#include "helper.h"

using namespace std;

int main(){
    WordleHelper wh;
    wh.addGreen("_____");
    wh.addYellow("_____");
    wh.addGray("a");
    vector<string> result = wh.possibleSolutions();
    for (string& word : result) {
        cout<<word<<endl;
    }

    return 0;

}