package com.vip.vipverify.net_data_parse;

import java.io.Serializable;

import com.vip.vipverify.operator.DoOperator;

public abstract class NetDataParse implements Serializable{
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	protected abstract boolean is_type(byte[] buf_data, int len);

	protected abstract boolean parse_data(byte[] buf_data, int len);

	public abstract DoOperator createOperator();

	final public boolean Parse(byte[] buf_data, int len) {
		boolean bret = false;
		if (buf_data != null) {
			if (is_type(buf_data, len)) {
				bret = parse_data(buf_data, len);
			}
		}

		return bret;
	}
}
