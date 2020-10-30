# -*- coding: utf-8 -*-
import re
import time
from functools import reduce
from typing import List

import pwn
import pwnlib.tubes.process

HOST = '172.16.35.175'
PORT = 2000


def get_piles(process: pwnlib.tubes.process.process) -> List[int]:
    process.recvuntil(b'[')
    piles = [int(x) for x in process.recvuntil(b']')[:-1].split(b', ')]
    return piles


def calc_piles_xor(piles: List[int]) -> int:
    return reduce(lambda a, b: a ^ b, piles, 0)


process: pwnlib.tubes.process.process = pwn.remote(HOST, PORT)

while True:
    piles = get_piles(process)
    xor = calc_piles_xor(piles)
    print(f'piles = {piles}, xor = {xor}')

    (take_index, take_quantity) = (-1, -1)
    for (idx, pile) in enumerate(piles):
        if pile ^ xor <= pile and pile > 0:
            take_index = idx
            take_quantity = pile - (pile ^ xor)
            break
    assert (take_index, take_quantity) != (-1, -1)

    piles[take_index] -= take_quantity
    print(f'take index = {take_index}, quantity = {take_quantity}, sum = {sum(piles)}')
    process.sendline(str(take_index))
    time.sleep(1e-2)
    process.recvline()
    process.sendline(str(take_quantity))

    if sum(piles) == 0:
        flag = re.search(rb'(zsctf{.*?})', process.recvall()).group(1).decode()
        print(flag)
        break
    print(f'after zhakul move: {get_piles(process)}')
    print()
