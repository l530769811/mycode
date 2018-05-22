package com.vip.vipverify.net_data_parse;

import org.json.JSONException;
import org.json.JSONObject;

import com.vip.vipverify.my_arg.MyArg;
import com.vip.vipverify.net.Jsonkey;
import com.vip.vipverify.operator.DoOperator;

public class ServerInfoNetDataParse extends NetDataParse {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	public static final String ResponseSearchServerType = "search_server_reponse";

	private String server_name = "unknow";
	private String server_ip = "";
	private int server_port = -1;
	private int server_net_kind = -1;
	
	public ServerInfoNetDataParse(MyArg arg) {
		
	}

	public static String getResponsesearchservertype() {
		return ResponseSearchServerType;
	}

	public String getServer_name() {
		return server_name;
	}

	public String getServer_ip() {
		return server_ip;
	}

	public int getServer_port() {
		return server_port;
	}
	
	

	public int getServer_net_kind() {
		return server_net_kind;
	}

	public void setServer_net_kind(int server_net_kind) {
		this.server_net_kind = server_net_kind;
	}

	@Override
	protected boolean is_type(byte[] buf_data, int len) {
		// TODO Auto-generated method stub
		boolean bret = false;
		String string_data = new String(buf_data);
		try {
			JSONObject json_object = new JSONObject(string_data);
			JSONObject json_content = json_object.getJSONObject(Jsonkey.string_content_key);
			String string_type = "";
			if (json_content != null) {
				string_type = json_content.getString(Jsonkey.string_ctype_key);
			}

			bret = string_type.equalsIgnoreCase(ResponseSearchServerType);

		} catch (JSONException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		return bret;
	}

	@Override
	protected boolean parse_data(byte[] buf_data, int len) {
		// TODO Auto-generated method stub
		boolean bret = false;

		String str_data = new String(buf_data);
		try {
			JSONObject json_object = new JSONObject(str_data);
			JSONObject json_content = json_object.getJSONObject(Jsonkey.string_content_key);
			JSONObject json_cvalue = null;

			if (json_content != null) {
				json_cvalue = json_content.getJSONObject(Jsonkey.string_cvalue_key);
			}

			if (json_cvalue != null) {
				server_name = json_cvalue.getString(Jsonkey.string_servername_key);
				server_ip = json_cvalue.getString(Jsonkey.string_ip_key);
				server_port = json_cvalue.getInt(Jsonkey.string_port_key);
				server_net_kind = json_cvalue.getInt(Jsonkey.string_net_kind);
				bret = true;
			}
			
		} catch (JSONException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		return bret;
	}

	@Override
	public DoOperator createOperator() {
		// TODO Auto-generated method stub
		return null;
	}

}
