package com.vip.vipverify.preferences;

import java.util.Iterator;
import java.util.Map;

import android.content.Context;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.preference.PreferenceManager;

public class MyPreferences {
	public final static String preferences_cur_server_kind = "prefernces_cur_server_kind";
	public final static String preferences_spinner_server_tcp_kind = "preferences_spinner_server_tcp_kind";
	public final static String preferences_edittext_sever_tcp_ip = "preferences_edittext_sever_tcp_ip";
	public final static String preferences_edittext_sever_tcp_port = "preferences_edittext_sever_tcp_port";

	public final static String preferences_spinner_server_udp_kind = "preferences_spinner_server_udp_kind";
	public final static String preferences_edittext_sever_udp_ip = "preferences_edittext_sever_udp_ip";
	public final static String preferences_edittext_sever_udp_port = "preferences_edittext_sever_udp_port";

	private Context context = null;

	public MyPreferences(Context context, int resourceId) {
		super();
		this.context = context;
		PreferenceManager.setDefaultValues(context, resourceId, false);
	}

	public String readString(String key, String default_string) {
		String ret = default_string;
		SharedPreferences pre_share = PreferenceManager.getDefaultSharedPreferences(context);
		if (pre_share != null) {
			ret = pre_share.getString(key, default_string);
		}
		return ret;
	}

	public boolean writeString(String key, String write_string) {
		boolean bret = false;
		SharedPreferences pre_share = PreferenceManager.getDefaultSharedPreferences(context);
		if (pre_share != null) {
			Editor editor = pre_share.edit();
			if (editor != null) {
				editor.putString(key, write_string);
				bret = editor.commit();
			}
		}
		return bret;
	}

	public boolean readBoolean(String key, boolean default_boolean) {
		boolean bret = default_boolean;
		SharedPreferences pre_share = PreferenceManager.getDefaultSharedPreferences(context);
		if (pre_share != null) {
			bret = pre_share.getBoolean(key, default_boolean);
		}
		return bret;
	}

	public boolean writeBoolean(String key, boolean default_boolean) {
		boolean bret = false;
		SharedPreferences pre_share = PreferenceManager.getDefaultSharedPreferences(context);
		if (pre_share != null) {
			Editor editor = pre_share.edit();
			if (editor != null) {
				editor.putBoolean(key, default_boolean);
				bret = editor.commit();
			}
		}
		return bret;
	}

	public boolean writeStringList(Map<String, String> write_string_list) {
		boolean bret = false;
		SharedPreferences pre_share = PreferenceManager.getDefaultSharedPreferences(context);
		if (pre_share != null) {
			Editor editor = pre_share.edit();
			if (editor != null) {
				Iterator<String> itor = write_string_list.keySet().iterator();
				while (itor.hasNext()) {
					String write_string_key = itor.next();
					String write_string = write_string_list.get(write_string_key);
					editor.putString(write_string_key, write_string);
				}
				bret = editor.commit();
			}
		}
		return bret;
	}

}
