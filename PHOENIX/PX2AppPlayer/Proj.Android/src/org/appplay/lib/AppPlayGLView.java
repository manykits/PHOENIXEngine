package org.appplay.lib;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.inputmethod.InputMethodManager;
import android.widget.TextView;

public class AppPlayGLView extends GLSurfaceView
{

	private final static int HANDLER_OPEN_IME_KEYBOARD = 2;
	private final static int HANDLER_CLOSE_IME_KEYBOARD = 3;

	public static AppPlayGLView sTheAppPlayGLView;
	private static AppPlayTextInputWraper msTextInputWraper;
	private static Handler msHandler;

	private AppPlayRenderer mRenderer = null;
	private AppPlayEditText mEditText = null;

	public AppPlayGLView(Context context)
	{
		super(context);

		setEGLContextClientVersion(2);
		setEGLConfigChooser(8, 8, 8, 8, 24, 8);

		mRenderer = new AppPlayRenderer((AppPlayBaseActivity) context);
		setRenderer(mRenderer);

		_InitView();

		Log.d("appplay.lib", "info -AppPlayGLView created.");
	}

	public TextView GetEditText()
	{
		return mEditText;
	}

	private void _InitView()
	{
		Log.d("appplay.lib", "AppPlayGLView::_InitView");
		
		setFocusableInTouchMode(true);

		sTheAppPlayGLView = this;
		msTextInputWraper = new AppPlayTextInputWraper(this);

		msHandler = new Handler()
		{
			@Override
			public void handleMessage(final Message msg)
			{
				switch (msg.what)
				{
				case HANDLER_OPEN_IME_KEYBOARD:

					if (null != AppPlayGLView.this.mEditText
							&& AppPlayGLView.this.mEditText.requestFocus())
					{
						AppPlayGLView.this.mEditText
								.removeTextChangedListener(AppPlayGLView.msTextInputWraper);
						AppPlayGLView.this.mEditText.setText("");
						final String text = (String) msg.obj;
						AppPlayGLView.this.mEditText.append(text);
						AppPlayGLView.msTextInputWraper.SetOriginText(text);
						AppPlayGLView.this.mEditText
								.addTextChangedListener(AppPlayGLView.msTextInputWraper);

						final InputMethodManager imm = (InputMethodManager) AppPlayGLView.sTheAppPlayGLView
								.getContext().getSystemService(
										Context.INPUT_METHOD_SERVICE);
						imm.showSoftInput(AppPlayGLView.this.mEditText, 0);
					}
					break;

				case HANDLER_CLOSE_IME_KEYBOARD:

					if (null != AppPlayGLView.this.mEditText)
					{
						AppPlayGLView.this.mEditText
								.removeTextChangedListener(AppPlayGLView.msTextInputWraper);

						final InputMethodManager imm = (InputMethodManager) AppPlayGLView.sTheAppPlayGLView
								.getContext().getSystemService(
										Context.INPUT_METHOD_SERVICE);
						imm.hideSoftInputFromWindow(
								AppPlayGLView.this.mEditText.getWindowToken(),
								0);
						AppPlayGLView.this.requestFocus();
					}
					break;
				}
			}
		};
	}

	@Override
	protected void onSizeChanged(int w, int h, int oldw, int oldh)
	{
		Log.d("appplay.lib", "info - AppPlayGLView::onSizeChanged:" + w + " " + h);
		
		if (null != mRenderer)
			mRenderer.SetSize(w, h);
	}

	@Override
	public void onPause()
	{
		Log.d("appplay.lib", "info - AppPlayGLView::onPause");
		
		queueEvent(new Runnable()
		{
			public void run()
			{
				AppPlayNatives.nativeOnPause();
			}
		});

		super.onPause();
	}

	@Override
	public void onResume()
	{
		Log.d("appplay.lib", "info - AppPlayGLView::onResume");
		
		super.onResume();

		queueEvent(new Runnable()
		{

			public void run()
			{
				AppPlayNatives.nativeOnResume();
			}
		});
	}

