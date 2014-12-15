package android.game.onsteroids;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.pm.ActivityInfo;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.WindowManager;

public class GameActivity extends Activity implements GLSurfaceView.Renderer
{	
	static
	{
		System.loadLibrary("OnSteroids"); 
	}
	
	public GLSurfaceView view;
	
	@Override
	protected void onPause()
	{		
		super.onPause();
		drawPause();
		if (isFinishing())
			nativeFinalize();
	}
	
	@Override
	protected void onResume()
	{
		super.onResume();
		drawResume();
	}
	
	public void drawPause()
	{
		view.onPause();
		nativePause();
	}
	
	public void drawResume()
	{
		view.onResume();
		nativeResume();
	}

	@Override
	public void onDrawFrame(GL10 gl)
	{		
//		processKeyEvents();
//		processMotionEvents();
		nativeDrawFrame();
	}

	@Override
	public void onSurfaceChanged(GL10 gl, int width, int height)
	{
		nativeSurfaceChanged(width, height);
	}

	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig config)
	{
		nativeSurfaceCreated();
	}
	
	@Override
	public void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);

		int flags = WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON
				| WindowManager.LayoutParams.FLAG_FULLSCREEN
				| WindowManager.LayoutParams.FLAG_SHOW_WHEN_LOCKED;
		getWindow().setFlags(flags, flags);
		
		view = new GLSurfaceView(this);
		view.setRenderer(this);
		
		nativeInitialize();

		setContentView(view);
		setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
	}
//	
//	ArrayList<KeyEvent> keyEvents = new ArrayList<KeyEvent>();
//	ArrayList<MotionEvent> motionEvents = new ArrayList<MotionEvent>();
//	
//	void processKeyEvents()
//	{
//		ByteBuffer data = this.keyData;
//		synchronized (keyEvents)
//		{
//			for(KeyEvent event : keyEvents)
//			{
//				data.position(0);
//				int action = event.getAction();
//				data.putInt(event.getDeviceId());		// 4
//				data.putInt(event.getSource());  		// 4
//				data.putInt(action);					// 4
//			    data.putInt(event.getKeyCode());		// 4
//				data.putInt(event.getUnicodeChar());	// 4
//				data.putLong(event.getDownTime());		// 8
//			    data.putLong(event.getEventTime());		// 8
//			    data.putInt(event.getMetaState());		// 4
//				
//			    nativeKeyEvent(data.array());		    		
//			}
//			
//			keyEvents.clear();
//		}
//	}
//	
//	void processMotionEvents()
//	{
//		ByteBuffer data = this.touchData;
//		synchronized (motionEvents)
//		{
//			for(MotionEvent event : motionEvents)
//			{
//				data.position(0);
//				int action = event.getAction();	
//				int actionMasked = action & MotionEvent.ACTION_MASK;
//				data.putInt(action);		// 4
//				data.putInt(actionMasked);	// 4
//				data.putInt((action & MotionEvent.ACTION_POINTER_INDEX_MASK) >> MotionEvent.ACTION_POINTER_INDEX_SHIFT); // 4
//				
//				int cnt = event.getPointerCount();
//				data.putInt(cnt);	// 4
//				for (int i = 0; i < cnt; i++)
//				{
//					data.putInt(event.getPointerId(i)); // 4
//					data.putFloat(event.getX(i));		// 4
//					data.putFloat(event.getY(i));		// 4
//				}
//				
//				nativeTouchEvent(touchData.array());
//			}
//			
//			motionEvents.clear();
//		}
//	}
	
	@SuppressLint("NewApi") @Override
	public boolean onKeyDown(int keyCode, KeyEvent msg)
	{
		synchronized (keyData)
		{
//			keyEvents.add(msg);
//			return true;
			ByteBuffer data = this.keyData;
			data.position(0);
			int action = msg.getAction();
			data.putInt(msg.getDeviceId());		// 4
			data.putInt(msg.getSource());  		// 4
			data.putInt(action);					// 4
		    data.putInt(msg.getKeyCode());		// 4
			data.putInt(msg.getUnicodeChar());	// 4
			data.putLong(msg.getDownTime());		// 8
		    data.putLong(msg.getEventTime());		// 8
		    data.putInt(msg.getMetaState());		// 4
			
		    nativeKeyEvent(data.array());	
		    return true;
		}
	}
	
	@Override
	public boolean onKeyUp(int keyCode, KeyEvent msg)
	{
		synchronized (keyData)
		{
//			keyEvents.add(msg);
//			return true;
			ByteBuffer data = this.keyData;
			data.position(0);
			int action = msg.getAction();
			data.putInt(msg.getDeviceId());		// 4
			data.putInt(msg.getSource());  		// 4
			data.putInt(action);					// 4
		    data.putInt(msg.getKeyCode());		// 4
			data.putInt(msg.getUnicodeChar());	// 4
			data.putLong(msg.getDownTime());		// 8
		    data.putLong(msg.getEventTime());		// 8
		    data.putInt(msg.getMetaState());		// 4
			
		    nativeKeyEvent(data.array());	
		    return true;
		}
	}
	
	@Override
	public boolean onTouchEvent(MotionEvent event)
	{
		synchronized (touchData)
		{
//			motionEvents.add(event);
//			return true;
			ByteBuffer data = this.touchData;
			data.position(0);
			int action = event.getAction();	
			int actionMasked = action & MotionEvent.ACTION_MASK;
			data.putInt(action);		// 4
			data.putInt(actionMasked);	// 4
			data.putInt((action & MotionEvent.ACTION_POINTER_INDEX_MASK) >> MotionEvent.ACTION_POINTER_INDEX_SHIFT); // 4
			
			int cnt = event.getPointerCount();
			data.putInt(cnt);	// 4
			for (int i = 0; i < cnt; i++)
			{
				data.putInt(event.getPointerId(i)); // 4
				data.putFloat(event.getX(i));		// 4
				data.putFloat(event.getY(i));		// 4
			}
			
			nativeTouchEvent(touchData.array());
			return true;
		}
	}
	
	ByteBuffer keyData = ByteBuffer.allocateDirect(4 + 4 + 4 + 4 + 4 + 8 + 8 + 4).order(ByteOrder.nativeOrder());
	ByteBuffer touchData = ByteBuffer.allocateDirect(4 + 4 + 4 + 4 + (4 + 4 + 4) * 10).order(ByteOrder.nativeOrder());
	
	private native void nativeInitialize();
	
	private static native void nativeFinalize();
	
	private static native void nativeSurfaceCreated();

	private static native void nativeSurfaceChanged(int width, int height);

	private static native void nativeDrawFrame();
	
	private static native void nativePause();
	
	private static native void nativeResume();
	
	private static native void nativeKeyEvent(byte[] data);
	
	private static native void nativeTouchEvent(byte[] data);
}
