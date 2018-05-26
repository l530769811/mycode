package com.vip.vipverify.client;

import com.vip.vipverify.db.MyBaseDataProxy;
import com.vip.vipverify.net.ServerNetInfo;
import com.vip.vipverify.net.SocketReceiveListeningChannelList;
import com.vip.vipverify.thread.WakeThread;

public class OnlineClientUserCreator extends ClientUserCreator {

	
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private ServerNetInfo info;
	private ClientUserInfo user_info;
	private MyBaseDataProxy db;
	private WakeThread send_thread;

	

	public OnlineClientUserCreator(ServerNetInfo info, ClientUserInfo user_info, MyBaseDataProxy db,
			WakeThread send_thread, SocketReceiveListeningChannelList socket_receive_list) {
		super();
		this.info = info;
		this.user_info = user_info;
		this.db = db;
		this.send_thread = send_thread;
		
	}


	@Override
	public
	ClientUser createClientUser() {
		// TODO Auto-generated method stub
		return new OnlineClientUser(info, user_info, db, send_thread, null);
	}

}
