package com.vip.vipverify.thread;

import java.io.Serializable;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

import com.vip.vipverify.operator.DoOperator;

public abstract class WakeThread extends Thread implements Serializable {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private boolean bwakeup = false;
	private Lock wakelock = new ReentrantLock();

	final public void wakeUp() {
		synchronized (this) {
			bwakeup = true;
			this.notify();
		}
	}

	final public void wakeDown() {
		bwakeup = false;
	}

	final public boolean isWakeUp() {
		return bwakeup;
	}

	final private boolean wake() {
		boolean bret = false;

		synchronized (this) {
			if (isWakeUp() == false) {
				try {
					this.wait();
					bret = true;
					wakeDown();
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
					bret = false;
				} finally {

				}
			} else {
				bret = true;
			}
		}

		return bret;
	}

	public void exit() {
		this.wakeUp();
		this.interrupt();
		try {
			this.join();
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	@Override
	final public void run() {
		// TODO Auto-generated method stub
		while (!currentThread().isInterrupted()) {
			if (wake() == false)
				break;
			try {
				myRun();
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
				break;
			}
		}
	}

	public abstract void myRun() throws InterruptedException;

	public abstract boolean postOperate(DoOperator operate);
	public abstract DoOperator GetOperate(); 
	
}