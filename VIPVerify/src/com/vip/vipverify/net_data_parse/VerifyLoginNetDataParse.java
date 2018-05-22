package com.vip.vipverify.net_data_parse;

import org.json.JSONException;
import org.json.JSONObject;

import com.vip.vipverify.my_arg.MyArg;
import com.vip.vipverify.net.Jsonkey;
import com.vip.vipverify.operator.DoOperator;

public class VerifyLoginNetDataParse extends NetDataParse {
	public static final String ResponseSearchServerType = "verify_login_reponse";
	private int nresult = -1;
	private String str_string = "";

	public VerifyLoginNetDataParse(MyArg arg) {
		// TODO Auto-generated constructor stub
	}

	public int getNresult() {
		return nresult;
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
				nresult = json_cvalue.getInt(Jsonkey.string_result_key);
				try {
					str_string = json_cvalue.getString(Jsonkey.string_result_info_key);
				} catch (Exception e) {
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
		return null;
	}

}
