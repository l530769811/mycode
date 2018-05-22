
package com.vip.vipverify.operator;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.UnknownHostException;

public class UdpSendDoOperator implements DoOperator {
	
	private DatagramSocket udpsocket = null;
	private String strIp = "0,0,0,0";
	private int socket_port = -1;
	private byte[] send_data_pack;
	private int nlen;
	private boolean bneed_connect = false;
	
	public UdpSendDoOperator(DatagramSocket udpsocket, 
			String strIp, 
			int socket_port,
			byte[] send_data_pack,
			int nlen,
			boolean bconnect) {
		
		this.udpsocket = udpsocket;
		this.strIp = strIp;
		this.socket_port = socket_port;
		this.send_data_pack = send_data_pack;
		this.nlen = nlen;
		this.bneed_connect = bconnect;
	}

	@Override
	public int ToDoOperate() {
		// TODO Auto-generated method stub
		if(udpsocket==null)
			return -1;
		
		InetAddress send_addr = null;

		try {

			send_addr = InetAddress.getByName(strIp);

		} catch (UnknownHostException e1) {

			// TODO Auto-generated catch block

			e1.printStackTrace();

			return -1;
		}

	
		int len = send_data_pack.length;
		DatagramPacket send_pack = null;
		if(bneed_connect==false) {
			if(udpsocket.isConnected())
				udpsocket.disconnect();
			send_pack = new DatagramPacket(send_data_pack, len, send_addr, socket_port);
		}
		else
		{
			udpsocket.connect(send_addr, socket_port);			
			send_pack = new DatagramPacket(send_data_pack, len);
		}
		
		int ret = -1;
		try {
			udpsocket.send(send_pack);
			ret = len;
		} catch (IOException e) {

			// TODO Auto-generated catch block
			e.printStackTrace();
			ret = -1;
		}

		return ret;
	}

}
