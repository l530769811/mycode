package com.vip.vipverify.preferences;

import com.vip.vipverify.client.ClientManager;
import com.vip.vipverify.net.ServerNetInfo;

import android.content.Context;

public class MyLoginPreferences extends MyPreferences {

	public final static String preferences_login_user_name_key = "login_user_name";
	public final static String preferences_login_user_password_key = "login_user_password";
	public final static String preferences_login_remember_password = "login_remember_password";
	public final static String preferences_login_autologin = "login_auto_login";

	private boolean bRemPassword = false;
	private boolean bLoginAuto = false;

	private int server_net_kind = -1;
	private String server_ip = "0.0.0.0";
	private int server_port = -1;

	private String user_name = "";

	private String user_password = "";

	private ClientManager client_manager = null;

	public interface AutoLoginDoListening {
		public void ToAutoLogin(ServerNetInfo info, String user_name, String user_password);
	}

	public MyLoginPreferences(Context context, int resourceId, ClientManager client_manager) {
		super(context, resourceId);
		// TODO Auto-generated constructor stub
		this.client_manager = client_manager;
		_loadPreferenceSetting();
	}

	private void _loadPreferenceSetting() {
		user_name = this.readString(MyLoginPreferences.preferences_login_user_name_key, "");
		user_password = this.readString(MyLoginPreferences.preferences_login_user_password_key, "");
		bRemPassword = this.readBoolean(MyLoginPreferences.preferences_login_remember_password, false);
		bLoginAuto = this.readBoolean(MyLoginPreferences.preferences_login_autologin, false);

		String cur_kind_string = this.readString(MyPreferences.preferences_cur_server_kind, "0");
		switch (server_net_kind = Integer.valueOf(cur_kind_string)) {
		case 0:
			server_ip = this.readString(MyPreferences.preferences_edittext_sever_tcp_ip, "000.000.000.000");
			server_port = Integer.valueOf(this.readString(MyPreferences.preferences_edittext_sever_tcp_port, "000"));
			break;
		case 1:
			server_ip = this.readString(MyPreferences.preferences_edittext_sever_udp_ip, "111.111.111.111");
			server_port = Integer.valueOf(this.readString(MyPreferences.preferences_edittext_sever_udp_port, "111"));
			break;
		default:
			break;

		}
	}

	public int getServer_net_kind() {
		return server_net_kind;
	}

	public void setServer_net_kind(int server_net_kind) {
		this.server_net_kind = server_net_kind;
	}

	public String getServerIp() {
		return server_ip;
	}

	public int getServerPort() {
		return server_port;
	}
	
	public String getUser_name() {
		return user_name;
	}

	public String getUser_password() {
		return user_password;
	}


	public void setRememberPassword(boolean bIsRem, String user_name, String user_password) {
		this.bRemPassword = bIsRem;
		this.user_name = user_name;
		this.user_password = user_password;
	}

	public void setAutoLogin(boolean bIsAuto, String user_name, String user_password) {
		bLoginAuto = bIsAuto;
		if (bLoginAuto == true) {
			bRemPassword = true;
		}
		this.user_name = user_name;
		this.user_password = user_password;
	}

	public boolean isAutoLogin() {
		return this.bLoginAuto;
	}
	
	public boolean isRemember() {
		return this.bRemPassword;
	}
	

	public void login_result(ServerNetInfo info, String user_name, String user_password, boolean isLogin) {
		if (isLogin) {
			this.user_name = user_name;
			this.user_password = user_password;
			saveUserName();
			saveAutoLoginPreference();
			saveRememberPassword();

		} else {
			saveUserName();
		}
	}

	public void saveUserName() {
		String name = this.readString(preferences_login_user_name_key, "");
		if (name.equalsIgnoreCase(user_name) == false) {
			if (user_name.isEmpty() == false) {
				this.writeString(preferences_login_user_name_key, user_name);
			}
		}

	}

	public void saveRememberPassword() {
		boolean temp = this.readBoolean(preferences_login_remember_password, false);
		if (temp != this.bRemPassword) {
			this.writeBoolean(preferences_login_remember_password, this.bRemPassword);
		}
		
		if (this.bRemPassword) {
			this.writeString(preferences_login_user_password_key, user_password);
		} else {
			this.writeString(preferences_login_user_password_key, "");
		}
	}

	public void saveAutoLoginPreference() {
		boolean tempAutoLogin = this.readBoolean(preferences_login_autologin, false);
		if (tempAutoLogin != this.bLoginAuto) {
			this.writeBoolean(preferences_login_autologin, this.bLoginAuto);
		}
	}

	public void autoLogin(AutoLoginDoListening listener) {
		if (listener != null) {
			if (server_ip == null || server_ip.isEmpty()) {
				return;
			}

			if (server_port <= 0) {
				return;
			}

			if (user_name == null || user_name.isEmpty()) {
				return;
			}

			if (user_password == null || user_password.isEmpty()) {
				return;
			}

			if (isAutoLogin() == false) {
				return;
			}

			listener.ToAutoLogin(new ServerNetInfo("unknow", server_ip, server_port, server_net_kind), this.user_name,
					this.user_password);

		}
	}

}
