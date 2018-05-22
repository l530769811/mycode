package com.vip.vipverify.operator;

import java.io.Serializable;

import com.common.my_message.MessageSpreader;
import com.vip.vipverify.db.MyBaseDataProxy;

import android.os.Message;

public class DeleteSqlDoOperator implements DoOperator, Serializable {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private String string_sql = null;
	private MyBaseDataProxy mdb = null;
	private MessageSpreader message_handler = null;
	private int message_key_suc = 0;
	private int message_key_fail = 0;

	public DeleteSqlDoOperator(String string_sql, MyBaseDataProxy mdb, MessageSpreader message_handler,
			int message_key_suc, int message_key_fail) {

		this.string_sql = string_sql;
		this.mdb = mdb;
		this.message_handler = message_handler;
		this.message_key_suc = message_key_suc;
		this.message_key_fail = message_key_fail;
	}

	@Override
	public int ToDoOperate() {
		// TODO Auto-generated method stub
		int ret = -1;
		if (string_sql != null && mdb != null) {
			try {
				boolean result = mdb.execSql(string_sql);
				if (result != false) {
					if (message_handler != null) {
						Message message = Message.obtain();
						message.what = this.message_key_suc;
						
						message_handler.sendMessage(message);
					}
					ret = 0;

				}

			} catch (Exception e) {
				// TODO: handle exception
			}
			if (ret == -1) {
				if (message_handler != null) {
					Message message = Message.obtain();
					message.what = this.message_key_fail;
					message_handler.sendMessage(message);

				}
			}
		}
		return ret;
	}

}
