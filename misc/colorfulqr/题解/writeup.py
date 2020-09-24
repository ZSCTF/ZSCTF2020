# -*- coding: utf-8 -*-
from io import BytesIO
from typing import List

import pyzbar.pyzbar as pyzbar
from PIL import Image, ImageChops


def extract_bit(image: Image.Image, channel: int, bit: int) -> Image.Image:
    channel_image = image.getchannel(channel)
    result = channel_image.point(lambda x: 255 if x & (1 << bit) else 0)
    return result


def decode_image(image: Image.Image) -> List[bytes]:
    result = [i.data for i in pyzbar.decode(image)]
    return result


def binarize_image(image: Image.Image, threshold: int) -> Image.Image:
    result = image.convert('L').point(lambda x: 255 if x > threshold else 0)
    return result


def main():
    image = Image.open('../题目/qr.png')

    data_list = []
    for channel in range(len(image.getbands())):
        for bit in range(8):
            extracted = extract_bit(image, channel, bit)
            data_list.append(decode_image(extracted)[0])

    data_list.sort()  # make sure data index is from 01 to 24

    result_hex = ''.join(i.decode().split('\n')[1] for i in data_list)
    result = bytes.fromhex(result_hex)

    flag_image = Image.open(BytesIO(result))
    # flag_image.show()
    flag_image.save('flag_qr.png')

    flag_image = ImageChops.invert(binarize_image(flag_image, 0))
    # flag_image.show()

    print(decode_image(flag_image)[0].decode())


if __name__ == '__main__':
    main()
