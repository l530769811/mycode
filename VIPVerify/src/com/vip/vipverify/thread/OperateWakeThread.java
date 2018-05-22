package com.vip.vipverify.thread;

import java.util.ArrayList;
import java.util.List;

import com.vip.vipverify.operator.DoOperator;

public class OperateWakeThread extends WakeThread {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	private List<DoOperator> listOperate = new ArrayList<DoOperator>();

	@Override
	public boolean postOperate(DoOperator operate) {
		boolean bret = false;
		synchronized (listOperate) {
			bret = listOperate.add(operate);
		}
		this.wakeUp();

		return bret;
	}

	@Override
	public DoOperator GetOperate() {
		DoOperator operate_ret = null;
		synchronized (listOperate) {
			if (listOperate.size() > 0)
				operate_ret = listOperate.remove(0);
		}
		return operate_ret;
	}

	@Override
	public void myRun() throws InterruptedException {
		// TODO Auto-generated method stub
		// MyBaseDataProxy.this.mdb =
		// SQLiteDatabase.openDatabase(MyBaseDataProxy.this.string_db_name,
		// null,SQLiteDatabase.OPEN_READWRITE);
		DoOperator operate = null;
		while ((operate = GetOperate()) != null) {
			operate.ToDoOperate();
		}
		// MyBaseDataProxy.this.mdb.close();
	}

}
