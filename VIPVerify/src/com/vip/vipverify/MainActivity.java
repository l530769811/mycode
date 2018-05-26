package com.vip.vipverify;

import java.io.Serializable;
import java.util.Timer;
import java.util.TimerTask;

import com.common.my_message.HandleMessageSpreader;
import com.common.my_message.MessageSpreader;
import com.vip.vipverify.SearchServerDialog.SelectServerListening;
import com.vip.vipverify.SignupActivity.SignupListening;
import com.vip.vipverify.client.ClientManager;
import com.vip.vipverify.client.ClientUser;
import com.vip.vipverify.client.ClientUserCreator;
import com.vip.vipverify.client.ClientUserInfo;
import com.vip.vipverify.client.OnlineClientUser;
import com.vip.vipverify.my_arg.EmptyMyArg;
import com.vip.vipverify.net.NetSocketData;
import com.vip.vipverify.net.ServerNetInfo;
import com.vip.vipverify.net.SocketReceiveListeningChannelList;
import com.vip.vipverify.net.UserSignupInfo;
import com.vip.vipverify.net.UserSignupNetSocketData;
import com.vip.vipverify.net_data_parse.NetDataParse;
import com.vip.vipverify.net_data_parse.NetDataParsesCollection;
import com.vip.vipverify.net_data_parse.NetDataParsesCollection.ParseResultListening;
import com.vip.vipverify.net_data_parse.UserSignupNetDataParse;
import com.vip.vipverify.net_data_parse.VerifyLoginNetDataParse;
import com.vip.vipverify.preferences.MyLoginPreferences;
import com.vip.vipverify.preferences.MyLoginPreferences.AutoLoginDoListening;
import com.vip.vipverify.util.SystemUiHider;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.Dialog;
import android.app.ProgressDialog;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Message;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.View.OnFocusChangeListener;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.CompoundButton.OnCheckedChangeListener;
import android.widget.EditText;
import android.widget.PopupMenu;
import android.widget.TextView;
import android.widget.Toast;

/**
 * An example full-screen activity that shows and hides the system UI (i.e.
 * status bar and navigation/system bar) with user interaction.
 *
 * @see SystemUiHider
 */
@SuppressLint("SdCardPath")
public class MainActivity extends Activity implements OnClickListener, OnCheckedChangeListener {
	/**
	 * The instance of the {@link SystemUiHider} for this activity.
	 */

	private ClientUserCreator loginUser = null;
	public static final String key_loginuser = "login_user";
	public static final String key_client_manager = "client_manager";
	public final String string_db_name = "datadb.db";

	public final static int notify_key = 0x0011;
	public final static int login_notify_key = 0x0012;

	private ClientManager client_manager = null;

	
	
	private String server_ip = "0.0.0.0";
	private int server_port = 000;
	private int server_net_kind = -1;

	private String user_name = "";
	private String user_password = "";

	private PopupMenu popup_menu = null;
	private Menu menu = null;

	private boolean isExit = false;

	private Dialog searchserver_dialog = null;
	private Dialog network_setting_dialog = null;

	private Dialog singn_up_dialog = null;

	private ProgressDialog loading_dialog = null;
	private SocketReceiveListeningChannelList listener = new MainActivitySocketReceiveListening();

	private NetDataParsesCollection parses = new NetDataParsesCollection();
	private MyLoginPreferences preferences_login = null;

