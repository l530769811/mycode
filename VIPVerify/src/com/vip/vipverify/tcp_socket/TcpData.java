package com.vip.vipverify.tcp_socket;

public class TcpData {
	private byte[] data = null;
	private int nlen = 0;

	public TcpData(byte[] data, int nlen) {
		super();
		this.data = data;
		this.nlen = nlen;
	}

	public byte[] getData() {
		return data;
	}

	public void setData(byte[] data) {
		this.data = data;
	}

	public int getlen() {
		return nlen;
	}

	public void setlen(int nlen) {
		this.nlen = nlen;
	}
}
