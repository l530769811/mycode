package com.vip.vipverify.operator;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

import com.vip.vipverify.client.OnlineClientUser;
import com.vip.vipverify.client.CardRegistInfo;

public class OnlineUserSyncDoOperator implements DoOperator {
	private String[] data = null;
	private OnlineClientUser client =  null;
	
	public OnlineUserSyncDoOperator(String[] data, OnlineClientUser client) {
		super();
		this.data = data;
		this.client = client;
	}

	@Override
	public int ToDoOperate() {
		// TODO Auto-generated method stub
		if(client!=null && data!=null) {
			for (int i = 0; i < data.length; i++) {
				String input = data[i];
				// String[] data = s.split("\n");
				String regular = ":[\\s]*[a-zA-Z]+[-]*[a-zA-z]+[\\s]*\n";
				Pattern pattern = Pattern.compile(regular, Pattern.CASE_INSENSITIVE);
				Matcher matcher = pattern.matcher(input);

				String string_carnumber = "";
				String string_phone = "";
				String string_password = "";
				String n_sex = "";
				String string_first_name = "";

				if (matcher.find()) {
					string_carnumber = matcher.group(1).trim();
				}

				if (matcher.find()) {
					string_phone = matcher.group(1).trim();
				}

				if (matcher.find()) {
					string_password = matcher.group(1).trim();
				}

				if (matcher.find()) {
					n_sex = matcher.group(1).trim();
				}

				if (matcher.find()) {
					string_first_name = matcher.group(1).trim();
				}

				if (client.commit_regist(new CardRegistInfo(string_carnumber, string_phone,
						string_password, Integer.valueOf(n_sex), string_first_name))) {
					client.delete_card(string_carnumber);
				}
			}
		}
		return 0;
	}

}
