package com.vip.vipverify.net;

import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.util.Enumeration;

import org.json.JSONException;
import org.json.JSONObject;

import android.util.Log;

public class SearchServerNetSocketData extends NetSocketData {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private String local_ip = "";
	private int local_port = -1;

	public SearchServerNetSocketData(String local_ip, int local_port) {
		super();
		this.local_ip = local_ip;
		this.local_port = local_port;
	}

	public static String getLocalIpAddress() {
		try {
			for (Enumeration<NetworkInterface> en = NetworkInterface.getNetworkInterfaces(); en.hasMoreElements();) {
				NetworkInterface intf = en.nextElement();
				for (Enumeration<InetAddress> enumIpAddr = intf.getInetAddresses(); enumIpAddr.hasMoreElements();) {
					InetAddress inetAddress = enumIpAddr.nextElement();
					if (!inetAddress.isLoopbackAddress()) {
						return inetAddress.getHostAddress().toString();
					}
				}
			}
		} catch (SocketException ex) {
			Log.e("WifiPreference IpAddress", ex.toString());
		}
		return null;
	}

	@Override
	protected byte[] prepare_data() {
		// TODO Auto-generated method stub
		byte[] ret_byte =  null;
		JSONObject json_object = new JSONObject();
		try {
			json_object.put(Jsonkey.string_transitionid_key, "");
			json_object.put(Jsonkey.string_magicid_key, "");
			JSONObject json_content = new JSONObject();
			json_object.put(Jsonkey.string_content_key, json_content);
			json_content.put(Jsonkey.string_ctype_key, "search_server_request");
			JSONObject json_cvalue = new JSONObject();
			json_content.put(Jsonkey.string_cvalue_key, json_cvalue);
			json_cvalue.put(Jsonkey.string_ip_key, local_ip);
			json_cvalue.put(Jsonkey.string_port_key, local_port);
			
			String json_string = json_object.toString();
			ret_byte = json_string.getBytes();
		} catch (JSONException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		return ret_byte;
	}

}
