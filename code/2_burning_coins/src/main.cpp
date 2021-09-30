#include <iostream>
#include <vector>

using namespace std;

vector<vector<int>> memo;
vector<int> coins;
int n;

int get_max(int i, int j) {
    if(j < i) {
        return 0;
    }
    if(i == j) {
        return coins[i];
    }
    int result = memo[i][j];
    if(result != -1) {
        return result;
    }
    result = max(coins[i] + min(get_max(i+2,j), get_max(i+1,j-1)), coins[j] + min(get_max(i+1,j-1), get_max(i,j-2)));
    memo[i][j] = result;
    return result;
}

void testcase() {
    cin >> n;
    coins = vector<int>(n);
    for(int& e : coins) {
        cin >> e;
    }
    memo = vector<vector<int>>(n,vector<int>(n,-1));
    cout << get_max(0,n-1) << endl;
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    for (int i = 0; i < t; ++i) {
        testcase();
    }
    return 0;
}