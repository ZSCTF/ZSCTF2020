from random import randint
import socketserver

PORT = 9000
flag = open('flag').read().strip()


class MACHINE:
    def waitToDie(self, nim):
        x = 0
        for i in range(len(nim.Piles)):
            x ^= nim.Piles[i]
        return x != 0

    def xorToWin(self, nim):
        x = 0
        for i in range(len(nim.Piles)):
            x ^= nim.Piles[i]
        return x

    def move(self, nim):
        if not self.waitToDie(nim):
            pile = randint(0, len(nim.Piles) - 1)
            while nim.Piles[pile] == 0:
                pile = randint(0, len(nim.Piles) - 1)
            quant = randint(1, nim.Piles[pile])
            return pile, quant
        else:
            xor = self.xorToWin(nim)
            pile = randint(0, len(nim.Piles) - 1)
            while nim.Piles[pile] == 0 or nim.Piles[pile] ^ xor > nim.Piles[pile]:
                pile = randint(0, len(nim.Piles) - 1)

            return pile, nim.Piles[pile] - (xor ^ nim.Piles[pile])


class NIM:
    def __init__(self, piles):
        self.Piles = [randint(1, 100) for _ in range(piles)]
        self.sum = sum(self.Piles)

    def remove(self, pile, quant):
        if self.Piles[pile] < quant:
            return "Invalid move!"
        else:
            self.sum -= quant
            self.Piles[pile] -= quant
            return "Ok, valid move!"

    def finished(self):
        return self.sum == 0


class ThreadedTCPRequestHandler(socketserver.BaseRequestHandler):
    def handle(self):
        self.request.settimeout(100)
        self.request.send(
            "--------------------拈------------------\n"
            "规则：\n"
            "请从以下石子堆中选中一堆\n"
            "并取走若干个\n"
            "拿走最后石子的人赢\n"
            "---------------------------------------\n\n"
            "You start.\n\n".encode('utf-8'))

        machine = MACHINE()
        nim = NIM(15)
        turn = 0
        while not nim.finished():
            self.request.send("{}\n\n".format(str(nim.Piles)).encode('utf-8'))
            if turn % 2:
                p, q = machine.move(nim)
                self.request.send("The zhakul move is: pile = {}, quantity = {}\n".format(p, q).encode('utf-8'))
                nim.remove(p, q)
            else:
                try:
                    self.request.send("Choose the pile:\n".encode('utf-8'))
                    p = int(self.request.recv(2048))
                    self.request.send("Input the quantity:\n".encode('utf-8'))
                    q = int(self.request.recv(2048))
                    if q <= 0:
                        self.request.send("Invalid move!\n".encode('utf-8'))
                        break
                    if nim.remove(p, q) == "Invalid move!":
                        self.request.send("Invalid move!\n".encode('utf-8'))
                        break
                except :
                    self.request.send("Bad Input.\n".encode('utf-8'))
                    break
            turn += 1
        if turn % 2 == 0:
            self.request.send("Sorry, you lost. Try again!\n".encode('utf-8'))
        else:
            self.request.send("congratulation! {}\n".format(flag).encode('utf-8'))


class ThreadedTCPServer(socketserver.ThreadingMixIn, socketserver.TCPServer): pass


if __name__ == '__main__':
    server = ThreadedTCPServer(('0.0.0.0', PORT), ThreadedTCPRequestHandler)
    server.allow_reuse_address = True
    server.serve_forever()
