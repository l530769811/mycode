package com.vip.vipverify.net_data_parse;

import java.lang.reflect.InvocationTargetException;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import com.vip.vipverify.my_arg.MyArg;

public class NetDataParsesCollection {
	public enum NetDataParses {
		NullNetDataParse(null, "", ""),

		CardVerifyResultNetDataParse(CardVerifyResultNetDataParse.class, "card_verify_request", "card_verify_reponse"),

		CardRegistResultNetDataParse(CardRegistResultNetDataParse.class, "card_regist_request", "card_regist_reponse"),

		ServerInfoNetDataParse(ServerInfoNetDataParse.class, "search_server_request", "search_server_reponse"),

		UserRegistNetDataParse(UserSignupNetDataParse.class, "user_regist_request", "user_regist_reponse"),

		VerifyLoginNetDataParse(VerifyLoginNetDataParse.class, "verify_login_request", "verify_login_reponse"),

		LoginAffirmNetDataParse(LoginAffirmNetDataParse.class, "login_affirm_request", "login_affirm_reponse"),
		
		ConsumeCardResultNetDataParse(ConsumeCardResultNetDataParse.class, "consume_card_request", "consume_card_reponse");
		
		private final Class<? extends NetDataParse> NetDataParseClass;
		private final String request_string;
		private final String reponse_string;

		NetDataParses(final Class<? extends NetDataParse> activityClass, String request_string, String reponse_string) {
			this.NetDataParseClass = activityClass;
			this.request_string = request_string;
			this.reponse_string = reponse_string;

		}
	}

	private final List<NetDataParse> list_net = new ArrayList<NetDataParse>();
	private boolean bIsCreate = false;

	public interface ParseResultListening {
		public void parse_suc(NetDataParse parser);

		public void parse_fail(byte[] rev_data, int len);
	}

	public final static String getRequestType(Class<? extends NetDataParse> obj_class) {
		String ret = "";
		for (NetDataParses net_data : NetDataParses.values()) {
			if (obj_class.equals(net_data.NetDataParseClass)) {
				ret = net_data.request_string;
				break;
			}
		}
		return ret;
	}

	public final static String getReponseType(Class<? extends NetDataParse> obj_class) {
		String ret = "";
		for (NetDataParses net_data : NetDataParses.values()) {
			if (obj_class.equals(net_data.NetDataParseClass)) {
				ret = net_data.reponse_string;
				break;
			}
		}
		return ret;
	}

	public boolean parse(byte[] rev_data, int len, ParseResultListening listener) {
		boolean bret = false;
		Iterator<NetDataParse> itor = list_net.iterator();
		while (itor.hasNext()) {
			NetDataParse parser = itor.next();
			if (parser.Parse(rev_data, len)) {
				if (listener != null) {
					listener.parse_suc(parser);
				}

				bret = true;
				break;
			}
		}

		if (bret==false && listener != null) {
			listener.parse_fail(rev_data, len);
		}
		
		return bret;
	}

	public void createObject(MyArg argv) {
		if (bIsCreate == false) {
			for (NetDataParses a : NetDataParses.values()) {
				try {
					if (a.NetDataParseClass != null) {
						addNetDataParse(a.NetDataParseClass.getConstructor(MyArg.class).newInstance(argv));
					}
				} catch (InstantiationException | IllegalAccessException | IllegalArgumentException
						| InvocationTargetException | NoSuchMethodException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		}
	}

	public final List<NetDataParse> getNetDataParses() {
		return list_net;

	}

	public final boolean addNetDataParse(NetDataParse obj) {
		boolean bret = false;
		bret = list_net.add(obj);
		return bret;
	}
}
