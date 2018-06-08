package com.vip.vipverify.operator;

import java.io.Serializable;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import com.vip.vipverify.client.OnlineClientUser;
import com.vip.vipverify.client.CardRegistInfo;

public class OnlineUserSyncDoOperator implements DoOperator, Serializable {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private String[] data = null;
	private OnlineClientUser client = null;

	public OnlineUserSyncDoOperator(String[] data, OnlineClientUser client) {
		super();
		this.data = data;
		this.client = client;
	}

	@Override
	public int ToDoOperate() {
		// TODO Auto-generated method stub
		if (client != null && data != null) {
			for (int i = 0; i < data.length; i++) {
				String input = data[i];
				// String[] data = s.split("\n");
				String regular = "[\\s]*:+([^:.]+)\n+";
				Pattern pattern = Pattern.compile(regular, Pattern.CASE_INSENSITIVE);
				Matcher matcher = pattern.matcher(input);

				String string_id = "";
				String string_carnumber = "";
				
				String string_password = "";
				String string_password_encrypt = "";
				String string_first_name = "";
				String string_describe = "";
				String string_CardRegistOwnUser = "";
				String string_phone = "";
				String string_sex = "";
				String string_CardRegistTime = "";
				String string_ExtraText = "";
				
				String[] strs = new String[11];
				strs[0] = string_id;
				strs[1] = string_carnumber;
				strs[2] = string_password;
				strs[3] = string_password_encrypt;
				strs[4] = string_first_name;
				strs[5] = string_describe;
				strs[6] = string_CardRegistOwnUser;
				strs[7] = string_phone;
				strs[8] = string_sex;
				strs[9] = string_CardRegistTime;
				strs[10] = string_ExtraText;
				for(int i1=0; i1< strs.length; i1++)
				{
					if (matcher.find()) {
						strs[i1] = matcher.group(1).trim();
					}
				}
				

	
				
				
				if (strs[1].isEmpty() == false && strs[8].isEmpty() == false) {
					try {
						if (client.commit_regist(new CardRegistInfo(strs[1], strs[7], strs[2],
								Integer.valueOf(strs[8]), strs[4], strs[6]), false)) {
							client.delete_card(strs[1]);
						}
					} catch (NumberFormatException e) {
						// TODO: handle exception
					}
				}
			}
		}
		return 0;
	}

}
