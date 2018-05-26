package com.vip.vipverify;

import java.io.IOException;
import java.io.Serializable;
import java.util.Collection;
import java.util.Map;
import com.common.my_message.HandleMessageSpreader;
import com.common.my_message.MessageSpreader;
import com.google.zxing.BarcodeFormat;
import com.google.zxing.DecodeHintType;
import com.google.zxing.Result;
import com.google.zxing.ResultMetadataType;
import com.google.zxing.ResultPoint;
import com.google.zxing.client.android.AmbientLightManager;
import com.google.zxing.client.android.BeepManager;
import com.google.zxing.client.android.CaptureActivityHandler;
import com.google.zxing.client.android.DecodeFormatManager;
import com.google.zxing.client.android.DecodeHintManager;
import com.google.zxing.client.android.FinishListener;
import com.google.zxing.client.android.InactivityTimer;
import com.google.zxing.client.android.IntentSource;
import com.google.zxing.client.android.Intents;
import com.google.zxing.client.android.PreferencesActivity;
import com.google.zxing.client.android.ScanFromWebPageManager;
import com.google.zxing.client.android.ViewfinderView;
import com.google.zxing.client.android.camera.CameraManager;
import com.google.zxing.client.android.clipboard.ClipboardInterface;
import com.google.zxing.client.android.result.ResultHandler;
import com.google.zxing.client.android.result.ResultHandlerFactory;
import com.vip.vipverify.AutoAlertDialog;
import com.vip.vipverify.AutoAlertDialog.LeaveMyDialogListener;
import com.vip.vipverify.client.ClientUser;
import com.vip.vipverify.client.ClientUserCreator;
import com.vip.vipverify.client.CardRegistInfo;
import com.vip.vipverify.client.CardVerifyInfo;
import com.vip.vipverify.net_data_parse.CardVerifyResultNetDataParse;
import com.vip.vipverify.operator.DoOperator;
import com.vip.vipverify.operator.OnlineUserSyncDoOperator;
import com.vip.vipverify.preferences.MyPreferences;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.app.ProgressDialog;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.content.pm.ActivityInfo;
import android.content.res.Configuration;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Rect;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.preference.PreferenceManager;
import android.text.Editable;
import android.text.TextUtils;
import android.text.TextWatcher;
import android.util.Log;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuItem;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.View.OnFocusChangeListener;
import android.view.animation.AlphaAnimation;
import android.view.animation.Animation;
import android.view.animation.LinearInterpolator;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.CompoundButton.OnCheckedChangeListener;
import android.widget.EditText;
import android.widget.RadioButton;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.ToggleButton;

/**
 * @author Administrator
 *
 */
public class VeriryActivity extends Activity implements SurfaceHolder.Callback, Runnable, OnClickListener {

	private static final String TAG = VeriryActivity.class.getSimpleName();
	public static final int KeyCardUserVerifySuc = 0x0001;
	public static final int KeyCardUserVerifyFail = 0x0002;
	public static final int KeyCardUserRegistSuc = 0x0003;
	public static final int KeyCardUserRegistFail = 0x0004;

	public static final int KeyCardUserVerifyDeletetSuc = 0x0005;
	public static final int KeyCardUserVerifyDeleteFail = 0x0006;
	public static final int KeYSyncToServer = 0x0007;
	public static final int KeyConectResult = 0x0008;

	private static final long DEFAULT_INTENT_RESULT_DURATION_MS = 1500L;
	private static final long BULK_MODE_SCAN_DELAY_MS = 1000L;

	private static final String[] ZXING_URLS = { "http://zxing.appspot.com/scan", "zxing://scan/" };

	private CaptureActivityHandler handler;
	private boolean hasSurface;
	private CameraManager cameraManager;

	private Collection<BarcodeFormat> decodeFormats;
	private Map<DecodeHintType, ?> decodeHints;
	private String characterSet;

	private Result savedResultToShow;
	private Result lastResult;
	private ViewfinderView viewfinderView;

	private InactivityTimer inactivityTimer;
	private BeepManager beepManager;
	private AmbientLightManager ambientLightManager;

	private IntentSource source;

	private boolean copyToClipboard;
	private String sourceUrl;
	private ScanFromWebPageManager scanFromWebPageManager;

	/////////////
	private ClientUser loginUser = null;
	private CardRegistInfo registInfo = null;
	private boolean is_regist = false;

	final int DIALOG_VERIFY_CONFIRM_ID = 1;
	private Dialog verify_confirm_dialog = null;
	final String key_verify_info = "key_verify_info";
	
	private ProgressDialog loading_dialog = null;

	@Override
	protected Dialog onCreateDialog(int id, Bundle bundle) {
		// TODO Auto-generated method stub
		switch (id) {
		case DIALOG_VERIFY_CONFIRM_ID:
			if (verify_confirm_dialog == null) {
				verify_confirm_dialog = new Dialog(this);
				verify_confirm_dialog.setContentView(R.layout.verify_info_firm);
				verify_confirm_dialog.setTitle(R.string.string_title_confirm);
				verify_confirm_dialog.setCancelable(false);
				Button button_confirm = (Button) verify_confirm_dialog.findViewById(R.id.button_info_confirm);
				button_confirm.setOnClickListener(this);
				Button button_recycle = (Button) verify_confirm_dialog.findViewById(R.id.button_info_recycle);
				button_recycle.setOnClickListener(this);
			}

			if (bundle != null) {
				Serializable obj = bundle.getSerializable(key_verify_info);
				String[] text = null;
				if (obj instanceof String[]) {
					text = (String[]) obj;
				}
				if (text != null) {
					TextView text_view = (TextView) verify_confirm_dialog.findViewById(R.id.textView_verify_info);
					text_view.setText(text[0]);
				}
			}

			break;
		default:
			break;
		}

		return verify_confirm_dialog;
	}

	class SyncLeaveMyDialogListener implements LeaveMyDialogListener {
		DoOperator operator = null;

		public SyncLeaveMyDialogListener(DoOperator operator) {
			this.operator = operator;
		}

		@Override
		public void onClick(View view) {
			// TODO Auto-generated method stub
			switch (view.getId()) {
			case R.id.button_autodialog_ok:
				if (operator != null) {
					operator.ToDoOperate();
				}

				break;

			default:
				break;
			}

		}

	}

