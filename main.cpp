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
    int n = 6;  // Total number of shares
    int k = 3;  // Threshold

    secret_sharing(secret, n, k);

    return 0;
}
