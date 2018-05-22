package com.vip.vipverify.client;

import java.io.Serializable;

import com.common.my_message.MessageSpreader;
import com.vip.vipverify.db.QueryUnit;

public abstract class ClientUser implements Serializable {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	public abstract void loginIn();

	public abstract void loginOut();

	public abstract boolean commit_regist(CardRegistInfo info);
	//abstract boolean commit_regist_after_do();

	public abstract boolean commit_verify(CardVerifyInfo info);
	public abstract boolean commit_verify_after_do(CardVerifyInfo info);
	
	abstract boolean commit_find(QueryUnit unit);

	

	public abstract void bindHandler(MessageSpreader h);

}
