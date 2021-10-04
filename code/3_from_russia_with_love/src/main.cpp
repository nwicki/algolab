#include <iostream>
#include <vector>

using namespace std;

const int MAX = (1L << 31) - 1;

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

const int MIN = 1 << 31;
vector<vector<int>> memo_60;

int pick_a_coin_60(int i, int j, int step) {
    int mem = memo_60[i][j];
    if(mem != -1) {
        return mem;
    }
    if(i == j) {
        mem = coins[i];
    }
    else if(j - i <= step) {
        mem = max(coins[i],coins[j]);
    }
    else {
        int result_left = MAX;
        int end = j + 1;
        for(int ii = i+1, jj = j - step; jj < end; ii++,jj++) {
            result_left = min(result_left, pick_a_coin_60(ii, jj, step));
        }
        result_left += coins[i];
        int result_right = MAX;
        end = j;
        for(int ii = i, jj = j - step - 1; jj < end; ii++,jj++) {
            result_right = min(result_right, pick_a_coin_60(ii, jj, step));
        }
        result_right += coins[j];
        mem = max(result_left,result_right);
    }
    memo_60[i][j] = mem;
    return mem;
}

void compute_60(int n, int m, int k) {
    memo_60 = vector<vector<int>>(n,vector<int>(n,-1));
    int result = MAX;
    int step = m - 1;
    for(int i = 0,j = n - 1 - k; j < n; i++,j++) {
        result = min(result, pick_a_coin_60(i, j, step));
    }
    cout << result << endl;
}

void testcase() {
  int n, m, k; cin >> n >> m >> k;
  coins = vector<int>(n);
  for(int& e : coins) {
    cin >> e;
  }
  compute_60(n,m,k);
}

int main(int argc, char** argv) {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for(int i = 0; i < t; i++) {
    testcase();
  }
}