	@SuppressLint("HandlerLeak")
	private MessageSpreader ui_message_handler = new HandleMessageSpreader() {
		@Override
		@SuppressLint("SimpleDateFormat")
		public void handleMessage(Message msg) {
			Log.e(TAG, "rev msg: " + msg.what);
			switch (msg.what) {
			case KeyCardUserVerifySuc:
			case CardVerifyResultNetDataParse.verify_suc_message_id:
				// Toast.makeText(VeriryActivity.this, R.string.string_textnotify_verifysuc,
				// Toast.LENGTH_SHORT).show();
				// if (verify_confirm_dialog != null)
				// verify_confirm_dialog.show();
				String[] info = null;
				if (msg.obj instanceof String[]) {
					info = (String[]) msg.obj;
				}
				Bundle bundle = new Bundle();
				bundle.putSerializable(key_verify_info, info);
				if (verify_confirm_dialog != null) {

					if (info != null) {
						TextView text_view = (TextView) verify_confirm_dialog.findViewById(R.id.textView_verify_info);
						text_view.setText(info[0]);
					}
				}
				VeriryActivity.this.showDialog(DIALOG_VERIFY_CONFIRM_ID, bundle);

				break;
			case KeyCardUserVerifyFail:
			case CardVerifyResultNetDataParse.verify_fail_message_id:
				Toast.makeText(VeriryActivity.this, R.string.string_textnotify_verifyfail, Toast.LENGTH_SHORT).show();
				break;
			case KeyCardUserRegistFail:
				Toast.makeText(VeriryActivity.this, R.string.string_textnotify_registfail, Toast.LENGTH_SHORT).show();
				break;
			case KeyCardUserRegistSuc:
				Toast.makeText(VeriryActivity.this, R.string.string_textnotify_registsuc, Toast.LENGTH_SHORT).show();
				break;
			case KeyCardUserVerifyDeletetSuc:
				Toast.makeText(VeriryActivity.this, R.string.string_textnotify_recycle_suc, Toast.LENGTH_SHORT).show();
				break;
			case KeyCardUserVerifyDeleteFail:
				Toast.makeText(VeriryActivity.this, R.string.string_textnotify_recycle_fail, Toast.LENGTH_SHORT).show();
				break;
			case KeYSyncToServer:
				Object obj = msg.obj;
				if (obj instanceof OnlineUserSyncDoOperator) {
					Dialog notify_sync = new AutoAlertDialog(VeriryActivity.this,
							VeriryActivity.this.getResources().getString(R.string.string_sync_data),
							VeriryActivity.this.getResources().getString(R.string.string_sync_local_data_to_server), 5,
							new SyncLeaveMyDialogListener((OnlineUserSyncDoOperator) obj));
					notify_sync.setTitle(R.string.string_sync_data);

					notify_sync.show();
				}
				break;

			case KeyConectResult:
				boolean connect_result = (boolean) msg.obj;
				if(loading_dialog!=null) {
					loading_dialog.dismiss();
				}
				if (connect_result) {
					VeriryActivity.this.setVisible(true);
				} else {
					Toast.makeText(VeriryActivity.this, VeriryActivity.this.getResources().getString(R.string.string_connect_fail), Toast.LENGTH_LONG).show();
					VeriryActivity.this.finish();
				}
				break;
			default:
				break;
			}
		}
	};

	class TextFilter implements TextWatcher {

		private View view = null;

		public TextFilter(View v) {
			view = v;
		}

		@Override
		public void beforeTextChanged(CharSequence s, int start, int count, int after) {
			// TODO Auto-generated method stub

		}

		@Override
		public void onTextChanged(CharSequence s, int start, int before, int count) {
			// TODO Auto-generated method stub
			EditText vv = null;
			if (view instanceof EditText) {
				vv = (EditText) view;
			}
			if (vv != null && vv.getId() == R.id.edit_cardpassword)// password edit
			{
				// so set verify password edit
				EditText edit = (EditText) VeriryActivity.this.findViewById(R.id.edit_carduser_passwordverify);
				VeriryActivity.this.stopFlick(edit);
			}
			if (vv != null && vv.getId() == R.id.edit_carduser_passwordverify)// password verify edit
			{
				// so set password edit
				EditText edit = (EditText) VeriryActivity.this.findViewById(R.id.edit_cardpassword);
				VeriryActivity.this.stopFlick(edit);
			}
			VeriryActivity.this.stopFlick(vv);
		}

		@Override
		public void afterTextChanged(Editable s) {
			// TODO Auto-generated method stub
			EditText vv = null;
			if (view instanceof EditText) {
				vv = (EditText) view;
			}

			if (vv != null && vv.getId() == R.id.edit_carduser_firstname) {
				VeriryActivity.this.setCallNameView();
			}
		}

	}

	private void stopFlick(View view) {
		if (null == view) {
			return;
		}
		view.clearAnimation();
	}

	private void startFlick(View view) {
		if (null == view) {
			return;
		}
		Animation alphaAnimation = new AlphaAnimation(1, 0.4f);
		alphaAnimation.setDuration(300);
		alphaAnimation.setInterpolator(new LinearInterpolator());
		alphaAnimation.setRepeatCount(Animation.INFINITE);
		alphaAnimation.setRepeatMode(Animation.REVERSE);
		view.startAnimation(alphaAnimation);
	}

	@SuppressLint("CommitPrefEdits")
	private void initLayout() {

		// SharedPreferences prefs =
		// PreferenceManager.getDefaultSharedPreferences(this);
		// Editor editor = prefs.edit();
		// editor.putBoolean(PreferencesActivity.KEY_PLAY_BEEP, true);// when

		RadioButton radioButton_men = (RadioButton) this.findViewById(R.id.radio_sex_men);
		RadioButton radioButton_women = (RadioButton) this.findViewById(R.id.radio_sex_women);
		radioButton_men.setChecked(true);
		radioButton_women.setChecked(false);

		EditText sexEdit = (EditText) this.findViewById(R.id.edit_carduser_firstname);
		String string_firstname = sexEdit.getText().toString();
		if (string_firstname.isEmpty()) {
			string_firstname = this.getString(R.string.string_carduser_call_name);
		}

		String string_call_name = string_firstname
				+ (radioButton_men.isChecked() == true ? this.getString(R.string.string_sex_men)
						: this.getString(R.string.string_sex_women));
		TextView callTextView = (TextView) this.findViewById(R.id.textview_call_name);
		callTextView.setText(string_call_name);

		ToggleButton sound_button = (ToggleButton) this.findViewById(R.id.toggleButton_sound);
		SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(this);

		boolean sound_bool = prefs.getBoolean(PreferencesActivity.KEY_PLAY_BEEP, true);
		sound_button.setChecked(sound_bool);
	}

