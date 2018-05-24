package com.vip.vipverify.client;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.Serializable;
import java.net.Socket;
import java.util.ArrayList;
import java.util.List;

import com.common.my_message.HandleMessageSpreader;
import com.common.my_message.MessageSpreader;
import com.vip.vipverify.VeriryActivity;
import com.vip.vipverify.client.MyClientSocket.SockketDataListening;
import com.vip.vipverify.db.MyBaseDataProxy;
import com.vip.vipverify.db.QueryUnit;
import com.vip.vipverify.net.AnalyzeNetData;
import com.vip.vipverify.net.AnalyzeTcpNetData;
import com.vip.vipverify.net.AnalyzeTcpNetData.TcpAnalyzeRunningListener;
import com.vip.vipverify.net.CheckInLoginInfoNetSocketData;
import com.vip.vipverify.net.CheckInLoginoutInfoNetSocketData;
import com.vip.vipverify.net.NetSocketData;
import com.vip.vipverify.net.RegistCardNetSocketData;
import com.vip.vipverify.net.ServerNetInfo;
import com.vip.vipverify.net.SocketProxy;
import com.vip.vipverify.net.SocketReceiveListeningChannelList;
import com.vip.vipverify.net.VerifyCardNetSocketData;
import com.vip.vipverify.operator.DeleteSqlDoOperator;
import com.vip.vipverify.operator.DoOperator;
import com.vip.vipverify.operator.OnlineUserSyncDoOperator;
import com.vip.vipverify.operator.SelectSqlDoOperator;
import com.vip.vipverify.operator.SocketSendDoOperator;
import com.vip.vipverify.thread.WakeThread;

import android.annotation.SuppressLint;
import android.os.Message;

public class OnlineClientUser extends ClientUser implements Serializable, Runnable {
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
	private MessageSpreader message_handler = null;

	private Socket socketClient = null;
	private MyClientSocket client_socket = null;

	private String addr_ip;
	private int port;
	private int net_kind = -1;

	private ClientUserInfo user_info = null;

	private Thread socketThread = null;
	private WakeThread send_thread = null;

	private DataInputStream in;
	private DataOutputStream out;

	private final int buff_len = 1024;
	private AnalyzeNetData net_data_analyzer = null;

	private SocketReceiveListeningChannelList socket_receive_list = null;

	final int message_key_suc = 0x1011;
	final int message_key_fail = 0x1012;

