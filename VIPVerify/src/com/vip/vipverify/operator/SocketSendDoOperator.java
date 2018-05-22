package com.vip.vipverify.operator;

import com.vip.vipverify.net.NetSocketData;
import com.vip.vipverify.net.SocketProxy;

public class SocketSendDoOperator implements DoOperator {

	private NetSocketData data = null;
	private SocketProxy net_proxy = null;

	public SocketSendDoOperator(NetSocketData data, SocketProxy proxy) {
		super();
		this.data = data;
		this.net_proxy = proxy;
	}

	@Override
	public int ToDoOperate() {
		// TODO Auto-generated method stub
		int nret = 0;
		if (data != null && net_proxy != null) {
			nret = data.be_send(net_proxy);
		}
		return nret;
	}

}
