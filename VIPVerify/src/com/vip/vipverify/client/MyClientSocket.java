package com.vip.vipverify.client;

import java.io.Serializable;

import com.vip.vipverify.net.AnalyzeNetData;

public abstract class MyClientSocket implements Runnable, Serializable {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	public static final int No_Error = 0x0001;
	public static final int Net_error = 0x0002;

	protected abstract boolean how_connect_socket(String addr, int port);

	protected abstract boolean how_disconnect_socket();

	protected abstract boolean how_close_socket();

	protected abstract boolean how_init_socket();

	protected abstract int how_send_data(byte[] data, int len, String addr, int port);

	protected abstract int how_rev_data(byte[] data, int len, String addr, int port);

	private Thread rev_thread = null;
	private boolean brev_thread_exit = true;
	private boolean isRevThreadOutside = false;

	final int MaxBuff = 1024;
	private String ip = "";
	private int port = 0;

	public interface SockketDataListening {
		public int rev_data(byte[] data, int len);

		public int send_data(byte[] data, int len);
	}

	public interface NetConnectResultListening {
		public void connect_result(boolean bresult, int reason, String ip, int port);

		public void net_state(boolean bresult, int reason, String ip, int port);
	}

	public MyClientSocket(Thread rev_thread) {
		this.rev_thread = rev_thread;
		if (this.rev_thread != null) {
			isRevThreadOutside = true;
		}
	}

	public final boolean connect_socket(String addr, int port) {
		boolean ret = false;
		if (isRevThreadOutside == true) {
			ret = this.how_connect_socket(addr, port);
		} else {
			ip = addr;
			this.port = port;
			ret = true;
		}
		return ret;
	}

	public final boolean disconnect_socket() {
		boolean ret = false;
		if (how_disconnect_socket() == true) {
			;
		}
		return ret;
	}

	public final boolean close_socket() {
		boolean ret = false;
		if (this.rev_thread != null && isRevThreadOutside == false) {

			this.brev_thread_exit = true;
			
			try {
				this.rev_thread.join();
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			
			this.rev_thread.interrupt();
			

		}

		ret = this.disconnect_socket();
		ret = this.how_close_socket();
		return ret;
	}

	public final boolean init_socket() {
		boolean ret = false;
		if ((ret = this.how_init_socket()) == true) {
			if (this.rev_thread == null && isRevThreadOutside == false) {
				this.rev_thread = new Thread(this);
				this.brev_thread_exit = false;
				this.rev_thread.start();
			}
		}

		return ret;
	}

	public final int send_data(byte[] data, int len, String addr, int port) {
		int bret = 0;

		bret = this.how_send_data(data, len, addr, port);

		return bret;
	}

	public final int rev_data(byte[] data, int len, String addr, int port) {
		return how_rev_data(data, len, addr, port);
	}

	@Override
	public final void run() {
		// TODO Auto-generated method stub

		while (!Thread.currentThread().isInterrupted()) {
			byte[] data = new byte[MaxBuff];
			int len = data.length;
			if(this.brev_thread_exit == true) {
				break;
			}
			if (this.how_connect_socket(this.ip, this.port)) {
				while ((how_rev_data(data, len, this.ip, this.port)) > 0) {
					;
				}
			}

			this.disconnect_socket();
			try {
				Thread.sleep(5000);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		int ndebug = 0;
		ndebug = 0;
	}

}
