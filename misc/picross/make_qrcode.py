# -*- coding: utf-8 -*-
import qrcode


data = b'The first part is zsctf{PiCross_PuZZLe_'

qr = qrcode.QRCode(
    version=None,
    error_correction=qrcode.ERROR_CORRECT_M,
    box_size=1,
    border=0
)
qr.add_data(data=data, optimize=0)
img = qr.make_image(fill_color='black', back_color='white')
img.save('zsctf2020_qrcode.png')

qr.print_ascii()
