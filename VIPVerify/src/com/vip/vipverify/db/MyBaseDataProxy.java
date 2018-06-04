package com.vip.vipverify.db;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import com.vip.vipverify.operator.DoOperator;
import com.vip.vipverify.thread.WakeThread;

import android.content.Context;
import android.database.Cursor;
import android.database.SQLException;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteDatabase.CursorFactory;

public class MyBaseDataProxy implements Serializable {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private SQLiteDatabase mdb = null;
	private String string_db_name = null;
	// private Context context = null;

	SqlExecOperateWakeThread operateWakeThread = new SqlExecOperateWakeThread();

	class SqlExecOperateWakeThread extends WakeThread implements Serializable {
		/**
		 * 
		 */
		private static final long serialVersionUID = 1L;
		private List<DoOperator> listOperate = new ArrayList<DoOperator>();

		public SqlExecOperateWakeThread() {
			super();
			// TODO Auto-generated constructor stub
			this.setPriority(MIN_PRIORITY);
		}

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

	};

	final String open_pragma = "PRAGMA FOREIGN_KEYS=ON;";

	final String create_user_table = "CREATE TABLE tlVeriryClientUser( ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "
			+ " UserName VARCHAR(20) NOT NULL, " + " Password VARCHAR(20) NOT NULL, " + " Phone VARCHAR(20) NOT NULL,"
			+ " Describe VARCHAR(255), " + " CreateTime DATE NOT NULL , " + " ExtraText TEXT, UNIQUE (UserName));";
	final String create_user_index = "CREATE UNIQUE INDEX UserNameIndex ON tlVeriryClientUser (UserName);";

	final String create_cardregister_table = "CREATE TABLE tlVeriryCardUser("
			+ " ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL," + " CardNumber VARCHAR(20) NOT NULL,"
			+ " CardUserPassword  VARCHAR(20) DEFAULT NULL," + " CardPasswordEncrypt VARCHAR(20) DEFAULT NULL,"
			+ " CarUserFirname  VARCHAR(10) DEFAULT NULL," + " Describe VARCHAR(255) DEFAULT NULL, "
			+ " CardRegistOwnUser VARCHAR(20) DEFAULT NULL, " + " CardRegistUserPhone VARCHAR(20) DEFAULT NULL,"
			+ " CardRegistUserSex INT DEFAULT 0," + " CardRegistTime DATE NOT NULL DEFAULT '0000-00-00 00:00:00',"
			+ " ExtraText TEXT," + " UNIQUE (CardNumber),"
			+ "FOREIGN KEY (CardRegistOwnUser) REFERENCES tlVeriryClientUser (UserName));";
	final String create_cardregister_index = "CREATE UNIQUE INDEX VeriryCardUserIndex ON tlVeriryCardUser (CardNumber);";

	public MyBaseDataProxy(Context context, String name, CursorFactory factory, int version) {

		// TODO Auto-generated constructor stub
		mdb = context.openOrCreateDatabase(name, Context.MODE_PRIVATE, null);
		if (mdb != null) {
			mdb.execSQL(open_pragma);
			try {
				mdb.execSQL(create_user_table);
			} catch (Exception e) {
				// TODO: handle exception
			}

			try {
				mdb.execSQL(create_user_index);
			} catch (Exception e) {
				// TODO: handle exception
			}
			try {
				mdb.execSQL(create_cardregister_table);
			} catch (Exception e) {
				// TODO: handle exception
			}
			try {
				mdb.execSQL(create_cardregister_index);
			} catch (Exception e) {
				// TODO: handle exception
			}

			mdb.close();
			mdb = null;
		}
		this.string_db_name = name;
		// this.context = context;
	}

	public boolean execSql(String sql) {
		boolean bret = false;
		try {
			if (mdb != null) {
				mdb.execSQL(sql);
				bret = true;
			}

		} catch (SQLException e) {
			throw new SQLException(e.toString());
		}
		return bret;
	}

	public boolean Delete(String sql) {
		boolean bret = false;

		try {
			if (mdb != null) {
				final String delete = "delete";
				String table = null;
				String whereClause = null;
				String[] whereArgs = null;
				if (sql.regionMatches(true, 0, delete, 0, delete.length()) == true) {
					final String regex_table_string = "FROM(.*)WHERE";
					Pattern pattern = Pattern.compile(regex_table_string, Pattern.CASE_INSENSITIVE);
					Matcher matcher = pattern.matcher(sql);
					if (matcher.find()) {
						table = matcher.group(1).trim();
					}

					final String regex_where_string = "where(.*)";
					pattern = Pattern.compile(regex_table_string, Pattern.CASE_INSENSITIVE);
					matcher = pattern.matcher(sql);
					if (matcher.find()) {
						whereClause = matcher.group(1).replace(';', '\0').trim();
					}

					bret = mdb.delete(table, whereClause, whereArgs) == 0 ? false : true;

				}
			}
		} catch (Exception e) {
			// TODO: handle exception
		}

		return bret;
	}

