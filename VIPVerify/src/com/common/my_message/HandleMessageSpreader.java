package com.common.my_message;

import java.io.Serializable;
import java.lang.ref.WeakReference;

import android.annotation.SuppressLint;
import android.os.Handler;
import android.os.Message;

@SuppressLint("HandlerLeak")
public class HandleMessageSpreader implements MessageSpreader, Serializable {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	MyHandler message_handle = new MyHandler(this);

	private static class MyHandler extends Handler {
		private WeakReference<HandleMessageSpreader> spreader = null;
		//private HandleMessageSpreader spreader = null;
		MyHandler(HandleMessageSpreader spreader) {
			this.spreader = new WeakReference<HandleMessageSpreader>(spreader);
			//this.spreader = spreader;
		}

		@Override
		public void handleMessage(Message msg) {
			// TODO Auto-generated method stub
			if (spreader != null) {
				spreader.get().revMessage(msg);
				//spreader.revMessage(msg);;
			}
		}

	};

	private final void revMessage(Message msg) {
		handleMessage(msg);
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
