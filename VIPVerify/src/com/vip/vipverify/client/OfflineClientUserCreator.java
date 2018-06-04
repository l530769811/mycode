package com.vip.vipverify.client;

import com.vip.vipverify.db.MyBaseDataProxy;

public class OfflineClientUserCreator extends ClientUserCreator {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private MyBaseDataProxy db;
	private ClientUserInfo user_info;

	public OfflineClientUserCreator(ClientUserInfo user_info, MyBaseDataProxy db) {
		super();
		this.user_info = user_info;
		this.db = db;
	}

	@Override
	public
	ClientUser createClientUser() {
		// TODO Auto-generated method stub
		return new OfflineClientUser(user_info, db);
	}

}
