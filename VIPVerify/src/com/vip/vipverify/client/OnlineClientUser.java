package com.vip.vipverify.client;

import java.io.Serializable;

import com.common.my_message.HandleMessageSpreader;
import com.common.my_message.MessageSpreader;
import com.vip.vipverify.MyErrors;
import com.vip.vipverify.VeriryActivity;
import com.vip.vipverify.client.MyClientSocket.NetConnectResultListening;
import com.vip.vipverify.client.MyClientSocket.SockketDataListening;
import com.vip.vipverify.db.MyBaseDataProxy;
import com.vip.vipverify.db.QueryUnit;
import com.vip.vipverify.my_arg.MessageSpreaderMyArg;
import com.vip.vipverify.net.AnalyzeTcpNetData;
import com.vip.vipverify.net.CheckInLoginInfoNetSocketData;
import com.vip.vipverify.net.CheckInLoginoutInfoNetSocketData;
import com.vip.vipverify.net.ConsumeCardNetSocketData;
import com.vip.vipverify.net.NetSocketData;
import com.vip.vipverify.net.RegistCardNetSocketData;
import com.vip.vipverify.net.ServerNetInfo;
import com.vip.vipverify.net.SocketProxy;
import com.vip.vipverify.net.SocketReceiveListeningChannelList;
import com.vip.vipverify.net.VerifyCardNetSocketData;
import com.vip.vipverify.net_data_parse.LoginAffirmNetDataParse;
import com.vip.vipverify.net_data_parse.NetDataParse;
import com.vip.vipverify.net_data_parse.NetDataParsesCollection;
import com.vip.vipverify.net_data_parse.NetDataParsesCollection.ParseResultListening;
import com.vip.vipverify.operator.DeleteSqlDoOperator;
import com.vip.vipverify.operator.DoOperator;
import com.vip.vipverify.operator.OnlineUserSyncDoOperator;
import com.vip.vipverify.operator.SelectSqlDoOperator;
import com.vip.vipverify.operator.SocketSendDoOperator;
import com.vip.vipverify.thread.WakeThread;

import android.annotation.SuppressLint;
import android.os.Message;

public class OnlineClientUser extends ClientUser implements Serializable {
	final static String insert_clientuser_data = "INSERT INTO tlVeriryClientUser ( "
			+ "										 UserName, " + "										 Password, "
			+ "										 Describe, "
			+ "										 ExtraText, "
			+ "										 CreateTime "
			+ "										 ) VALUES ( " + "										 '%s', "
			+ "										 '%s', " + "										 '%s', "
			+ "										 '%s', " + "										 DATE('NOW'));";
	final static String update_user_password_data = "UPDATE tlVeriryClientUser SET Password='%s' WHERE UserName='%s';";
	final static String select_user_data = "SELECT * FROM tlVeriryClientUser;";
	final static String select_Login = "SELECT * FROM tlVeriryClientUser WHERE UserName='%s' and Password='%s';";

	final static String insert_carduser_data = "INSERT INTO tlVeriryCardUser ( "
			+ "									  CardNumber,  "
			+ "									  CardUserPassword,  "
			+ "									  CardPasswordEncrypt,  "
			+ "									  CarUserFirname,  "
			+ "									  Describe,  "
			+ "									  CardRegistOwnUser,  "
			+ "									  CardRegistUserPhone,  "
			+ "									  CardRegistUserSex,  "
			+ "									  CardRegistTime,  "
			+ "									  ExtraText) VALUES (" + "									  '%s',"
			+ " '%s'," + "									  '%s',  " + "									  '%s',  "
			+ "									  '%s',  " + "									  '%s',  "
			+ "									  '%s',  " + "									  %d,  "
			+ "									  DATE('NOW'),  " + "									  '%s');";
	public final static String delete_carduser_data = "DELETE FROM tlVeriryCardUser WHERE CardNumber='%s';";

