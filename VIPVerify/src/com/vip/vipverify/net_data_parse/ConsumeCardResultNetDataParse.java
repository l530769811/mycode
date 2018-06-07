package com.vip.vipverify.net_data_parse;

import org.json.JSONException;
import org.json.JSONObject;

import com.common.my_message.MessageSpreader;
import com.vip.vipverify.my_arg.MyArg;
import com.vip.vipverify.net.Jsonkey;
import com.vip.vipverify.operator.DoOperator;
import com.vip.vipverify.operator.MessageReponseDoOperator;

public class ConsumeCardResultNetDataParse extends NetDataParse {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	public static final String string_type_const = "consume_card_reponse";

	private MessageSpreader message_handle = null;

	private int nresult = -1;
	private String strResult = "";
	private int msg_what = 0;

	public ConsumeCardResultNetDataParse(MyArg arg) {
		if (arg != null) {
			
			int[] int_args = arg.getArgInts();
			if(int_args!=null) {
				this.msg_what = int_args[1];
			}
			
			Object[] objs = arg.getArgObjects();
			if(objs!=null) {
				this.message_handle = (MessageSpreader)objs[0];
			}
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
				try {
					strResult = json_cvalue.getString(Jsonkey.string_strresult_key);
				} catch (JSONException e) {
					// TODO: handle exception
				}
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

		do_operator = new MessageReponseDoOperator(message_handle, nresult, msg_what, strResult);

		return do_operator;
	}
}
