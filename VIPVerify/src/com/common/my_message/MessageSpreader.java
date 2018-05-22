package com.common.my_message;

import android.os.Message;

public interface  MessageSpreader {

	public abstract boolean sendMessage(Message msg);
	public abstract boolean sendEmptyMessage(int what);
	
	public void handleMessage(Message msg);
}
