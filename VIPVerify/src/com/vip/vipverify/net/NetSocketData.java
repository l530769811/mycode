package com.vip.vipverify.net;

import java.io.Serializable;

public abstract class NetSocketData implements Serializable {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	protected abstract byte[] prepare_data();

	final public int be_send(SocketProxy socket_proxy) {
		int nret = -1;
		byte[] send_byte = prepare_data();
		if (socket_proxy != null && send_byte != null) {
			nret = socket_proxy.send_data(send_byte, send_byte.length);
		}
		return nret;
	}
}