	private void setCallNameView() {
		RadioButton radioButton_men = (RadioButton) this.findViewById(R.id.radio_sex_men);

		EditText sexEdit = (EditText) this.findViewById(R.id.edit_carduser_firstname);
		String string_firstname = sexEdit.getText().toString();
		if (string_firstname.isEmpty()) {
			string_firstname = this.getString(R.string.string_carduser_call_name);
		}

		String string_call_name = string_firstname
				+ (radioButton_men.isChecked() == true ? this.getString(R.string.string_sex_men)
						: this.getString(R.string.string_sex_women));
		TextView callTextView = (TextView) this.findViewById(R.id.textview_call_name);
		callTextView.setText(string_call_name);
	}

	private boolean isRightRegistInfo() {
		boolean ret = true;

		EditText edit = (EditText) this.findViewById(R.id.edit_cardnumber);
		if (TextUtils.isEmpty(edit.getText().toString())) {
			this.startFlick(edit);
			return ret = false;
		}

		EditText edit_cardpassword = (EditText) this.findViewById(R.id.edit_cardpassword);
		String first_password = edit_cardpassword.getText().toString();
		if (TextUtils.isEmpty(first_password)) {
			this.startFlick(edit_cardpassword);
			return ret = false;
		}

		EditText edit_carduser_passwordverify = (EditText) this.findViewById(R.id.edit_carduser_passwordverify);
		String verify_password = edit_carduser_passwordverify.getText().toString();
		if (TextUtils.isEmpty(verify_password)) {
			this.startFlick(edit_carduser_passwordverify);
			return ret = false;
		}

		if (first_password.equals(verify_password) == false) {
			this.startFlick(edit_cardpassword);
			this.startFlick(edit_carduser_passwordverify);
			return ret = false;
		}

		edit = (EditText) this.findViewById(R.id.edit_carduser_firstname);
		String string_first_name = edit.getText().toString();
		if (TextUtils.isEmpty(string_first_name)) {
			this.startFlick(edit);
			return ret = false;
		}

		RadioButton radioButton_men = (RadioButton) this.findViewById(R.id.radio_sex_men);
		RadioButton radioButton_women = (RadioButton) this.findViewById(R.id.radio_sex_women);
		if (!radioButton_men.isChecked() && !radioButton_women.isChecked()) {
			this.startFlick(radioButton_men);
			this.startFlick(radioButton_women);
			return ret = false;
		}

		return ret;
	}

	private boolean isRightVerifyInfo() {
		boolean ret = true;

		EditText edit = (EditText) this.findViewById(R.id.edit_cardnumber);
		if (TextUtils.isEmpty(edit.getText().toString())) {
			this.startFlick(edit);
			return ret = false;
		}

		EditText edit_cardpassword = (EditText) this.findViewById(R.id.edit_cardpassword);
		String first_password = edit_cardpassword.getText().toString();
		if (TextUtils.isEmpty(first_password)) {
			this.startFlick(edit_cardpassword);
			return ret = false;
		}

		return ret;
	}

	private CardRegistInfo createRegistInfoByUi() {

		EditText edit = (EditText) this.findViewById(R.id.edit_cardnumber);
		String string_card_number = edit.getText().toString();

		edit = (EditText) this.findViewById(R.id.edit_carduser_passwordverify);
		String first_password = edit.getText().toString();

		edit = (EditText) this.findViewById(R.id.edit_carduser_firstname);
		String string_first_name = edit.getText().toString();

		RadioButton radioButton_men = (RadioButton) this.findViewById(R.id.radio_sex_men);
		String string_call_name = string_first_name
				+ (radioButton_men.isChecked() == true ? this.getString(R.string.string_sex_men)
						: this.getString(R.string.string_sex_women));

		int nSex = (radioButton_men.isChecked() ? 0 : 1);

		edit = (EditText) this.findViewById(R.id.edit_carduser_phone);
		String string_phone = edit.getText().toString();

		CardRegistInfo info = new CardRegistInfo(string_card_number, string_phone, first_password, nSex,
				string_first_name);
		info.setString_call_name(string_call_name);

		return info;
	}

	private CardVerifyInfo createVerifyInfoByUi() {
		EditText edit = (EditText) this.findViewById(R.id.edit_cardnumber);
		String string_card_number = edit.getText().toString();

		edit = (EditText) this.findViewById(R.id.edit_cardpassword);
		String first_password = edit.getText().toString();

		CardVerifyInfo info = new CardVerifyInfo(string_card_number, first_password);

		return info;
	}

	private boolean commit_regist(CardRegistInfo info) {
		boolean ret = false;
		if (loginUser != null) {
			ret = loginUser.commit_regist(info);
		}

		return ret;
	}

