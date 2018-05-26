package com.vip.vipverify.client;

import com.vip.vipverify.db.MyBaseDataProxy;

public class OfflineClientUserCreator extends ClientUserCreator {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private MyBaseDataProxy db;

	public OfflineClientUserCreator(MyBaseDataProxy db) {
		super();
		this.db = db;
	}

	@Override
	public
	ClientUser createClientUser() {
		// TODO Auto-generated method stub
		return new OfflineClientUser(db);
	}

}
