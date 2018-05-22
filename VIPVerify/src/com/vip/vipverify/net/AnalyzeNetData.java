package com.vip.vipverify.net;

public interface AnalyzeNetData{
	public boolean analyze(byte[] buf_data, int len);
	public boolean unanalyze(byte[] buff_data, int len);
}
