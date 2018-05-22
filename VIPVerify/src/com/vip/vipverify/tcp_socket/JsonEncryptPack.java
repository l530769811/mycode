package com.vip.vipverify.tcp_socket;

import java.io.IOException;
import java.security.InvalidKeyException;
import java.security.NoSuchAlgorithmException;
import java.util.Random;

import javax.crypto.BadPaddingException;
import javax.crypto.Cipher;
import javax.crypto.IllegalBlockSizeException;
import javax.crypto.NoSuchPaddingException;
import javax.crypto.spec.SecretKeySpec;


import Decoder.BASE64Decoder;
import Decoder.BASE64Encoder;
import android.util.Log;

public class JsonEncryptPack {
	private static final String TAG = "@_@";
	private final String combinaSrc = new String("idverahs-opqlkjm");
	private String TransitionID = new String("sharevdi20152016");
	private String MagicID = new String();
	private String Content = new String();

	private String key = new String(TransitionID);
	private final int key_len = 16;// key lenght 16;

	public JsonEncryptPack() {
		/*
		 * create key from TransitionID get th MagicID(whith int) count char at
		 * left concat the string from combinaSrc whith (16-MagicID) count char
		 * at right
		 */
		int nmagicid = 0;
		Random random = new Random(System.currentTimeMillis());
		nmagicid = Math.abs(random.nextInt()) % (key_len);
		if (nmagicid >= key_len - 2)
			nmagicid = key_len - 2;
		MagicID = String.valueOf(nmagicid);
		_combinaDecryptokey(nmagicid);
		;
	}

	public static String _encrypt(String content, String passwd) {
		try {
			Cipher aesECB = Cipher.getInstance("AES/ECB/PKCS5Padding");
			SecretKeySpec key = new SecretKeySpec(passwd.getBytes(), "AES");
			aesECB.init(Cipher.ENCRYPT_MODE, key);
			byte[] in_byte = content.getBytes();
			int n = in_byte.length;
			if (n < 48) {
				/*
				 * because the PKCS5Padding type will set to 16 multiple then
				 * add to the length most 48, the aesECB encrypt to 64 length;
				 * 64 length for base64 Encode
				 */

				byte[] tmp = new byte[48];
				System.arraycopy(in_byte, 0, tmp, 0, n);
				in_byte = tmp;
			}

			byte[] result = aesECB.doFinal(in_byte);

			return new BASE64Encoder().encode(result);
		} catch (NoSuchAlgorithmException e) {
			e.printStackTrace();
		} catch (NoSuchPaddingException e) {
			e.printStackTrace();
		} catch (InvalidKeyException e) {
			e.printStackTrace();
		} catch (IllegalBlockSizeException e) {
			e.printStackTrace();
		} catch (BadPaddingException e) {
			e.printStackTrace();
		}
		return null;
	}

	public static String _encrypt_b64(String content) {

		return new BASE64Encoder().encode(content.getBytes());

	}

	@SuppressWarnings("unused")
	private String _decrypt_b64(String content) {
		try {
			byte[] result = new BASE64Decoder().decodeBuffer(content);
			return new String(result, "UTF-8"); // ����
		} catch (IOException e) {
			e.printStackTrace();
		}
		return null;
	}

	private void _combinaDecryptokey(int nmagicid) {
		String s1 = combinaSrc.substring(nmagicid, key_len);
		String s2 = TransitionID.substring(0, nmagicid);
		key = s2.concat(s1);
	}

	public String getContent() {
		return Content;
	}

	public void setContent(String content) {
		Content = content;
		Log.d(TAG, "Content : " + Content);
	}

	public void encrypt() {
		Content = _encrypt(Content, key);
	}

	public String getTransitionID() {
		return TransitionID;
	}

	public String getMagicID() {
		return MagicID;
	}
}
