package com.vip.vipverify.client;

public class MyUdpClientSocket extends MyClientSocket {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	public final static int net_client_code = 1;
	

	public MyUdpClientSocket(Thread rev_thread, SockketDataListening listrenling) {
		super(rev_thread);
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
	protected int how_send_data(byte[] data, int len, String addr, int port) {
		// TODO Auto-generated method stub
		return 0;
	}

	@Override
	protected int how_rev_data(byte[] data, int len, String addr, int port) {
		// TODO Auto-generated method stub
		return 0;
	}

}
