package com.vip.vipverify.net;

import org.json.JSONException;
import org.json.JSONObject;

import com.vip.vipverify.client.CardVerifyInfo;
import com.vip.vipverify.net_data_parse.CardVerifyResultNetDataParse;
import com.vip.vipverify.net_data_parse.NetDataParsesCollection;

public class VerifyCardNetSocketData extends NetSocketData {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private CardVerifyInfo info = null;

	public VerifyCardNetSocketData(CardVerifyInfo info) {
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
			json_content.put(Jsonkey.string_ctype_key, NetDataParsesCollection.getRequestType(CardVerifyResultNetDataParse.class));
			JSONObject json_cvalue = new JSONObject();
			json_content.put(Jsonkey.string_cvalue_key, json_cvalue);
			if (info != null) {
				json_cvalue.put(Jsonkey.string_card_number, info.getString_card_number());
				json_cvalue.put(Jsonkey.string_card_password_key, info.getString_card_password());
				
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
