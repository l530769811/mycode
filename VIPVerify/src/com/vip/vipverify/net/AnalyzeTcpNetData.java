package com.vip.vipverify.net;

import java.io.Serializable;
import java.io.UnsupportedEncodingException;
import java.util.Iterator;
import java.util.List;

import org.json.JSONException;

import com.vip.vipverify.tcp_socket.TcpStreamPack;

public class AnalyzeTcpNetData implements AnalyzeNetData, Serializable{

	

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private TcpStreamPack pack = new TcpStreamPack(1024);
	private TcpAnalyzeRunningListener revListener = null;
	
	public interface TcpAnalyzeRunningListener{
		public int rev_data(final AnalyzeNetData data,  byte[] buf_data, int len);
		public int send_data(final AnalyzeNetData data,  byte[] buf_data, int len);
	}	

	public AnalyzeTcpNetData(TcpAnalyzeRunningListener revListener) {
		super();
		this.revListener = revListener;
	}

	@Override
	public boolean analyze( byte[] buf_data, int len) {
		// TODO Auto-generated method stub
		boolean bret = false;
		
		try {
			List<byte[]> listPack = pack.pack_parse(buf_data, len);
			Iterator<byte[]> it = listPack.iterator();
			while(it.hasNext())
			{
				byte[] pack_data = it.next();
				if ( revListener!=null )
				{
					revListener.rev_data(this, pack_data, pack_data.length);
				}
			}
			bret = true;
			
		} catch (UnsupportedEncodingException | JSONException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		return bret;
	}

	@Override
	public boolean unanalyze(byte[] buff_data, int len) {
		// TODO Auto-generated method stub
		boolean bret = false;
		try {
			
			byte[] send_data = pack.pack_prepare(buff_data, len);
			if ( revListener!=null )
			{
				revListener.send_data(this, send_data, send_data.length);
			}
			bret = true;
		} catch (UnsupportedEncodingException | JSONException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return bret;
	}

}
