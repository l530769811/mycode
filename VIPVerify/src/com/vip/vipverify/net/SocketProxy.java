package com.vip.vipverify.net;

import java.io.Serializable;

public abstract class SocketProxy implements Serializable{
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	protected abstract int send_data(byte[] send_byte, int len);
}
