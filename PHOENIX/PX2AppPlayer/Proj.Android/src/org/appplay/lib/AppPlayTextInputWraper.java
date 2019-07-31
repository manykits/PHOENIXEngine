package org.appplay.lib;

import android.content.Context;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.Log;
import android.view.KeyEvent;
import android.view.inputmethod.InputMethodManager;
import android.widget.TextView;
import android.widget.TextView.OnEditorActionListener;

public class AppPlayTextInputWraper implements TextWatcher, OnEditorActionListener
{
	private static final Boolean sIsDebug = false;
	private AppPlayGLView mMainView;
	private String mText;
	private String mOriginText;
	
	public AppPlayTextInputWraper(AppPlayGLView view)
	{
		mMainView = view;
	}
	
	public void SetOriginText(String text)
	{
		mOriginText = text;
	}
	
	@Override
	public void afterTextChanged (Editable s)
	{
		if (_IsFullScreenEdit())
			return;
		
		_LogD("AfterTextChanged:" + s);
		
		int numModified = s.length() - mText.length();
		
		if (numModified > 0)
		{
			final String insertText = s.subSequence(mText.length(), s.length()).toString();
			
			mMainView.InsertText(insertText);
			
			_LogD("InsertText(" + insertText +")");
		}
		else
		{
			for (; numModified<0; ++numModified)
			{
				mMainView.DeleteBackward();
				
				_LogD("DeleteBackward");
			}
		}
		
		mText = s.toString();
	}
	
	@Override
	public boolean onEditorAction (TextView v, int actionId, KeyEvent event)
	{
		if (mMainView.GetEditText()==v && _IsFullScreenEdit())
		{
			// user press the action button, delete all old text and insert new text
			for (int i=mOriginText.length(); i>0; --i)
			{			
				mMainView.DeleteBackward();
				_LogD("DeleteBackward");
			}
			
			String text = v.getText().toString();
			
			if (0 == text.compareTo(""))
			{
				text = "\n";
			}
			
			if ('\n' != text.charAt(text.length()-1))
			{
				text += '\n';
			}
			
			final String insertText = text;
			
			mMainView.InsertText(insertText);

			_LogD("InsertText(" + insertText + ")");
		}
		
		return false;
	}
	
	@Override
	public void beforeTextChanged (CharSequence s, int start, int count, int after)
	{
		_LogD("BeforeTextChanged(" + s + ")start:" + start + ",count:" + ",after:" + after);
		mText = s.toString();
	}
	
	@Override
	public void onTextChanged (CharSequence s, int start, int before, int count)
	{		
	}
	
	private Boolean _IsFullScreenEdit ()
	{
		InputMethodManager imm = (InputMethodManager)mMainView.GetEditText().
				getContext().getSystemService(Context.INPUT_METHOD_SERVICE);
		return imm.isFullscreenMode();
	}
	
	private void _LogD (String msg)
	{
		if (sIsDebug)
			Log.d("PX2TextInputWraper", msg);
	}
}