	final static String update_carduser_password_data = "UPDATE tlVeriryCardUser SET CardUserPassword='%s',  "
			+ "												  CardPasswordEncrypt='%s' WHERE CardNumber='%s';";
	final static String update_carduser_firstname_data = "UPDATE tlVeriryCardUser SET CarUserFirname='%s' WHERE CardNumber='%s';";
	final static String update_carduser_sex_data = "UPDATE tlVeriryCardUser SET CardRegistUserSex='%d' WHERE CardNumber='%s';";
	final static String select_card_info = "SELECT * FROM tlVeriryCardUser WHERE CardNumber='%s';";
	final static String select_card_info_name_pass = "SELECT * FROM tlVeriryCardUser WHERE CardNumber='%s'   "
			+ "											   and CardUserPassword='%s';";
	final static String select_card_info_name_pass_encrypt = "SELECT * FROM tlVeriryCardUser WHERE CardNumber='%s'   "
			+ "											   and CardPasswordEncrypt='%s';";
	final static String select_card_info_all = "SELECT * FROM tlVeriryCardUser;";
	/**
	 * 
	 */
	public final String TAG = "OnlineClientUser";
	private static final long serialVersionUID = 1L;
	private MyBaseDataProxy mdb = null;
	private MessageSpreader ui_message_handler = null;

	private MyClientSocket client_socket = null;

	private String addr_ip;
	private int port;
	private int net_kind = -1;

	private ClientUserInfo user_info = null;

	private Thread socketThread = null;
	private WakeThread send_thread = null;
	private int login_result = 0;

	private SocketReceiveListeningChannelList socket_receive_list = null;
	final int message_key_suc = 0x1011;
	final int message_key_fail = 0x1012;

	private NetDataParsesCollection parsers = new NetDataParsesCollection();