	private MessageSpreader ui_message_handler = new HandleMessageSpreader() {
		@Override
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case notify_key:
				if (msg.obj instanceof String) {
					String string_error = (String) msg.obj;
					Toast.makeText(MainActivity.this, string_error, Toast.LENGTH_SHORT).show();
				}				
				break;
				
			case login_notify_key:
				//if(msg.obj instanceof int)
				{
					boolean result = (boolean)msg.obj;
					if(result)
					{
						_start_login();
					}
				}

			default:
				break;
			}
		}
	};
	
	class MySignupListening implements SignupListening {

		@Override
		public void Signup(UserSignupInfo info) {
			// TODO Auto-generated method stub
			if(client_manager!=null) {
				loading_dialog = ProgressDialog.show(MainActivity.this, "", getResources().getString(R.string.string_notify_signuping));
				NetSocketData data = new UserSignupNetSocketData(info);
				client_manager.signin_user(data, server_ip, server_port);
			}
		}
		
	}
	

	class MySelectServerListening implements SelectServerListening, Serializable {
		/**
		 * 
		 */
		private static final long serialVersionUID = 1L;

		@Override
		public void SelectServer(ServerNetInfo info) {
			// TODO Auto-generated method stub
			// TODO Auto-generated method stub
			if (info != null) {
				server_ip = info.getIp();
				server_port = info.getPort();
				String server_name = server_ip + " : " + String.valueOf(server_port);
				TextView textview_ipport = (TextView) MainActivity.this.findViewById(R.id.textView_ipport);
				textview_ipport.setText(server_name);
			}
		}

	}

	class MainActivitySocketReceiveListening extends SocketReceiveListeningChannelList implements Serializable {

		/**
		 * 
		 */
		private static final long serialVersionUID = 1L;

		@Override
		public boolean receive_data(byte[] data, int len) {
			// TODO Auto-generated method stub
			
			if (parses != null) {
				parses.parse(data, len, new ParseResultListening() {

					@Override
					public void parse_suc(NetDataParse parser) {
						// TODO Auto-generated method stub
						if ((parser instanceof VerifyLoginNetDataParse)) {
							VerifyLoginNetDataParse net_parse_login = (VerifyLoginNetDataParse) parser;
							int result = net_parse_login.getNresult();
							int no_error = MyErrors.NoError.nid;
							if (result !=   no_error) {
								Message msg = Message.obtain();
								msg.what = notify_key;
								int string_id = MyErrors.GetStringIdFromErrorID(result);
								msg.obj = MainActivity.this.getResources().getString(string_id);
								ui_message_handler.sendMessage(msg);
								preferences_login.login_result(
										new ServerNetInfo("unknow", server_ip, server_port, server_net_kind), user_name,
										(user_password), false);
							} else {
								preferences_login.login_result(
										new ServerNetInfo("unknow", server_ip, server_port, server_net_kind), user_name,
										(user_password), true);
								//_start_login();
								Message msg = Message.obtain();
								msg.what = login_notify_key;
								msg.obj = true;
								ui_message_handler.sendMessage(msg);
							}

						} else if (parser instanceof UserSignupNetDataParse) {
							UserSignupNetDataParse net_parse_login = (UserSignupNetDataParse) parser;
							int result = net_parse_login.getNresult();
							if (result != MyErrors.NoError.nid) {
								Message msg = Message.obtain();
								msg.what = notify_key;
								int string_id = MyErrors.GetStringIdFromErrorID(result);
								msg.obj = MainActivity.this.getResources().getString(string_id);
								ui_message_handler.sendMessage(msg);
							}
							else {
								Message msg = Message.obtain();
								msg.what = notify_key;
								int string_id = MyErrors.GetStringIdFromErrorID(result);
								msg.obj = MainActivity.this.getResources().getString(string_id);
								ui_message_handler.sendMessage(msg);
							}
						}

						if (loading_dialog != null) {
							loading_dialog.dismiss();
						}
					}

					@Override
					public void parse_fail(byte[] rev_data, int len) {
						// TODO Auto-generated method stub
						if (loading_dialog != null) {
							loading_dialog.dismiss();
						}
					}
				});
			}
			return false;
		}

	}

	class LoginTask extends AsyncTask<String, String, Exception> implements Runnable {
		private ServerNetInfo info = null;
		private String user_name = "";
		private String user_password = "";

		public LoginTask(ServerNetInfo info, String user_nam, String user_password) {
			super();
			this.info = info;
			this.user_name = user_nam;
			this.user_password = user_password;

		}

		@Override
		public void run() {
			// TODO Auto-generated method stub
			execute(" ");
		}

		@Override
		protected Exception doInBackground(String... params) {
			// TODO Auto-generated method stub
			if (info != null && user_name != null && user_name.isEmpty() == false && user_password != null
					&& user_password.isEmpty() == false) {
				MainActivity.this._loginUser(this.info, this.user_name, this.user_password);
			}
			return null;
		}

	}

	private AutoLoginDoListening my_auito_login = new AutoLoginDoListening() {

		@Override
		public void ToAutoLogin(ServerNetInfo info, String user_name, String user_password) {
			// TODO Auto-generated method stub
			View view_main = MainActivity.this.findViewById(android.R.id.content);
			//MainActivity.this.ContentView
			if (view_main != null) {
				view_main.post(new LoginTask(info, user_name, user_password));
			}
		}
	};

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		// MenuInflater menuInflater = getMenuInflater();
		// menuInflater.inflate(R.menu.main_menu, menu);

		getMenuInflater().inflate(R.menu.main_menu, menu);
		return true;// super.onCreateOptionsMenu(menu);;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		// TODO Auto-generated method stub
		switch (item.getItemId()) {
		case R.id.main_menu_searchserver:
			if (searchserver_dialog != null) {

				searchserver_dialog.show();
			}
			break;

		case R.id.main_menu_setserver:
			if (network_setting_dialog != null) {
				network_setting_dialog.show();
			}
			break;
		default:

			break;

		}
		return super.onOptionsItemSelected(item);
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		setContentView(R.layout.activity_main);
		
		
				 
		client_manager = new ClientManager(this);
		client_manager.init();
		preferences_login = new MyLoginPreferences(this, R.xml.preferences, client_manager);
		preferences_login.autoLogin(my_auito_login);

		parses.createObject(new EmptyMyArg());
		
		popup_menu = new PopupMenu(this, this.findViewById(R.id.button_main_menu));
		menu = popup_menu.getMenu();
		MenuInflater menuInflater = getMenuInflater();
		menuInflater.inflate(R.menu.main_menu, menu);

		this.server_ip = preferences_login.getServerIp();
		this.server_port = preferences_login.getServerPort();
		this.server_net_kind = preferences_login.getServer_net_kind();
		
		this.user_name = preferences_login.getUser_name();
		this.user_password = preferences_login.getUser_password();
		
		String server_name = server_ip + " : " + String.valueOf(server_port);
		TextView textview_ipport = (TextView) MainActivity.this.findViewById(R.id.textView_ipport);
		textview_ipport.setText(server_name);

		SelectServerListening select_server_listenner = new MySelectServerListening();
		if (searchserver_dialog == null) {

			searchserver_dialog = new SearchServerDialog(this, client_manager, select_server_listenner, listener);
			searchserver_dialog.setTitle(R.string.string_menu_searceserver);

		}

		if (network_setting_dialog == null) {
			network_setting_dialog = new MyNetWorkDialog(this, select_server_listenner);
			network_setting_dialog.setTitle(R.string.string_menu_setserver);

		}

		EditText usernameEdit = (EditText) findViewById(R.id.editText_login_username);
		usernameEdit.setText(this.user_name);
		EditText userpassEdit = (EditText) findViewById(R.id.editText_login_userpassword);
		userpassEdit.setText(this.user_password);
		userpassEdit.setOnFocusChangeListener(new OnFocusChangeListener() {

			@Override
			public void onFocusChange(View v, boolean hasFocus) {
				// TODO Auto-generated method stub
			}
		});

		usernameEdit.setOnFocusChangeListener(new OnFocusChangeListener() {
			@Override
			public void onFocusChange(View v, boolean hasFocus) {

			}
		});

		Button button_loginin = (Button) findViewById(R.id.button_login);
		button_loginin.setOnClickListener(this);
		Button button_regist_user = (Button) findViewById(R.id.button_signup);
		button_regist_user.setOnClickListener(this);

		TextView textbutton_offlinemode = (TextView) findViewById(R.id.textbutton_offline);
		textbutton_offlinemode.setOnClickListener(this);

		TextView textbutton_mainmenu = (TextView) findViewById(R.id.button_main_menu);
		textbutton_mainmenu.setOnClickListener(this);

		CheckBox remember_password = (CheckBox) findViewById(R.id.checkBox_main_remember);
		remember_password.setChecked(preferences_login.isRemember());
		remember_password.setOnCheckedChangeListener(this);
		CheckBox autologin_password = (CheckBox) findViewById(R.id.checkBox_main_autologin);
		autologin_password.setChecked(preferences_login.isAutoLogin());
		autologin_password.setOnCheckedChangeListener(this);
	}

	@Override
	protected void onDestroy() {
		// TODO Auto-generated method stub
		super.onDestroy();			 
		
		if (client_manager != null) {
			client_manager.exit();
		}
	}

	private void _loginUser(ServerNetInfo info, String user_name, String user_password) {
		loading_dialog = ProgressDialog.show(this, "", getResources().getString(R.string.string_notify_logining));
		if (client_manager != null) {

			client_manager.verify_login(new ClientUserInfo(user_name, user_password),
					new ServerNetInfo("unknow", server_ip, server_port, server_net_kind));
		}
	}

	private void _start_login() {

		if (client_manager != null) {
			loginUser = client_manager.get_cur_user_Creator();
		}

		
		Intent intent = null;
		
		Bundle bundle = new Bundle();
		intent = new Intent(this, VeriryActivity.class);
	
		bundle.putSerializable(key_loginuser, loginUser);
		intent.putExtras(bundle);
		
		this.startActivity(intent);
	}

	@Override
	public void onClick(View v) {
		switch (v.getId()) {
		case R.id.button_login:
			EditText edit_user_name = (EditText) this.findViewById(R.id.editText_login_username);
			EditText edit_user_password = (EditText) this.findViewById(R.id.editText_login_userpassword);
			if (edit_user_name != null) {
				user_name = edit_user_name.getText().toString();
			}
			if (edit_user_password != null) {
				user_password = edit_user_password.getText().toString();
			}

			_loginUser(new ServerNetInfo("unknow", server_ip, server_port, server_net_kind),
					user_name,
					(user_password));
			

			break;
		case R.id.button_signup:
			
			if (singn_up_dialog == null) {				
				singn_up_dialog = new SignupActivity(this, client_manager, new MySignupListening());
				singn_up_dialog.setTitle(R.string.string_signin);
			}
			singn_up_dialog.show();

			break;

		case R.id.textbutton_offline:
			_start_login();
			
			break;
		case R.id.textView_ipport:
			break;
			
		case R.id.button_main_menu:
			if (popup_menu != null) {
				popup_menu.show();
			}
			break;
		}
	}

	@Override
	public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
		// TODO Auto-generated method stub
		switch (buttonView.getId()) {
		case R.id.checkBox_main_autologin:
			if (isChecked == true) {
				CheckBox remember_password = (CheckBox) findViewById(R.id.checkBox_main_remember);
				remember_password.setChecked(true);
			}

			if (preferences_login != null) {
				preferences_login.setAutoLogin(isChecked, user_name, user_password);
			}

			break;

		case R.id.checkBox_main_remember:
			if (preferences_login != null) {
				preferences_login.setRememberPassword(isChecked, user_name, user_password);
			}
			break;

		default:
			break;
		}
	};

	private void exitBy2Click() {
		Timer tExit = null;
		if (isExit == false) {
			isExit = true; // 准备退出
			Toast.makeText(this, R.string.string_exit_notify, Toast.LENGTH_SHORT).show();
			tExit = new Timer();
			tExit.schedule(new TimerTask() {
				@Override
				public void run() {
					isExit = false; // 取消退出
				}
			}, 1000); // 如果2秒钟内没有按下返回键，则启动定时器取消掉刚才执行的任务

		} else {
			finish();
			
		}
	}

	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		// TODO Auto-generated method stub
		if (keyCode == KeyEvent.KEYCODE_BACK) {
			exitBy2Click();
		}
		return true;
	}

	/**
	 * Schedules a call to hide() in [delay] milliseconds, canceling any previously
	 * scheduled calls.
	 */

}
