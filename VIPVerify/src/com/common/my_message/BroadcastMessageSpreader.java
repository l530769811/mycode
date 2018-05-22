package com.common.my_message;


import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Message;

public class BroadcastMessageSpreader implements MessageSpreader {

	private static final String arg1 = "arg1";
	private static final String arg2 = "arg2";
	private static final String what = "what";
	
	private class MessageReceiver extends BroadcastReceiver {
		@Override
		public void onReceive(Context arg0, Intent intent) {
			// TODO Auto-generated method stub
			Message msg = new Message();
			msg.arg1 = intent.getIntExtra(arg1, 0);
			msg.arg2 = intent.getIntExtra(arg2, 0);
			msg.what = intent.getIntExtra(what, 0);
			
			if(receiver!=null)
			{
				receiver.rev(msg);
			}
			handleMessage(msg);
		}		
	}
	
	public interface BroadcastMessageCallBack{
		public void rev(Message msg);
	}
	
	BroadcastMessageCallBack receiver = null;
	String name = null;
	MessageReceiver messageReceiver = null;
	Context con = null;
	
	public BroadcastMessageSpreader(Context con, String name, BroadcastMessageCallBack Receiver) {
		// TODO Auto-generated constructor stub
		this.con = con;
		this.receiver = Receiver;
		this.name = name;
	}

	@Override
	public boolean sendMessage(Message msg) {
		// TODO Auto-generated method stub
		boolean bret = false;
		if(con!=null && msg!=null && name!=null)
		{
			Intent send = new Intent();
			send.setAction(name);
			send.putExtra(arg1, msg.arg1);
			send.putExtra(arg2, msg.arg2);
			send.putExtra(what, msg.what);
			con.sendBroadcast(send);
		}
		return bret;
	}

	@Override
	public boolean sendEmptyMessage(int nwhat) {
		// TODO Auto-generated method stub
		boolean bret = false;
		if(con!=null)
		{
			Message msg = Message.obtain();
			msg.what = nwhat;
			Intent send = new Intent();		
			send.setAction(name);
			send.putExtra(arg1, msg.arg1);
			send.putExtra(arg2, msg.arg2);
			send.putExtra(what, msg.what);
			con.sendBroadcast(send);
		}
		return bret;
	}
	
	public boolean register()
	{
		boolean bret = false;
		if(name!=null)
		{
			IntentFilter IntentFilter = new IntentFilter();
			IntentFilter.addAction(name);
			if(messageReceiver == null)
				messageReceiver = new MessageReceiver();
			
			if(messageReceiver!=null && con!=null)
				con.registerReceiver(messageReceiver, IntentFilter);
			
			bret = true;
		}
		
		return bret;
	}
	
	public boolean unregister()
	{
		boolean bret = false;
	
		IntentFilter IntentFilter = new IntentFilter();
		IntentFilter.addAction("action.com.sop_online");
		if(messageReceiver!=null && con!=null)
		{
			con.unregisterReceiver(messageReceiver);
			bret = true;
		}
		
		return bret;	
	}

	@Override
	public void handleMessage(Message msg) {
		// TODO Auto-generated method stub
		
	}
}
