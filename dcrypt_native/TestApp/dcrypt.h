#pragma once

#ifndef DCRYPT_H
#define DCRYPT_H

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <time.h>
#include <string.h>

#pragma comment(lib, "dcrypt_native.lib")

using namespace std;

class dcrypt
{
	vector<vector<unsigned char> > key;
	int failed = 0;
public:
	~dcrypt(void){};
	dcrypt();
	dcrypt(char* filepath);

	unsigned char *Encrypt(unsigned char* data, int offset, int count);
	unsigned char *Decrypt(unsigned char* data, int offset, int count);
	int OutputKey(char *filepath);
	void NewKey();
	unsigned char *GetKey();
	int Error() { return failed; }
	void PrintHex(unsigned char* output, int count);
};

#endif