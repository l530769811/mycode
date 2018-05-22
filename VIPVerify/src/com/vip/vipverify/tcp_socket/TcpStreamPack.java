package com.vip.vipverify.tcp_socket;

import java.io.*;
import java.util.ArrayList;
import java.util.List;

import android.util.Log;

import com.alibaba.fastjson.JSON;
//import org.json.JSONObject;
//import com.alibaba.fastjson.JSONObject;
import org.json.JSONException;
import org.json.JSONObject;

public class TcpStreamPack implements Serializable {

	private static final String TAG = "@_@";
	private final String TransitionID = new String("TransitionID");
	private final String MagicID = new String("MagicID");
	private final String Content = new String("Content");

	private int nMaxrevbufLen = 2048;
	private int nMaxRevNoFullPackLen = 2048;

	private byte[] revbuf = null;

	private byte[] revNoFullPack = null;
	private int nNoFullPackLen = 0;

	public TcpStreamPack(int nBuffLen) {
		nMaxrevbufLen = nBuffLen * 2;
		revbuf = new byte[nMaxrevbufLen];
		nMaxRevNoFullPackLen = nBuffLen * 2;
		revNoFullPack = new byte[nMaxRevNoFullPackLen];
	}

	public byte[] pack_prepare(byte[] in_buf, int buf_len, int param)
			throws UnsupportedEncodingException, JSONException {
		// 发送数据格式
		// 字节数: 2 4 4 x 4 1
		// 内容 : 'S:'+ 参数 + 长度 + 数据 + 检验码 +'E'

		String str_in = new String(in_buf, "UTF-8");

		JSONObject json_object = new JSONObject(str_in);
		// json_object = JSONObject.parseObject(str_in);

		JSONObject json_content = new JSONObject();
		json_content = json_object.getJSONObject(Content);
		// String str_content = json_content.toJSONString();
		String str_content = json_content.toString();

		JsonEncryptPack crypto_pack = new JsonEncryptPack();
		crypto_pack.setContent(str_content);
		crypto_pack.encrypt();

		String str_json = JSON.toJSONString(crypto_pack);

		in_buf = str_json.getBytes("UTF-8");
		buf_len = in_buf.length;

		int index = 0;
		byte[] out_buf = new byte[buf_len + 15];
		out_buf[index++] = 'S';
		out_buf[index++] = ':';

		out_buf[index++] = (byte) (param & 0xff);
		out_buf[index++] = (byte) (param >> 8 & 0xff);
		out_buf[index++] = (byte) (param >> 16 & 0xff);
		out_buf[index++] = (byte) (param >> 24 & 0xff);

		int pack_len = buf_len + 15;
		out_buf[index++] = (byte) (pack_len & 0xff);
		out_buf[index++] = (byte) (pack_len >> 8 & 0xff);
		out_buf[index++] = (byte) (pack_len >> 16 & 0xff);
		out_buf[index++] = (byte) (pack_len >> 24 & 0xff);

		System.arraycopy(in_buf, 0, out_buf, index, buf_len);
		index += buf_len;

		int ecode = 0;
		for (byte bit : in_buf) {
			ecode += bit;
		}

		out_buf[index++] = (byte) (ecode & 0xff);
		out_buf[index++] = (byte) (ecode >> 8 & 0xff);
		out_buf[index++] = (byte) (ecode >> 16 & 0xff);
		out_buf[index++] = (byte) (ecode >> 24 & 0xff);
		out_buf[index] = 'E';

		return out_buf;
	}

