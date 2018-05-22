package com.vip.vipverify.net;

import org.json.JSONException;
import org.json.JSONObject;

public class VerifyLoginNetSocketData extends NetSocketData {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private String user_name = null;
	private String user_password = null;
	
	
	public VerifyLoginNetSocketData(String user_name, String user_password) {
		super();
		this.user_name = user_name;
		this.user_password = (user_password);
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
			json_content.put(Jsonkey.string_ctype_key, "verify_login_request");
			JSONObject json_cvalue = new JSONObject();
			json_content.put(Jsonkey.string_cvalue_key, json_cvalue);
			json_cvalue.put(Jsonkey.string_user_name_key, user_name);
			json_cvalue.put(Jsonkey.string_user_password_key, user_password);
			
			String json_string = json_object.toString();
			ret_byte = json_string.getBytes();
		} catch (JSONException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		return ret_byte;
	}

}
