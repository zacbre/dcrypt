#include "dcrypt.h"
#define HEIGHT 256
#define WIDTH 256

dcrypt::dcrypt()
{
	this->NewKey();
}

dcrypt::dcrypt(char *filepath)
{
	//load file.
	srand(time(NULL));
	FILE *fil = fopen(filepath, "rb");
	if (fil)
	{
		int siz = 0;
		fseek(fil, 0, SEEK_END);
		siz = ftell(fil);
		rewind(fil);
		unsigned char* buf = (unsigned char*)malloc(sizeof(unsigned char)*siz);
		size_t read = fread(buf, sizeof(unsigned char), siz, fil);
		fclose(fil);
		if (this->DeserializeKey(buf, read))
			failed = 1;
		return;
	}
	failed = 1;
}

vector<vector<unsigned char> > dcrypt::populateGraph()
{
	key.resize(HEIGHT);
	for (int i = 0; i < HEIGHT; ++i)
		key[i].resize(WIDTH);

	for (int i = 0; i < HEIGHT; i++)
	{
		unsigned char* bytes = this->FillBytes(256);
		unsigned char* rbytes = this->rearrageBytes(bytes, 256);
		free(bytes);
		for (int j = 0; j < WIDTH; j++)
		{
			key[i][j] = rbytes[j];
		}
		free(rbytes);
	}
	return key;
}

void dcrypt::NewKey()
{
	srand(time(NULL));
	key = populateGraph();
}

int dcrypt::OutputKey(char *filename)
{
	unsigned char* x = SerializeKey();
	FILE *keyfile = fopen(filename, "wb");
	if (keyfile == NULL)
		return 1;
	fwrite(x, sizeof(unsigned char), 65536, keyfile);
	fclose(keyfile);
	free(x);
	return 0;
}

unsigned char* dcrypt::GetKey()
{
	return SerializeKey();
}

int dcrypt::DeserializeKey(unsigned char* data, int count)
{
	if (count != 65536) return 1;
	key.resize(HEIGHT);
	for (int i = 0; i < HEIGHT; ++i)
		key[i].resize(WIDTH);

	int j = 0;
	int p = 0;
	for (int i = 0; i < HEIGHT; i++)
	{
		//get all 256 bytes and reverse them you DUMB SHIT.
		unsigned char *buff = (unsigned char*)malloc(WIDTH);
		for (int j = 0; j < WIDTH; j++)
		{
			buff[j] = data[p];
			p++;
		}
		unsigned char *buf = this->Reverse(buff, WIDTH);
		free(buff);
		//somehow copy this shit into memory.
		for (int j = 0; j < WIDTH; j++)
		{
			key[i][j] = buf[j];
		}
		free(buf);
	}
	return 0;
}

unsigned char *dcrypt::SerializeKey()
{
	unsigned char *data = (unsigned char*)malloc(65536);
	int p = 0;
	for (int i = 1; i < HEIGHT+1; i++)
	{
		vector<unsigned char> xd = key[i-1];
		unsigned char* arr = (unsigned char*)malloc(WIDTH);
		memset(arr, 0, WIDTH);

		for (int i = 0; i < WIDTH; i++)
			arr[i] = xd[i];

		arr = this->Reverse(arr, WIDTH);
		for (int j = 0; j < WIDTH; j++)
		{
			data[p] = arr[j];			
			p++;
		}
		free(arr);
	}
	return data;
}

unsigned char *dcrypt::Encrypt(unsigned char* data, int offset, int count)
{
	unsigned char *enddata = (unsigned char*)malloc(count+1);
	memset(enddata, 0, count+1);
	int y = 0;
	int randchr = rand() % 255;
	unsigned char chr = key[randchr][randchr];
	for (int i = offset; i < count; i++)
	{
		chr = key[chr][data[i]];
		enddata[i] = chr;
	}
	enddata[count] = randchr;
	return enddata;
}

unsigned char *dcrypt::Decrypt(unsigned char* data, int offset, int count)
{
	unsigned char *newdata = (unsigned char*)malloc(count - 1);
	memset(newdata, 0, count - 1);
	unsigned char chr = data[count - 1];
	unsigned char xchr = 0;
	chr = key[chr][chr];
	for (int i = offset; i < count-1; i++)
	{
		xchr = data[i];
		for (int j = 0; j < WIDTH; j++)
		{
			if (key[chr][j] == xchr)
			{
				newdata[i] = j;
				chr = key[chr][j];
				break;
			}
		}
	}
	return newdata;
}

unsigned char *dcrypt::FillBytes(int count)
{
	unsigned char *bytes = (unsigned char*)malloc(count);
	for (int i = 0; i < count; i++)
		bytes[i] = i;
	return bytes;
}

unsigned char *dcrypt::Reverse(unsigned char *data, int count)
{
	unsigned char *bytes = (unsigned char*)malloc(count);
	for (int i = 0; i < count; i++)
	{
		bytes[i] = data[count - i - 1];
	}
	return bytes;
}

unsigned char *dcrypt::rearrageBytes(unsigned char* data, int count)
{
	unsigned char *x = (unsigned char*)malloc(count);
	memset(x, 0, count);
	int i = 0;
	while (true)
	{
		unsigned char p = data[rand() % (count)];
		if ((i - count) == 0)
		{
			x[i] = p;
			i++;
			continue;
		}
		if (memchr(x, p, i) == NULL)
		{
			x[i] = p;
			i++;
		}
		if (i == count)break;
	}
	return x;
}

void dcrypt::PrintHex(unsigned char* output, int count)
{
	for (int i = 0; i < count; i++)
	{
		if (i + 1 == count)
			printf("0x%02x\n", output[i]);
		else
			printf("0x%02x, ", output[i]);
	}
}