	public byte[] pack_prepare(byte[] in_buf, int buf_len) throws UnsupportedEncodingException, JSONException {
		// 发送数据格式
		// 字节数: 2 4 4 x 4 1
		// 内容 : 'S:'+ 参数 + 长度 + 数据 + 检验码 +'E'
		/*
		 * aes+base64 to encrpt send data;
		 */
		String str_in = new String(in_buf, "UTF-8");

		JSONObject json_object = new JSONObject(str_in);
		// json_object = JSONObject.parseObject(str_in);

		JSONObject json_content = new JSONObject();
		json_content = json_object.getJSONObject("content");
		String str_content = json_content.toString();

		JsonEncryptPack crypto_pack = new JsonEncryptPack();
		crypto_pack.setContent(str_content);
		crypto_pack.encrypt();

		String str_json = JSON.toJSONString(crypto_pack);

		in_buf = str_json.getBytes("UTF-8");
		buf_len = in_buf.length;

		final int param = 50;
		int index = 0;
		byte[] out_buf = new byte[buf_len + 15];

		out_buf[index++] = 'S';
		out_buf[index++] = ':';

		out_buf[index++] = (byte) (param & 0xff);
		out_buf[index++] = (byte) (param >> 8 & 0xff);
		out_buf[index++] = (byte) (param >> 16 & 0xff);
		out_buf[index++] = (byte) (param >> 24 & 0xff);

		int pack_len = buf_len + 15;
		out_buf[index++] = (byte) (pack_len & 0xff);
		out_buf[index++] = (byte) (pack_len >> 8 & 0xff);
		out_buf[index++] = (byte) (pack_len >> 16 & 0xff);
		out_buf[index++] = (byte) (pack_len >> 24 & 0xff);

		System.arraycopy(in_buf, 0, out_buf, index, buf_len);
		index += buf_len;

		int ecode = 0;
		for (byte bit : in_buf) {
			ecode += bit;
		}

		out_buf[index++] = (byte) (ecode & 0xff);
		out_buf[index++] = (byte) (ecode >> 8 & 0xff);
		out_buf[index++] = (byte) (ecode >> 16 & 0xff);
		out_buf[index++] = (byte) (ecode >> 24 & 0xff);
		out_buf[index] = 'E';

		return out_buf;
	}

	private int _getPackLen(byte[] in_buf, int len) {
		int ret_len = 0xffffffff;

		if (len < 10) {
			return 0xffffffff;
		}

		int index = 0;
		if (in_buf[index++] == 'S' && in_buf[index++] == ':') {
			index += 4;

			ret_len = ret_len & ((in_buf[index++] & 0xff) << 0);
			ret_len = ret_len | ((in_buf[index++] & 0xff) << 8);
			ret_len = ret_len | ((in_buf[index++] & 0xff) << 16);
			ret_len = ret_len | ((in_buf[index++] & 0xff) << 24);
		}

		return ret_len;
	}

	private void ByteArrayZero(byte[] bytes, int nlen) {
		for (int i = 0; i < nlen; i++)
			bytes[i] = 0;
	}

	private void CopySelf(byte[] in_buf, int in_len, int begin_src, int begin_des, int len) {
		ByteArrayZero(revbuf, nMaxrevbufLen);
		System.arraycopy(in_buf, 0, revbuf, 0, in_len);
		System.arraycopy(revbuf, begin_src, in_buf, begin_des, len);
	}

