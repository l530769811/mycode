package com.vip.vipverify.client;

import java.io.IOException;
import java.io.Serializable;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;

import com.vip.vipverify.net.SocketReceiveListeningChannelList;
import com.vip.vipverify.operator.DoOperator;
import com.vip.vipverify.operator.UdpSendDoOperator;
import com.vip.vipverify.thread.OperateWakeThread;
import com.vip.vipverify.thread.WakeThread;

import android.content.Context;
import android.net.wifi.WifiManager;

public class UDPClient implements Runnable, Serializable {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	private DatagramSocket m_client = null;

	private int m_nport = 0;
	private String m_ip = null;
	private Thread rev_thread = null;
	private boolean isRevThreadOutside = false;

	private WakeThread m_threadSend = null;
	private boolean bisOutsideSendThread = false;
	private SocketReceiveListeningChannelList listener = null;

	private boolean bexit = false;

	private String tmp_ip = "";
	
	private Context context = null;
	private  WifiManager.MulticastLock multicastLock = null;

	public UDPClient(Context context, int m_nPort, Thread receive_thread, WakeThread threadSend,
			SocketReceiveListeningChannelList listener) {
		this.context = context;
		this.m_nport = m_nPort;
		this.m_threadSend = threadSend;
		this.rev_thread = receive_thread;
		if (this.rev_thread != null) {
			this.isRevThreadOutside = true;
		}
		this.listener = listener;
		if (this.m_threadSend != null)
			bisOutsideSendThread = true;
		
		WifiManager wm = (WifiManager)this.context.getSystemService(Context.WIFI_SERVICE);
		multicastLock = wm.createMulticastLock("lock");
		
	}

	public UDPClient(String ip, int m_nPort, WakeThread threadSend, SocketReceiveListeningChannelList listener) {
		this.m_nport = m_nPort;
		this.m_ip = ip;
		this.m_threadSend = threadSend;
		this.listener = listener;
		if (this.m_threadSend != null)
			bisOutsideSendThread = true;
	}

	@Override
	public void run() {
		// TODO Auto-generated method stub

		while (true) {
			if (bexit)
				break;

			byte[] rev_byte = new byte[1024];
		
			DatagramPacket rev_pack = null;
			InetAddress addr_rev = null;
			try {
				addr_rev = InetAddress.getByName("0.0.0.0");
				rev_pack = new DatagramPacket(rev_byte, rev_byte.length, addr_rev, ClientManager.udp_receiver_port);
			} catch (UnknownHostException e1) {
				// TODO Auto-generated catch block
				e1.printStackTrace();
			}
			
			try {
				 if(multicastLock!=null) {
					 multicastLock.acquire();
				 }
				 
				m_client.receive(rev_pack);
				
				 if(multicastLock!=null) {
					 multicastLock.release();
				 }
				 
				if (listener != null) {
					listener.RevData(rev_pack.getData(), rev_pack.getLength());
				}

			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
				 if(multicastLock!=null) {
					 multicastLock.release();
				 }
			}

		}

	}

	public boolean UDPClientInit() {

		if (m_client == null || m_client.isClosed()) {
			try {
	
				m_client = new DatagramSocket(m_nport, InetAddress.getByName("0.0.0.0"));
				m_client.setBroadcast(true);
				
//				 SocketAddress localAddr = new InetSocketAddress(InetAddress.getByName("0.0.0.0"), m_nport);
//				 m_client.bind(localAddr);

				
				
				if (rev_thread == null && isRevThreadOutside == false) {
					rev_thread = new Thread(this);
					rev_thread.start();
				}

				return true;

			} catch (SocketException | UnknownHostException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();

			}
		}
		return false;
	}

	public void UDPClientColse() {
		bexit = true;
		
		if (m_client != null && m_client.isClosed() == false)
			m_client.close();

		try {
			if (rev_thread != null && isRevThreadOutside == false) {
				rev_thread.join();
			}
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		if (bisOutsideSendThread == false) {
			if (m_threadSend != null) {
				m_threadSend.exit();
			}
		}
	}

	public int SendDataInQueue(String strIp, int port, byte[] bDataPack, int nSize) {
		int nret = -1;

		if (m_threadSend == null || (m_threadSend.isAlive() == false && bisOutsideSendThread == false)) {
			m_threadSend = new OperateWakeThread();
			m_threadSend.start();
		}

		DoOperator operate = new UdpSendDoOperator(m_client, strIp, port, bDataPack, nSize,
				false);

		if (m_threadSend.postOperate(operate) == true)
			nret = nSize;

		return nret;
	}

	public int SendDataPack(String strIp, int port, byte[] bDataPack, int nSize) {

		InetAddress send_addr = null;

		try {

			send_addr = InetAddress.getByName(strIp);

		} catch (UnknownHostException e1) {

			// TODO Auto-generated catch block

			e1.printStackTrace();
			return -1;
		}

		int len = bDataPack.length;
		DatagramPacket send_pack = new DatagramPacket(bDataPack, len, send_addr, port);

		int ret = -1;
		try {
			if(multicastLock!=null) {
				boolean a = multicastLock.isHeld();
				multicastLock.acquire();
				boolean b = multicastLock.isHeld();
			}

			m_client.send(send_pack);
			if(multicastLock!=null) {
				multicastLock.release();
			}
			
			ret = nSize;

		} catch (IOException e) {

			// TODO Auto-generated catch block
			e.printStackTrace();

			ret = -1;
		}

		return ret;
	}
}
