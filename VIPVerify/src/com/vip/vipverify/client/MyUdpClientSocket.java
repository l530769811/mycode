package com.vip.vipverify.client;

import com.vip.vipverify.thread.WakeThread;

public class MyUdpClientSocket extends MyClientSocket {
	public final static int net_client_code = 1;
	

	public MyUdpClientSocket(Thread rev_thread, WakeThread send_thread, SockketDataListening listrenling) {
		super(rev_thread, send_thread, listrenling);
		// TODO Auto-generated constructor stub
	}

	@Override
	public boolean how_connect_socket(String addr, int port) {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public boolean how_disconnect_socket() {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public boolean how_close_socket() {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public boolean how_init_socket() {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	protected int how_send_data(byte[] data, int len) {
		// TODO Auto-generated method stub
		return 0;
	}

	@Override
	protected int how_rev_data(byte[] data, int len) {
		// TODO Auto-generated method stub
		return 0;
	}

}
