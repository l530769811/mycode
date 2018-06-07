package com.vip.vipverify.net_data_parse;

import org.json.JSONException;
import org.json.JSONObject;

import com.common.my_message.MessageSpreader;
import com.vip.vipverify.MyErrors;
import com.vip.vipverify.my_arg.MyArg;
import com.vip.vipverify.net.Jsonkey;
import com.vip.vipverify.operator.DoOperator;
import com.vip.vipverify.operator.MessageNotifyTextDoOperator;
import com.vip.vipverify.operator.MessageReponseDoOperator;

public class CardVerifyResultNetDataParse extends NetDataParse {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	public static final String string_transitionid = "transitionid";
	public static final String string_magicid = "magicid";
	public static final String string_content = "content";
	public static final String string_ctype = "ctype";
	public static final String string_cvalue_key = "cvalue";
	public static final String string_result_key = "nresult";
	public static final String string_strresult_key = "strresult";

	public static final int verify_suc_message_id = 0x1001;
	public static final int verify_fail_message_id = 0x1002;

	public static final String string_type_const = "card_verify_reponse";

	private MessageSpreader message_handle = null;
	private int msg_what = 0;
	private int nresult = -1;
	private String strResult = "";

	public CardVerifyResultNetDataParse(MyArg message_handle) {

		if (message_handle != null) {
			int[] int_args = message_handle.getArgInts();
			if(int_args!=null) {
				this.msg_what = int_args[1];
			}
			
			Object[] objs = message_handle.getArgObjects();
			if(objs!=null) {
				this.message_handle = (MessageSpreader)objs[0];
			}			
		}
	}

	public int getNresult() {
		return nresult;
	}

	public String getstrResult() {
		return strResult;
	}

	@Override
	protected boolean is_type(byte[] buf_data, int len) {
		// TODO Auto-generated method stub
		boolean bret = false;

		String str_data = new String(buf_data);
		try {
			JSONObject json_object = new JSONObject(str_data);
			JSONObject json_content = json_object.getJSONObject(string_content);

			String string_type = "";
			if (json_content != null)
				string_type = json_content.getString(string_ctype);

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
				try {
					strResult = json_cvalue.getString(Jsonkey.string_strresult_key);
				} catch (JSONException e) {
					// TODO: handle exception
				}
			}

			bret = true;
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

		do_operator = new MessageReponseDoOperator(message_handle, nresult, msg_what, strResult);

		return do_operator;
	}

}
