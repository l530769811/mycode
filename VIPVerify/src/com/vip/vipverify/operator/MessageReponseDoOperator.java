package com.vip.vipverify.operator;

import com.common.my_message.MessageSpreader;

import android.os.Message;

public class MessageReponseDoOperator implements DoOperator {

	private MessageSpreader message_handler = null;
	private  int nmessage_verify = -1;
	private String strMessage = null;
	
	public MessageReponseDoOperator(MessageSpreader message_handler, int nmessage_verify, String message_string) {
		this.message_handler = message_handler;
		this.nmessage_verify = nmessage_verify;
		this.strMessage = message_string;
	}
	
	protected void _reponse()
	{
		if (message_handler != null) {
			Message message = Message.obtain();
			message.what = this.nmessage_verify;
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
