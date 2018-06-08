package com.vip.vipverify.client;

import java.io.Serializable;

import com.alibaba.fastjson.JSONObject;
import com.common.my_message.HandleMessageSpreader;
import com.common.my_message.MessageSpreader;
import com.vip.vipverify.MainActivity;
import com.vip.vipverify.MyErrors;
import com.vip.vipverify.SingletonTemplete;
import com.vip.vipverify.db.MyBaseDataProxy;
import com.vip.vipverify.my_arg.EmptyMyArg;
import com.vip.vipverify.net.Jsonkey;
import com.vip.vipverify.net.NetSocketData;
import com.vip.vipverify.net.ServerNetInfo;
import com.vip.vipverify.net.SocketProxy;
import com.vip.vipverify.net.SocketReceiveListeningChannelList;
import com.vip.vipverify.net.UdpSocketProxy;
import com.vip.vipverify.net.VerifyLoginNetSocketData;
import com.vip.vipverify.net_data_parse.NetDataParse;
import com.vip.vipverify.net_data_parse.NetDataParsesCollection;
import com.vip.vipverify.net_data_parse.NetDataParsesCollection.ParseResultListening;
import com.vip.vipverify.net_data_parse.VerifyLoginNetDataParse;
import com.vip.vipverify.operator.DoOperator;
import com.vip.vipverify.operator.ResendDoOperator;
import com.vip.vipverify.operator.SocketSendDoOperator;
import com.vip.vipverify.thread.OperateWakeThread;
import com.vip.vipverify.thread.WakeThread;

import android.annotation.SuppressLint;
import android.content.Context;
import android.os.Message;

public class ClientManager implements Serializable {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	public static final int udp_receiver_port = 3601;
	public static final int udp_send_port = 3602;
	public static final int tcp_login_port = 3601;

	private static final int login_verify_suc_key = 0x1101;
	private static final int login_verify_fail_key = 0x1102;

	public final String string_db_name = "datadb.db";

	private UDPClient udp_client = null;
	private Context context = null;

	private MyBaseDataProxy db = null;
	private ClientUserCreator cur_login_user = null;
	private ServerNetInfo server_info = null;
	protected String user_password;
	protected String user_name;

	private WakeThread m_threadSend = null;
	private SocketReceiveListeningChannelList client_listener = null;
	private boolean isInsert = false;

	private NetDataParsesCollection parsers = new NetDataParsesCollection();
	private MessageSpreader ui_message_handler = null;

