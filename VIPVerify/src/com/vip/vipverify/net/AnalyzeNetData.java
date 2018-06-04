package com.vip.vipverify.net;

import java.util.List;

public interface AnalyzeNetData{
	public List<byte[]> analyze(byte[] buf_data, int len);
	public byte[] unanalyze(byte[] buff_data, int len);
}
