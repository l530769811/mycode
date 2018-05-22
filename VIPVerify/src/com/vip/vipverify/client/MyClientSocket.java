package com.vip.vipverify.client;

import com.vip.vipverify.thread.WakeThread;

public abstract class MyClientSocket implements Runnable {

	protected abstract boolean how_connect_socket(String addr, int port);

	protected abstract boolean how_disconnect_socket();

	protected abstract boolean how_close_socket();

	protected abstract boolean how_init_socket();

	protected abstract int how_send_data(byte[] data, int len);

	protected abstract int how_rev_data(byte[] data, int len);

	private Thread rev_thread = null;
	private boolean isRevThreadOutside = false;

	private WakeThread send_thread = null;
	final int MaxBuff = 1024;
	private SockketDataListening listener = null;

	public interface SockketDataListening {
		public int rev_data(byte[] data, int len);

		public int send_data(byte[] data, int len);
	}

	public MyClientSocket(Thread rev_thread, WakeThread send_thread, SockketDataListening listener) {
		this.rev_thread = rev_thread;
		if (this.rev_thread != null) {
			isRevThreadOutside = true;
		}

		this.send_thread = send_thread;
		if (this.send_thread != null) {
			;
		}

		this.listener = listener;
	}

	public final boolean connect_socket(String addr, int port) {
		boolean ret = false;

		if (this.how_connect_socket(addr, port) == true) {
			if (this.rev_thread == null && isRevThreadOutside == false) {
				this.rev_thread = new Thread(this);
				this.rev_thread.start();
			}
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
		ret = this.how_close_socket();
		return ret;
	}

	public final boolean init_socket() {
		boolean ret = false;
		ret = this.how_init_socket();
		return ret;
	}

	public final int send_data(byte[] data, int len) {
		int bret = 0;
		if (send_thread == null) {
			this.how_send_data(data, len);
		} else {
			this.how_send_data(data, len);
		}
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
			if ((len = how_rev_data(data, len)) > 0) {
				if (listener != null) {
					listener.rev_data(data, len);
				}
			}
		}
	}

}
