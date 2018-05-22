package com.vip.vipverify.client;

public class CardVerifyInfo {
	private String string_card_number = null;
	private String string_card_password = null;
		
	public CardVerifyInfo(String string_card_number, String string_card_password) {
		super();
		this.string_card_number = string_card_number;
		this.string_card_password = string_card_password;
	}
	public String getString_card_number() {
		return string_card_number;
	}
	public void setString_card_number(String string_card_number) {
		this.string_card_number = string_card_number;
	}
	public String getString_card_password() {
		return string_card_password;
	}
	public void setString_card_password(String string_card_password) {
		this.string_card_password = string_card_password;
	}
}
