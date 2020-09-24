# -*- coding: utf-8 -*-
import random
from typing import Tuple

import qrcode
from more_itertools import divide
from PIL import Image, ImageChops, ImageOps

from make_flag_qrcode import FLAG

QRCODE_NUM = 3 * 8
IMAGE_SIZE = (912, 912)
TICKS = [2, 4, 6]
OFFSETS = [
    (tick * x, tick * y)
    for (x, y) in [(1, 0), (1, 1), (0, 1), (-1, 1), (-1, 0), (-1, -1), (0, -1), (1, -1)]
    for tick in TICKS
]
print(OFFSETS)


def make_qrcode_image(data: str, color: Tuple[int, int, int]) -> Image.Image:
    qr = qrcode.QRCode(
        version=9,
        error_correction=qrcode.ERROR_CORRECT_L,
        box_size=16,
        border=0
    )
    qr.add_data(data)
    qr.make()
    print([i.__dict__ for i in qr.data_list])
    print(qr.version)
    # qr.print_ascii()
    image: Image.Image = qr.make_image(fill_color='rgb(%d,%d,%d)' % color, back_color='transparent').convert('RGBA')
    return image


def combine_qrcode_image(result: Image.Image, image: Image.Image, position: Tuple[int, int]) -> Image.Image:
    temp: Image.Image = Image.new(result.mode, result.size)
    temp.paste(image, position, image)
    return ImageChops.add(result, temp)


def get_qrcode_data(header: str, data: bytes) -> str:
    return header + '\n' + data.hex().upper()


def main():
    result = Image.new('RGB', IMAGE_SIZE, (0, 0, 0))
    combined_qrcode_image = Image.new('RGBA', IMAGE_SIZE, (0, 0, 0, 0))

    with open('flag_qr.png', 'rb') as f:
        s = f.read()

    data_list = [bytes(i) for i in divide(QRCODE_NUM, s)]
    print([len(i) for i in data_list])

    qrcode_data_list = []
    for (i, data) in enumerate(data_list):
        header = 'flag.png {:02d}/{:02d}'.format(i + 1, len(data_list))
        qrcode_data = get_qrcode_data(header, data)
        qrcode_data_list.append(qrcode_data)

    random.seed(FLAG)
    random.shuffle(qrcode_data_list)

    for (i, qrcode_data) in enumerate(qrcode_data_list):
        color = [0, 0, 0]
        value = 1 << (i % 8)
        channel_index = i // 8
        color[channel_index] = value

        image = make_qrcode_image(qrcode_data, tuple(color))
        # image.show()
        print(image.size)

        position = (combined_qrcode_image.width // 2 - image.width // 2 + OFFSETS[i][0],
                    combined_qrcode_image.height // 2 - image.height // 2 + OFFSETS[i][1])
        combined_qrcode_image = combine_qrcode_image(combined_qrcode_image, image, position)
    # combined_qrcode_image.show()

    result.paste(combined_qrcode_image, (0, 0), combined_qrcode_image)
    result = ImageOps.invert(result)
    result.show()

    result.save('题目/qr.png', optimize=True)


if __name__ == '__main__':
    main()
