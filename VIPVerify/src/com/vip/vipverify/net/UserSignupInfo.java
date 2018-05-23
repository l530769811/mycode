package com.vip.vipverify.net;

import com.vip.vipverify.Md5Unit;

public class UserSignupInfo {
	private String user_name = "";
	private String user_password = "";
	private String user_verify_password = "";
	private String user_phone = "";
	private String identify_code = "000000";
	
	
	public UserSignupInfo(String user_name, String user_password, String user_verify_password, String user_phone,
			String identify_code) {
		super();
		this.user_name = user_name;
		this.user_password = Md5Unit.EncodeToMd5String(user_password);
		this.user_verify_password = Md5Unit.EncodeToMd5String(user_verify_password);
		this.user_phone = user_phone;
		this.identify_code = identify_code;
	}
	
	public String getUser_name() {
		return user_name;
	}
	public void setUser_name(String user_name) {
		this.user_name = user_name;
	}
	public String getUser_password() {
		return user_password;
	}
	public void setUser_password(String user_password) {
		this.user_password = user_password;
	}
	public String getUser_verify_password() {
		return user_verify_password;
	}
	public void setUser_verify_password(String user_verify_password) {
		this.user_verify_password = user_verify_password;
	}
	public String getUser_phone() {
		return user_phone;
	}
	public void setUser_phone(String user_phone) {
		this.user_phone = user_phone;
	}
	public String getIdentify_code() {
		return identify_code;
	}
	public void setIdentify_code(String identify_code) {
		this.identify_code = identify_code;
	}	
	
	

}
