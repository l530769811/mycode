package com.vip.vipverify;

public class SingletonTemplete<Type extends Object> {
	private static volatile SingletonTemplete<Object> instance = null;
	
	public static synchronized SingletonTemplete<Object> getInstance() {
		if(instance==null) {
			synchronized (SingletonTemplete.class) {
				if(instance==null) {
					instance = new SingletonTemplete<Object>();
				}
			}
			
		}
		return instance;
	}
}
