package com.vip.vipverify.net;

import org.json.JSONException;
import org.json.JSONObject;

import com.vip.vipverify.Md5Unit;
import com.vip.vipverify.client.CardRegistInfo;
import com.vip.vipverify.net_data_parse.CardRegistResultNetDataParse;
import com.vip.vipverify.net_data_parse.NetDataParsesCollection;

public class RegistCardNetSocketData extends NetSocketData {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private CardRegistInfo info = null;
	private boolean bPasswordEncrypt = true;

	public RegistCardNetSocketData(CardRegistInfo info, boolean bPasswordEncrypt) {
		super();
		this.info = info;
		this.bPasswordEncrypt = bPasswordEncrypt;
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
			json_content.put(Jsonkey.string_ctype_key,
					NetDataParsesCollection.getRequestType(CardRegistResultNetDataParse.class));
			JSONObject json_cvalue = new JSONObject();
			json_content.put(Jsonkey.string_cvalue_key, json_cvalue);
			if (info != null) {
				json_cvalue.put(Jsonkey.string_card_number, info.getString_card_number());
				if (bPasswordEncrypt) {
					json_cvalue.put(Jsonkey.string_card_password_key,
							Md5Unit.EncodeToMd5String(info.getString_password()));
				} else {
					json_cvalue.put(Jsonkey.string_card_password_key, (info.getString_password()));
				}
				
				if (bPasswordEncrypt) {
					json_cvalue.put(Jsonkey.string_card_password_encode_key,
							Md5Unit.EncodePasswordByCardnumber(info.getString_card_number(), info.getString_password()));
				}
				else
				{
					json_cvalue.put(Jsonkey.string_card_password_encode_key,
							Md5Unit.EncodeToMd5String(info.getString_password()));
				}
				json_cvalue.put(Jsonkey.string_card_user_name_key, info.getString_card_own());
				json_cvalue.put(Jsonkey.string_phone_number, info.getString_phone());
				json_cvalue.put(Jsonkey.string_sex, info.getN_sex());
				json_cvalue.put(Jsonkey.string_user_first_name, info.getString_first_name());
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
