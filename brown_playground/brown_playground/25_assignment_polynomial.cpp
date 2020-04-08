//
//  25_assignment_polynomial.cpp
//  brown_playground
//
//  Created by Artsiom Kaliaha on 4/8/20.
//  Copyright © 2020 Artsiom Kaliaha. All rights reserved.
//

#include <iostream>

#include <vector>
#include <iostream>
#include <algorithm>

#include <cassert>
#include <chrono>

using namespace std;
using namespace std::chrono;

template<typename T>
void PrintCoeff(std::ostream& out, int i, const T& coef, bool printed) {
    bool coeffPrinted = false;
    if (coef == 1 && i > 0) {
        out << (printed ? "+" : "");
    } else if (coef == -1 && i > 0) {
        out << "-";
    } else if (coef >= 0 && printed) {
        out << "+" << coef;
        coeffPrinted = true;
    } else {
        out << coef;
        coeffPrinted = true;
    }
    if (i > 0) {
        out << (coeffPrinted ? "*" : "") << "x";
    }
    if (i > 1) {
        out << "^" << i;
    }
}

template<typename T>
class Polynomial {
private:
    std::vector<T> coeffs_ = {0};
    
    void Shrink() {
        while (coeffs_.size() > 1 && coeffs_.back() == 0) {
            coeffs_.pop_back();
        }
    }
    
    T checkOverflowAndReturn(size_t degree) const {
         return degree < coeffs_.size() ? coeffs_[degree] : 0;
    }
    
    struct PolynomialProxy {
        Polynomial& polynomial;
        size_t index;
        
        void operator=(const T& rhs) {
            if (polynomial.Degree() < index) {
                polynomial.coeffs_.resize(index + 1);
            }
            
            polynomial.coeffs_[index] = rhs;
            polynomial.Shrink();
        }
        
        operator const T&() const { return polynomial.checkOverflowAndReturn(index); }
    };
    
public:
    Polynomial() = default;
    Polynomial(vector<T> coeffs) : coeffs_(std::move(coeffs)) {
        Shrink();
    }
    
    template<typename Iterator>
    Polynomial(Iterator first, Iterator last) : coeffs_(first, last) {
        Shrink();
    }
    
    bool operator ==(const Polynomial& other) const {
        return coeffs_ == other.coeffs_;
    }
    
    bool operator !=(const Polynomial& other) const {
        return !operator==(other);
    }
    
    int Degree() const {
        return coeffs_.size() - 1;
    }
    
    Polynomial& operator +=(const Polynomial& r) {
        if (r.coeffs_.size() > coeffs_.size()) {
            coeffs_.resize(r.coeffs_.size());
        }
        for (size_t i = 0; i != r.coeffs_.size(); ++i) {
            coeffs_[i] += r.coeffs_[i];
        }
        Shrink();
        return *this;
    }
    
    Polynomial& operator -=(const Polynomial& r) {
        if (r.coeffs_.size() > coeffs_.size()) {
            coeffs_.resize(r.coeffs_.size());
        }
        for (size_t i = 0; i != r.coeffs_.size(); ++i) {
            coeffs_[i] -= r.coeffs_[i];
        }
        Shrink();
        return *this;
    }
    
    T operator [](size_t degree) const {
        return checkOverflowAndReturn(degree);
    }
    
    // implement non-const version of operator[]
    PolynomialProxy operator [](size_t degree) {
        return PolynomialProxy { *this, degree };
    }
    
    T operator ()(const T& x) const {
        T res = 0;
        for (auto it = coeffs_.rbegin(); it != coeffs_.rend(); ++it) {
            res *= x;
            res += *it;
        }
        return res;
    }
    
    using const_iterator = typename std::vector<T>::const_iterator;
    
    const_iterator begin() const {
        return coeffs_.cbegin();
    }
    
    const_iterator end() const {
        return coeffs_.cend();
    }
};

template <typename T>
std::ostream& operator <<(std::ostream& out, const Polynomial<T>& p) {
    bool printed = false;
    for (int i = p.Degree(); i >= 0; --i) {
        if (p[i] != 0) {
            PrintCoeff(out, i, p[i], printed);
            printed = true;
        }
    }
    return out;
}

template <typename T>
Polynomial<T> operator +(Polynomial<T> lhs, const Polynomial<T>& rhs) {
    lhs += rhs;
    return lhs;
}

template <typename T>
Polynomial<T> operator -(Polynomial<T> lhs, const Polynomial<T>& rhs) {
    lhs -= rhs;
    return lhs;
}

