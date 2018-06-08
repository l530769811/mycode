package com.vip.vipverify.operator;

import com.common.my_message.MessageSpreader;

import android.os.Message;

public class MessageReponseDoOperator implements DoOperator {

	private MessageSpreader message_handler = null;
	private  int msg_what = -1;
	private int result = 0;
	private String[] strMessage = new String[10];
	
	public MessageReponseDoOperator(MessageSpreader message_handler, int result, int msg_what, String message_string) {
		this.message_handler = message_handler;
		this.msg_what = msg_what;
		this.result = result;
		this.strMessage[0] = message_string;
	}
	
	protected void _reponse()
	{
		if (message_handler != null) {
			Message message = Message.obtain();
			message.what = this.msg_what;
			message.arg1 = result;
			if(this.strMessage!=null)
				message.obj = this.strMessage;
			message_handler.sendMessage(message);
		}
	}
	
	@Override
	public  int ToDoOperate() {
		// TODO Auto-generated method stub
		_reponse();
		return 0;
	}

}
