a_str = "-80538738812075974"
b_str = "80435758145817515"
c_str = "12602123297335631"

a = parse(BigInt, a_str)
b = parse(BigInt, b_str)
c = parse(BigInt, c_str)

println(a * a * a + b * b * b + c * c * c)