	private boolean commit_verify(CardVerifyInfo info) {
		boolean ret = false;
		if (loginUser != null) {
			ret = loginUser.commit_verify(info);
		}
		return ret;
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// TODO Auto-generated method stub
		getMenuInflater().inflate(R.menu.login_user_menu, menu);
		MenuItem item = menu.getItem(0);

		if (item != null && this.loginUser != null) {
			item.setTitle(loginUser.GetUserName());
		}
		return super.onCreateOptionsMenu(menu);
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		// TODO Auto-generated method stub

		switch (item.getItemId()) {
		case R.id.login_user_menu_info:

			break;

		case R.id.login_user_menu_loginout:
			AutoAlertDialog dialog = new AutoAlertDialog(this, this.getResources().getString(R.string.string_exit),
					this.getResources().getString(R.string.string_sure_loginout), 5,
					new ExitLeaveMyDialogListener(this));
			dialog.show();
			break;
		default:

			break;

		}

		return super.onOptionsItemSelected(item);
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		setContentView(R.layout.activity_veriry);

		Intent intent = this.getIntent();
		Serializable obj = intent.getSerializableExtra(MainActivity.key_loginuser);
		if (obj != null && obj instanceof ClientUserCreator) {
			ClientUserCreator loginUserCreator = (ClientUserCreator) obj;
			loginUser = loginUserCreator.createClientUser();
		}

		if (loginUser != null) {
			this.setVisible(false);
			
			loading_dialog = ProgressDialog.show(this, "", getResources().getString(R.string.string_connectting));
			loginUser.bindUiHandler(ui_message_handler);
			loginUser.loginIn();
		}
		else
		{
			this.finish();
		}

		hasSurface = false;
		inactivityTimer = new InactivityTimer(this);
		beepManager = new BeepManager(this);
		ambientLightManager = new AmbientLightManager(this);

		PreferenceManager.setDefaultValues(this, R.xml.preferences, false);
		SharedPreferences share_pre = PreferenceManager.getDefaultSharedPreferences(this);
		if (share_pre != null) {
			String cur_server_kind = share_pre.getString(MyPreferences.preferences_cur_server_kind, "x");
			String tcp_ip = share_pre.getString(MyPreferences.preferences_edittext_sever_tcp_ip, "xxx.xxx.xxx.xxx");
			String tcp_port = share_pre.getString(MyPreferences.preferences_edittext_sever_tcp_port, "xxx");
			String tcp_kind_name = share_pre.getString(MyPreferences.preferences_spinner_server_tcp_kind, "xxxxxx");
		}

		initLayout();

		EditText edit = (EditText) this.findViewById(R.id.edit_cardnumber);
		edit.addTextChangedListener(new TextFilter(edit));

		edit = (EditText) this.findViewById(R.id.edit_cardpassword);
		edit.addTextChangedListener(new TextFilter(edit));

		edit = (EditText) this.findViewById(R.id.edit_carduser_passwordverify);
		edit.addTextChangedListener(new TextFilter(edit));

		edit = (EditText) this.findViewById(R.id.edit_carduser_firstname);
		edit.addTextChangedListener(new TextFilter(edit));

		edit = (EditText) this.findViewById(R.id.edit_carduser_phone);
		edit.addTextChangedListener(new TextFilter(edit));

		RadioButton radioButton_men = (RadioButton) this.findViewById(R.id.radio_sex_men);
		RadioButton radioButton_women = (RadioButton) this.findViewById(R.id.radio_sex_women);
		radioButton_men.setOnFocusChangeListener(new OnFocusChangeListener() {

			@Override
			public void onFocusChange(View v, boolean hasFocus) {
				// TODO Auto-generated method stub

				VeriryActivity.this.stopFlick(v);
			}

		});
		radioButton_men.setOnCheckedChangeListener(new OnCheckedChangeListener() {

			@Override
			public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
				// TODO Auto-generated method stub
				setCallNameView();
			}

		});

