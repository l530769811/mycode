package com.vip.vipverify.tcp_socket;

public class tcp_stream_pack {
	private byte[] revbuf = new byte[1024];
	private static int param = 50;

	public static byte[] pack_prepare(byte[] in_buf, int buf_len, int param) {
		// 锟斤拷锟斤拷锟斤拷莞锟绞�
		// 锟街斤拷锟斤拷: 2 4 4 x 4 1
		// 锟斤拷锟斤拷 : 'S:'+ 锟斤拷锟斤拷 + 锟斤拷锟斤拷 + 锟斤拷锟� + 锟斤拷锟斤拷锟斤拷 +'E'

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

	public byte[] pack_prepare(byte[] in_buf, int buf_len) {
		int param = 50;
		// 锟斤拷锟斤拷锟斤拷莞锟绞�
		// 锟街斤拷锟斤拷: 2 4 4 x 4 1
		// 锟斤拷锟斤拷 : 'S:'+ 锟斤拷锟斤拷 + 锟斤拷锟斤拷 + 锟斤拷锟� + 锟斤拷锟斤拷锟斤拷 +'E'

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

	public byte[] pack_parse(byte[] in_buf, int buf_len) {
		// 锟斤拷锟斤拷锟斤拷莞锟绞�
		// 锟街斤拷锟斤拷: 2 4 4 x 4 1
		// 锟斤拷锟斤拷 : 'S:'+ 锟斤拷锟斤拷 + 锟斤拷锟斤拷 + 锟斤拷锟� + 锟斤拷锟斤拷锟斤拷 +'E'
		byte[] out_buf = null;
		int pack_len = 0;
		if ((pack_len = _getPackLen(in_buf, buf_len)) != 0xffffffff) {
			out_buf = new byte[pack_len - 15];
			System.arraycopy(in_buf, 10, out_buf, 0, pack_len - 15);
		} else {
			out_buf = new byte[1];
		}

		return out_buf;
	}

}
