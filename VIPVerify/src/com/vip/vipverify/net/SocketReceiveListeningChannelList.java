package com.vip.vipverify.net;

public abstract class SocketReceiveListeningChannelList {
	private SocketReceiveListeningChannelList next_listener = null;

	private SocketReceiveListeningChannelList GetNextListener() {
		return next_listener;
	}

	public final void SetOnSocketReceiveListening(SocketReceiveListeningChannelList listener) {
//		if (next_listener != null) {
//			SocketReceiveListeningChannelList nextlistener = this.next_listener.GetNextListener();
//			if(nextlistener!=null) {
//				nextlistener = nextlistener.GetNextListener();
//			}
//			
//			listener.SetOnSocketReceiveListening(nextlistener);
//		}
//		next_listener = listener;
		
		if (next_listener != null) {
			SocketReceiveListeningChannelList nextlistener = this.next_listener.GetNextListener();
			listener.SetOnSocketReceiveListening(nextlistener);
		}
		next_listener = listener;
	}
	
	public final void InsertOnSocketReceiveListening(SocketReceiveListeningChannelList listener) {
		
		if(listener!=null) {
			listener.SetOnSocketReceiveListening(next_listener);
		}
		next_listener = listener;
	}
	
	public final void RemoveOnSocketReceiveListening(SocketReceiveListeningChannelList listener) {
		if(next_listener!=null) {
			
		}
	}

	protected abstract boolean receive_data(byte[] data, int len);

	public final void RevData(byte[] data, int len) {
		if (receive_data(data, len) == true) {
			if (next_listener != null) {
				next_listener.RevData(data, len);
			}
		}
	}
}