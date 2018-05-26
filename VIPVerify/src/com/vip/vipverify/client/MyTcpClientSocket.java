package com.vip.vipverify.client;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.Serializable;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketAddress;
import java.nio.channels.SocketChannel;
import com.vip.vipverify.thread.WakeThread;

public class MyTcpClientSocket extends MyClientSocket implements Serializable {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private Socket socketClient = null;
	private DataInputStream in = null;
	private DataOutputStream out = null;
	public final static int net_client_code = 0;

	public MyTcpClientSocket(Thread rev_thread, SockketDataListening listrenling, ConnectResultListening connect_listener) {
		super(rev_thread, listrenling, connect_listener);
		// TODO Auto-generated constructor stub
	}

	@Override
	public boolean how_connect_socket(String addr, int port) {
		// TODO Auto-generated method stub
		boolean bret = false;

		// if (socketClient != null && socketClient.isClosed() == false) {
		// try {
		// socketClient.close();
		// } catch (IOException e) {
		// // TODO Auto-generated catch block
		// e.printStackTrace();
		// }
		// }

		try {

			SocketAddress socketAddress = new InetSocketAddress(addr, port);
			if (socketClient != null) {
				socketClient.connect(socketAddress);
				out = new DataOutputStream(socketClient.getOutputStream());
				in = new DataInputStream(socketClient.getInputStream());
				bret = true;
			}

		} catch (Exception e) {
			// TODO: handle exception
			if (!(e instanceof IOException)) {
				how_disconnect_socket();
			}

		}
		return bret;
	}

	@Override
	public boolean how_disconnect_socket() {
		// TODO Auto-generated method stub
		boolean bret = false;
		try {
			// flag = false;
			if (socketClient != null) {
				// Toast.makeText(getApplicationContext(),
				// "socket_closeConnect()", Toast.LENGTH_LONG).show();
				// socket.shutdownOutput();
				// socket.shutdownInput();
				// if (socketClient.isClosed() == false)
				// socketClient.close();
				if (socketClient.isConnected()) {
					SocketChannel channel = socketClient.getChannel();
					if (channel != null) {
						channel.close();
					}

					if (out != null) {
						out.close();
						out = null;
					}

					if (in != null) {
						in.close();
						in = null;
					}
				}

				// .purge();
				// Heart.cancel();
			}
			//
			// if (out != null) {
			// out.close();
			// out = null;
			// }
			//
			// if (in != null) {
			// in.close();
			// in = null;
			// }

			bret = true;

		} catch (IOException e) {
			e.printStackTrace();
		}

		return bret;
	}

	@Override
	public boolean how_close_socket() {
		// TODO Auto-generated method stub
		boolean bret = false;
		try {
			// flag = false;
			if (socketClient != null) {
				if (socketClient.isClosed() == false)
					socketClient.close();
			}

			bret = true;

		} catch (IOException e) {
			e.printStackTrace();
		}
		return bret;
	}

	@Override
	public boolean how_init_socket() {
		// TODO Auto-generated method stub
		boolean bret = false;

		if (socketClient != null && socketClient.isClosed() == false) {
			try {
				socketClient.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}

		try {
			socketClient = new Socket();

			bret = true;
		} catch (Exception e) {
			// TODO: handle exception
			how_close_socket();

		}

		return bret;
	}

	@Override
	protected int how_send_data(byte[] data, int len) {
		// TODO Auto-generated method stub
		int nlen = -1;
		if (out != null) {

			try {
				out.write(data);
				nlen = len;
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}

		return nlen;
	}

	@Override
	protected int how_rev_data(byte[] rev_data_buff, int len) {
		// TODO Auto-generated method stub
		int length = -1;
		if (in != null) {

			try {
				length = in.read(rev_data_buff);
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();

			}
		}
		return length;
	}

}