	public String[] Select(String sql) {
		String[] ret = null;
		try {
			if (mdb != null) {

				final String select = "select";
				final String from = "from";

				String table = new String();
				String[] columns = null;
				String selection = null;
				String[] selectionArgs = null;
				String groupBy = null;
				String having = null;
				String orderBy = null;

				if (sql.regionMatches(true, 0, select, 0, select.length()) == true) {

					////// get table name//

					final String regex_table_string = "FROM(.*)WHERE";
					Pattern pattern = Pattern.compile(regex_table_string, Pattern.CASE_INSENSITIVE);
					Matcher matcher = pattern.matcher(sql);

					if (matcher.find()) {
						table = matcher.group(1).trim();
					}
					//////////////////

					////// column list get////////////

					final String regex_string = "select(.*)from";
					final String regex_column_string = "([\\*]+|[a-z]+[\\s]*)[\\,]*";
					pattern = Pattern.compile(regex_string, Pattern.CASE_INSENSITIVE);
					matcher = pattern.matcher(sql);
					if (matcher.find()) {
						String str_column_list = matcher.group(1).trim();
						if (str_column_list != null && str_column_list.isEmpty() == false) {
							pattern = Pattern.compile(regex_column_string, Pattern.CASE_INSENSITIVE);
							matcher = pattern.matcher(str_column_list);
							List<String> columns_s = new ArrayList<String>();
							while (matcher.find()) {
								String str_column = matcher.group(1).trim();
								if (str_column != null && str_column.indexOf("*") != -1) {
									columns_s.add(str_column);
								}
							}

							if (columns_s.size() > 0)
								columns_s.toArray(columns = new String[0]);
						}
					}
					//////////////////////////////////

					/////////////////// selection get ////////
					final String where_regex_string = "where(.*)";
					final String select_regex_string = "where(.*)";
					pattern = Pattern.compile(where_regex_string, Pattern.CASE_INSENSITIVE);
					matcher = pattern.matcher(sql);
					if (matcher.find()) {
						selection = matcher.group(1).replace(';', '\0').trim();
					}
					////////////////////////////////////////

					////////// having///////////
					final String having_regex_string = "having(.*)";

					pattern = Pattern.compile(having_regex_string, Pattern.CASE_INSENSITIVE);
					matcher = pattern.matcher(sql);
					if (matcher.find()) {
						having = matcher.group(1).replace(';', '\0').trim();
					}
					///////////////////////////

					////////// group by///////////
					final String groupby_regex_string = "group[\\s]+by(.*)";

					pattern = Pattern.compile(groupby_regex_string, Pattern.CASE_INSENSITIVE);
					matcher = pattern.matcher(sql);
					if (matcher.find()) {
						groupBy = matcher.group(1).replace(';', '\0').trim();
					}
					///////////////////////////

					////////// order by///////////
					final String orderby_regex_string = "order[\\s]+by(.*)";

					pattern = Pattern.compile(orderby_regex_string, Pattern.CASE_INSENSITIVE);
					matcher = pattern.matcher(sql);
					if (matcher.find()) {
						orderBy = matcher.group(1).replace(';', '\0').trim();
					}
					///////////////////////////

					Cursor sor = mdb.query(table, columns, selection, selectionArgs, groupBy, having, orderBy);

					// sql.replace(';', '\0').trim();
					// Cursor sor = mdb.rawQuery(sql, null);
					if (sor != null) {
						if (sor.moveToFirst() == true) {
							int count_result = sor.getColumnCount();
							int row_result = sor.getCount();
							ret = new String[row_result];
							int icount = 0;
							String[] names = sor.getColumnNames();
							do {
								int i = 0;
								while (i < count_result) {
									ret[icount] += names[i] + " : " + sor.getString(i) + "\n";
									i++;
								}
								icount++;

							} while (sor.moveToNext() && icount < row_result);
						}
					}
				}
			}

		} catch (SQLException e) {
			throw new SQLException(e.toString());
		}
		return ret;
	}

	public boolean PostExecSql(DoOperator sql) {
		boolean ret = true;
		try {
			operateWakeThread.postOperate(sql);
		} catch (Exception e) {
			// TODO: handle exception
			ret = false;
		}
		return ret;

	}

	public void start() {
		try {
			mdb = SQLiteDatabase.openOrCreateDatabase(string_db_name, null, null);
		} catch (Exception e) {
			// TODO: handle exception
			mdb = null;
		}

		if (mdb != null) {
			if (operateWakeThread == null) {
				operateWakeThread = new SqlExecOperateWakeThread();
			}
			if (operateWakeThread.isAlive() == false) {
				operateWakeThread.start();
			}
		}
	}

	public void stop() {
		if (operateWakeThread != null) {
			operateWakeThread.exit();
			operateWakeThread = null;
		}
		if (mdb != null)
			mdb.close();
	}

}
