#pragma once

#ifndef DCRYPT_H
#define DCRYPT_H

#ifdef MAKEDLL
#  define EXPORT __declspec(dllexport)
#else
#  define EXPORT __declspec(dllimport)
#endif

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <time.h>
#include <string.h>

using namespace std;

class dcrypt
{
	vector<vector<unsigned char> > key;
	public:
		EXPORT ~dcrypt(void){};
		EXPORT dcrypt();
		EXPORT dcrypt(char* filepath);
		
		EXPORT unsigned char *Encrypt(unsigned char* data, int offset, int count);
		EXPORT unsigned char *Decrypt(unsigned char* data, int offset, int count);
		EXPORT int OutputKey(char *filepath);
		EXPORT void NewKey();
		EXPORT unsigned char *GetKey();
		EXPORT int Error() { return failed; }
		EXPORT void PrintHex(unsigned char* output, int count);
		
	private:
		int failed = 0;
		unsigned char *rearrageBytes(unsigned char*, int);
		vector<vector<unsigned char> > populateGraph();
		unsigned char *SerializeKey();
		int DeserializeKey(unsigned char*, int);
		unsigned char *FillBytes(int);
		unsigned char *Reverse(unsigned char*, int);
		
};

#endif