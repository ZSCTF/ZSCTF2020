using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Threading;

namespace Decrypt
{
    class FileDecrypter
    {
        public string data;
        public string description;
        public string ipAddress;
        public string fileExtension;
        public ulong encryptedPassword;
        public string encryptedData;
        private const string FILE_HEADER = "#DEC_ENC";

        public FileDecrypter(string encryptedData)
        {
            this.encryptedData = encryptedData;
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

        public string DecryptString(string data, ulong encryptedPassword)
        {
            StringBuilder stringBuilder = new StringBuilder();
            string[] array = data.Split(new char[] { ' ' }, StringSplitOptions.RemoveEmptyEntries);
            foreach (string i in array)
            {
                ulong num = Convert.ToUInt64(i);
                char c = (char)((num - encryptedPassword - 12345678) / 19260817);
                stringBuilder.Append(c);
            }
            return stringBuilder.ToString();
        }

        public void DecryptHeader()
        {
            string[] array = encryptedData.Split(new string[] { "\r\n", "\n" }, StringSplitOptions.RemoveEmptyEntries);
            if (array.Length != 2)
            {
                throw new FormatException("文件格式不正确！");
            }
            string[] headers = array[0].Split(new string[] { "::" }, StringSplitOptions.None);
            if (headers.Length != 5 || headers[0] != FILE_HEADER)
            {
                throw new FormatException("文件格式不正确！");
            }
            ulong encryptedEmptyPassword = EncryptPassword("");
            description = DecryptString(headers[1], encryptedEmptyPassword);
            ipAddress = DecryptString(headers[2], encryptedEmptyPassword);
            fileExtension = DecryptString(headers[4], encryptedEmptyPassword);

        }

        public void DecryptBody(string password)
        {
            encryptedPassword = EncryptPassword(password);
            string[] array = encryptedData.Split(new string[] { "\r\n", "\n" }, StringSplitOptions.RemoveEmptyEntries);
            if (array.Length != 2)
            {
                throw new FormatException("文件格式不正确！");
            }
            string[] headers = array[0].Split(new string[] { "::" }, StringSplitOptions.None);
            if (headers.Length != 5 || headers[0] != FILE_HEADER)
            {
                throw new FormatException("文件格式不正确！");
            }
            if (DecryptString(headers[3], encryptedPassword) != "ENCODED")
            {
                throw new FormatException("密码错误！");
            }
            data = DecryptString(array[1], encryptedPassword);
        }

    }

    class Program
    {
        public static void Main(String[] args)
        {
            if (!File.Exists("flag.txt"))
            {
                Console.WriteLine("当前目录下不存在 flag.txt！");
                Thread.Sleep(2000);
                return;
            }

            string encryptedData = File.ReadAllText("flag.txt");
            FileDecrypter decrypter = new FileDecrypter(encryptedData);

            try
            {
                decrypter.DecryptHeader();
                Console.WriteLine("文件描述：" + decrypter.description);
                Console.WriteLine("IP地址：" + decrypter.ipAddress);
                Console.WriteLine("扩展名：" + decrypter.fileExtension);

                Console.Write("请输入密码：");
                string password = Console.ReadLine();

                decrypter.DecryptBody(password);
            }
            catch (FormatException e)
            {
                Console.WriteLine("解密失败：" + e.Message);
                Thread.Sleep(2000);
                return;
            }

            Console.WriteLine("解密成功！");
            byte[] data = Convert.FromBase64String(decrypter.data);
            File.WriteAllBytes("flag.jpg", data);
            Thread.Sleep(2000);
        }
    }
}
