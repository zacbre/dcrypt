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
            Random r = new Random(Environment.TickCount);
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
            Console.WriteLine("Testing string encryption: \"Congrats\"");
            string encd = f.Encrypt("Test String");
            Console.WriteLine("Encrypted String: {0}", encd);
            Console.WriteLine("Decrypted String: {0}", f.Decrypt(encd));
            Console.ReadLine();
            //test encrypt 100mb
            w = DateTime.Now;
            Console.WriteLine("Generating 100mb of random data...");
            byte[] p = new byte[100000000];
            for (int i = 0; i < p.Length; i++)
            {
                p[i] = (byte)r.Next(0, 254);
            }
            Console.WriteLine("Took: {0}ms", DateTime.Now.Subtract(w).TotalMilliseconds);
            Console.ReadLine();

            w = DateTime.Now;
            Console.WriteLine("Encrypting 100mb test data...");
            byte[] enc1 = f.Encrypt(p, dcryptCompressionOptions.NoCompress);
            Console.WriteLine("Took: {0}ms", DateTime.Now.Subtract(w).TotalMilliseconds);
            Console.ReadLine();

            Console.WriteLine("Encrypting & Compressing 100mb test data...");
            w = DateTime.Now;
            byte[] enc = f.Encrypt(p, dcryptCompressionOptions.Compress);
            Console.WriteLine("Encrypted Byte Size: {0}b (~{1}mb)", enc.Length, (enc.Length / 1024) / 1024);
            Console.WriteLine("Took: {0}ms", DateTime.Now.Subtract(w).TotalMilliseconds);
            Console.ReadLine();

            w = DateTime.Now;
            Console.WriteLine("Decrypting 100mb of test data...");
            byte[] dec = f.Decrypt(enc1);
            Console.WriteLine("Took: {0}ms", DateTime.Now.Subtract(w).TotalMilliseconds);
            Console.ReadLine();

            w = DateTime.Now;
            Console.WriteLine("Decrypting 100mb of compressed test data...");
            byte[] dec1 = f.Decrypt(enc);
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
