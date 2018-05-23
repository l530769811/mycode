package com.vip.vipverify;
import com.vip.vipverify.R;

public enum MyErrors {
	NetError(-1, R.string.string_net_error),
	NoError(0, R.string.string_do_done),
	LoginVerifyError(1, R.string.string_login_verify_error);
	
	
	
	final int nid;
	final int string_id;
	
	MyErrors(int nid, int string_id){
		this.nid = nid;
		this.string_id = string_id;
	}
	
	static int GetStringIdFromErrorID(int error_id)
	{		
		int nid = 0;
		
		for(MyErrors errors : MyErrors.values())
		{
			if(errors.nid == error_id)
			{
				nid = errors.string_id;
			}
		}
		
		return nid;		
	}
}
