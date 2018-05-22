package com.vip.vipverify.net;

import com.vip.vipverify.client.UDPClient;

public class UdpSocketProxy extends SocketProxy {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private UDPClient socket_client = null;
	private String to_ip = "255.255.255.255";
	private int to_port = -1;

	public UdpSocketProxy(UDPClient socket_client, String to_ip, int to_port) {
		super();
		this.socket_client = socket_client;
		this.to_ip = to_ip;
		this.to_port = to_port;
	}

	@Override
	protected int send_data(byte[] send_byte, int len) {
		// TODO Auto-generated method stub
		if (socket_client != null) {
			socket_client.SendDataInQueue(to_ip, to_port, send_byte, len);
		}
		return 0;
	}

}
