#include <iostream>
#include <vector>

using namespace std;

const int MAX = (2L << 31) - 1;

vector<int> coins;
vector<vector<vector<int>>> memo;

int pick_a_coin(int i, int j, int p, int k, int m) {
  if(p == m) {
    p = 0;
  }
  int mem = memo[i][j][p];
  if(mem != -1) {
    return mem;
  }
  int coins_left = j - i;
  int persons_before_k = (k - p) % m;
  if(coins_left < persons_before_k) {
    mem = 0;
  }
  else if(i == j) {
    if(p == k) {
      mem = coins[i];
    }
    else {
     mem = 0; 
    }
  }
  else {
    int left = pick_a_coin(i+1,j,p+1,k,m);
    int right = pick_a_coin(i,j-1,p+1,k,m);
    if(p == k) {
      mem = max(coins[i] + left, coins[j] + right);
    }
    else {
      mem = min(left,right);
    }
  }
  memo[i][j][p] = mem;
  return mem;
}

void compute_20(int n, int m, int k) {
  memo = vector<vector<vector<int>>>(n,vector<vector<int>>(n,vector<int>(m,-1)));
  cout << pick_a_coin(0,n-1,0,k,m) << endl;
}

void compute_60(int n, int m, int k) {
  vector<vector<pair<int,int>>> rounds;
  cout << pick_a_coin(0,n-1,0,k,m) << endl;
}

void testcase() {
  int n, m, k; cin >> n >> m >> k;
  coins = vector<int>(n);
  for(int& e : coins) {
    cin >> e;
  }
  compute_20(n,m,k);
}

int main(int argc, char** argv) {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for(int i = 0; i < t; i++) {
    testcase();
  }
}