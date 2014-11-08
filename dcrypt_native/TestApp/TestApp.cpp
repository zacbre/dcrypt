#include "dcrypt.h"

int main(int argc, char* argv[])
{
	dcrypt p = dcrypt();
	if (!p.Error())
	{
		unsigned char data[] = { 0x02, 0x04, 0x06, 0x08, 0x10 };
		printf("Original: ");
		p.PrintHex(data, 5);
		unsigned char* enc = p.Encrypt(data, 0, 5);
		printf("Encrypted: ");
		p.PrintHex(enc, 6);
		unsigned char* dec = p.Decrypt(enc, 0, 6);
		printf("Decrypted: ");
		p.PrintHex(dec, 5);
	}
	getchar();
	return 0;
}

