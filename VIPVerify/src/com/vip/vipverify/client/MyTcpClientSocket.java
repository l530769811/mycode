package com.vip.vipverify.client;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.Serializable;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketAddress;
import java.nio.channels.SocketChannel;
import java.util.Iterator;
import java.util.List;

import com.vip.vipverify.net.AnalyzeNetData;
import com.vip.vipverify.net.AnalyzeTcpNetData;
import com.vip.vipverify.net.AnalyzeTcpNetData.TcpAnalyzeRunningListener;
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
	private AnalyzeNetData analyzer = null;
	private ConnectResultListening connect_listener = null;
	private SockketDataListening listrenling = null;


	public MyTcpClientSocket(Thread rev_thread, SockketDataListening listrenling,
			ConnectResultListening connect_listener, AnalyzeNetData analyzer) {
		super(rev_thread);
		// TODO Auto-generated constructor stub
		this.listrenling = listrenling;
		this.connect_listener = connect_listener;
		this.analyzer = analyzer;
	}

	@Override
	public boolean how_connect_socket(String addr, int port) {
		// TODO Auto-generated method stub
		boolean bret = false;

		int connect_times = 0;
		while (connect_times <= 4) {
			try {

				SocketAddress socketAddress = new InetSocketAddress(addr, port);
				if (socketClient != null) {
					socketClient.connect(socketAddress, 5000);
					out = new DataOutputStream(socketClient.getOutputStream());
					in = new DataInputStream(socketClient.getInputStream());
					bret = true;
					if (this.connect_listener != null) {
						this.connect_listener.connect_result(true, addr, port);
					}
					break;
				}

			} catch (Exception e) {
				// TODO: handle exception
				if (!(e instanceof IOException)) {
					how_disconnect_socket();
					connect_times = 5;
				}
				if(connect_times>=4)
				if (this.connect_listener != null) {
					this.connect_listener.connect_result(false, addr, port);
				}
			}
			connect_times++;
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

			}

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
			byte[] send_data = null;
			try {
				if (analyzer != null) {
					send_data = analyzer.unanalyze(data, len);
				}
				if (send_data != null) {
					out.write(send_data);
					if ( listrenling!=null )
					{
						listrenling.send_data(send_data, send_data.length);
					}
					nlen = send_data.length;
				}

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
				if (analyzer != null) {
					List<byte[]> listPack = analyzer.analyze(rev_data_buff, len);
					Iterator<byte[]> it = listPack.iterator();
					while(it.hasNext())
					{
						byte[] pack_data = it.next();
						if ( listrenling!=null )
						{
							listrenling.rev_data(pack_data, pack_data.length);
						}
					}
				}
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();

			}
		}
		return length;
	}

}
