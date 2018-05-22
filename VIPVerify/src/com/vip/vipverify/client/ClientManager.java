package com.vip.vipverify.client;

import java.io.Serializable;

import com.vip.vipverify.Md5Unit;
import com.vip.vipverify.db.MyBaseDataProxy;
import com.vip.vipverify.my_arg.EmptyMyArg;
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

import android.content.Context;

public class ClientManager implements Serializable {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	public static final int udp_receiver_port = 3601;
	public static final int udp_send_port = 3602;
	public static final int tcp_login_port = 3601;
	public final String string_db_name = "datadb.db";

	private UDPClient udp_client = null;
	private Context context = null;

	private MyBaseDataProxy db = null;
	private ClientUser cur_login_user = null;
	private ServerNetInfo server_info = null;
	protected String user_password;
	protected String user_name;

	private WakeThread m_threadSend = null;
	private SocketReceiveListeningChannelList client_listener = null;
	private boolean isInsert = false;

	private NetDataParsesCollection parsers = new NetDataParsesCollection();

	public ClientManager(Context context) {
		// TODO Auto-generated constructor stub
		this.client_listener = new ClientManagerSocketReceiveListening();
		if (this.m_threadSend != null) {
		}
		this.context = context;
		if (this.context != null) {
			String sdPath = "/data/data";

			String db_path = sdPath + "/" + this.context.getPackageName() + "/" + "databases/" + string_db_name;
			this.db = new MyBaseDataProxy(context, db_path, null, 1);

			cur_login_user = new OfflineClientUser(db);
		}

		parsers.createObject(new EmptyMyArg());
	}

	class EmptySocketReceiveListening extends SocketReceiveListeningChannelList {

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
						if (login_verify_parser.getNresult() >= 0) {
							// login verify success
							if (server_info != null) {
								// switch (server_info.getNet_kind()) {
								// case MyTcpClientSocket.net_client_code:
								//
								// break;
								// case MyUdpClientSocket.net_client_code:
								//
								// break;
								//
								// default:
								// break;
								// }
								cur_login_user = new OnlineClientUser(server_info,
										new ClientUserInfo(user_name, user_password), db, m_threadSend,
										client_listener);
							}
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

	public ClientUser get_cur_loginuser() {
		return this.cur_login_user;
	}

	public void verify_login(ClientUserInfo user_info, ServerNetInfo info) {

		if (udp_client == null) {
			udp_client = new UDPClient(this.context, udp_receiver_port, null, m_threadSend, client_listener);
		}
		if(user_info !=null) {
			user_name = user_info.getUser_name();
			user_name = user_info.getUser_password();
		}

		udp_client.UDPClientInit();
		if (info != null) {
			server_info = info;
			SocketProxy udp_proxy = new UdpSocketProxy(udp_client, info.getIp(), info.getPort());
			if (isInsert == false) {
				client_listener.InsertOnSocketReceiveListening(new EmptySocketReceiveListening());
				isInsert = true;
			}
			NetSocketData data = new VerifyLoginNetSocketData(user_name, Md5Unit.EncodeToMd5String(user_password));
			DoOperator operator = new ResendDoOperator(data, udp_proxy, "verify_login_reponse", client_listener);

			if (m_threadSend != null) {
				m_threadSend.postOperate(operator);
			} else {
				operator.ToDoOperate();
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
