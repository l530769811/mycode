package com.vip.vipverify;

import java.util.Timer;
import java.util.TimerTask;

import android.app.Dialog;
import android.content.Context;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;

public class AutoAlertDialog extends Dialog implements OnClickListener {

	private LeaveMyDialogListener listener = null;
	private Timer timer = null;
	private int times = 10;

	String title = "";
	String text = "";

	public interface LeaveMyDialogListener {
		public void onClick(View view);
	}

	class AutoHideTimerTask extends TimerTask {
		
		private String button_text = "";
		private int times = AutoAlertDialog.this.times;

		public AutoHideTimerTask() {
			super();
			// TODO Auto-generated constructor stub
			Button v = (Button) findViewById(R.id.button_autodialog_ok);
			button_text = String.valueOf(v.getText());
		}

		class UiUpdataRunable implements Runnable {
			private int times = 0;
			private String text = "";

			public UiUpdataRunable(String text, int times) {
				super();
				this.times = times;
				this.text = text;
			}

			@Override
			public void run() {
				// TODO Auto-generated method stub
				Button v = (Button) findViewById(R.id.button_autodialog_ok);
				v.setText(text + " ( " + String.valueOf(times) + " ) ");
			}

		}

		@Override
		public void run() {
			// TODO Auto-generated method stub
			if (this.times >= 0) {
				
				// Button v = (Button) findViewById(R.id.button_autodialog_ok);
				// v.setText(v.getText() + " ( " + String.valueOf(times) + " ) ");
				View v = findViewById(R.id.linearLayout_auto_dialog_main);
				if (v != null) {
					v.post(new UiUpdataRunable(button_text, times));
				}
				times--;
			} else {
				times = AutoAlertDialog.this.times;
				if (timer != null) {
					timer.cancel();
					timer = null;
				}
				AutoAlertDialog.this.onClick(findViewById(R.id.button_autodialog_ok));
			}
		}

	}

	public AutoAlertDialog(Context context, String title, String text, int timers, LeaveMyDialogListener listener) {
		super(context);
		// TODO Auto-generated constructor stub
		this.text = text;
		this.title = title;
		this.times = times;
		this.listener = listener;

	}

	@Override
	public void show() {
		// TODO Auto-generated method stub
		super.show();

	}

	@Override
	protected void onStart() {
		// TODO Auto-generated method stub
		super.onStart();
	}

	@Override
	protected void onStop() {
		// TODO Auto-generated method stu
		// if (timer != null) {
		// timer.cancel();
		// timer = null;
		// }
		super.onStop();
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_auto_dialog);
		if (title != null) {
			this.setTitle(title);
		}
		Button button_v = (Button) findViewById(R.id.button_autodialog_ok);
		button_v.setOnClickListener(this);
		button_v = (Button) findViewById(R.id.button_autodialog_cancel);
		button_v.setOnClickListener(this);
		
		TextView v = (TextView) this.findViewById(R.id.textView_autodialog_text);
		if (text != null) {
			v.setText(text);
		}

		if (timer == null) {
			timer = new Timer();
			timer.schedule(new AutoHideTimerTask(), 500, 1000);
		}
	}

	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
		if (listener != null) {
			listener.onClick(v);
		}
		
		if (this.isShowing()) {
			// if (timer != null) {
			// timer.cancel();
			// timer = null;
			// }
			this.dismiss();
		}
	}

}
