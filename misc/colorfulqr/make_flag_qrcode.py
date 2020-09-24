# -*- coding: utf-8 -*-
import math

import qrcode
from PIL import Image, ImageChops, ImageColor, ImageDraw, ImageStat

FLAG = 'zsctf{ColorFul_QuiCk_ResPonse_CODE}'


def lerp(a: float, b: float, t: float) -> float:
    t = max(min(t, 1), 0)
    return a + (b - a) * t


def mosaic(image: Image.Image, size: int) -> Image.Image:
    draw: ImageDraw.ImageDraw = ImageDraw.Draw(image)
    for y in range(int(math.ceil(image.height / size))):
        for x in range(int(math.ceil(image.width / size))):
            start = (x * size, y * size)
            end = (min(image.width, start[0] + size - 1), min(image.height, start[1] + size - 1))
            mask = image.crop((*start, *end))

            color = tuple(int(i + 0.5) for i in ImageStat.Stat(mask).mean)
            draw.rectangle([start, end], fill=tuple(color))
    return image


def main():
    qr = qrcode.QRCode(
        version=None,
        error_correction=qrcode.ERROR_CORRECT_L,
        box_size=8,
        border=2
    )

    qr_data = FLAG.encode()
    print(qr_data)

    qr.add_data(qr_data)
    qr.print_ascii()

    qr_image: Image.Image = qr.make_image(fill_color='white', back_color='black').convert('RGB')

    bg = Image.new('RGB', qr_image.size)
    data_list = []

    center = (bg.width // 2, bg.height // 2)
    for y in range(bg.height):
        for x in range(bg.width):
            hue = math.atan2(y - center[1], x - center[0]) + math.pi
            distance = math.sqrt((y - center[1]) ** 2 + (x - center[0]) ** 2)
            lightness = lerp(100, 50, distance / (bg.width / 2))

            hsl = (math.degrees(hue), 100, lightness)
            color = ImageColor.getrgb('hsl({}, {}%, {}%)'.format(*hsl))
            data_list.append(color)

    bg.putdata(data_list)

    bg.save('bg.png')
    bg = mosaic(bg, qr.box_size)
    # bg.show()
    bg.save('bg_mosaic.png')

    result = ImageChops.darker(bg, qr_image)
    result.show()
    result.save('flag_qr.png', optimize=True)


if __name__ == '__main__':
    main()
