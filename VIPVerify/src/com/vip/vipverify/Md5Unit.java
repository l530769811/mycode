package com.vip.vipverify;

import java.security.MessageDigest;

public class Md5Unit {
	static final String string_extra = "lichengqian";
	private static final char[] HEX_DIGITS = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd',
			'e', 'f' };

	/**
	 * Takes the raw bytes from the digest and formats them correct.
	 *
	 * @param bytes
	 *            the raw bytes from the digest.
	 * @return the formatted bytes.
	 */
	private static String getFormattedText(byte[] bytes) {
		int len = bytes.length;
		StringBuilder buf = new StringBuilder(len * 2);
		// 把密文转换成十六进制的字符串形式
		for (int j = 0; j < len; j++) {
			buf.append(HEX_DIGITS[(bytes[j] >> 4) & 0x0f]);
			buf.append(HEX_DIGITS[bytes[j] & 0x0f]);
		}
		return buf.toString();
	}

	public static String EncodeToMd5String(String string_dst) {
		String str_encode = new String();

		if (string_dst == null || string_dst.isEmpty()) {
			return "000000";
		}

		String algorithm = "MD5";
		try {
			MessageDigest messageDigest = MessageDigest.getInstance(algorithm);
			String str_be = string_dst;
			messageDigest.update(str_be.getBytes());
			str_encode = getFormattedText(messageDigest.digest());
		} catch (Exception e) {
			// TODO: handle exception
			throw new RuntimeException(e);
		}

		return str_encode;
	}

	public static String EncodePasswordByCardnumber(String cardnumber, String strpassword) {
		String str_encode = new String();

		if (cardnumber == null || cardnumber.isEmpty() || strpassword == null || strpassword.isEmpty()) {
			return "000000";
		}

		String algorithm = "MD5";
		try {
			MessageDigest messageDigest = MessageDigest.getInstance(algorithm);
			String str_be = cardnumber + strpassword;
			messageDigest.update(str_be.getBytes());
			str_encode = getFormattedText(messageDigest.digest());
		} catch (Exception e) {
			// TODO: handle exception
			throw new RuntimeException(e);
		}

		return str_encode;
	}
}
