package com.vip.vipverify;
import com.vip.vipverify.R;

public enum MyErrors {
	NetError(-1, R.string.string_net_error),
	NoError(0, R.string.string_do_done),
	LoginVerifyError(1, R.string.string_login_verify_error),
	SignupError(2, R.string.string_signup_error),
	ReloginUserError(3, R.string.string_relogin_user),
	DiffLoginUserError(4, R.string.string_diff_loginuser),
	CardVerifyError(5, R.string.string_card_verify_error),
	RegistError(6,R.string.string_regist_error),
	NetConnectBreakError(7, R.string.string_net_break_error),
	CardConsumeError(8, R.string.string_consume_error);
	
	public final int nid;
	public final int string_id;
	
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
