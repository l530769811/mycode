package com.vip.vipverify.net;

import org.json.JSONException;
import org.json.JSONObject;

public class UserSignupNetSocketData extends NetSocketData {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private UserRegistInfo info = null;

	public UserSignupNetSocketData(UserRegistInfo info) {
		super();
		this.info = info;
	}

	@Override
	protected byte[] prepare_data() {
		// TODO Auto-generated method stub
		byte[] ret_byte = null;
		JSONObject json_object = new JSONObject();
		try {
			json_object.put(Jsonkey.string_transitionid_key, "");
			json_object.put(Jsonkey.string_magicid_key, "");
			JSONObject json_content = new JSONObject();
			json_object.put(Jsonkey.string_content_key, json_content);
			json_content.put(Jsonkey.string_ctype_key, "user_regist_request");
			JSONObject json_cvalue = new JSONObject();
			json_content.put(Jsonkey.string_cvalue_key, json_cvalue);
			if (info != null) {
				json_cvalue.put(Jsonkey.string_user_name_key, info.getUser_name());
				json_cvalue.put(Jsonkey.string_user_password_key, info.getUser_password());
				json_cvalue.put(Jsonkey.string_phone_number, info.getUser_phone());
				json_cvalue.put(Jsonkey.string_identify_code, info.getIdentify_code());
			}

			String json_string = json_object.toString();
			ret_byte = json_string.getBytes();
		} catch (JSONException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		return ret_byte;
	}

}
