# -*- coding: utf-8 -*-
import base64

with open('../题目/flag.txt', 'r', encoding='utf-8') as f:
    s = f.read()

(_, encoded_flag, file_extension, data) = s.split('::')

# 根据 ENCODED 明文和密文计算出加密后的密码
encrypted_password = int(encoded_flag.split()[0]) - 1926081719260817 - ord('E') * 1145141919810
print(encrypted_password)

# 直接使用加密后的密码解密
decrypted = ''.join(
    chr((int(i) - encrypted_password - 1926081719260817) // 1145141919810)
    for i in data.split()
)

with open('./flag.jpg', 'wb') as f:
    f.write(base64.b64decode(decrypted))
