package com.vip.vipverify;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;

import com.vip.vipverify.client.ClientManager;
import com.vip.vipverify.my_arg.EmptyMyArg;
import com.vip.vipverify.net.NetSocketData;
import com.vip.vipverify.net.ServerNetInfo;
import com.vip.vipverify.net.SearchServerNetSocketData;
import com.vip.vipverify.net.SocketReceiveListeningChannelList;
import com.vip.vipverify.net_data_parse.ServerInfoNetDataParse;

import android.app.Dialog;
import android.content.Context;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.BaseAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.Toast;
import android.view.View.OnClickListener;

public class SearchServerDialog extends Dialog implements OnClickListener {

	private SelectServerListening listener = null;
	private ClientManager client_manager = null;
	private SocketReceiveListeningChannelList broadcast_rev_listener = null;

	private BaseAdapter list_adapter = null;
	private List<ServerNetInfo> list_item_string = new ArrayList<ServerNetInfo>();
	private ListView list_view = null;
	

	class UdpBroadcastListening extends SocketReceiveListeningChannelList implements Serializable {
		/**
		 * 
		 */
		private static final long serialVersionUID = 1L;
	

		@Override
		public boolean receive_data(byte[] data, int len) {
			// TODO Auto-generated method stub

			boolean bret = true;
			ServerInfoNetDataParse net_data = new ServerInfoNetDataParse(new EmptyMyArg());
			if (net_data.Parse(data, len) == true) {
				//list_view = (ListView) findViewById(R.id.listview_searchservser_name);
				if (list_view != null) {
					if (list_view != null && list_adapter != null) {
						list_item_string.add(new ServerNetInfo(net_data.getServer_name(), net_data.getServer_ip(),
								net_data.getServer_port(), net_data.getServer_net_kind()));
						list_view.post(new Runnable() {
							
							@Override
							public void run() {
								// TODO Auto-generated method stub
								list_view.setAdapter(list_adapter);
							}
						});
					}
				}
				bret =  false;
			}
			return bret; 
		}
	}

	public interface SelectServerListening {
		public void SelectServer(ServerNetInfo info);
	}

	public SearchServerDialog(Context context) {
		super(context);
	}

	public SearchServerDialog(Context context, ClientManager client_manager, SelectServerListening listener, SocketReceiveListeningChannelList socket_listener) {
		super(context);
		// TODO Auto-generated constructor stub
		this.listener = listener;
		this.client_manager = client_manager;
		broadcast_rev_listener = new UdpBroadcastListening();

		if (client_manager != null) {
			this.client_manager.setOnSocketReceiveListener(broadcast_rev_listener);
			broadcast_rev_listener.SetOnSocketReceiveListening(socket_listener);
		}
	}

	private MyOrdersAdapter old = null;

	// @Override
	// protected void onCreate(Bundle savedInstanceState) {
	// // TODO Auto-generated method stub
	// super.onCreate(savedInstanceState);
	// }

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_search_server);


		list_view = (ListView) findViewById(R.id.listview_searchservser_name);
		if (list_view != null) {
			// old = new OrdersAdapter();
			list_adapter = new ArrayAdapter<ServerNetInfo>(this.getContext(), android.R.layout.simple_list_item_checked,
					list_item_string);
			list_view.setAdapter(list_adapter);
		}

		Button btn_searchserver = (Button) this.findViewById(R.id.button_search_server);
		btn_searchserver.setOnClickListener(this);
		Button btn_search_cancle = (Button) this.findViewById(R.id.button_search_ok);
		btn_search_cancle.setOnClickListener(this);

	}

	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
		switch (v.getId()) {
		case R.id.button_search_server:
			if (client_manager != null) {
				EditText view = (EditText)this.findViewById(R.id.edittext_searseserver_ip);
				String ip = "";
				if(view!=null)
				{
					ip = view.getText().toString();
				}
				if(ip==null || ip.isEmpty()==true) {
					ip = "255.255.255.255";
				}
				
				NetSocketData data = new SearchServerNetSocketData(SearchServerNetSocketData.getLocalIpAddress(),
						ClientManager.udp_receiver_port);
				
				list_item_string.clear();
				list_view.setAdapter(list_adapter);
				client_manager.find_server(data, ip, ClientManager.udp_send_port);
			}

			break;
		case R.id.button_search_ok:
			boolean bexit = true;
			if (listener != null) {
				
				if (list_view != null && list_adapter != null) {
					if (list_view.getCount() > 0 && list_adapter.getCount() > 0) {
						int position = list_view.getCheckedItemPosition();
						if (position != AdapterView.INVALID_POSITION) {
							ServerNetInfo info = (ServerNetInfo) list_adapter.getItem(position);
							if (info != null) {
								listener.SelectServer(info);
							}
						} else {
							Toast.makeText(this.getContext(), R.string.string_notify_select_server, 1000).show();
							
						}
					}
				}
			}
			if (bexit == true) {
				this.dismiss();
			}
			break;
		}

	}
}