		radioButton_women.setOnFocusChangeListener(new OnFocusChangeListener() {

			@Override
			public void onFocusChange(View v, boolean hasFocus) {
				// TODO Auto-generated method stub
				VeriryActivity.this.stopFlick(v);
			}

		});
		radioButton_women.setOnCheckedChangeListener(new OnCheckedChangeListener() {

			@Override
			public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
				// TODO Auto-generated method stub
				setCallNameView();
			}

		});

		Button scan_button = (Button) this.findViewById(R.id.button_scan);
		scan_button.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				if (VeriryActivity.this.cameraManager != null) {
					if (VeriryActivity.this.cameraManager.isOpen() == false) {
						(VeriryActivity.this).openScan();
					}
				}

			}
		});

		Button veriry_button = (Button) this.findViewById(R.id.button_veriry);
		veriry_button.setOnClickListener(new OnClickListener() {

			@SuppressLint("ShowToast")
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				// (VeriryActivity.this).onResume();
				if (VeriryActivity.this.isRightVerifyInfo() == true) {
					if (VeriryActivity.this.commit_verify(VeriryActivity.this.createVerifyInfoByUi())) {

						;
					} else {
						;
					}
				}
			}

		});

		Button rigist_button = (Button) this.findViewById(R.id.button_regist);
		rigist_button.setOnClickListener(new OnClickListener() {

			@SuppressLint("ShowToast")
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				// (VeriryActivity.this).onResume();
				// SurfaceView surface =
				// (SurfaceView)VeriryActivity.this.findViewById(R.id.surface_preview_view);
				// surface.setVisibility(View.GONE);
				if (!is_regist) {
					ViewfinderView viewFinder = (ViewfinderView) VeriryActivity.this.findViewById(R.id.viewfinder_view);
					viewFinder.setVisibility(View.GONE);

					RelativeLayout registInfoLayout = (RelativeLayout) VeriryActivity.this
							.findViewById(R.id.relativelayout_regist_info);
					registInfoLayout.setVisibility(View.VISIBLE);
					is_regist = true;

				} else {

					if (VeriryActivity.this.isRightRegistInfo()) {
						is_regist = false;

						if (VeriryActivity.this.commit_regist(VeriryActivity.this.createRegistInfoByUi())) {
							;
						} else {
							;
						}

						ViewfinderView viewFinder = (ViewfinderView) VeriryActivity.this
								.findViewById(R.id.viewfinder_view);
						viewFinder.setVisibility(View.VISIBLE);

						RelativeLayout registInfoLayout = (RelativeLayout) VeriryActivity.this
								.findViewById(R.id.relativelayout_regist_info);
						registInfoLayout.setVisibility(View.GONE);
					}
				}

			}

		});

		ToggleButton light_button = (ToggleButton) this.findViewById(R.id.toggleButton_light);
		light_button.setOnCheckedChangeListener(new OnCheckedChangeListener() {

			@Override
			public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
				// TODO Auto-generated method stub

				cameraManager.setTorch(isChecked);

			}
		});

		ToggleButton sound_button = (ToggleButton) this.findViewById(R.id.toggleButton_sound);
		sound_button.setOnCheckedChangeListener(new OnCheckedChangeListener() {

			@Override
			public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
				// TODO Auto-generated method stub
				SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(VeriryActivity.this);

				Editor editor = prefs.edit();
				editor.putBoolean(PreferencesActivity.KEY_PLAY_BEEP, isChecked);// when

				editor.commit();
			}
		});

	}

	@Override
	protected void onStart() {
		// TODO Auto-generated method stub
		super.onStart();
	}

	@Override
	protected void onDestroy() {
		this.closeScan();
		inactivityTimer.shutdown();
		if (loginUser != null) {
			loginUser.loginOut();
		}

		if (verify_confirm_dialog != null) {
			this.removeDialog(DIALOG_VERIFY_CONFIRM_ID);
		}
		super.onDestroy();
	}

	protected void closeScan() {
		if (handler != null) {
			handler.quitSynchronously();
			handler = null;
		}

		inactivityTimer.onPause();

		ambientLightManager.stop();
		beepManager.close();
		if (cameraManager != null) {
			if (cameraManager.isOpen()) {
				cameraManager.closeDriver();
			}
		}
		// historyManager = null; // Keep for onActivityResult
		if (!hasSurface) {
			SurfaceView surfaceView = (SurfaceView) findViewById(R.id.surface_preview_view);
			SurfaceHolder surfaceHolder = surfaceView.getHolder();
			surfaceHolder.removeCallback(this);
		}
	}

	@SuppressLint("InlinedApi")
	protected void openScan() {

		cameraManager = new CameraManager(getApplication());

		viewfinderView = (ViewfinderView) findViewById(R.id.viewfinder_view);
		viewfinderView.setCameraManager(cameraManager);

		// resultView = findViewById(R.id.result_view);
		// statusView = (TextView) findViewById(R.id.status_view);

		handler = null;
		lastResult = null;

		SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(this);

		if (prefs.getBoolean(PreferencesActivity.KEY_DISABLE_AUTO_ORIENTATION, true)) {
			setRequestedOrientation(getCurrentOrientation());
		} else {
			setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE);
		}

		resetStatusView();

		beepManager.updatePrefs();
		ambientLightManager.start(cameraManager);

		inactivityTimer.onResume();

		Intent intent = getIntent();

		copyToClipboard = prefs.getBoolean(PreferencesActivity.KEY_COPY_TO_CLIPBOARD, true)
				&& (intent == null || intent.getBooleanExtra(Intents.Scan.SAVE_HISTORY, true));

		source = IntentSource.NONE;
		sourceUrl = null;
		scanFromWebPageManager = null;
		decodeFormats = null;
		characterSet = null;

		if (intent != null) {

			String action = intent.getAction();
			String dataString = intent.getDataString();

			if (Intents.Scan.ACTION.equals(action)) {

				// Scan the formats the intent requested, and return the result to the calling
				// activity.
				source = IntentSource.NATIVE_APP_INTENT;
				decodeFormats = DecodeFormatManager.parseDecodeFormats(intent);
				decodeHints = DecodeHintManager.parseDecodeHints(intent);

				if (intent.hasExtra(Intents.Scan.WIDTH) && intent.hasExtra(Intents.Scan.HEIGHT)) {
					int width = intent.getIntExtra(Intents.Scan.WIDTH, 0);
					int height = intent.getIntExtra(Intents.Scan.HEIGHT, 0);
					if (width > 0 && height > 0) {
						cameraManager.setManualFramingRect(width, height);
					}
				} else {
					// add by liqj
					int finderview_height = viewfinderView.getHeight();
					int finderview_width = viewfinderView.getWidth();
					//////////////////////////

					Rect inrect = cameraManager.getFramingRect(finderview_width, finderview_height);

					int width = inrect.width();
					int height = inrect.height();
					cameraManager.setManualFramingRect(finderview_width, finderview_height, width, height);
				}

				if (intent.hasExtra(Intents.Scan.CAMERA_ID)) {
					int cameraId = intent.getIntExtra(Intents.Scan.CAMERA_ID, -1);
					if (cameraId >= 0) {
						cameraManager.setManualCameraId(cameraId);
					}
				}

				String customPromptMessage = intent.getStringExtra(Intents.Scan.PROMPT_MESSAGE);
				if (customPromptMessage != null) {
					;// statusView.setText(customPromptMessage);
				}

			} else if (dataString != null && dataString.contains("http://www.google")
					&& dataString.contains("/m/products/scan")) {

				// Scan only products and send the result to mobile Product Search.
				source = IntentSource.PRODUCT_SEARCH_LINK;
				sourceUrl = dataString;
				decodeFormats = DecodeFormatManager.PRODUCT_FORMATS;

			} else if (isZXingURL(dataString)) {

				// Scan formats requested in query string (all formats if none specified).
				// If a return URL is specified, send the results there. Otherwise, handle it
				// ourselves.
				source = IntentSource.ZXING_LINK;
				sourceUrl = dataString;
				Uri inputUri = Uri.parse(dataString);
				scanFromWebPageManager = new ScanFromWebPageManager(inputUri);
				decodeFormats = DecodeFormatManager.parseDecodeFormats(inputUri);
				// Allow a sub-set of the hints to be specified by the caller.
				decodeHints = DecodeHintManager.parseDecodeHints(inputUri);

			}

			characterSet = intent.getStringExtra(Intents.Scan.CHARACTER_SET);

		}

		SurfaceView surfaceView = (SurfaceView) findViewById(R.id.surface_preview_view);
		SurfaceHolder surfaceHolder = surfaceView.getHolder();
		if (hasSurface) {
			// The activity was paused but not stopped, so the surface still exists.
			// Therefore
			// surfaceCreated() won't be called, so init the camera here.

			// and by liqj
			// surfaceHolder.setFixedSize(surfaceView.getWidth(), surfaceView.getHeight());
			//////////////
			initCamera(surfaceHolder);
		} else {
			// Install the callback and wait for surfaceCreated() to init the camera.
			surfaceHolder.addCallback(this);
		}
	}

	@Override
	protected void onPause() {
		/*
		 * if (handler != null) { handler.quitSynchronously(); handler = null; }
		 * inactivityTimer.onPause(); ambientLightManager.stop(); beepManager.close();
		 * cameraManager.closeDriver(); // historyManager = null; // Keep for
		 * onActivityResult if (!hasSurface) { SurfaceView surfaceView = (SurfaceView)
		 * findViewById(R.id.surface_preview_view); SurfaceHolder surfaceHolder =
		 * surfaceView.getHolder(); surfaceHolder.removeCallback(this); }
		 */
		this.closeScan();
		super.onPause();
	}

	@SuppressLint("InlinedApi")
	private int getCurrentOrientation() {
		int rotation = getWindowManager().getDefaultDisplay().getRotation();
		if (getResources().getConfiguration().orientation == Configuration.ORIENTATION_LANDSCAPE) {
			switch (rotation) {
			case Surface.ROTATION_0:
			case Surface.ROTATION_90:
				return ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE;
			default:
				return ActivityInfo.SCREEN_ORIENTATION_REVERSE_LANDSCAPE;
			}
		} else {
			switch (rotation) {
			case Surface.ROTATION_0:
			case Surface.ROTATION_270:
				return ActivityInfo.SCREEN_ORIENTATION_PORTRAIT;
			default:
				return ActivityInfo.SCREEN_ORIENTATION_REVERSE_PORTRAIT;
			}
		}
	}

	@Override
	protected void onResume() {
		super.onResume();

		this.openScan();

	}

	private static boolean isZXingURL(String dataString) {
		if (dataString == null) {
			return false;
		}
		for (String url : ZXING_URLS) {
			if (dataString.startsWith(url)) {
				return true;
			}
		}
		return false;
	}

	public CameraManager getCameraManager() {
		return cameraManager;
	}

	public Handler getHandler() {
		return handler;
	}

	public ViewfinderView getViewfinderView() {
		return viewfinderView;
	}

	private void maybeSetClipboard(ResultHandler resultHandler) {
		if (copyToClipboard && !resultHandler.areContentsSecure()) {
			ClipboardInterface.setText(resultHandler.getDisplayContents(), this);
		}
	}

	private void sendReplyMessage(int id, Object arg, long delayMS) {
		if (handler != null) {
			Message message = Message.obtain(handler, id, arg);
			if (delayMS > 0L) {
				handler.sendMessageDelayed(message, delayMS);
			} else {
				handler.sendMessage(message);
			}
		}
	}

	private void handleDecodeInternally(Result rawResult, ResultHandler resultHandler, Bitmap barcode) {

		maybeSetClipboard(resultHandler);

		SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(this);

		if (resultHandler.getDefaultButtonID() != null
				&& prefs.getBoolean(PreferencesActivity.KEY_AUTO_OPEN_WEB, false)) {
			resultHandler.handleButtonPress(resultHandler.getDefaultButtonID());
			return;
		}

		// statusView.setVisibility(View.GONE);
		viewfinderView.setVisibility(View.GONE);
		this.closeScan();
		String resultText = rawResult.getText();
		EditText viewResult = (EditText) findViewById(R.id.edit_cardnumber);
		viewResult.setText(resultText);

		/*
		 * ImageView barcodeImageView = (ImageView)
		 * findViewById(R.id.barcode_image_view); if (barcode == null) {
		 * barcodeImageView.setImageBitmap(BitmapFactory.decodeResource(getResources(),
		 * R.drawable.launcher_icon)); } else {
		 * barcodeImageView.setImageBitmap(barcode); }
		 * 
		 * TextView formatTextView = (TextView) findViewById(R.id.format_text_view);
		 * formatTextView.setText(rawResult.getBarcodeFormat().toString());
		 * 
		 * TextView typeTextView = (TextView) findViewById(R.id.type_text_view);
		 * typeTextView.setText(resultHandler.getType().toString());
		 * 
		 * DateFormat formatter = DateFormat.getDateTimeInstance(DateFormat.SHORT,
		 * DateFormat.SHORT); TextView timeTextView = (TextView)
		 * findViewById(R.id.time_text_view);
		 * timeTextView.setText(formatter.format(rawResult.getTimestamp()));
		 * 
		 * 
		 * TextView metaTextView = (TextView) findViewById(R.id.meta_text_view); View
		 * metaTextViewLabel = findViewById(R.id.meta_text_view_label);
		 * metaTextView.setVisibility(View.GONE);
		 * metaTextViewLabel.setVisibility(View.GONE); Map<ResultMetadataType,Object>
		 * metadata = rawResult.getResultMetadata(); if (metadata != null) {
		 * StringBuilder metadataText = new StringBuilder(20); for
		 * (Map.Entry<ResultMetadataType,Object> entry : metadata.entrySet()) { if
		 * (DISPLAYABLE_METADATA_TYPES.contains(entry.getKey())) {
		 * metadataText.append(entry.getValue()).append('\n'); } } if
		 * (metadataText.length() > 0) { metadataText.setLength(metadataText.length() -
		 * 1); metaTextView.setText(metadataText);
		 * metaTextView.setVisibility(View.VISIBLE);
		 * metaTextViewLabel.setVisibility(View.VISIBLE); } }
		 * 
		 * CharSequence displayContents = resultHandler.getDisplayContents(); TextView
		 * contentsTextView = (TextView) findViewById(R.id.contents_text_view);
		 * contentsTextView.setText(displayContents); int scaledSize = Math.max(22, 32 -
		 * displayContents.length() / 4);
		 * contentsTextView.setTextSize(TypedValue.COMPLEX_UNIT_SP, scaledSize);
		 * 
		 * TextView supplementTextView = (TextView)
		 * findViewById(R.id.contents_supplement_text_view);
		 * supplementTextView.setText(""); supplementTextView.setOnClickListener(null);
		 * do not need if
		 * (PreferenceManager.getDefaultSharedPreferences(this).getBoolean(
		 * PreferencesActivity.KEY_SUPPLEMENTAL, true)) {
		 * SupplementalInfoRetriever.maybeInvokeRetrieval(supplementTextView,
		 * resultHandler.getResult(), historyManager, this); }
		 * 
		 * 
		 * int buttonCount = resultHandler.getButtonCount(); ViewGroup buttonView =
		 * (ViewGroup) findViewById(R.id.result_button_view); buttonView.requestFocus();
		 * for (int x = 0; x < ResultHandler.MAX_BUTTON_COUNT; x++) { TextView button =
		 * (TextView) buttonView.getChildAt(x); if (x < buttonCount) {
		 * button.setVisibility(View.VISIBLE);
		 * button.setText(resultHandler.getButtonText(x)); button.setOnClickListener(new
		 * ResultButtonListener(resultHandler, x)); } else {
		 * button.setVisibility(View.GONE); } }
		 */

	}

	public void restartPreviewAfterDelay(long delayMS) {
		if (handler != null) {
			handler.sendEmptyMessageDelayed(R.id.restart_preview, delayMS);
		}
		resetStatusView();
	}

	private void resetStatusView() {
		// resultView.setVisibility(View.GONE);
		// statusView.setText(R.string.msg_default_status);
		// statusView.setVisibility(View.VISIBLE);
		viewfinderView.setVisibility(View.VISIBLE);
		lastResult = null;
	}

	public void drawViewfinder() {
		viewfinderView.drawViewfinder();
	}

	private void drawResultPoints(Bitmap barcode, float scaleFactor, Result rawResult) {
		ResultPoint[] points = rawResult.getResultPoints();
		if (points != null && points.length > 0) {
			Canvas canvas = new Canvas(barcode);
			Paint paint = new Paint();
			paint.setColor(getResources().getColor(R.color.result_points));
			if (points.length == 2) {
				paint.setStrokeWidth(4.0f);
				drawLine(canvas, paint, points[0], points[1], scaleFactor);
			} else if (points.length == 4 && (rawResult.getBarcodeFormat() == BarcodeFormat.UPC_A
					|| rawResult.getBarcodeFormat() == BarcodeFormat.EAN_13)) {
				// Hacky special case -- draw two lines, for the barcode and metadata
				drawLine(canvas, paint, points[0], points[1], scaleFactor);
				drawLine(canvas, paint, points[2], points[3], scaleFactor);
			} else {
				paint.setStrokeWidth(10.0f);
				for (ResultPoint point : points) {
					if (point != null) {
						canvas.drawPoint(scaleFactor * point.getX(), scaleFactor * point.getY(), paint);
					}
				}
			}
		}
	}

	private static void drawLine(Canvas canvas, Paint paint, ResultPoint a, ResultPoint b, float scaleFactor) {
		if (a != null && b != null) {
			canvas.drawLine(scaleFactor * a.getX(), scaleFactor * a.getY(), scaleFactor * b.getX(),
					scaleFactor * b.getY(), paint);
		}
	}

	private void handleDecodeExternally(Result rawResult, ResultHandler resultHandler, Bitmap barcode) {

		if (barcode != null) {
			viewfinderView.drawResultBitmap(barcode);
		}

		long resultDurationMS;
		if (getIntent() == null) {
			resultDurationMS = DEFAULT_INTENT_RESULT_DURATION_MS;
		} else {
			resultDurationMS = getIntent().getLongExtra(Intents.Scan.RESULT_DISPLAY_DURATION_MS,
					DEFAULT_INTENT_RESULT_DURATION_MS);
		}

		if (resultDurationMS > 0) {
			String rawResultString = String.valueOf(rawResult);
			if (rawResultString.length() > 32) {
				rawResultString = rawResultString.substring(0, 32) + " ...";
			}
			// statusView.setText(getString(resultHandler.getDisplayTitle()) + " : " +
			// rawResultString);
		}

		maybeSetClipboard(resultHandler);

		switch (source) {
		case NATIVE_APP_INTENT:
			// Hand back whatever action they requested - this can be changed to
			// Intents.Scan.ACTION when
			// the deprecated intent is retired.
			Intent intent = new Intent(getIntent().getAction());
			intent.addFlags(Intents.FLAG_NEW_DOC);
			intent.putExtra(Intents.Scan.RESULT, rawResult.toString());
			intent.putExtra(Intents.Scan.RESULT_FORMAT, rawResult.getBarcodeFormat().toString());
			byte[] rawBytes = rawResult.getRawBytes();
			if (rawBytes != null && rawBytes.length > 0) {
				intent.putExtra(Intents.Scan.RESULT_BYTES, rawBytes);
			}
			Map<ResultMetadataType, ?> metadata = rawResult.getResultMetadata();
			if (metadata != null) {
				if (metadata.containsKey(ResultMetadataType.UPC_EAN_EXTENSION)) {
					intent.putExtra(Intents.Scan.RESULT_UPC_EAN_EXTENSION,
							metadata.get(ResultMetadataType.UPC_EAN_EXTENSION).toString());
				}
				Number orientation = (Number) metadata.get(ResultMetadataType.ORIENTATION);
				if (orientation != null) {
					intent.putExtra(Intents.Scan.RESULT_ORIENTATION, orientation.intValue());
				}
				String ecLevel = (String) metadata.get(ResultMetadataType.ERROR_CORRECTION_LEVEL);
				if (ecLevel != null) {
					intent.putExtra(Intents.Scan.RESULT_ERROR_CORRECTION_LEVEL, ecLevel);
				}
				@SuppressWarnings("unchecked")
				Iterable<byte[]> byteSegments = (Iterable<byte[]>) metadata.get(ResultMetadataType.BYTE_SEGMENTS);
				if (byteSegments != null) {
					int i = 0;
					for (byte[] byteSegment : byteSegments) {
						intent.putExtra(Intents.Scan.RESULT_BYTE_SEGMENTS_PREFIX + i, byteSegment);
						i++;
					}
				}
			}
			sendReplyMessage(R.id.return_scan_result, intent, resultDurationMS);
			break;

		case PRODUCT_SEARCH_LINK:
			// Reformulate the URL which triggered us into a query, so that the request goes
			// to the same
			// TLD as the scan URL.
			int end = sourceUrl.lastIndexOf("/scan");
			String productReplyURL = sourceUrl.substring(0, end) + "?q=" + resultHandler.getDisplayContents()
					+ "&source=zxing";
			sendReplyMessage(R.id.launch_product_query, productReplyURL, resultDurationMS);
			break;

		case ZXING_LINK:
			if (scanFromWebPageManager != null && scanFromWebPageManager.isScanFromWebPage()) {
				String linkReplyURL = scanFromWebPageManager.buildReplyURL(rawResult, resultHandler);
				scanFromWebPageManager = null;
				sendReplyMessage(R.id.launch_product_query, linkReplyURL, resultDurationMS);
			}
			break;
		default:
			break;
		}
	}

	public void handleDecode(Result rawResult, Bitmap barcode, float scaleFactor) {
		inactivityTimer.onActivity();
		lastResult = rawResult;
		ResultHandler resultHandler = ResultHandlerFactory.makeResultHandler(this, rawResult);

		boolean fromLiveScan = barcode != null;
		if (fromLiveScan) {
			// historyManager.addHistoryItem(rawResult, resultHandler);
			// Then not from history, so beep/vibrate and we have an image to draw on
			beepManager.playBeepSoundAndVibrate();
			drawResultPoints(barcode, scaleFactor, rawResult);
		}

		switch (source) {
		case NATIVE_APP_INTENT:
		case PRODUCT_SEARCH_LINK:
			handleDecodeExternally(rawResult, resultHandler, barcode);
			break;
		case ZXING_LINK:
			if (scanFromWebPageManager == null || !scanFromWebPageManager.isScanFromWebPage()) {
				handleDecodeInternally(rawResult, resultHandler, barcode);
			} else {
				handleDecodeExternally(rawResult, resultHandler, barcode);
			}
			break;
		case NONE:
			SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(this);
			if (fromLiveScan && prefs.getBoolean(PreferencesActivity.KEY_BULK_MODE, false)) {
				Toast.makeText(getApplicationContext(),
						getResources().getString(R.string.msg_bulk_mode_scanned) + " (" + rawResult.getText() + ')',
						Toast.LENGTH_SHORT).show();
				maybeSetClipboard(resultHandler);
				// Wait a moment or else it will scan the same barcode continuously about 3
				// times
				restartPreviewAfterDelay(BULK_MODE_SCAN_DELAY_MS);
			} else {
				handleDecodeInternally(rawResult, resultHandler, barcode);
			}
			break;
		}
	}

	private void decodeOrStoreSavedBitmap(Bitmap bitmap, Result result) {
		// Bitmap isn't used yet -- will be used soon
		if (handler == null) {
			savedResultToShow = result;

		} else {
			if (result != null) {
				savedResultToShow = result;
			}
			if (savedResultToShow != null) {
				Message message = Message.obtain(handler, R.id.decode_succeeded, savedResultToShow);
				handler.sendMessage(message);
			}
			savedResultToShow = null;
		}
	}

	private void displayFrameworkBugMessageAndExit() {
		AlertDialog.Builder builder = new AlertDialog.Builder(this);
		builder.setTitle(getString(R.string.app_name));
		builder.setMessage(getString(R.string.msg_camera_framework_bug));
		builder.setPositiveButton(R.string.button_ok, new FinishListener(this));
		builder.setOnCancelListener(new FinishListener(this));
		builder.show();
	}

	private void initCamera(SurfaceHolder surfaceHolder) {
		if (surfaceHolder == null) {
			throw new IllegalStateException("No SurfaceHolder provided");
		}
		if (cameraManager.isOpen()) {
			Log.w(TAG, "initCamera() while already open -- late SurfaceView callback?");
			return;
		}
		try {
			cameraManager.openDriver(surfaceHolder);
			// Creating the handler starts the preview, which can also throw a
			// RuntimeException.
			if (handler == null) {
				handler = new CaptureActivityHandler(this, decodeFormats, decodeHints, characterSet, cameraManager);
			}
			decodeOrStoreSavedBitmap(null, null);
		} catch (IOException ioe) {
			Log.w(TAG, ioe);
			displayFrameworkBugMessageAndExit();
		} catch (RuntimeException e) {
			// Barcode Scanner has seen crashes in the wild of this variety:
			// java.?lang.?RuntimeException: Fail to connect to camera service
			Log.w(TAG, "Unexpected error initializing camera", e);
			displayFrameworkBugMessageAndExit();
		}
	}

	@Override
	public void surfaceCreated(SurfaceHolder holder) {
		// TODO Auto-generated method stub
		if (holder == null) {
			Log.e(TAG, "*** WARNING *** surfaceCreated() gave us a null surface!");
		}
		if (!hasSurface) {
			hasSurface = true;
			initCamera(holder);
		}
	}

	@Override
	public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
		// TODO Auto-generated method stub

	}

	@Override
	public void surfaceDestroyed(SurfaceHolder holder) {
		// TODO Auto-generated method stub
		hasSurface = false;
	}

	@Override
	public void run() {
		// TODO Auto-generated method stub

	}

	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
		switch (v.getId()) {
		case R.id.button_info_confirm:
			if (verify_confirm_dialog != null) {
				verify_confirm_dialog.dismiss();
			}
			break;
		case R.id.button_info_recycle:
			if (verify_confirm_dialog != null) {
				verify_confirm_dialog.dismiss();
				if (loginUser != null) {
					loginUser.commit_verify_after_do(this.createVerifyInfoByUi());
				}
			}
			break;
		default:
			break;
		}
	}

	class ExitLeaveMyDialogListener implements LeaveMyDialogListener {
		private Activity app = null;

		public ExitLeaveMyDialogListener(Activity app) {
			super();
			this.app = app;
		}

		@Override
		public void onClick(View view) {
			// TODO Auto-generated method stub
			switch (view.getId()) {
			case R.id.button_autodialog_ok:
				if (app != null) {
					app.finish();
				}
				break;

			default:
				break;
			}
		}

	}

	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		switch (keyCode) {
		case KeyEvent.KEYCODE_BACK:

			if (is_regist) {
				is_regist = false;
				ViewfinderView viewFinder = (ViewfinderView) VeriryActivity.this.findViewById(R.id.viewfinder_view);
				viewFinder.setVisibility(View.VISIBLE);

				RelativeLayout registInfoLayout = (RelativeLayout) VeriryActivity.this
						.findViewById(R.id.relativelayout_regist_info);
				registInfoLayout.setVisibility(View.GONE);

				return true;
			}

			if (source == IntentSource.NATIVE_APP_INTENT) {
				setResult(RESULT_CANCELED);
				AutoAlertDialog dialog = new AutoAlertDialog(this, this.getResources().getString(R.string.string_exit),
						this.getResources().getString(R.string.string_sure_loginout), 5,
						new ExitLeaveMyDialogListener(this));
				dialog.show();
				// finish();
				return true;
			}
			if ((source == IntentSource.NONE || source == IntentSource.ZXING_LINK) && lastResult != null) {
				restartPreviewAfterDelay(0L);
				return true;
			}
			break;
		case KeyEvent.KEYCODE_FOCUS:
		case KeyEvent.KEYCODE_CAMERA:
			// Handle these events so they don't launch the Camera app
			return true;
		// Use volume up/down to turn on light
		case KeyEvent.KEYCODE_VOLUME_DOWN:
			cameraManager.setTorch(false);
			return true;
		case KeyEvent.KEYCODE_VOLUME_UP:
			cameraManager.setTorch(true);
			return true;
		}
		AutoAlertDialog dialog = new AutoAlertDialog(this, this.getResources().getString(R.string.string_exit),
				this.getResources().getString(R.string.string_sure_loginout), 5, new ExitLeaveMyDialogListener(this));
		dialog.show();
		return true;
	}

}
