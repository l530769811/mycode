package com.vip.vipverify.net;

public class MyNetworkInfo {
	private String network_kind_string = "";
	private int network_kind = 0;// 0:tcp, 1:udp
	private String network_ip = "";
	private int network_port = -1;
	public MyNetworkInfo(int network_kind, String network_ip, int network_port, String network_kind_string) {
		super();
		this.network_kind = network_kind;
		this.network_ip = network_ip;
		this.network_port = network_port;
		this.network_kind_string = network_kind_string;
	}
	public int getNetwork_kind() {
		return network_kind;
	}
	public void setNetwork_kind(int network_kind) {
		this.network_kind = network_kind;
	}
	public String getNetwork_ip() {
		return network_ip;
	}
	public void setNetwork_ip(String network_ip) {
		this.network_ip = network_ip;
	}
	public int getNetwork_port() {
		return network_port;
	}
	public void setNetwork_port(int network_port) {
		this.network_port = network_port;
	}
	
	@Override
	public String toString() {
		// TODO Auto-generated method stub
		return network_kind_string;
	}
	
	
	
}
