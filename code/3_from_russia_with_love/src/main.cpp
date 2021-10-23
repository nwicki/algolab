#include <iostream>
#include <vector>

using namespace std;

const int MAX = (1L << 31) - 1;

vector<int> coins;

vector<vector<vector<int>>> memo_20;

int pick_a_coin_20(int i, int j, int p, int k, int m) {
  if(p == m) {
    p = 0;
  }
  int mem = memo_20[i][j][p];
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
    int left = pick_a_coin_20(i + 1, j, p + 1, k, m);
    int right = pick_a_coin_20(i, j - 1, p + 1, k, m);
    if(p == k) {
      mem = max(coins[i] + left, coins[j] + right);
    }
    else {
      mem = min(left,right);
    }
  }
    memo_20[i][j][p] = mem;
  return mem;
}

void compute_20(int n, int m, int k) {
    memo_20 = vector<vector<vector<int>>>(n, vector<vector<int>>(n, vector<int>(m, -1)));
  cout << pick_a_coin_20(0, n - 1, 0, k, m) << endl;
}

vector<vector<int>> memo_100;

int pick_a_coin_100(int i, int j, int step) {
    int mem = memo_100[i][j];
    if(mem != -1) {
        return mem;
    }
    if(j - i <= step) {
        mem = max(coins[i],coins[j]);
    }
    else {
        int result_left = MAX;
        int end = j + 1;
        for(int ii = i+1, jj = j - step; jj < end; ii++,jj++) {
            result_left = min(result_left, pick_a_coin_100(ii, jj, step));
        }
        result_left += coins[i];
        int result_right = MAX;
        end = j;
        for(int ii = i, jj = j - step - 1; jj < end; ii++,jj++) {
            result_right = min(result_right, pick_a_coin_100(ii, jj, step));
        }
        result_right += coins[j];
        mem = max(result_left,result_right);
    }
    memo_100[i][j] = mem;
    return mem;
}

void compute_100(int n, int m, int k) {
    memo_100 = vector<vector<int>>(n, vector<int>(n, -1));
    int result = MAX;
    int step = m - 1;
    for(int i = 0,j = n - 1 - k; j < n; i++,j++) {
        result = min(result, pick_a_coin_100(i, j, step));
    }
    cout << result << endl;
}

void testcase() {
  int n, m, k; cin >> n >> m >> k;
  coins = vector<int>(n);
  for(int& e : coins) {
    cin >> e;
  }
    compute_100(n, m, k);
}

int main(int argc, char** argv) {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for(int i = 0; i < t; i++) {
    testcase();
  }
}