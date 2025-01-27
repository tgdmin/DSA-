#include <iostream>
using namespace std;

const int MOD = 1000003;

// Function to multiply two 2x2 matrices modulo MOD
void multiplyMatrix(long long A[2][2], long long B[2][2]) {
    long long a = (A[0][0] * B[0][0] + A[0][1] * B[1][0]) % MOD;
    long long b = (A[0][0] * B[0][1] + A[0][1] * B[1][1]) % MOD;
    long long c = (A[1][0] * B[0][0] + A[1][1] * B[1][0]) % MOD;
    long long d = (A[1][0] * B[0][1] + A[1][1] * B[1][1]) % MOD;

    A[0][0] = a;
    A[0][1] = b;
    A[1][0] = c;
    A[1][1] = d;
}

// Function to raise a 2x2 matrix to the power `p` using binary exponentiation
void matrixExponentiation(long long mat[2][2], long long p) {
    if (p <= 1)
        return;

    long long base[2][2] = {{1, 1}, {1, 0}};

    matrixExponentiation(mat, p / 2);
    multiplyMatrix(mat, mat);

    if (p % 2 != 0)
        multiplyMatrix(mat, base);
}

// Function to calculate the nth Fibonacci number modulo MOD
int calculateFibonacci(long long n) {
    if (n == 1) return 0; // First Fibonacci number is 0
    if (n == 2) return 1; // Second Fibonacci number is 1

    long long fibMatrix[2][2] = {{1, 1}, {1, 0}};
    matrixExponentiation(fibMatrix, n - 2); // Compute the (n-2)th power

    return fibMatrix[0][0]; // Return the top-left element as F(n)
}

int main() {
    long long n;
    cin >> n;
    cout << calculateFibonacci(n) << endl;
    return 0;
}