	private MessageSpreader ui_message_handler = new HandleMessageSpreader() {
		@Override
		@SuppressLint("SimpleDateFormat")
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case message_key_suc:
				String[] info = null;
				if (msg.obj instanceof String[]) {
					info = (String[]) msg.obj;
				}

				if (message_handler != null) {
					OnlineUserSyncDoOperator operator = new OnlineUserSyncDoOperator(info, OnlineClientUser.this);
					Message syncMsg = Message.obtain();
					syncMsg.what = VeriryActivity.KeYSyncToServer;
					syncMsg.obj = operator;
					message_handler.sendMessage(syncMsg);
				}

				break;
			case message_key_fail:
				break;

			default:
				break;
			}
		}
	};

	class DataRevRunningListener implements TcpAnalyzeRunningListener, Serializable {

		/**
		 * 
		 */
		private static final long serialVersionUID = 1L;

		@Override
		public int rev_data(final AnalyzeNetData data, byte[] buf_data, int len) {
			// TODO Auto-generated method stub

			if (socket_receive_list != null) {
				socket_receive_list.RevData(buf_data, len);
			}

			return 0;
		}

		@Override
		public int send_data(AnalyzeNetData data, byte[] buf_data, int len) {
			// TODO Auto-generated method stub
			int nlen = -1;
			if (OnlineClientUser.this.out != null) {

				try {
					out.write(buf_data);
					nlen = len;
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}

			return nlen;
		}

	}

	class OnlineUserSockketDataListening implements SockketDataListening {

		@Override
		public int rev_data(byte[] data, int len) {
			// TODO Auto-generated method stub
			revTcpData(data, len);
			return 0;
		}

		@Override
		public int send_data(byte[] data, int len) {
			// TODO Auto-generated method stub
			return 0;
		}

	}

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
				OnlineClientUser.this.client_socket.send_data(send_byte, len);
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
		NetSocketData data = new CheckInLoginoutInfoNetSocketData(info);
		SocketProxy proxy = new SocketProxy() {
			/**
			 * 
			 */
			private static final long serialVersionUID = 1L;

			@Override
			protected int send_data(byte[] send_byte, int len) {
				// TODO Auto-generated method stub
				OnlineClientUser.this.client_socket.send_data(send_byte, len);
				return 0;
			}
		};
		DoOperator operator = new SocketSendDoOperator(data, proxy);
		
		//do not use thread to send data in here
		operator.ToDoOperate();

	}

	private boolean connect_to_server(String addr, int port) throws IOException, InterruptedException {
		boolean bret = false;

		// if (socketClient != null && socketClient.isClosed() == false) {
		// socketClient.close();
		// }
		//
		// try {
		// socketClient = new Socket();
		// SocketAddress socketAddress = new InetSocketAddress(addr, port);
		// socketClient.connect(socketAddress, 1000);
		// out = new DataOutputStream(socketClient.getOutputStream());
		// in = new DataInputStream(socketClient.getInputStream());
		//
		// bret = true;
		// } catch (Exception e) {
		// // TODO: handle exception
		// close_connect();
		// throw new IOException(e);
		// }
		if (client_socket != null) {
			bret = client_socket.connect_socket(addr, port);
		}

		return bret;
	}

	private boolean close_connect() throws IOException {
		boolean bret = false;
		// try {
		// // flag = false;
		// if (socketClient != null) {
		// // Toast.makeText(getApplicationContext(),
		// // "socket_closeConnect()", Toast.LENGTH_LONG).show();
		// // socket.shutdownOutput();
		// // socket.shutdownInput();
		// if (socketClient.isClosed() == false)
		// socketClient.close();
		//
		// // .purge();
		// // Heart.cancel();
		// }
		//
		// if (out != null) {
		// out.close();
		// out = null;
		// }
		//
		// if (in != null) {
		// in.close();
		// in = null;
		// }
		//
		// bret = true;
		//
		// } catch (IOException e) {
		// e.printStackTrace();
		// throw e;
		// }
		if (client_socket != null) {
			client_socket.disconnect_socket();
		}
		return bret;
	}

	private void revTcpData(byte[] buff, int len) {
		if (len <= 0)
			return;
		net_data_analyzer.analyze(buff, len);
	}

	private void syncLocalToServer() {
		if (mdb != null) {
			mdb.PostExecSql(new SelectSqlDoOperator(select_card_info_all, mdb, ui_message_handler, message_key_suc,
					message_key_fail));
		}
	}

	@Override
	public void loginIn() {
		// TODO Auto-generated method stub-
		// try {
		// if (connect_to_server(this.addr_ip, port)) {
		// socketThread = new Thread(this);
		// socketThread.start();
		// }
		// } catch (IOException e) {
		// // TODO Auto-generated catch block
		// e.printStackTrace();
		// } catch (InterruptedException e) {
		// // TODO Auto-generated catch block
		// e.printStackTrace();
		// }
		if (client_socket == null) {
			switch (net_kind) {
			case -1:
			case MyTcpClientSocket.net_client_code:
				client_socket = new MyTcpClientSocket(socketThread, null, null);
				net_data_analyzer = new AnalyzeTcpNetData(new DataRevRunningListener());
				break;
			// case MyUdpClientSocket.net_client_code:
			// client_socket = new MyUdpClientSocket(socketThread, null, null);
			// net_data_analyzer = new AnalyzeUdpNetData();
			// break;

			default:
				break;
			}
			client_socket.init_socket();
			if (client_socket.connect_socket(this.addr_ip, this.port)) {
				loginin_to_server(user_info);
				syncLocalToServer();
			}
		}

		if (mdb != null) {
			mdb.start();
		}
	}

	@Override
	public void loginOut() {
		// TODO Auto-generated method stub
		// try {
		// close_connect();
		// } catch (IOException e) {
		// // TODO Auto-generated catch block
		// e.printStackTrace();
		// }
		//
		// if (socketThread != null) {
		// socketThread.interrupt();
		// try {
		// socketThread.join();
		// } catch (InterruptedException e) {
		// // TODO Auto-generated catch block
		// e.printStackTrace();
		// }
		// }
		if (client_socket != null) {
			loginout_to_server(user_info);
			client_socket.disconnect_socket();
			client_socket.close_socket();
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

		NetSocketData card_regist_data = new RegistCardNetSocketData(info);
		SocketProxy proxy = new SocketProxy() {
			/**
			 * 
			 */
			private static final long serialVersionUID = 1L;

			@Override
			protected int send_data(byte[] send_byte, int len) {
				// TODO Auto-generated method stub
				OnlineClientUser.this.client_socket.send_data(send_byte, len);
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
				OnlineClientUser.this.client_socket.send_data(send_byte, len);
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
		return false;
	}

	@Override
	boolean commit_find(QueryUnit unit) {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public void bindHandler(MessageSpreader h) {
		// TODO Auto-generated method stub
		message_handler = h;
	}

	@Override
	public void run() {
		// TODO Auto-generated method stub

		while (!Thread.currentThread().isInterrupted()) {
			if (in != null) {
				byte[] rev_buf = new byte[buff_len];
				int length = -1;
				try {
					while ((length = in.read(rev_buf)) != -1) {
						revTcpData(rev_buf, length);
					}
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();

					break;
				}
			} else {

				break;
			}
		}

		try {
			close_connect();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	class SocketSendWakeThread extends WakeThread implements Serializable {
		/**
		 * 
		 */
		private static final long serialVersionUID = 1L;
		private List<byte[]> listOperate = new ArrayList<byte[]>();

		//
		// public boolean postOperate(DoOperator operate) {
		// boolean bret = false;
		// synchronized (listOperate) {
		// bret = listOperate.add(operate);
		// }
		// this.wakeUp();
		//
		// return bret;
		// }
		//
		// private DoOperator GetOperate() {
		// DoOperator operate_ret = null;
		// synchronized (listOperate) {
		// if (listOperate.size() > 0)
		// operate_ret = listOperate.remove(0);
		// }
		// return operate_ret;
		// }
		public boolean send_data(byte[] send_buff, int len) {
			boolean bret = false;
			if (OnlineClientUser.this.socketClient != null && socketClient.isClosed() == false) {
				synchronized (send_buff) {
					listOperate.add(send_buff);
				}
			}
			return bret;
		}

		private byte[] get_data() {
			byte[] ret = null;
			synchronized (listOperate) {
				ret = listOperate.remove(0);
			}
			return ret;
		}

		private void clear_data() {
			synchronized (listOperate) {
				listOperate.clear();
			}
		}

		@Override
		public void myRun() throws InterruptedException {
			// TODO Auto-generated method stub
			if (OnlineClientUser.this.out != null) {
				byte[] send_buff = get_data();
				if (net_data_analyzer != null) {
					net_data_analyzer.unanalyze(send_buff, send_buff.length);
				}
			} else {
				clear_data();
			}

		}

		@Override
		public boolean postOperate(DoOperator operate) {
			// TODO Auto-generated method stub
			return false;
		}

		@Override
		public DoOperator GetOperate() {
			// TODO Auto-generated method stub
			return null;
		}

	}

	@Override
	public String GetUserName() {
		// TODO Auto-generated method stub
		return user_info.getUser_name();
	}

}
