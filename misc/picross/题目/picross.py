# -*- coding: utf-8 -*-
import hashlib
import sys
from pathlib import Path
from typing import List

import colorama
import getch
from colorama import Back
from more_itertools.more import split_at


class PicrossPuzzle:
    __slots__ = ('row_numbers', 'col_numbers', 'width', 'height', 'cells')

    row_numbers: List[List[int]]
    col_numbers: List[List[int]]
    width: int
    height: int
    cells: List[List[bool]]

    def __init__(self, row_numbers: List[List[int]], col_numbers: List[List[int]]) -> None:
        if not row_numbers or not col_numbers:
            raise ValueError('length of row_numbers and col_numbers should be greater than 0')
        self.row_numbers = row_numbers
        self.col_numbers = col_numbers
        self.width = len(col_numbers)
        self.height = len(row_numbers)

    def set_cells(self, cells: List[List[bool]]) -> None:
        if len(cells) != self.height:
            raise ValueError('height is not correct')
        if not all(len(row) == self.width for row in cells):
            raise ValueError('width is not correct')
        if not all(isinstance(cell, bool) for row in cells for cell in row):
            raise ValueError('all cells should be bool')
        self.cells = cells

    def check(self) -> bool:
        for r in range(self.height):
            row = self.cells[r]
            row_number = [len(x) for x in split_at(row, lambda x: x is False) if x]
            if row_number != self.row_numbers[r]:
                return False
        for c in range(self.width):
            col = [self.cells[r][c] for r in range(self.height)]
            col_number = [len(x) for x in split_at(col, lambda x: x is False) if x]
            if col_number != self.col_numbers[c]:
                return False
        return True

    def print_cells(self) -> None:
        for r in range(self.height):
            for c in range(self.width):
                if self.cells[r][c]:
                    sys.stdout.write(f'{Back.WHITE}##')
                else:
                    sys.stdout.write(f'{Back.RESET}  ')
            sys.stdout.write('\n')
        sys.stdout.flush()


def main():
    row_numbers = [
        [7, 4, 1, 2, 3, 7],
        [1, 1, 2, 2, 1, 1, 1],
        [1, 3, 1, 4, 1, 1, 1, 3, 1],
        [1, 3, 1, 2, 2, 2, 1, 1, 3, 1],
        [1, 3, 1, 1, 1, 3, 2, 1, 3, 1],
        [1, 1, 1, 2, 2, 2, 1, 1, 1],
        [7, 1, 1, 1, 1, 1, 1, 1, 7],
        [3, 3, 1],
        [1, 5, 1, 1, 1, 1, 5, 1],
        [2, 1, 1, 1, 4, 2, 3, 2],
        [1, 1, 2, 1, 4, 1, 2, 2, 1],
        [5, 2, 1, 3, 2, 2, 2, 1],
        [1, 1, 1, 3, 3, 1, 1],
        [2, 3, 2, 5, 4, 2],
        [1, 1, 3, 1, 5, 1, 2, 1, 1, 1],
        [1, 2, 4, 1, 1, 1, 1, 2, 1],
        [1, 3, 2, 1, 1, 3, 3, 1, 3],
        [1, 1, 1, 1, 2, 2, 7, 3],
        [1, 1, 1, 1, 2, 1, 4, 1, 2],
        [1, 2, 4, 1, 1, 1, 1, 1, 2, 2],
        [1, 4, 4, 2, 7],
        [2, 4, 1, 1, 1],
        [7, 1, 8, 1, 1, 1],
        [1, 1, 2, 1, 1, 1, 1, 2, 1],
        [1, 3, 1, 2, 1, 4, 7],
        [1, 3, 1, 2, 1, 1, 5, 2],
        [1, 3, 1, 4, 4, 2, 4],
        [1, 1, 2, 1, 4, 2, 2],
        [7, 1, 1, 1, 4, 1, 1, 4, 1]
    ]
    col_numbers = [
        [7, 1, 5, 5, 7],
        [1, 1, 1, 1, 1, 1, 1],
        [1, 3, 1, 2, 2, 1, 4, 1, 3, 1],
        [1, 3, 1, 4, 1, 2, 2, 1, 3, 1],
        [1, 3, 1, 1, 1, 6, 1, 1, 3, 1],
        [1, 1, 2, 2, 2, 1, 1],
        [7, 1, 1, 1, 1, 1, 3, 7],
        [2, 2, 1],
        [1, 6, 2, 2, 1, 1, 4, 1],
        [1, 2, 1, 1, 2, 1, 1, 2, 4],
        [1, 1, 1, 3, 1, 2, 3, 1, 3],
        [3, 1, 1, 2, 1, 1, 5],
        [1, 4, 1, 2, 5, 1],
        [1, 1, 1, 2, 3, 3, 1],
        [1, 1, 7, 1, 1, 1, 2, 1, 3],
        [2, 2, 4, 5, 2, 1, 1, 1],
        [1, 2, 1, 6, 7, 3, 1],
        [1, 3, 2, 1, 1, 1, 3, 1],
        [1, 1, 2, 2, 2, 2, 2, 1],
        [1, 1, 1, 1, 1, 1, 3, 1, 1, 2],
        [1, 5, 2, 2, 2, 6, 1],
        [2, 7, 1, 3],
        [7, 2, 1, 2, 1, 1, 1, 1, 2],
        [1, 1, 3, 1, 2, 1, 1, 1],
        [1, 3, 1, 4, 3, 8, 3],
        [1, 3, 1, 1, 1, 1, 1, 3, 2, 3],
        [1, 3, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1],
        [1, 1, 2, 1, 3, 1, 3],
        [7, 2, 1, 2, 2, 1, 1, 1, 1, 2]
    ]

    if not Path('puzzle.txt').is_file():
        print('puzzle.txt not found')
        return

    with open('puzzle.txt', 'r', encoding='utf-8') as f:
        puzzle_cells = [line for line in f.read().splitlines() if line]
        puzzle_cells = [[True if cell == 'X' else False for cell in row] for row in puzzle_cells]

    puzzle = PicrossPuzzle(row_numbers, col_numbers)
    puzzle.set_cells(puzzle_cells)

    if not puzzle.check():
        print('wrong, please check your answer again')
        return

    print('right, now scan the qr code to get the first part of the flag')
    puzzle.print_cells()

    data = ''.join(str(int(cell)) for row in puzzle.cells for cell in row)
    hash = hashlib.sha1(data.encode()).hexdigest()
    print('and the second part of the flag is %s}' % hash)


if __name__ == '__main__':
    colorama.init(autoreset=True)
    main()
    getch.pause('Press any key to exit...')
