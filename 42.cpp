#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <cassert>
using namespace std;

class UBigInt {
public:
  UBigInt() {}
  UBigInt(const string& str) {
    data.resize(str.size());
    auto p = data.rbegin();
    for (int i = 0; i < str.size(); ++i) {
      assert(isdigit(str[i]));
      *(p++) = str[i] - '0';
    }
  }
  bool IsZero() {return data.size() == 1 && data[0] == 0;}
  friend UBigInt operator+(const UBigInt& a, const UBigInt& b);
  friend UBigInt operator-(const UBigInt& a, const UBigInt& b);
  friend UBigInt operator*(const UBigInt& a, const UBigInt& b);
  friend bool operator==(const UBigInt& a, const UBigInt& b);
  friend bool operator<(const UBigInt& a, const UBigInt& b);
  friend ostream& operator<<(ostream& os, const UBigInt& num);
private:
  void DeleteLeaderZero();
  void Carry();
private:
  vector<int> data;
};

UBigInt operator+(const UBigInt& a, const UBigInt& b) {
  auto pa = a.data.begin();
  auto pb = b.data.begin();
  int carry = 0;
  UBigInt c;
  while (pa != a.data.end() || pb != b.data.end() || carry > 0) {
    if (pa != a.data.end())
      carry += *(pa++);
    if (pb != b.data.end())
      carry += *(pb++);
    c.data.push_back(carry % 10);
    carry /= 10;
  }
  if (c.data.size() == 0) c.data.push_back(0);
  return c;
}

UBigInt operator-(const UBigInt& a, const UBigInt& b) {
  // assume a >= b
  if (a < b) return b - a;
  auto pa = a.data.begin();
  auto pb = b.data.begin();
  bool borrow = false;
  UBigInt c;
  while (pa != a.data.end()) {
    int v = *(pa++) - borrow;
    if (pb != b.data.end()) v -= *(pb++);
    if (v < 0) {
      v += 10;
      borrow = true;
    } else {
      borrow = false;
    }
    c.data.push_back(v);
  }
  // delete leader 0
  c.DeleteLeaderZero();
  return c;
}

UBigInt operator*(const UBigInt& a, const UBigInt& b) {
  UBigInt c;
  c.data.resize(a.data.size() + b.data.size(), 0);
  for (int bi = 0; bi < b.data.size(); ++bi) {
    for (int ai = 0; ai < a.data.size(); ++ai) {
      int v = a.data[ai] * b.data[bi];
      c.data[ai + bi] += v;
    }
  }
  c.Carry();
  c.DeleteLeaderZero();
  return c;
}

bool operator==(const UBigInt& a, const UBigInt& b) {
  if (a.data.size() != b.data.size()) return false;
  for (auto pa = a.data.rbegin(), pb = b.data.rbegin(); pa != a.data.rend(); ++pa, ++pb) {
    if (*pa != *pb) return false; 
  }
  return true;
}

bool operator<(const UBigInt& a, const UBigInt& b) {
  if (a.data.size() != b.data.size()) return a.data.size() < b.data.size();
  for (auto pa = a.data.rbegin(), pb = b.data.rbegin(); pa != a.data.rend(); ++pa, ++pb) {
    if (*pa != *pb) return *pa < *pb;
  }
  return false;
}

ostream& operator<<(ostream& os, const UBigInt& num) {
  for (auto p = num.data.rbegin(); p != num.data.rend(); ++p) os << *p;
  return os;
}

void UBigInt::DeleteLeaderZero() {
  auto pc = data.rbegin();
  while (pc != data.rend() && *pc == 0) ++pc;
  int size = data.rend() - pc;
  if (size == 0) size = 1;
  data.resize(size);
}

void UBigInt::Carry() {
  int carry = 0;
  for (auto p = data.begin(); p != data.end(); ++p) {
    *p += carry;
    carry = *p / 10;
    *p %= 10;
  }
  while (carry > 0) {
    data.push_back(carry % 10);
    carry /= 10;
  }
}


class BigInt {
public:
  BigInt() {}
  BigInt(const string& str) {
    int i = 0;
    if (str[0] == '-') {
      minus = true;
      ++i;
    }
    else {
      minus = false;
      if (str[0] == '+') ++i; 
    }
    data = UBigInt(str.substr(i));
  }
  friend ostream& operator<<(ostream& os, const BigInt& num);
  friend BigInt operator+(const BigInt& a, const BigInt& b);
  friend BigInt operator-(const BigInt& a, const BigInt& b);
  friend BigInt operator*(const BigInt& a, const BigInt& b);
private:
  void PostProcess();
private:
  bool minus;
  UBigInt data;
};

ostream& operator<<(ostream& os, const BigInt& num) {
  if (num.minus) os << '-';
  cout << num.data;
  return os;
}

BigInt operator+(const BigInt& a, const BigInt& b) {
  BigInt c;
  if (a.minus != b.minus) {
    if (a.data < b.data) {
      c.minus = b.minus;
      c.data = b.data - a.data;
    } else {
      c.minus = a.minus;
      c.data = a.data - b.data;
    }
    return c;
  }
  c.minus = a.minus; 
  c.data = a.data + b.data;
  c.PostProcess();
  return c;
}

BigInt operator-(const BigInt& a, const BigInt& b) {
  BigInt c;
  if (a.minus != b.minus) {
    c.minus = a.minus;
    c.data = a.data + b.data;
    return c;
  }
  if (a.data < b.data) {
    c.minus = !a.minus;
    c.data = b.data - a.data;
  } else {
    c.minus = !a.minus;
    c.data = a.data - b.data;
  }
  c.PostProcess();
  return c;
}

BigInt operator*(const BigInt& a, const BigInt& b) {
  BigInt c;
  c.minus = a.minus ^ b.minus;
  c.data = a.data * b.data;
  c.PostProcess();
  return c;
}

void BigInt::PostProcess() {
  if (data.IsZero())
    minus = false;
}

const string a_str = "-80538738812075974";
const string b_str = "80435758145817515";
const string c_str = "12602123297335631";

int main() {
  BigInt a(a_str);
  BigInt b(b_str);
  BigInt c(c_str);
  BigInt wa = a * a * a;
  BigInt wb = b * b * b;
  BigInt wc = c * c * c;
  BigInt out = a * a * a + b * b * b + c * c * c;
  cout << out << endl;
  return 0;
}
