package com.vip.vipverify.operator;

import java.io.Serializable;
import java.util.Timer;
import java.util.TimerTask;

import com.alibaba.fastjson.JSONObject;
import com.vip.vipverify.my_arg.EmptyMyArg;
import com.vip.vipverify.net.Jsonkey;
import com.vip.vipverify.net.NetSocketData;
import com.vip.vipverify.net.SocketProxy;
import com.vip.vipverify.net.SocketReceiveListeningChannelList;
import com.vip.vipverify.net_data_parse.NetDataParse;
import com.vip.vipverify.net_data_parse.NetDataParsesCollection;
import com.vip.vipverify.net_data_parse.NetDataParsesCollection.ParseResultListening;

public class ResendDoOperator implements DoOperator {

	private Timer reverify_timer = null;
	private int reverify_times = 0;

	private NetSocketData netsocketdata = null;
	private SocketProxy socketproxy = null;
	private String type = "";
	private NetDataParsesCollection parsers = new NetDataParsesCollection();
	

	private SocketReceiveListeningChannelList listener = new OperatorSocketRecevieListening();

	public ResendDoOperator(NetSocketData netsocketdata, SocketProxy socketproxy, String type,
			SocketReceiveListeningChannelList listener) {
		super();
		this.netsocketdata = netsocketdata;
		this.socketproxy = socketproxy;
		this.type = type;
		listener.SetOnSocketReceiveListening(this.listener);
		parsers.createObject(new EmptyMyArg());
	
	}

	class ReverifyLoginTimertask extends TimerTask implements Serializable {

		/**
		 * 
		 */
		private static final long serialVersionUID = 1L;
		private DoOperator operator = null;

		public ReverifyLoginTimertask(DoOperator operator) {
			super();
			this.operator = operator;
		}

		@Override
		public void run() {
			// TODO Auto-generated method stub
			if (reverify_times <= 2) {
				reverify_times++;
				if (operator != null) {
					operator.ToDoOperate();
				}
			} else {

				reverify_timer.cancel();
				reverify_timer = null;
				reverify_times = 0;
				JSONObject json_object = new JSONObject();
				try {
					json_object.put(Jsonkey.string_transitionid_key, "");
					json_object.put(Jsonkey.string_magicid_key, "");
					JSONObject json_content = new JSONObject();
					json_object.put(Jsonkey.string_content_key, json_content);
					json_content.put(Jsonkey.string_ctype_key, type);
					JSONObject json_cvalue = new JSONObject();
					json_content.put(Jsonkey.string_cvalue_key, json_cvalue);
					json_cvalue.put(Jsonkey.string_result_key, -1);
					json_cvalue.put(Jsonkey.string_result_info_key, "0x0001");
				} catch (Exception e) {
					// TODO: handle exception
				}
				String json_string = json_object.toString();
				byte[] ret_byte = json_string.getBytes();

				listener.RevData(ret_byte, ret_byte.length);
			}

		}

	}

	class OperatorSocketRecevieListening extends SocketReceiveListeningChannelList {

		@Override
		public boolean receive_data(byte[] data, int len) {
			// TODO Auto-generated method stub

			parsers.parse(data, len, new ParseResultListening() {
				
				@Override
				public void parse_suc(NetDataParse parser) {
					// TODO Auto-generated method stub
					if (reverify_timer != null) {
						reverify_timer.cancel();
						reverify_timer = null;
					}
					reverify_times = 0;
				}
				
				@Override
				public void parse_fail(byte[] rev_data, int len) {
					// TODO Auto-generated method stub
					
				}
			});
			return true;
		}
	}

	@Override
	public int ToDoOperate() {
		// TODO Auto-generated method stub
		if (reverify_timer == null) {
			reverify_timer = new Timer(true);
			reverify_timer.schedule(new ReverifyLoginTimertask(this), 5000, 5000);
		}

		if (netsocketdata != null && socketproxy != null) {
			netsocketdata.be_send(socketproxy);
		}

		return 0;
	}

}
