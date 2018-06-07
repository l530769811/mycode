/**
 * 
 */
package com.vip.vipverify.net;

import org.json.JSONException;
import org.json.JSONObject;

import com.vip.vipverify.Md5Unit;
import com.vip.vipverify.client.CardVerifyInfo;

/**
 * @author Administrator
 *
 */
public class ConsumeCardNetSocketData extends NetSocketData {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private String strCardNumber = "";
	private String strCardPassword = "";
	
	
	
	public ConsumeCardNetSocketData(String strCardNumber, String strCardPassword) {

		this.strCardNumber = strCardNumber;
		this.strCardPassword = strCardPassword;
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
			json_content.put(Jsonkey.string_ctype_key, "consume_card_request");
			JSONObject json_cvalue = new JSONObject();
			json_content.put(Jsonkey.string_cvalue_key, json_cvalue);

			json_cvalue.put(Jsonkey.string_card_number, strCardNumber);
			json_cvalue.put(Jsonkey.string_card_password_key, Md5Unit.EncodeToMd5String(strCardPassword));

			String json_string = json_object.toString();
			ret_byte = json_string.getBytes();
		} catch (

		JSONException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		return ret_byte;
	}

}