	private byte[] _pack_save(TcpData data) {
		// 接收数据格式
		// 字节数: 2 4 4 x 4 1
		// 内容 : 'S:'+ 参数 + 长度 + 数据 + 检验码 +'E'

		int pack_len = 0;
		byte[] ret_byte = null;
		if (data == null || data.getlen() <= 0)
			return null;
		byte[] in_buf = null;
		int buf_len = 0;
		in_buf = data.getData();
		buf_len = data.getlen();

		if ((pack_len = _getPackLen(in_buf, buf_len)) != 0xffffffff) {
			// receive buffer have the
			// head info, but no a full
			// pack;
			if (pack_len > buf_len || buf_len <= 10) {
				if (nNoFullPackLen <= 0)// only no has head info;
				{
					System.arraycopy(in_buf, 0, revNoFullPack, nNoFullPackLen, buf_len);
					nNoFullPackLen += buf_len;
					data.setlen(buf_len - buf_len);
					ByteArrayZero(in_buf, buf_len);
				} else {
					System.out.println("rev the head info and has head info");
				}
			} else if (pack_len == buf_len)// receive buffer is a pack;
			{
				ret_byte = new byte[pack_len + 1];
				ret_byte[pack_len] = 0;
				System.arraycopy(in_buf, 0, ret_byte, 0, pack_len);
				// ret_byte = in_buf.clone();
				data.setlen(buf_len - pack_len);
				CopySelf(in_buf, buf_len, pack_len, 0, buf_len - pack_len);
			} else // receive buffer more then a pack
			{
				ret_byte = new byte[pack_len + 1];
				ret_byte[pack_len] = 0;
				System.arraycopy(in_buf, 0, ret_byte, 0, pack_len);
				data.setlen(buf_len - pack_len);
				CopySelf(in_buf, buf_len, pack_len, 0, buf_len - pack_len);
			}
		} else {
			// has head info;
			if (nNoFullPackLen > 0) {
				try {
					System.arraycopy(in_buf, 0, revNoFullPack, nNoFullPackLen, buf_len);
					if ((pack_len = _getPackLen(revNoFullPack, buf_len + nNoFullPackLen)) != 0xffffffff) {
						ret_byte = new byte[pack_len + 1];
						ret_byte[pack_len] = 0;
						System.arraycopy(revNoFullPack, 0, ret_byte, 0, pack_len);
						ByteArrayZero(in_buf, buf_len);
						System.arraycopy(revNoFullPack, pack_len, in_buf, 0, buf_len + nNoFullPackLen - pack_len);
						data.setlen(buf_len + nNoFullPackLen - pack_len);
						ByteArrayZero(revNoFullPack, nMaxRevNoFullPackLen);
						nNoFullPackLen = 0;
					} else {
						Log.d(TAG, "has head info and join in it no a full pack");
					}
				} catch (ArrayIndexOutOfBoundsException e) {
					System.out.println("arraycopyexception");
				}

			} else {
				// byte[] b = in_buf;
				// int n = buf_len;
				// while((pack_len=_getPackLen(b, n))== 0xffffffff)
				// {
				// b = ;
				// }
				if (buf_len <= 10) {
					System.arraycopy(in_buf, 0, revNoFullPack, nNoFullPackLen, buf_len);
					nNoFullPackLen += buf_len;
					data.setlen(buf_len - buf_len);
					ByteArrayZero(in_buf, buf_len);
					Log.d(TAG, "rev the leave pack and no has head info, push into the revNoFullPack buff");
				} else {
					Log.d(TAG, "rev the leave pack and no has head info");
				}
			}
		}

		return ret_byte;
	}

	@SuppressWarnings("unused")
	public List<byte[]> pack_parse(byte[] in_buf, int buf_len) throws UnsupportedEncodingException, JSONException {
		// 接收数据格式
		// 字节数: 2 4 4 x 4 1
		// 内容 : 'S:'+ 参数 + 长度 + 数据 + 检验码 +'E'
		TcpData data = new TcpData(in_buf, buf_len);
		byte[] pack_buf = null;
		List<byte[]> pack_list = new ArrayList<byte[]>();
		while ((pack_buf = _pack_save(data)) != null) {
			int pack_len = 0;
			if ((pack_len = _getPackLen(pack_buf, pack_buf.length)) != 0xffffffff) {
				byte[] out_buf = null;
				out_buf = new byte[pack_len - 15];
				System.arraycopy(pack_buf, 10, out_buf, 0, pack_len - 15);
				/*
				 * decrypto
				 */

				String str_in = new String(out_buf, 0, pack_len - 15, "UTF-8");

				JsonDecryptPack decrypt_pack = new JsonDecryptPack();

				JSONObject json_object = new JSONObject(str_in);
				Log.d(TAG, "parse-json: " + str_in);
				// json_object = JSONObject.parseObject(str_in);
				String str_transitionid = null;
				try {
					str_transitionid = json_object.getString(TransitionID);
				} catch (JSONException e) {
					int debug = 0;

				}

				String str_magicID = null;
				try {
					str_magicID = json_object.getString(MagicID);
				} catch (JSONException e) {
					int debug = 0;
				}

				if ((str_transitionid != null && str_transitionid != "null")
						&& (str_magicID != null && str_magicID != "null")) {
					decrypt_pack.setTransitionID(str_transitionid);
					decrypt_pack.setMagicID(str_magicID);
					String str_content = json_object.getString(Content);

					decrypt_pack.decrypt(str_content);

					String str_out = JSON.toJSONString(decrypt_pack);
					Log.d(TAG, "decrypt json : " + str_out);
					out_buf = str_out.getBytes("UTF-8");
				}

				// /////////////////////////////////////////////////////
				pack_list.add(out_buf);
			}
		}

		return pack_list;

		// System.arraycopy(in_buf, pack_len, mRevBuf, destPos, length)

	}

}
