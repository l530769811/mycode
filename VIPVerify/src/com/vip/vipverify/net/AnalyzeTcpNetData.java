package com.vip.vipverify.net;

import java.io.Serializable;
import java.io.UnsupportedEncodingException;
import java.util.Iterator;
import java.util.List;

import org.json.JSONException;

import com.vip.vipverify.tcp_socket.TcpStreamPack;

public class AnalyzeTcpNetData implements AnalyzeNetData, Serializable {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private TcpStreamPack pack = new TcpStreamPack(1024);
	private Encrypter crypter = null;

	public interface TcpAnalyzeRunningListener {
		public int analyze_data(final AnalyzeNetData data, byte[] buf_data, int len);

		public int unanalyze_data(final AnalyzeNetData data, byte[] buf_data, int len);
	}

	public AnalyzeTcpNetData(Encrypter crypter) {
		super();
		this.crypter = crypter;

	}

	@Override
	public List<byte[]> analyze(byte[] buf_data, int len) {
		List<byte[]> listPack = null;
		try {
			listPack = pack.pack_parse(buf_data, len);
			
			if (crypter != null) {
				Iterator<byte[]> it = listPack.iterator();
				List<byte[]> list = null; 
				while (it.hasNext()) {
					byte[] pack_data = it.next();
					
					list.add(crypter.Decrypt(pack_data, pack_data.length));
				}
				listPack = list;
			}

		} catch (UnsupportedEncodingException | JSONException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		return listPack;
	}

	@Override
	public byte[] unanalyze(byte[] buff_data, int len) {
		byte[] send_data = null;
		try {
			if (crypter != null) {
				send_data = crypter.Encrypt(send_data, len);
			}
			send_data = pack.pack_prepare(buff_data, len);
		} catch (UnsupportedEncodingException | JSONException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return send_data;
	}

}
