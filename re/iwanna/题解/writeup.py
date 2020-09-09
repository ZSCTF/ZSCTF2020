# -*- coding: utf-8 -*-
import sympy


r = 998244353


def randint(a: int, b: int) -> int:
    global r
    r = (r * 114514 + 1919810) ^ (r >> 4)
    r &= 0x7fffffff
    return r % (b - a + 1) + a


result = [
    0x345613a, 0x3f413c3, 0x1c9a745, 0x4395677, 0x4ad275a, 0x3cb8fac, 0x4137723, 0x400fc90, 0x4bad7f2, 0x27fef27,
    0x2869ad5, 0x3d56b1e, 0x41caba9, 0x55b759a, 0x2dffebd, 0x3cac245, 0x213d73f, 0x4b3aaa1, 0x44dbf4e, 0x290c8e9,
    0x407358d, 0x4345f26, 0x1a972f3, 0x31339a8, 0x37ff49c, 0x45f3f4c, 0x39cffba, 0x2fce5b5, 0x3980c6a, 0x3703ffb,
    0x23cf97f, 0x4704989, 0x440e56b, 0x3798a18, 0x384ba59, 0x5296ebc, 0x3d7081f, 0x207269c, 0x3ef77d0, 0x1bbd203,
    0x4870653, 0x38a25e3, 0x3e828ed, 0x32fc3cd, 0x2cb3751, 0x3a43752, 0x38bed5a, 0x3a731f0, 0x4578592, 0x434d099,
    0x323b8d9, 0x4aab660, 0x2086041, 0x245e9f9, 0x33f9561, 0x375f3a3, 0x38dea89, 0x317abcb, 0x3454b92, 0x430b29e,
    0x33344a8, 0x42b4708, 0x25a6ed8, 0x303baff, 0x3136c57, 0x3c5f476, 0x327502c, 0x3995a92, 0x1d01949, 0x591c957,
    0x3e2d475, 0x226499e, 0x4429c41, 0x5a0e544, 0x2984e9e, 0x287c601, 0x21063b8, 0x2e92ac6, 0x4f554ee, 0x3b6e001,
    0x29f1033, 0x3d92aad, 0x3131cb0, 0x2eeee91, 0x286e5d0, 0x36a0c31, 0x34384a4, 0x35f45d5, 0x3d9b5c6, 0x4d08950,
    0x39b4bf6, 0x4ae3bc1, 0x31ff358, 0x2202a1a, 0x2f6db61, 0x29f1990, 0x2ac0072, 0x3d09327, 0x32a82bf, 0x473d38d
]

flag = sympy.symbols('flag[:41]')

equations = []

for i in range(100):
    equation = -result[i]
    for j in range(5):
        coef = randint(0, 233333)
        index = randint(0, 40)
        equation += coef * flag[index]
    equations.append(equation)


ans = sympy.solve(equations, flag)

print(''.join(chr(ans[i]) for i in flag))  # zsctf{WoW_You_Can_REALLY_SoLve_EquaTions}