void TestCreation() {
    {
        Polynomial<int> default_constructed;
        assert(default_constructed.Degree() == 0);
        assert(default_constructed[0] == 0);
    }
    {
        Polynomial<double> from_vector({1.0, 2.0, 3.0, 4.0});
        assert(from_vector.Degree() == 3);
        assert(from_vector[0] == 1.0);
        assert(from_vector[1] == 2.0);
        assert(from_vector[2] == 3.0);
        assert(from_vector[3] == 4.0);
    }
    {
        const vector<int> coeffs = {4, 9, 7, 8, 12};
        Polynomial<int> from_iterators(begin(coeffs), end(coeffs));
        assert(from_iterators.Degree() == coeffs.size() - 1);
        assert(std::equal(cbegin(from_iterators), cend(from_iterators), begin(coeffs)));
    }
}

void TestEqualComparability() {
    Polynomial<int> p1({9, 3, 2, 8});
    Polynomial<int> p2({9, 3, 2, 8});
    Polynomial<int> p3({1, 2, 4, 8});

    assert(p1 == p2);
    assert(p1 != p3);
}

void TestAddition() {
    Polynomial<int> p1({9, 3, 2, 8});
    Polynomial<int> p2({1, 2, 4});

    p1 += p2;
    assert(p1 == Polynomial<int>({10, 5, 6, 8}));

    auto p3 = p1 + p2;
    assert(p3 == Polynomial<int>({11, 7, 10, 8}));

    p3 += Polynomial<int>({-11, 1, -10, -8});
    assert(p3.Degree() == 1);
    const Polynomial<int> expected{{0, 8}};
    assert(p3 == expected);
}

void TestSubtraction() {
    Polynomial<int> p1({9, 3, 2, 8});
    Polynomial<int> p2({1, 2, 4});

    p1 -= p2;
    assert(p1 == Polynomial<int>({8, 1, -2, 8}));

    auto p3 = p1 - p2;
    assert(p3 == Polynomial<int>({7, -1, -6, 8}));

    p3 -= Polynomial<int>({7, -5, -6, 8});
    assert(p3.Degree() == 1);
    const Polynomial<int> expected{{0, 4}};
    assert(p3 == expected);
}

void TestEvaluation() {
    const Polynomial<int> default_constructed;
    assert(default_constructed(0) == 0);
    assert(default_constructed(1) == 0);
    assert(default_constructed(-1) == 0);
    assert(default_constructed(198632) == 0);

    const Polynomial<int64_t> cubic({1, 1, 1, 1});
    assert(cubic(0) == 1);
    assert(cubic(1) == 4);
    assert(cubic(2) == 15);
    assert(cubic(21) == 9724);
}

void TestConstAccess() {
    const Polynomial<int> poly(std::initializer_list<int> {1, 2, 3, 4, 5});

    assert(poly[0] == 1);
    assert(poly[1] == 2);
    assert(poly[2] == 3);
    assert(poly[3] == 4);
    assert(poly[4] == 5);
    assert(poly[5] == 0);
    assert(poly[6] == 0);
    assert(poly[608] == 0);
}

void TestNonconstAccess() {
    Polynomial<int> poly;
    
    poly[0] = 1;
    poly[3] = 12;
    poly[5] = 4;
    assert(poly.Degree() == 5);
    
    assert(poly[0] == 1);
    assert(poly[1] == 0);
    assert(poly[2] == 0);
    assert(poly[3] == 12);
    assert(poly[4] == 0);
    assert(poly[5] == 4);
    assert(poly[6] == 0);
    assert(poly[608] == 0);
    
    assert(poly.Degree() == 5);
    
    poly[608] = 0;
    assert(poly.Degree() == 5);
    
    {
        auto start = system_clock::now();
        
        for (size_t i = 10; i < 50000; ++i) {
            assert(std::as_const(poly)[i] == 0);
            assert(poly.Degree() == 5);
        }
        
        cout << duration_cast<milliseconds>(system_clock::now() - start).count() << " Iteration over const" << endl;
    }
    {
        auto start = system_clock::now();
        
        for (size_t i = 10; i < 50000; ++i) {
            assert(poly[i] == 0);
            assert(poly.Degree() == 5);
        }
        
        cout << duration_cast<milliseconds>(system_clock::now() - start).count() << " Iteration over non-const" << endl;
    }
}

int main() {
    TestCreation();
    TestEqualComparability();
    TestAddition();
    TestSubtraction();
    TestEvaluation();
    TestConstAccess();
    TestNonconstAccess();
    return 0;
}
