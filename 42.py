class UBigInt:
    def __init__(self, string=None):
        if string is None:
            self.data = []
            return
        assert isinstance(string, str),\
            TypeError('the type of input should be `str`')
        for c in string[::-1]:
            assert ord('0') <= ord(c) <= ord('9'), c
        self.data = [ord(c) - ord('0') for c in string[::-1]]

    def is_zero(self):
        return len(self.data) == 1 and self.data[0] == 0

    def __str__(self):
        return ''.join(str(v) for v in self.data[::-1])

    def __lt__(self, other):
        assert isinstance(other, UBigInt)
        if len(self.data) != len(other.data):
            return len(self.data) < len(other.data)
        for a, b in zip(self.data[::-1], other.data[::-1]):
            if a != b:
                return a < b
        return False

    def __add__(self, other):
        assert isinstance(other, UBigInt)
        out = UBigInt()
        i = j = 0
        v = 0
        while i < len(self.data) or j < len(other.data) or v > 0:
            if i < len(self.data):
                v += self.data[i]
                i += 1
            if j < len(other.data):
                v += other.data[j]
                j += 1
            out.data.append(v % 10)
            v //= 10
        return out

    def __sub__(self, other):
        assert isinstance(other, UBigInt)
        assert not self < other
        out = UBigInt()
        borrow = False
        for i, v in enumerate(self.data):
            u = v - borrow
            if i < len(other.data):
                u -= other.data[i]
            if u < 0:
                u += 10
                borrow = True
            else:
                borrow = False
            out.data.append(u)
        out._delete_leader_zero()
        return out

    def __mul__(self, other):
        out = UBigInt()
        out.data = [0] * (len(self.data) + len(other.data))
        for ai, av in enumerate(self.data):
            for bi, bv in enumerate(other.data):
                out.data[ai + bi] += av * bv
        out._carry()
        out._delete_leader_zero()
        return out

    def _carry(self):
        c = 0
        for i, v in enumerate(self.data):
            c += v
            self.data[i] = c % 10
            c //= 10
        while c > 0:
            self.data.append(c % 10)
            c //= 10

    def _delete_leader_zero(self):
        for i, v in enumerate(self.data[::-1]):
            if v != 0:
                break
        self.data = self.data[:len(self.data)-i]
        if len(self.data) == 0:
            self.data.append(0)


class BigInt:
    def __init__(self, string=None):
        if string is None:
            self.data = UBigInt()
            self.minus = False
            return
        assert isinstance(string, str),\
            TypeError('the type of input should be `str`')
        if string[0] == '-':
            self.minus = True
            string = string[1:]
        else:
            self.minus = False
        self.data = UBigInt(string)

    def __str__(self):
        if self.minus:
            return '-' + str(self.data)
        return str(self.data)

    def __add__(self, other):
        assert isinstance(other, BigInt)
        out = BigInt()
        if self.minus == other.minus:
            out.minus = self.minus
            out.data = self.data + other.data
        elif self.data < other.data:
            out.minus = other.minus
            out.data = other.data - self.data
        else:
            out.minus = self.minus
            out.data = self.data - other.data
        out._post_process()
        return out

    def __sub__(self, other):
        assert isinstance(other, BigInt)
        out = BigInt()
        if self.minus != other.minus:
            out.minus = self.minus
            out.data = self.data + other.data
        elif self.data < other.data:
            out.minus = not self.minus
            out.data = other.data - self.data
            print("!!!", out.data, other.data, self.data)
        else:
            out.minus = self.minus
            out.data = self.data - other.data
        out._post_process()
        return out

    def __mul__(self, other):
        assert isinstance(other, BigInt)
        out = BigInt()
        out.minus = self.minus ^ other.minus
        out.data = self.data * other.data
        out._post_process()
        return out

    def _post_process(self):
        if self.data.is_zero():
            self.minus = False


if __name__ == '__main__':
    a_str = "-80538738812075974"
    b_str = "80435758145817515"
    c_str = "12602123297335631"
    a = BigInt(a_str)
    b = BigInt(b_str)
    c = BigInt(c_str)
    out = a * a * a + b * b * b + c * c * c
    print(out)
