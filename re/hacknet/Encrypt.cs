using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

namespace Encrypt
{
    class FileEncrypter
    {
        public string data;
        public string description;
        public string ipAddress;
        public string password;
        public string fileExtension;
        public ulong encryptedPassword;
        private const string FILE_HEADER = "#DEC_ENC";

        public FileEncrypter()
        {

        }

        public ulong EncryptPassword(string password)
        {
            ulong hash1 = 5381;
            ulong hash2 = hash1;

            for (int i = 0; i < password.Length; i += 2)
            {
                hash1 = ((hash1 << 5) + hash1) ^ password[i];
                if (i == password.Length - 1)
                {
                    break;
                }
                hash2 = ((hash2 << 5) + hash2) ^ password[i + 1];
            }

            ulong hash = hash1 + hash2 * 1566083941;
            return hash % 333333333333;
        }

        public string Encrypt()
        {
            encryptedPassword = EncryptPassword(password);
            return EncryptHeader() + "\r\n" + EncryptBody();
        }

        public string EncryptString(string data, ulong encryptedPassword)
        {
            StringBuilder stringBuilder = new StringBuilder();
            foreach (char i in data)
            {
                ulong code = (ulong)i * 19260817 + 12345678 + encryptedPassword;
                stringBuilder.Append(code + " ");
            }
            return stringBuilder.ToString().Trim();
        }
        
        public string EncryptBody()
        {
            return EncryptString(data, encryptedPassword);
        }

        public string EncryptHeader()
        {
            ulong encryptedEmptyPassword = EncryptPassword("");
            StringBuilder stringBuilder = new StringBuilder();
            string encryptedHeader = string.Concat(new string[]
            {
                FILE_HEADER,
                "::",
                EncryptString(description, encryptedEmptyPassword),
                "::",
                EncryptString(ipAddress, encryptedEmptyPassword),
                "::",
                EncryptString("ENCODED", encryptedPassword),
                "::",
                EncryptString(fileExtension, encryptedEmptyPassword)
            });
            return encryptedHeader;
        }
    }

    class Program
    {
        public static void Main(String[] args)
        {
            byte[] data = File.ReadAllBytes(@".\题解\flag.jpg");
            string base64Data = Convert.ToBase64String(data);

            FileEncrypter encrypter = new FileEncrypter();

            encrypter.data = base64Data;
            encrypter.password = "SaN_X1aN-n1U-8I";
            encrypter.ipAddress = "33.33.33.33";
            encrypter.description = "全场AK的三线说：暴力破解你就输了";
            encrypter.fileExtension = ".png";

            string enc = encrypter.Encrypt();
            Console.WriteLine(encrypter.encryptedPassword); // 32289863206
            // Console.WriteLine(enc);
            File.WriteAllText(@".\题目\flag.txt", enc);
        }
    }
}
