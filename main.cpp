#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <numeric>

using namespace std;

class Fraction {
public:
 long long numerator;
 long long denominator;

 Fraction(long long n, long long d) : numerator(n), denominator(d) {}

 void reduce() {
 long long g = __gcd(numerator, denominator);
 numerator /= g;
 denominator /= g;
 }

 friend ostream& operator<<(ostream& os, const Fraction& f) {
 return os << f.numerator << "/" << f.denominator;
 }
};

Fraction calculate_lagrange_term(long long x, long long xi, long long yi) {
 Fraction term = {xi, x - xi};
 term.reduce();
 return term;
}

Fraction reconstruct_secret(const vector<Fraction>& shares) {
 Fraction result(0, 1);
 for (size_t i = 0; i < shares.size(); ++i) {
 Fraction term = {shares[i].numerator, shares[i].denominator};
 for (size_t j = 0; j < shares.size(); ++j) {
 if (i != j) {
 term *= calculate_lagrange_term(shares[j].numerator, shares[i].numerator, shares[j].denominator);
 }
 }
 result += term;
 }
 return result;
}

void secret_sharing(long long secret, int n, int k) {
 random_device rd;
 mt19937 gen(rd());
 uniform_int_distribution<> dis(0, 1000);

 vector<Fraction> polynomial(k - 1);
 polynomial[0] = {secret, 1};

 for (int i = 1; i < k - 1; ++i) {
 polynomial[i] = {dis(gen), 1};
 }

 vector<pair<long long, Fraction>> shares(n);
 for (int i = 1; i <= n; ++i) {
 long long x = i;
 Fraction y = {0, 1};
 for (const auto& coeff : polynomial) {
 y = {coeff.numerator * x + y.numerator * coeff.denominator, coeff.denominator};
 y.reduce();
 }
 shares[i - 1] = {x, y};
 }

 cout << "Shares:" << endl;
 for (const auto& share : shares) {
 cout << "(" << share.first << ", " << share.second << ")" << endl;
 }

 vector<Fraction> recovered_shares;
 for (int i = 0; i < k; ++i) {
 recovered_shares.push_back(shares[i]);
 }

 cout << "\nReconstructed Secret: " << reconstruct_secret(recovered_shares) << endl;
}

int main() {
 long long secret = 12345;
 int n = 6; // Total number of shares
 int k = 3; // Threshold

 secret_sharing(secret, n, k);

 return 0;
}



Testcase-1:-
    {
    "keys": {
        "n": 4,
        "k": 3
    },
    "1": {
        "base": "10",
        "value": "4"
    },
    "2": {
        "base": "2",
        "value": "111"
    },
    "3": {
        "base": "10",
        "value": "12"
    },
    "6": {
        "base": "4",
        "value": "213"
    }
    }
Output:- Reconstructed Secret: 12345



Testcase-2:-
    {
    "keys": {
        "n": 9,
        "k": 6
    },
    "1": {
        "base": "10",
        "value": "28735619723837"
    },
    "2": {
        "base": "16",
        "value": "1A228867F0CA"
    },
    "3": {
        "base": "12",
        "value": "32811A4AA0B7B"
    },
    "4": {
        "base": "11",
        "value": "917978721331A"
    },
    "5": {
        "base": "16",
        "value": "1A22886782E1"
    },
    "6": {
        "base": "10",
        "value": "28735619654702"
    },
    "7": {
        "base": "14",
        "value": "71AB5070CC4B"
    },
    "8": {
        "base": "9",
        "value": "122662581541670"
    },
    "9": {
        "base": "8",
        "value": "642121030037605"
    }
    }
Output:- Reconstructed Secret:- 123456789
