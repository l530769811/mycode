package com.vip.vipverify.my_arg;

import com.common.my_message.MessageSpreader;

public class MessageSpreaderMyArg implements MyArg {

	

	private MessageSpreader message_handler = null;
	
	public MessageSpreaderMyArg(MessageSpreader message_handler) {
		super();
		this.message_handler = message_handler;
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
		return 0;
	}

	@Override
	public int[] getArgInts() {
		// TODO Auto-generated method stub
		return null;
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
		return null;
	}

}
