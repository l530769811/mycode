package com.vip.vipverify.net;

public class ServerNetInfo {
	private String name = "unknow";
	private String ip = "";
	private int port = -1;
	private int net_kind = -1;

	public ServerNetInfo(String name, String ip, int port, int net_kind) {
		super();
		this.name = name;
		this.ip = ip;
		this.port = port;
		this.net_kind = net_kind;
	}

	public int getNet_kind() {
		return net_kind;
	}

	public void setNet_kind(int net_kind) {
		this.net_kind = net_kind;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public String getIp() {
		return ip;
	}

	public void setIp(String ip) {
		this.ip = ip;
	}

	public int getPort() {
		return port;
	}

	public void setPort(int port) {
		this.port = port;
	}

	@Override
	public String toString() {
		// TODO Auto-generated method stub
		return "(" + this.name + ")" + this.ip + " : " + String.valueOf(this.port);
	}

}
