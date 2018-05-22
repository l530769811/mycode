package com.vip.vipverify.tcp_socket;

import Decoder.BASE64Decoder;

import com.alibaba.fastjson.JSON;
import com.alibaba.fastjson.JSONObject;
import java.io.IOException;
import java.security.InvalidKeyException;
import java.security.NoSuchAlgorithmException;

import javax.crypto.BadPaddingException;
import javax.crypto.Cipher;
import javax.crypto.IllegalBlockSizeException;
import javax.crypto.NoSuchPaddingException;
import javax.crypto.spec.SecretKeySpec;

public class JsonDecryptPack {
	private final String combinaSrc = new String("idverahs-opqlkjm");
	private String TransitionID = new String();
	private String MagicID = new String();
	private JSONObject Content = new JSONObject();

	private String key = new String();
	private final int key_len = 16;// key lenght 16;

	public JsonDecryptPack() {
	}

	@SuppressWarnings("unused")
	private String _decrypt(String content, String passwd) {
		try {
			Cipher cipher = Cipher.getInstance("AES/ECB/PKCS5Padding");// ����������
			SecretKeySpec key = new SecretKeySpec(passwd.getBytes(), "AES");
			cipher.init(Cipher.DECRYPT_MODE, key);// ��ʼ��
			byte[] result = new BASE64Decoder().decodeBuffer(content);
			String s = new String(result);
			return new String(cipher.doFinal(result)); // ����
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

	public void decrypt(String str_in) {
		if (TransitionID.isEmpty() == false && MagicID.isEmpty() == false) {
			int nmagicid = Integer.parseInt(MagicID);
			_combinaDecryptokey(nmagicid);
			String decrypt_str = _decrypt(str_in, key);
			Content = JSON.parseObject(decrypt_str);
		}
	}

	public String getTransitionID() {
		return TransitionID;
	}

	public void setTransitionID(String transitionID) {
		TransitionID = transitionID;
	}

	public String getMagicID() {
		return MagicID;
	}

	public void setMagicID(String magicID) {
		MagicID = magicID;
	}

	public JSONObject getContent() {
		return Content;
	}

	public void setContent(JSONObject content) {
		Content = content;
	}

}
