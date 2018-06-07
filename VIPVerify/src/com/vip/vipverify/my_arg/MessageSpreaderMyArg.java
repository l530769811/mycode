package com.vip.vipverify.my_arg;

import com.common.my_message.MessageSpreader;

public class MessageSpreaderMyArg implements MyArg {

	private MessageSpreader message_handler = null;
	private int msg_what = 0;
	
	private int[] int_args = new  int[2];
	private Object[] obj_args = new Object[2];
	
	public MessageSpreaderMyArg(MessageSpreader message_handler, int msg_what, int msg_what2) {
		super();
		this.message_handler = message_handler;
		this.msg_what = msg_what;
		obj_args[0] = message_handler;
		int_args[0] = msg_what;
		int_args[1]	= msg_what2;
	}

	@Override
	public String getArgString() {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public String getArgStrings() {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public int getArgInt() {
		// TODO Auto-generated method stub
		return msg_what;
	}

	@Override
	public int[] getArgInts() {
		// TODO Auto-generated method stub
		
		return int_args;
	}

	@Override
	public boolean getArgBoolean() {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public boolean[] getArgBooleans() {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public float getArgFloat() {
		// TODO Auto-generated method stub
		return 0;
	}

	@Override
	public float[] getArgFloats() {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public long getArgLong() {
		// TODO Auto-generated method stub
		return 0;
	}

	@Override
	public long getArgLongs() {
		// TODO Auto-generated method stub
		return 0;
	}

	@Override
	public byte getArgByte() {
		// TODO Auto-generated method stub
		return 0;
	}

	@Override
	public byte[] getArgBytes() {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public Object getArgObject() {
		// TODO Auto-generated method stub
		return message_handler;
	}

	@Override
	public Object[] getArgObjects() {
		// TODO Auto-generated method stub
		
 		return obj_args;
	}

}
