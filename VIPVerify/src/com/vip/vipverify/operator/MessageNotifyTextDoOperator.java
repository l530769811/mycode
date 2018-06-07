package com.vip.vipverify.operator;

import com.common.my_message.MessageSpreader;
import com.vip.vipverify.my_arg.MyArg;

import android.os.Message;

public class MessageNotifyTextDoOperator implements DoOperator {

	private MessageSpreader message_handler = null;
	private int result = 0;
	private int what = 0;

	public MessageNotifyTextDoOperator(MessageSpreader message_handler, int result, int what) {
		this.message_handler = message_handler;
		this.result = result;
		this.what = what;
	}

	@Override
	public int ToDoOperate() {
		// TODO Auto-generated method stub
		if (message_handler != null) {
			Message msg = Message.obtain();
			msg.what = this.what;
			msg.obj = result;
			message_handler.sendMessage(msg);
		}
		return 0;
	}

}
