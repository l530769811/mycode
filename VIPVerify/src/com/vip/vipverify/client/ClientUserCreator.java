package com.vip.vipverify.client;

import java.io.Serializable;

public abstract class ClientUserCreator  implements Serializable{
	
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	public abstract ClientUser createClientUser();
}
