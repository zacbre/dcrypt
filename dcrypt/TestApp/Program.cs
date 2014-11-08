using DCrypt;
using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace TestApp
{
    class Program
    {
        static void Main(string[] args)
        {
            DateTime w = DateTime.Now;
            dcrypt f = new dcrypt();
            byte[] x = new byte[] { 0x02, 0x04, 0x06, 0x08, 0x10 };
            Console.WriteLine("{0}: {1}", "Original", toByte(x));
            byte[] m = f.Encrypt(x);
            Console.WriteLine("{0}: {1}", "Encrypted", toByte(m));
            byte[] l = f.Decrypt(m);
            Console.WriteLine("{0}: {1}", "Decrypted", toByte(l));
            Console.WriteLine("Took: {0}ms", DateTime.Now.Subtract(w).TotalMilliseconds);
            Console.ReadLine();
        }
        static string toByte(byte[] bytes)
        {
            string x = "";
            for (int i = 0; i < bytes.Length; i++)
            {
                
                if (i < bytes.Length - 1)
                    x += dcrypt.ToHex((int)bytes[i]) + ", ";
                else
                    x += dcrypt.ToHex((int)bytes[i]);
            }
            return x;
        }
    }
}
