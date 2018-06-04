package com.vip.vipverify.net_data_parse;

import org.json.JSONException;
import org.json.JSONObject;

import com.common.my_message.MessageSpreader;
import com.vip.vipverify.my_arg.MyArg;
import com.vip.vipverify.net.Jsonkey;
import com.vip.vipverify.operator.DoOperator;
import com.vip.vipverify.operator.MessageReponseDoOperator;

public class CardRegistResultNetDataParse extends NetDataParse {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	
	public static final int regist_suc_message_id = 0x1001;
	public static final int regist_fail_message_id = 0x1002;

	public static final String string_type_const = "card_regist_reponse";

	private MessageSpreader message_handle = null;

	private int nresult = -1;
	private String strResult = null;

	public CardRegistResultNetDataParse(MyArg message_handle) {
		super();
		if (message_handle != null) {
			this.message_handle = (MessageSpreader) message_handle.getArgObject();
		}
	}

	@Override
	protected boolean is_type(byte[] buf_data, int len) {
		// TODO Auto-generated method stub
		boolean bret = false;

		String str_data = new String(buf_data);
		try {
			JSONObject json_object = new JSONObject(str_data);
			JSONObject json_content = json_object.getJSONObject(Jsonkey.string_content_key);

			String string_type = "";
			if (json_content != null) {
				string_type = json_content.getString(Jsonkey.string_ctype_key);
			}

			bret = string_type.equalsIgnoreCase(string_type_const);
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

			if (json_content != null)
				json_cvalue = json_content.getJSONObject(Jsonkey.string_cvalue_key);

			if (json_cvalue != null) {
				nresult = json_cvalue.getInt(Jsonkey.string_result_key);
				strResult = json_cvalue.getString(Jsonkey.string_strresult_key);
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
		DoOperator do_operator = null;
		if (nresult == 0) {
			do_operator = new MessageReponseDoOperator(message_handle, regist_suc_message_id, null);
		} else {
			do_operator = new MessageReponseDoOperator(message_handle, regist_fail_message_id, strResult);
		}
		return do_operator;
	}

}
