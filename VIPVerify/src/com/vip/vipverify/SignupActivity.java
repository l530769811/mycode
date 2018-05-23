package com.vip.vipverify;

import java.io.Serializable;

import com.vip.vipverify.client.ClientManager;
import com.vip.vipverify.net.UserSignupInfo;

import android.app.Dialog;
import android.content.Context;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextUtils;
import android.text.TextWatcher;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.animation.AlphaAnimation;
import android.view.animation.Animation;
import android.view.animation.LinearInterpolator;
import android.widget.Button;
import android.widget.EditText;

public class SignupActivity extends Dialog implements OnClickListener, Serializable {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private SignupListening listener = null;
	private UserSignupInfo info = new UserSignupInfo("", "", "", "", "000000");
	private ClientManager client_mgr = null;

	public interface SignupListening {
		public void Signup(UserSignupInfo info);
	}

	public SignupActivity(Context context, ClientManager client_manager, SignupListening listener) {
		super(context);
		this.client_mgr = client_manager;
		this.listener = listener;
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_sign_up);

		Button button_signin_ok = (Button) findViewById(R.id.button_signin_ok);
		button_signin_ok.setOnClickListener(this);

		Button button_signin_cancel = (Button) findViewById(R.id.button_signin_cancel);
		button_signin_cancel.setOnClickListener(this);

		EditText edit = (EditText) this.findViewById(R.id.editText_signin_username);
		edit.addTextChangedListener(new TextFilter(edit));

		edit = (EditText) this.findViewById(R.id.editText_signin_userpassword);
		edit.addTextChangedListener(new TextFilter(edit));

		edit = (EditText) this.findViewById(R.id.editText_signin_verify_userpassword);
		edit.addTextChangedListener(new TextFilter(edit));

		edit = (EditText) this.findViewById(R.id.editText_signin_phone);
		edit.addTextChangedListener(new TextFilter(edit));

		edit = (EditText) this.findViewById(R.id.editText_siginin_identify_code);
		edit.addTextChangedListener(new TextFilter(edit));
	}

	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
		switch (v.getId()) {
		case R.id.button_signin_ok:
			if (this.isRightRegistInfo()) {
				if (listener != null) {
					listener.Signup(info);
				}
			}

			break;

		case R.id.button_signin_cancel:
			// this.finish();
			this.dismiss();
			break;

		default:
			break;
		}
	}

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
			if (vv != null && vv.getId() == R.id.editText_signin_userpassword)// password edit
			{
				// so set verify password edit
				EditText edit = (EditText) SignupActivity.this.findViewById(R.id.editText_signin_verify_userpassword);
				SignupActivity.this.stopFlick(edit);
			}
			if (vv != null && vv.getId() == R.id.editText_signin_verify_userpassword)// password verify edit
			{
				// so set password edit
				EditText edit = (EditText) SignupActivity.this.findViewById(R.id.editText_signin_userpassword);
				SignupActivity.this.stopFlick(edit);
			}
			SignupActivity.this.stopFlick(vv);
		}

		@Override
		public void afterTextChanged(Editable s) {
			// TODO Auto-generated method stub

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

	private boolean isRightRegistInfo() {
		boolean ret = true;

		EditText edit = (EditText) this.findViewById(R.id.editText_signin_username);
		if (TextUtils.isEmpty(edit.getText().toString())) {
			this.startFlick(edit);
			return ret = false;
		}
	

		EditText edit_signin_password = (EditText) this.findViewById(R.id.editText_signin_userpassword);
		String first_password = edit_signin_password.getText().toString();
		if (TextUtils.isEmpty(first_password)) {
			this.startFlick(edit_signin_password);
			return ret = false;
		}

		EditText edit_singin_passwordverify = (EditText) this.findViewById(R.id.editText_signin_verify_userpassword);
		String verify_password = edit_singin_passwordverify.getText().toString();
		if (TextUtils.isEmpty(verify_password)) {
			this.startFlick(edit_singin_passwordverify);
			return ret = false;
		}

		if (first_password.equals(verify_password) == false) {
			this.startFlick(edit_signin_password);
			this.startFlick(edit_singin_passwordverify);
			return ret = false;
		}
		

		EditText edit_phone = (EditText) this.findViewById(R.id.editText_signin_phone);
		if (edit_phone.isEnabled()) {
			String string_first_name = edit_phone.getText().toString();
			if (TextUtils.isEmpty(string_first_name)) {
				this.startFlick(edit);
				return ret = false;
			}
		}
		

		EditText edit_identify_code = (EditText) this.findViewById(R.id.editText_siginin_identify_code);
		if (edit_identify_code.isEnabled()) {
			String string_identify_code = edit_identify_code.getText().toString();
			if (TextUtils.isEmpty(string_identify_code)) {
				this.startFlick(edit);
				return ret = false;
			}			
		}
		
		info.setUser_name(edit.getText().toString());
		info.setUser_password(edit_signin_password.getText().toString());
		info.setUser_phone( edit_phone.getText().toString());
		info.setIdentify_code(edit_identify_code.getText().toString());

		return ret;
	}
}
