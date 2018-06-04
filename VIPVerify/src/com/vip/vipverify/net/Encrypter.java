package com.vip.vipverify.net;

public abstract class Encrypter {
	public abstract byte[] Encrypt(byte[] in_data, int len);
	public abstract byte[] Decrypt(byte[] in_data, int len);
}
