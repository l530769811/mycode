package com.vip.vipverify.client;

import java.io.Serializable;

import com.common.my_message.MessageSpreader;
import com.vip.vipverify.Md5Unit;
import com.vip.vipverify.VeriryActivity;
import com.vip.vipverify.db.MyBaseDataProxy;
import com.vip.vipverify.db.QueryUnit;
import com.vip.vipverify.operator.DeleteSqlDoOperator;
import com.vip.vipverify.operator.ExecSqlOperator;
import com.vip.vipverify.operator.SelectSqlDoOperator;

import android.annotation.SuppressLint;

public class OfflineClientUser extends ClientUser implements Serializable {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	final String string_username = "unline user";
	final static String insert_clientuser_data = "INSERT INTO tlVeriryClientUser ( "
			+ "										 UserName, " + "										 Password, "
			+ "										 Phone, " 
			+ "										 Describe, "
			+ "										 ExtraText, "
			+ "										 CreateTime "
			+ "										 ) VALUES ( " + "										 '%s', "
			+ "										 '%s', " + "										 '%s', "
			+ "										 '%s', " + "										 DATE('NOW'));";
	final static String update_user_password_data = "UPDATE tlVeriryClientUser SET Password='%s' WHERE UserName='%s';";
	final static String select_user_data = "SELECT * FROM tlVeriryClientUser;";
	final static String select_Login = "SELECT * FROM tlVeriryClientUser WHERE UserName='%s' and Password='%s';";

	final static String insert_carduser_data = "INSERT INTO tlVeriryCardUser ( "
			+ "									  CardNumber,  "
			+ "									  CardUserPassword,  "
			+ "									  CardPasswordEncrypt,  "
			+ "									  CarUserFirname,  "
			+ "									  Describe,  "
			+ "									  CardRegistOwnUser,  "
			+ "									  CardRegistUserPhone,  "
			+ "									  CardRegistUserSex,  "
			+ "									  CardRegistTime,  "
			+ "									  ExtraText) VALUES (" + "									  '%s',"
			+ " '%s'," + "									  '%s',  " + "									  '%s',  "
			+ "									  '%s',  " + "									  '%s',  "
			+ "									  '%s',  " + "									  %d,  "
			+ "									  DATE('NOW'),  " + "									  '%s');";
	final static String delete_carduser_data = "DELETE FROM tlVeriryCardUser WHERE CardNumber='%s';";
	final static String update_carduser_password_data = "UPDATE tlVeriryCardUser SET CardUserPassword='%s',  "
			+ "												  CardPasswordEncrypt='%s' WHERE CardNumber='%s';";
	final static String update_carduser_firstname_data = "UPDATE tlVeriryCardUser SET CarUserFirname='%s' WHERE CardNumber='%s';";
	final static String update_carduser_sex_data = "UPDATE tlVeriryCardUser SET CardRegistUserSex='%d' WHERE CardNumber='%s';";
	final static String select_card_info = "SELECT * FROM tlVeriryCardUser WHERE CardNumber='%s';";
	final static String select_card_info_name_pass = "SELECT * FROM tlVeriryCardUser WHERE CardNumber='%s'   "
			+ "											   and CardUserPassword='%s';";
	final static String select_card_info_name_pass_encrypt = "SELECT * FROM tlVeriryCardUser WHERE CardNumber='%s'   "
			+ "											   and CardPasswordEncrypt='%s';";
	private MyBaseDataProxy mdb = null;
	private MessageSpreader message_handler = null;

	public OfflineClientUser(MyBaseDataProxy db) {
		mdb = db;
	}

	@SuppressLint("DefaultLocale")
	@SuppressWarnings("static-access")
	@Override
	public boolean commit_regist(CardRegistInfo info) {
		// TODO Auto-generated method stub
		boolean ret = false;

		if (mdb != null && info != null) {
			String str_regist_sql = new String();
			str_regist_sql = String.format(insert_carduser_data, info.getString_card_number(),
					info.getString_password(),
					Md5Unit.EncodePasswordByCardnumber(info.getString_card_number(), info.getString_password()),
					info.getString_first_name(), "no describe", this.string_username, info.getString_phone(),
					info.getN_sex(), "no extra text");
			ret = mdb.PostExecSql(new ExecSqlOperator(str_regist_sql, mdb, this.message_handler,
					VeriryActivity.KeyCardUserRegistSuc, VeriryActivity.KeyCardUserRegistFail));
		}

		return ret;
	}

	@Override
	public boolean commit_verify(CardVerifyInfo info) {
		// TODO Auto-generated method stub
		boolean ret = false;

		if (mdb != null && info != null) {
			String str_verify_sql = new String();
			str_verify_sql = String.format(select_card_info_name_pass_encrypt, info.getString_card_number(),
					Md5Unit.EncodePasswordByCardnumber(info.getString_card_number(), info.getString_card_password()));
			ret = mdb.PostExecSql(new SelectSqlDoOperator(str_verify_sql, mdb, this.message_handler,
					VeriryActivity.KeyCardUserVerifySuc, VeriryActivity.KeyCardUserVerifyFail));
		}

		return ret;
	}

	@Override
	boolean commit_find(QueryUnit unit) {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public void loginIn() {
		// TODO Auto-generated method stub
		if (mdb != null) {
			mdb.start();
		}
	}

	@Override
	public void loginOut() {
		// TODO Auto-generated method stub
		if (mdb != null) {
			mdb.stop();
		}
	}

	@Override
	public void bindHandler(MessageSpreader h) {
		// TODO Auto-generated method stub
		message_handler = h;
	}

	@Override
	public boolean commit_verify_after_do(CardVerifyInfo info) {
		// TODO Auto-generated method stub
		boolean bret = false;

		if (mdb != null && info != null) {
			String string_sql = new String();
			string_sql = String.format(delete_carduser_data, info.getString_card_number());
			bret = mdb.PostExecSql(new DeleteSqlDoOperator(string_sql, mdb, message_handler,
					VeriryActivity.KeyCardUserVerifyDeletetSuc, VeriryActivity.KeyCardUserVerifyDeleteFail));
		}

		return bret;
	}

	@Override
	public String GetUserName() {
		// TODO Auto-generated method stub
		return string_username;
	}

}
