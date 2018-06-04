package com.vip.vipverify.client;

import java.io.Serializable;

import com.vip.vipverify.net.AnalyzeNetData;

public abstract class MyClientSocket implements Runnable, Serializable {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	protected abstract boolean how_connect_socket(String addr, int port);

	protected abstract boolean how_disconnect_socket();

	protected abstract boolean how_close_socket();

	protected abstract boolean how_init_socket();

	protected abstract int how_send_data(byte[] data, int len);

	protected abstract int how_rev_data(byte[] data, int len);

	private Thread rev_thread = null;
	private boolean isRevThreadOutside = false;

	final int MaxBuff = 1024;
	private String ip = "";
	private int port = 0;

	public interface SockketDataListening {
		public int rev_data(byte[] data, int len);

		public int send_data(byte[] data, int len);
	}

	public interface ConnectResultListening {
		public void connect_result(boolean bresult, String ip, int port);
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
			if (this.rev_thread != null && this.isRevThreadOutside == false) {
				this.rev_thread.interrupt();
				try {
					this.rev_thread.join();
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		}
		return ret;
	}

	public final boolean close_socket() {
		boolean ret = false;
		ret = this.disconnect_socket();
		ret = this.how_close_socket();
		return ret;
	}

	public final boolean init_socket() {
		boolean ret = false;
		if ((ret = this.how_init_socket()) == true) {
			if (this.rev_thread == null && isRevThreadOutside == false) {
				this.rev_thread = new Thread(this);
				this.rev_thread.start();
			}
		}

		return ret;
	}

	public final int send_data(byte[] data, int len) {
		int bret = 0;

		bret = this.how_send_data(data, len);

		return bret;
	}

	public final int rev_data(byte[] data, int len) {
		return how_rev_data(data, len);
	}

	@Override
	public final void run() {
		// TODO Auto-generated method stub

		while (!Thread.currentThread().isInterrupted()) {
			byte[] data = new byte[MaxBuff];
			int len = data.length;
			if (this.how_connect_socket(this.ip, this.port)) {

				while ((len = how_rev_data(data, len)) > 0) {
					;
				}
			}

			this.close_socket();
			break;
		}
	}

}
