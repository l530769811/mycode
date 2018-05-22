package com.common.my_message;

import android.os.Handler;
import android.os.Message;

public class HandleMessageSpreader implements MessageSpreader{

	private Handler message_handle = new Handler(){

		@Override
		public void handleMessage(Message msg) {
			// TODO Auto-generated method stub
			revMessage(msg);
		}
		
	};
	private void revMessage(Message msg)
	{
		this.handleMessage(msg);
	}
	
	@Override
	public boolean sendMessage(Message msg) {
		// TODO Auto-generated method stub
		return message_handle.sendMessage(msg);
	}

	@Override
	public boolean sendEmptyMessage(int what) {
		// TODO Auto-generated method stub
		return message_handle.sendEmptyMessage(what);
	}

	@Override
	public void handleMessage(Message msg) {
		// TODO Auto-generated method stub
		
	}

}
