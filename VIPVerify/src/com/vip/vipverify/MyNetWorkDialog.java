package com.vip.vipverify;

import java.util.ArrayList;
import java.util.List;

import com.vip.vipverify.SearchServerDialog.SelectServerListening;
import com.vip.vipverify.net.MyNetworkInfo;
import com.vip.vipverify.net.ServerNetInfo;
import com.vip.vipverify.preferences.MyPreferences;

import android.annotation.SuppressLint;
import android.app.Dialog;
import android.content.Context;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.SpinnerAdapter;

@SuppressLint("CommitPrefEdits")
public class MyNetWorkDialog extends Dialog implements OnClickListener, OnItemSelectedListener {

	private SelectServerListening listener = null;
	private MyNetworkInfo cur_net_info = null;

	public MyNetWorkDialog(Context context, SelectServerListening listener) {
		super(context);
		// TODO Auto-generated constructor stub
		this.listener = listener;
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_net_work);

		PreferenceManager.setDefaultValues(this.getContext(), R.xml.preferences, false);

		Button button_ok = (Button) findViewById(R.id.button_network_ok);
		button_ok.setOnClickListener(this);

		Button button_cancel = (Button) findViewById(R.id.button_network_cannel);
		button_cancel.setOnClickListener(this);

		// Spinner net_kind_spinner = (Spinner) findViewById(R.id.spinner_network_kind);
		//
		List<MyNetworkInfo> list_item_string = new ArrayList<MyNetworkInfo>();
		// list_item_string.add("ddddd");
		//
		// if (net_kind_spinner != null) {
		//
		// net_kind_spinner.setAdapter(new ArrayAdapter<String>(this.getContext(),
		// android.R.layout.simple_spinner_item, list_item_string));
		// // ArrayAdapter<String> list_adapter = new
		// // ArrayAdapter<String>(this.getContext(),
		// // android.R.layout.simple_list_item_checked,
		// // list_item_string);
		// // net_kind_spinner.setAdapter(list_adapter);
		// }

		Spinner server_kind_spinner = (Spinner) findViewById(R.id.spinner_server_kind);
		SharedPreferences share_pre = PreferenceManager.getDefaultSharedPreferences(this.getContext());
		if (share_pre != null) {
			String cur_server_kind = share_pre.getString(MyPreferences.preferences_cur_server_kind, "0");
			String tcp_ip = share_pre.getString(MyPreferences.preferences_edittext_sever_tcp_ip, "000.000.000.000");
			String tcp_port = share_pre.getString(MyPreferences.preferences_edittext_sever_tcp_port, "000");
			String tcp_kind_name = share_pre.getString(MyPreferences.preferences_spinner_server_tcp_kind,
					this.getContext().getResources().getString(R.string.string_tcp_of_kind));

			String udp_ip = share_pre.getString(MyPreferences.preferences_edittext_sever_udp_ip, "111.111.111.111");
			String udp_port = share_pre.getString(MyPreferences.preferences_edittext_sever_udp_port, "111");
			String udp_kind_name = share_pre.getString(MyPreferences.preferences_spinner_server_udp_kind,
					this.getContext().getResources().getString(R.string.string_udp_of_kind));

			list_item_string.add(new MyNetworkInfo(0, tcp_ip, Integer.valueOf(tcp_port), tcp_kind_name));
			list_item_string.add(new MyNetworkInfo(1, udp_ip, Integer.valueOf(udp_port), udp_kind_name));

			if (server_kind_spinner != null) {

				SpinnerAdapter spinner_adapter = new ArrayAdapter<MyNetworkInfo>(this.getContext(),
						android.R.layout.simple_spinner_item, list_item_string);
				server_kind_spinner.setSelection(0);
				server_kind_spinner.setAdapter(spinner_adapter);
				server_kind_spinner.setSelection(Integer.valueOf(cur_server_kind));
				server_kind_spinner.setOnItemSelectedListener(this);
			}
		}

	}

	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
		SharedPreferences share_pre = PreferenceManager.getDefaultSharedPreferences(this.getContext());
		switch (v.getId()) {
		case R.id.button_network_ok:

			if (share_pre != null) {
				Editor editor = share_pre.edit();
				int cur = 0;
				Spinner server_kind_spinner = (Spinner) findViewById(R.id.spinner_server_kind);
				cur = (int) server_kind_spinner.getSelectedItemId();
				MyNetworkInfo net_info = (null);
				Object obj = server_kind_spinner.getSelectedItem();
				if(obj instanceof MyNetworkInfo) {
					net_info = cur_net_info = (MyNetworkInfo)obj;
				}
				editor.putString(MyPreferences.preferences_cur_server_kind, String.valueOf(cur));

				EditText ip_edittext = (EditText) findViewById(R.id.editText_server_ip_addr);
				EditText port_edittext = (EditText) findViewById(R.id.editText_server_port);
				switch (cur) {
				case 0:
					editor.putString(MyPreferences.preferences_edittext_sever_tcp_ip, ip_edittext.getText().toString());
					editor.putString(MyPreferences.preferences_edittext_sever_tcp_port,
							port_edittext.getText().toString());
					break;
				case 1:
					editor.putString(MyPreferences.preferences_edittext_sever_udp_ip, ip_edittext.getText().toString());
					editor.putString(MyPreferences.preferences_edittext_sever_udp_port,
							port_edittext.getText().toString());
					break;
				default:
					break;
				}
				editor.commit();
				if (this.listener != null) {
					String ip = "";
					int port = -1;
					int server_net_kind = -1;
					if(net_info!=null) {
						ip = net_info.getNetwork_ip();
						port = net_info.getNetwork_port();
						server_net_kind = net_info.getNetwork_kind();
					}
					ServerNetInfo cur_server_info = new ServerNetInfo("unknow", ip_edittext.getText().toString(),
							Integer.valueOf(port_edittext.getText().toString()), cur);
					this.listener.SelectServer(cur_server_info);
				}
			}

			this.dismiss();
			break;

		case R.id.button_network_cannel:
			this.dismiss();
			break;

		default:
			break;
		}
	}

	@Override
	public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
		// TODO Auto-generated method stub
		Spinner server_kind_spinner = (Spinner) findViewById(R.id.spinner_server_kind);
		if (server_kind_spinner != null) {
			MyNetworkInfo info = (MyNetworkInfo) server_kind_spinner.getItemAtPosition(position);
			Object obj = server_kind_spinner.getSelectedItem();
			if(obj instanceof MyNetworkInfo) {
				cur_net_info = (MyNetworkInfo)obj;
			}
			
			EditText edit_ip = (EditText) findViewById(R.id.editText_server_ip_addr);
			if (edit_ip != null && info != null) {
				edit_ip.setText(info.getNetwork_ip());
			}
			EditText edit_port = (EditText) findViewById(R.id.editText_server_port);
			if (edit_port != null && info != null) {
				edit_port.setText(String.valueOf(info.getNetwork_port()));
			}
		}
	}

	@Override
	public void onNothingSelected(AdapterView<?> parent) {
		// TODO Auto-generated method stub

	}
}