	private MessageSpreader message_handler = new HandleMessageSpreader() {
		/**
		 * 
		 */
		private static final long serialVersionUID = 1L;

		@Override
		@SuppressLint("SimpleDateFormat")
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case login_verify_suc_key:
				if (server_info != null) {
					cur_login_user = new OnlineClientUserCreator(server_info,
							new ClientUserInfo(user_name, user_password), db, m_threadSend, client_listener);
					if (ui_message_handler != null) {

					}
				}

				break;
			case login_verify_fail_key:
				break;

			default:
				break;
			}
		}
	};

	private static volatile ClientManager instance = null;

	public static synchronized ClientManager getInstance(Context context, MessageSpreader handler) {
		if (instance == null) {
			synchronized (ClientManager.class) {
				if (instance == null) {
					instance = new ClientManager(context, handler);
				}
			}

		}
		return instance;
	}

	public ClientManager(Context context, MessageSpreader handler) {
		// TODO Auto-generated constructor stub
		this.client_listener = new ClientManagerSocketReceiveListening();

		this.context = context;
		this.ui_message_handler = handler;

		if (this.context != null) {
			String sdPath = "/data/data";

			String db_path = sdPath + "/" + this.context.getPackageName() + "/" + "databases/" + string_db_name;
			this.db = new MyBaseDataProxy(context, db_path, null, 1);

			cur_login_user = new OfflineClientUserCreator(new ClientUserInfo("Unline User", "000000"), db);
		}

		parsers.createObject(new EmptyMyArg());
	}

	class EmptySocketReceiveListening extends SocketReceiveListeningChannelList {

		/**
		 * 
		 */
		private static final long serialVersionUID = 1L;

		@Override
		public boolean receive_data(byte[] data, int len) {
			// TODO Auto-generated method stub
			return true;
		}
	}

	class ClientManagerSocketReceiveListening extends SocketReceiveListeningChannelList implements Serializable {
		/**
		 * 
		 */
		private static final long serialVersionUID = 1L;

		@Override
		public boolean receive_data(byte[] data, int len) {
			// TODO Auto-generated method stub

			parsers.parse(data, len, new ParseResultListening() {

				@Override
				public void parse_suc(NetDataParse parser) {
					// TODO Auto-generated method stub
					if (parser instanceof VerifyLoginNetDataParse) {
						VerifyLoginNetDataParse login_verify_parser = (VerifyLoginNetDataParse) parser;
						if (login_verify_parser.getNresult() == 0) {
							// login verify success
							Message msg = Message.obtain();
							msg.what = ClientManager.login_verify_suc_key;
							ClientManager.this.message_handler.sendMessage(msg);
						} else {
							// login verify fail
							Message msg = Message.obtain();
							msg.what = ClientManager.login_verify_fail_key;
							ClientManager.this.message_handler.sendMessage(msg);
						}
					}

				}

				@Override
				public void parse_fail(byte[] rev_data, int len) {
					// TODO Auto-generated method stub

				}
			});
			return true;
		}
	}

	public interface TrueFalseResultListening {
		public void resulte(boolean bresult);
	}

	public void setOnSocketReceiveListener(SocketReceiveListeningChannelList listener) {
		if (this.client_listener == null) {
			this.client_listener = new ClientManagerSocketReceiveListening();
		} else {
			this.client_listener.SetOnSocketReceiveListening(listener);
		}
	}

	public void signin_user(NetSocketData data, String to_ip, int to_port) {

		if (udp_client == null) {
			udp_client = new UDPClient(this.context, udp_receiver_port, null, m_threadSend, client_listener);

		}

		udp_client.UDPClientInit();
		SocketProxy udp_proxy = new UdpSocketProxy(udp_client, to_ip, to_port);

		if (isInsert == false) {
			client_listener.InsertOnSocketReceiveListening(new EmptySocketReceiveListening());
			isInsert = true;
		}
		DoOperator operator = new ResendDoOperator(data, udp_proxy, "user_regist_reponse", client_listener);
		if (m_threadSend != null) {
			m_threadSend.postOperate(operator);
		} else {
			operator.ToDoOperate();
		}

	}

	public ClientUserCreator get_cur_user_Creator() {
		return this.cur_login_user;
	}

	public void verify_login(ClientUserInfo user_info, ServerNetInfo info) {

		if (info != null) {
			if (udp_client == null) {
				udp_client = new UDPClient(this.context, udp_receiver_port, null, m_threadSend, client_listener);
			}
			if (user_info != null) {
				user_name = user_info.getUser_name();
				user_password = user_info.getUser_password();
			}

			udp_client.UDPClientInit();
			server_info = info;
			SocketProxy udp_proxy = new UdpSocketProxy(udp_client, info.getIp(), info.getPort());
			if (isInsert == false) {
				client_listener.InsertOnSocketReceiveListening(new EmptySocketReceiveListening());
				isInsert = true;
			}
			NetSocketData data = new VerifyLoginNetSocketData(user_name, (user_password));
			DoOperator operator = new ResendDoOperator(data, udp_proxy, "verify_login_reponse", client_listener);

			if (m_threadSend != null) {
				m_threadSend.postOperate(operator);
			} else {
				operator.ToDoOperate();
			}
		} else {
			cur_login_user = new OfflineClientUserCreator(user_info, db);
			// Unlined user just send success
			if (ui_message_handler != null) {
				
				Message msg = Message.obtain();
				msg.what = MainActivity.verify_notify_key;
				ui_message_handler.sendMessage(msg);
				
				msg = Message.obtain();
				msg.what = MainActivity.login_notify_key;
				msg.obj = true;
				ui_message_handler.sendMessage(msg);
			}
		}

	}

	public void find_server(NetSocketData data, String ip, int port) {

		if (udp_client == null) {
			udp_client = new UDPClient(this.context, udp_receiver_port, null, m_threadSend, client_listener);
		}

		udp_client.UDPClientInit();
		UdpSocketProxy udp_proxy = new UdpSocketProxy(udp_client, ip, port);
		DoOperator operator = new SocketSendDoOperator(data, udp_proxy);
		if (m_threadSend != null) {
			m_threadSend.postOperate(operator);
		} else {
			operator.ToDoOperate();
		}
	}

	public void init() {
		if (udp_client != null) {
			udp_client.UDPClientInit();
		}

		if (m_threadSend == null) {
			m_threadSend = new OperateWakeThread();

		}
		m_threadSend.start();
	}

	public void exit() {
		if (udp_client != null) {
			udp_client.UDPClientColse();
		}

		if (m_threadSend != null) {
			m_threadSend.exit();
			m_threadSend = null;

		}
	}

	public boolean login_in_verify() {
		boolean bret = false;

		return bret;
	}

	public boolean login_out(ClientUser client) {
		boolean bret = false;

		return bret;
	}

}