	private MessageSpreader message_handler = new HandleMessageSpreader() {
		/**
		 * 
		 */
		private static final long serialVersionUID = 1L;

		@Override
		@SuppressLint("SimpleDateFormat")
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case message_key_suc:
				String[] info = null;
				if (msg.obj instanceof String[]) {
					info = (String[]) msg.obj;
				}

				if (ui_message_handler != null) {
					OnlineUserSyncDoOperator operator = new OnlineUserSyncDoOperator(info, OnlineClientUser.this);
					Message syncMsg = Message.obtain();
					syncMsg.what = VeriryActivity.KeYSyncToServer;
					syncMsg.obj = operator;
					ui_message_handler.sendMessage(syncMsg);
				}

				break;
			case message_key_fail:
				break;

			default:
				break;
			}
		}
	};

	class OnlineUserSockketDataListening implements SockketDataListening, Serializable {

		/**
		 * 
		 */
		private static final long serialVersionUID = 1L;

		@Override
		public int rev_data(byte[] data, int len) {
			// TODO Auto-generated method stub
			LoginAffirmNetDataParse parser = new LoginAffirmNetDataParse(null);
			if (parser.Parse(data, len) == true) {
				int result = parser.getNresult();
				login_result = result;
				if (result == 0) {
					String strUserName = parser.getStrUserName();
					if (user_info != null) {
						boolean b = strUserName.equalsIgnoreCase(user_info.getUser_name());
						// b = strUserPassword.equalsIgnoreCase(user_info.getUser_password());
						if (b) {
							if (ui_message_handler != null) {
								Message msg = Message.obtain();
								msg.what = VeriryActivity.KeyConectResult;
								msg.obj = MyErrors.NoError.nid;
								ui_message_handler.sendMessage(msg);
							}
							syncLocalToServer();
						} else {
							if (ui_message_handler != null) {
								Message msg = Message.obtain();
								msg.what = VeriryActivity.KeyConectResult;
								msg.obj = MyErrors.DiffLoginUserError.nid;
								ui_message_handler.sendMessage(msg);
							}
						}
					}

				} else {
					if (ui_message_handler != null) {
						Message msg = Message.obtain();
						msg.what = VeriryActivity.KeyConectResult;
						msg.obj = MyErrors.ReloginUserError.nid;
						ui_message_handler.sendMessage(msg);
					}
				}

				DoOperator op = parser.createOperator();
				if (op != null) {
					op.ToDoOperate();
				}
			} else {
				parsers.parse(data, len, new ParseResultListening() {

					@Override
					public void parse_suc(NetDataParse parser) {
						// TODO Auto-generated method stub
						DoOperator op = parser.createOperator();
						if (op != null) {
							op.ToDoOperate();
						}
					}

					@Override
					public void parse_fail(byte[] rev_data, int len) {
						// TODO Auto-generated method stub

					}

				});

				if (socket_receive_list != null) {
					socket_receive_list.RevData(data, len);
				}
			}

			return 0;
		}

		@Override
		public int send_data(byte[] data, int len) {
			// TODO Auto-generated method stub
			return 0;
		}

	}

	private NetConnectResultListening connect_listener = new NetConnectResultListening() {

		@Override
		public void connect_result(boolean bresult, int reason, String ip, int port) {
			// TODO Auto-generated method stub
			if (bresult) {
				loginin_to_server(user_info);

				if (ui_message_handler != null) {
					Message msg = Message.obtain();
					msg.what = VeriryActivity.KeyNotifyText;
					msg.obj = MyErrors.NoError.nid;
					ui_message_handler.sendMessage(msg);
				}
			}
		}

		@Override
		public void net_state(boolean bresult, int reason, String ip, int port) {
			// TODO Auto-generated method stub
			if (bresult == false && reason == MyClientSocket.Net_error) {
				if (ui_message_handler != null) {
					Message msg = Message.obtain();
					msg.what = VeriryActivity.KeyNotifyText;
					msg.obj = MyErrors.NetConnectBreakError.nid;
					ui_message_handler.sendMessage(msg);
				}
			}
		}
	};

	public OnlineClientUser(ServerNetInfo info, ClientUserInfo user_info, MyBaseDataProxy db, WakeThread send_thread,
			SocketReceiveListeningChannelList socket_receive_list) {
		// TODO Auto-generated constructor stub
		if (info != null) {
			addr_ip = info.getIp();
			this.port = info.getPort();
			this.net_kind = info.getNet_kind();
		}
		this.mdb = db;
		this.send_thread = send_thread;
		this.socket_receive_list = socket_receive_list;
		this.user_info = user_info;
	}

	private void loginin_to_server(ClientUserInfo info) {
		NetSocketData data = new CheckInLoginInfoNetSocketData(info);
		SocketProxy proxy = new SocketProxy() {
			/**
			 * 
			 */
			private static final long serialVersionUID = 1L;

			@Override
			protected int send_data(byte[] send_byte, int len) {
				// TODO Auto-generated method stub
				OnlineClientUser.this.client_socket.send_data(send_byte, len, addr_ip, port);
				return 0;
			}
		};
		DoOperator operator = new SocketSendDoOperator(data, proxy);
		if (send_thread != null) {
			send_thread.postOperate(operator);
		} else {
			operator.ToDoOperate();
		}
	}

	private void loginout_to_server(ClientUserInfo info) {
		NetSocketData data = new CheckInLoginoutInfoNetSocketData(info, login_result);
		SocketProxy proxy = new SocketProxy() {
			/**
			 * 
			 */
			private static final long serialVersionUID = 1L;

			@Override
			protected int send_data(byte[] send_byte, int len) {
				// TODO Auto-generated method stub
				OnlineClientUser.this.client_socket.send_data(send_byte, len, addr_ip, port);
				return 0;
			}
		};
		DoOperator operator = new SocketSendDoOperator(data, proxy);

		// do not use thread to send data in here
		operator.ToDoOperate();

	}

	private boolean connect_to_server(String addr, int port) {
		boolean bret = false;

		if (client_socket != null) {
			bret = client_socket.connect_socket(addr, port);
		}

		return bret;
	}

	private boolean close_connect() {
		boolean bret = false;

		if (client_socket != null) {
			client_socket.disconnect_socket();
			client_socket.close_socket();
		}

		return bret;
	}

	private void syncLocalToServer() {
		if (mdb != null) {

			mdb.PostExecSql(new SelectSqlDoOperator(select_card_info_all, mdb, message_handler, message_key_suc,
					message_key_fail));

		}
	}

	@Override
	public void loginIn() {
		// TODO Auto-generated method stub-
		if (mdb != null) {
			mdb.start();
		}

		if (client_socket == null) {
			switch (net_kind) {
			case -1:
			case MyTcpClientSocket.net_client_code:
				client_socket = new MyTcpClientSocket(socketThread, new OnlineUserSockketDataListening(),
						connect_listener, new AnalyzeTcpNetData(null));

				break;
			case MyUdpClientSocket.net_client_code:
				client_socket = new MyUdpClientSocket(socketThread, null);

				break;

			default:
				break;
			}
			client_socket.init_socket();
			if (connect_to_server(this.addr_ip, this.port)) {
				;
			}
		}
	}

	@Override
	public void loginOut() {
		// TODO Auto-generated method stub

		if (client_socket != null) {
			loginout_to_server(user_info);
			this.close_connect();

		}
		if (mdb != null) {
			mdb.stop();
		}

	}

	public boolean delete_card(String card_number) {
		String string_sql = new String();
		string_sql = String.format(OnlineClientUser.delete_carduser_data, card_number);
		return mdb.PostExecSql(new DeleteSqlDoOperator(string_sql, mdb, null, 0, 0));
	}

	@Override
	public boolean commit_regist(CardRegistInfo info) {
		// TODO Auto-generated method stub
		if (user_info != null) {
			info.setString_card_own(this.user_info.getUser_name());
		}
		NetSocketData card_regist_data = new RegistCardNetSocketData(info);
		SocketProxy proxy = new SocketProxy() {
			/**
			 * 
			 */
			private static final long serialVersionUID = 1L;

			@Override
			protected int send_data(byte[] send_byte, int len) {
				// TODO Auto-generated method stub
				OnlineClientUser.this.client_socket.send_data(send_byte, len, addr_ip, port);
				return 0;
			}
		};
		DoOperator operator = new SocketSendDoOperator(card_regist_data, proxy);
		if (send_thread != null) {
			send_thread.postOperate(operator);
		} else {
			operator.ToDoOperate();
		}
		return false;
	}

	@Override
	public boolean commit_verify(CardVerifyInfo info) {
		// TODO Auto-generated method stub
		NetSocketData card_verify_data = new VerifyCardNetSocketData(info);
		SocketProxy proxy = new SocketProxy() {
			/**
			 * 
			 */
			private static final long serialVersionUID = 1L;

			@Override
			protected int send_data(byte[] send_byte, int len) {
				// TODO Auto-generated method stub
				OnlineClientUser.this.client_socket.send_data(send_byte, len, addr_ip, port);
				return 0;
			}
		};

		DoOperator operator = new SocketSendDoOperator(card_verify_data, proxy);
		if (send_thread != null) {
			send_thread.postOperate(operator);
		} else {
			operator.ToDoOperate();
		}
		return false;
	}

	@Override
	public boolean commit_verify_after_do(CardVerifyInfo info) {
		// TODO Auto-generated method stub
		if (info != null) {
			NetSocketData card_verify_after_data = new ConsumeCardNetSocketData(info.getString_card_number(),
					info.getString_card_password());
			SocketProxy proxy = new SocketProxy() {
				/**
				 * 
				 */
				private static final long serialVersionUID = 1L;

				@Override
				protected int send_data(byte[] send_byte, int len) {
					// TODO Auto-generated method stub
					OnlineClientUser.this.client_socket.send_data(send_byte, len, addr_ip, port);
					return 0;
				}
			};
			
			DoOperator operator = new SocketSendDoOperator(card_verify_after_data, proxy);
			if (send_thread != null) {
				send_thread.postOperate(operator);
			} else {
				operator.ToDoOperate();
			}
		}
		return false;
	}

	@Override
	boolean commit_find(QueryUnit unit) {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public void bindUiHandler(MessageSpreader h) {
		// TODO Auto-generated method stub
		ui_message_handler = h;
		parsers.createObject(new MessageSpreaderMyArg(ui_message_handler, 
				VeriryActivity.KeyNotifyText,
				VeriryActivity.KeyCardUserVerifyResult));
	}

	@Override
	public String GetUserName() {
		// TODO Auto-generated method stub
		String str_name = "unknow";
		if (user_info != null) {
			str_name = user_info.getUser_name();
			;
		}

		return str_name;
	}
}
