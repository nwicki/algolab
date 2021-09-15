#include <iostream>
#include <vector>

using namespace std;

int get_sum(int i1, int i2, int j1, int j2, const vector<vector<int>>& matrix) {
    int sum = 0;
    for(int i = i1; i <= i2; i++) {
        for(int j = j1; j <= j2; j++) {
            sum += matrix[i][j];
        }
    }
    return sum;
}

void brute_force_even_matrices(const vector<vector<int>>& matrix) {
    int n = matrix.size();
    int count = 0;
    for(int i1 = 0; i1 < n; i1++) {
        for(int i2 = i1; i2 < n; i2++) {
            for(int j1 = 0; j1 < n; j1++) {
                for(int j2 = j1; j2 < n; j2++) {
                    count += get_sum(i1,i2,j1,j2,matrix) % 2 == 0;
                }
            }
        }
    }
    cout << count << endl;
}

void reuse_even_matrices_1(const vector<vector<int>>& matrix) {
    int n = (int) matrix.size();
    int count = 0;
    for(int i1 = 0; i1 < n; i1++) {
        for(int i2 = i1; i2 < n; i2++) {
            for(int j1 = 0; j1 < n; j1++) {
                int sum = 0;
                for(int i = i1; i <= i2; i++) {
                    sum += matrix[i][j1];
                }
                count += sum % 2 == 0;
                for(int j2 = j1+1; j2 < n; j2++) {
                    for(int i = i1; i <= i2; i++) {
                        sum += matrix[i][j2];
                    }
                    count += sum % 2 == 0;
                }
            }
        }
    }
    cout << count << endl;
}

int compute_sums(int row, int col, const vector<vector<int>>& matrix) {
    int n = (int) matrix.size();
    vector<vector<int>> row_sums(n);
    for(auto& v : row_sums) {
        v = vector<int>(n,0);
    }
    int sum;
    int count = 0;
    for(int i = row; i < n; i++) {
        sum = matrix[i][col];
        count += sum % 2 == 0 && row == i;
        row_sums[i][col] = sum;
        for (int j = col + 1; j < n; j++) {
            sum = row_sums[i][j - 1] + matrix[i][j];
            count += sum % 2 == 0 && row == i;
            row_sums[i][j] = sum;
        }
    }
    for(int i = row+1; i < n; i++) {
        for(int j = col; j < n; j++) {
            sum = row_sums[i][j] + row_sums[i-1][j];
            count += sum % 2 == 0;
            row_sums[i][j] = sum;
        }
    }
//    for(auto v : row_sums) {
//        for(int i : v) {
//            cout << i << " ";
//        }
//        cout << endl;
//    }
//    cout << endl;

    return count;
}

void reuse_even_matrices_2(const vector<vector<int>>& matrix) {
    int n = (int) matrix.size();
    int count = 0;
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            count += compute_sums(i,j,matrix);
        }
    }
    cout << count << endl;
}

void testcase() {
    int n; cin >> n;
    vector<vector<int>> matrix(n);
    for(int i = 0; i < n; i++) {
        matrix[i] = vector<int>(n);
        for(int j = 0; j < n; j++) {
            int xij; cin >> xij;
            matrix[i][j] = xij;
        }
    }
    reuse_even_matrices_2(matrix);
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