	@Override
	public boolean onTouchEvent(final MotionEvent event)
	{
		// these data are used in ACTION_MOVE and ACTION_CANCEL
		final int pointerNumber = event.getPointerCount();
		final int[] ids = new int[pointerNumber];
		final float[] xs = new float[pointerNumber];
		final float[] ys = new float[pointerNumber];

		for (int i = 0; i < pointerNumber; i++)
		{
			ids[i] = event.getPointerId(i);
			xs[i] = event.getX(i);
			ys[i] = event.getY(i);
		}

		switch (event.getAction() & MotionEvent.ACTION_MASK)
		{
		case MotionEvent.ACTION_POINTER_DOWN:
			final int indexPointerDown = event.getAction() >> MotionEvent.ACTION_POINTER_ID_SHIFT;
			final int idPointerDown = event.getPointerId(indexPointerDown);
			final float xPointerDown = event.getX(indexPointerDown);
			final float yPointerDown = event.getY(indexPointerDown);

			queueEvent(new Runnable()
			{
				public void run()
				{
					AppPlayNatives.nativeTouchPressed(idPointerDown,
							xPointerDown, yPointerDown);
				}
			});
			break;

		case MotionEvent.ACTION_DOWN:
			// there are only one finger on the screen
			final int idDown = event.getPointerId(0);
			final float xDown = xs[0];
			final float yDown = ys[0];

			queueEvent(new Runnable()
			{
				public void run()
				{
					AppPlayNatives.nativeTouchPressed(idDown, xDown, yDown);
				}
			});
			break;

		case MotionEvent.ACTION_MOVE:
			queueEvent(new Runnable()
			{
				public void run()
				{
					AppPlayNatives.nativeTouchMoved(ids, xs, ys);
				}
			});
			break;

		case MotionEvent.ACTION_POINTER_UP:
			final int indexPointUp = event.getAction() >> MotionEvent.ACTION_POINTER_ID_SHIFT;
			final int idPointerUp = event.getPointerId(indexPointUp);
			final float xPointerUp = event.getX(indexPointUp);
			final float yPointerUp = event.getY(indexPointUp);

			queueEvent(new Runnable()
			{

				public void run()
				{
					AppPlayNatives.nativeTouchReleased(idPointerUp, xPointerUp,
							yPointerUp);
				}
			});
			break;

		case MotionEvent.ACTION_UP:
			final int idUp = event.getPointerId(0);
			final float xUp = xs[0];
			final float yUp = ys[0];

			queueEvent(new Runnable()
			{
				public void run()
				{
					AppPlayNatives.nativeTouchReleased(idUp, xUp, yUp);
				}
			});
			break;

		case MotionEvent.ACTION_CANCEL:
			queueEvent(new Runnable()
			{
				public void run()
				{
					AppPlayNatives.nativeTouchCancelled(ids, xs, ys);
				}
			});
			break;
		}
		return true;
	}

	// -- functions internals

	public void SetEditText(AppPlayEditText edittext)
	{
		mEditText = edittext;

		if (null != mEditText && null != msTextInputWraper)
		{
			mEditText.setOnEditorActionListener(msTextInputWraper);
			mEditText.SetGLView(this);

			this.requestFocus();
		}
	}

	// -- functions implements ourself --

	public void Destory()
	{
		queueEvent(new Runnable()
		{

			public void run()
			{
				// PX2Natives.nativeOnTerm();
			}
		});
	}

	public static void OpenIMEKeyboard()
	{
		final Message msg = new Message();
		msg.what = AppPlayGLView.HANDLER_OPEN_IME_KEYBOARD;
		msg.obj = AppPlayGLView.sTheAppPlayGLView.GetContentText();
		AppPlayGLView.msHandler.sendMessage(msg);
	}

	public static void CloseIMEKeyboard()
	{
		final Message msg = new Message();
		msg.what = AppPlayGLView.HANDLER_CLOSE_IME_KEYBOARD;
		AppPlayGLView.msHandler.sendMessage(msg);
	}

	private String GetContentText()
	{
		return AppPlayNatives.nativeGetContentText();
	}

	public void InsertText(final String text)
	{
		queueEvent(new Runnable()
		{

			public void run()
			{
				AppPlayNatives.nativeInsertText(text);
			}
		});
	}

	public void DeleteBackward()
	{
		queueEvent(new Runnable()
		{
			public void run()
			{
				AppPlayNatives.nativeDeleteBackward();
			}
		});
	}

}
