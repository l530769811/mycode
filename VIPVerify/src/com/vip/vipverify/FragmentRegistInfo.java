package com.vip.vipverify;



import android.app.Fragment;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;

public class FragmentRegistInfo extends Fragment implements OnClickListener {

	private View view;
	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void onActivityCreated(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onActivityCreated(savedInstanceState);
		
		init();
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		view = inflater.inflate(R.layout.fragment_regist_info, container, false);
		return super.onCreateView(inflater, container, savedInstanceState);
	}
	
	public View GetView()
	{
		return view;		
	}
	
	private int init()
	{
		int ret = -1;
		return ret;
	}
	
	

}
