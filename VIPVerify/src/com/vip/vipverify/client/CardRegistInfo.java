package com.vip.vipverify.client;

public class CardRegistInfo {
	
	String string_card_number = null;
	
	String string_phone = null;
	String string_password = null;
	int n_sex = 0;
	String string_first_name = null;
	String string_call_name = null;
	public CardRegistInfo(String string_carnumber, 
			String string_phone,
			String string_password, 
			int n_sex,
			String string_first_name) {
		super();
		
		string_card_number = string_carnumber;
		this.string_phone = string_phone;
		this.string_password = string_password;
		this.n_sex = n_sex;
		this.string_first_name = string_first_name;
	}
	public String getString_card_number() {
		return string_card_number;
	}
	public void setString_card_number(String string_card_number) {
		this.string_card_number = string_card_number;
	}
	public String getString_phone() {
		return string_phone;
	}
	public void setString_phone(String string_phone) {
		this.string_phone = string_phone;
	}
	public String getString_password() {
		return string_password;
	}
	public void setString_password(String string_password) {
		this.string_password = string_password;
	}
	public int getN_sex() {
		return n_sex;
	}
	public void setN_sex(int n_sex) {
		this.n_sex = n_sex;
	}
	public String getString_first_name() {
		return string_first_name;
	}
	public void setString_first_name(String string_first_name) {
		this.string_first_name = string_first_name;
	}
	public String getString_call_name() {
		return string_call_name;
	}
	public void setString_call_name(String string_call_name) {
		this.string_call_name = string_call_name;
	}
	
	
}
