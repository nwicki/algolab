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

int compute_sums_2(int col, const vector<vector<int>>& matrix) {
    int n = (int) matrix.size();
    int count = 0;
    vector<vector<int>> row_sums(n);
    for(auto& v : row_sums) { v = vector<int>(n,0); }
    vector<vector<int>> sums(n);
    for(auto& v : sums) { v = vector<int>(n,0); }
    int sum;
    for(int i = 0; i < n; i++) {
        row_sums[i][col] = matrix[i][col];
        for (int j = col + 1; j < n; j++) {
            row_sums[i][j] = row_sums[i][j - 1] + matrix[i][j];
        }
    }
    for(int j = col; j < n; j++) {
        sum = row_sums[0][j];
        count += sum % 2 == 0;
        sums[0][j] = sum;
    }
    for(int i = 1; i < n; i++) {
        for(int j = col; j < n; j++) {
            sum = row_sums[i][j] + sums[i-1][j];
            count += sum % 2 == 0;
            sums[i][j] = sum;
        }
    }
    for(int row = 1; row < n; row++) {
        for(int i = row; i < n; i++) {
            for(int j = col; j < n; j++) {
                sum = sums[i][j] - row_sums[row-1][j];
                count += sum % 2 == 0;
                sums[i][j] = sum;
            }
        }
    }

    return count;
}

void reuse_even_matrices_3(const vector<vector<int>>& matrix) {
    int n = (int) matrix.size();
    int count = 0;
    vector<vector<int>> row_sums(n);
    for(auto& v : row_sums) { v = vector<int>(n); }
    int sum;
    // Define sums per row
    for(int i = 0; i < n; i++) {
        row_sums[i][0] = matrix[i][0];
        for (int j = 1; j < n; j++) {
            row_sums[i][j] = row_sums[i][j - 1] + matrix[i][j];
        }
    }
    // Compute consecutive sums per row and the first column, and count even results
    auto sums = row_sums;
    for(int j = 0; j < n; j++) {
        count += sums[0][j] % 2 == 0;
    }
    for(int i = 1; i < n; i++) {
        for(int j = 0; j < n; j++) {
            sum = sums[i][j] + sums[i-1][j];
            count += sum % 2 == 0;
            sums[i][j] = sum;
        }
    }
    for(int row = 1; row < n; row++) {
        for(int i = row; i < n; i++) {
            for(int j = 0; j < n; j++) {
                sum = sums[i][j] - row_sums[row-1][j];
                count += sum % 2 == 0;
                sums[i][j] = sum;
            }
        }
    }
    // Compute consecutive sums per column
    for(int col = 1; col < n; col++) {
        sums = row_sums;
        for(int i = 0; i < n; i++) {
            for(int j = col; j < n; j++) {
                sums[i][j] -= row_sums[i][col-1];
            }
        }
        row_sums = sums;
        for(int j = col; j < n; j++) {
            count += sums[0][j] % 2 == 0;
        }
        for(int i = 1; i < n; i++) {
            for(int j = col; j < n; j++) {
                sum = sums[i][j] + sums[i-1][j];
                count += sum % 2 == 0;
                sums[i][j] = sum;
            }
        }
        for(int row = 1; row < n; row++) {
            for(int i = row; i < n; i++) {
                for(int j = col; j < n; j++) {
                    sum = sums[i][j] - sums[row-1][j];
                    count += sum % 2 == 0;
                    sums[i][j] = sum;
                }
            }
        }
    }
    cout << count << endl;
}

class even_status {
public:
    int horizontal_even;
    int horizontal_odd;
    int vertical_even;
    int vertical_odd;
    even_status() {
        horizontal_even = 0;
        horizontal_odd = 0;
        vertical_even = 0;
        vertical_odd = 0;
    }
};

// Use approach from even_pairs
void reuse_even_matrices_4(const vector<vector<int>>& matrix) {
    int n = (int) matrix.size();
    int result = 0;
    for(int start = 0; start < n; start++) {
        vector<vector<int>> sums(n,vector<int>(n));
        sums[start][start] = matrix[start][start];
        for(int j = start + 1; j < n; j++) {
            sums[start][j] = sums[start][j-1] + matrix[start][j];
        }
        for(int j = start+1; j < n; j++) {
            sums[j][start] = sums[j-1][start] + matrix[j][start];
        }
        for(int i = start+1; i < n; i++) {
            sums[i][i] = matrix[i][i] + sums[i][i-1] + sums[i-1][i] - sums[i-1][i-1];
            for(int j = i+1; j < n; j++) {
                sums[i][j] = sums[i][j-1] + matrix[i][j] + sums[i-1][j] - sums[i-1][j-1];
            }
            for(int j = i+1; j < n; j++) {
                sums[j][i] = sums[j-1][i] + matrix[j][i] + sums[j][i-1] - sums[j-1][i-1];
            }
        }
        vector<vector<even_status>> even_odds(n,vector<even_status>(n));
        for(int i = start; i < n; i++) {
            int even = 0;
            int odd = 0;
            for(int j = n-1; j > i; j--) {
                int is_even = sums[i][j] % 2 == 0;
                even += is_even;
                odd += !is_even;
                even_odds[i][j-1].horizontal_even = even;
                even_odds[i][j-1].horizontal_odd = odd;
            }
            even = 0;
            odd = 0;
            for(int j = n-1; j > i; j--) {
                int is_even = sums[j][i] % 2 == 0;
                even += is_even;
                odd += !is_even;
                even_odds[j-1][i].vertical_even = even;
                even_odds[j-1][i].vertical_odd = odd;
            }
        }
        for(int i = n-1; i > start; i--) {
            int even = 0;
            int odd = 0;
            for(int j = n-1; j > start; j--) {
                int is_even = sums[i][j] % 2 == 0;
                even += is_even;
                odd += !is_even;
                even_odds[i][j-1].horizontal_even = even;
                even_odds[i][j-1].horizontal_odd = odd;
            }
            even = 0;
            odd = 0;
            for(int j = n-1; j > start; j--) {
                int is_even = sums[j][i] % 2 == 0;
                even += is_even;
                odd += !is_even;
                even_odds[j-1][i].vertical_even = even;
                even_odds[j-1][i].vertical_odd = odd;
            }
        }
        for(int i = start; i < n; i++) {
            for(int j = start; j < n; j++) {
                if(sums[i][j] % 2 == 0) {
                    result += even_odds[i][j].horizontal_even + even_odds[i][j].vertical_even + 1;
                }
                else {
                    result += even_odds[i][j].horizontal_odd + even_odds[i][j].vertical_odd;
                }
            }
        }
    }
    cout << result << endl;
}

void testcase() {
    int n; cin >> n;
    vector<vector<int>> matrix(n);
    for(int i = 0; i < n; i++) {
        matrix[i] = vector<int>(n);
        for(int j = 0; j < n; j++) {
            cin >> matrix[i][j];
        }
    }
    reuse_even_matrices_4(matrix);
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