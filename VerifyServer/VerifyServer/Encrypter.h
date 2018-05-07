#ifndef _ENCRYPTER_H__
#define _ENCRYPTER_H__ 

class CEncrypter
{
public:
	CEncrypter(void);
	virtual ~CEncrypter(void);
	virtual unsigned char* Encrypt(const unsigned char *input_data, long len) = 0;
	virtual unsigned char* Decrypt(const unsigned char *input_data, long len) = 0;
};

#endif //_ENCRYPTER_H__