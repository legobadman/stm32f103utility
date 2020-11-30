package com.rfstar.kevin.view;

import com.rfstar.kevin.main.R;
import com.rfstar.kevin.main.R;

import android.content.Context;
import android.text.Editable;
import android.text.InputType;
import android.text.TextWatcher;
import android.util.AttributeSet;
import android.util.Log;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.inputmethod.EditorInfo;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.TextView;

import java.security.PublicKey;

/**
 * 发送数据的Viwew
 * 
 * @author kevin
 * 
 */
public class SendDataView extends LinearLayout implements TextWatcher {
	private TextView lengthTxt = null, countTimesTxt = null;
	private EditText edit = null;
	private Context context = null;

	@SuppressWarnings("static-access")
	public SendDataView(Context context, AttributeSet attrs) {
		super(context, attrs);
		// TODO Auto-generated constructor stub
		this.context = context;
		LayoutInflater.from(context).inflate(R.layout.send_data_view, this,
				true);
		lengthTxt = (TextView) this.findViewById(R.id.lengthTxt);
		countTimesTxt = (TextView) this.findViewById(R.id.countTimesTxt);
		edit = (EditText) this.findViewById(R.id.editText1);
		edit.addTextChangedListener(this);
	}

	public int getLengthTxt() {
		return lengthTxt.toString().length();
	}

	// public void setLengthTxt(int length) {
	// this.lengthTxt.setText((lengthTxt.length() + length) + "");
	// }
	/**
	 * 获取发送的次数
	 * 
	 * @return
	 */
	public int getCountTimes() {
		return Integer.valueOf(countTimesTxt.getText().toString());
	}

	/**
	 * 次数递增1
	 */
	public void setCountTimes() {
		if (!edit.getText().toString().equals("")) {
			int times = this.getCountTimes();
			times++;
			if(this.countTimesTxt != null){
				this.countTimesTxt.setText(times + "");
			}
		} else {
			ToastUntil.makeText(this.context, R.string.to_send_data_not_be_empty, 1);
		}
	}

	public String getHexEdit() {

		String str = getEdit();
		String str2 = str.replace(" ", "");

		if (isHex) {
			return repairHexString(str2);
		}

		return stringToHex(str2);
	}

	public String getAsciiEdit() {

		if (isHex) {
			String str = getEdit();
			String str2 = str.replace(" ", "");
			String str3 = repairHexString(str2);
			return hexToString(str3);
		}

		return getEdit();
	}

	public String getEdit() {
		return edit.getText().toString();
	}

	public void setEdit(String edit) {
		this.edit.setText(edit);
		this.lengthTxt.setText("" + edit.length());
	}

	/**
	 * 清除
	 */
	public void clear() {
		this.countTimesTxt.setText("0");
		this.edit.setText("");
	}

	/**
	 * 重置
	 */
	public void reset() {

		this.countTimesTxt.setText("0");
		this.edit.setText("rfstar");

	}

	private CharSequence temp;
	private int editStart;
	private int editEnd;

	@Override
	public void beforeTextChanged(CharSequence s, int start, int count,
			int after) {
		// TODO Auto-generated method stub
		temp = s;
	}

	@Override
	public void onTextChanged(CharSequence s, int start, int before, int count) {
		// TODO Auto-generated method stub
		if (this.isHex) {
			String str = s.toString().replace(" ", "");
			int len = (str.length()%2==0)?str.length()/2:str.length()/2+1;
			this.lengthTxt.setText("" + len);
		} else  {
			this.lengthTxt.setText("" + s.length());
		}
	}

	@Override
	public void afterTextChanged(Editable s) {
		// TODO Auto-generated method stub
		editStart = this.edit.getSelectionStart();// 光标开始的位置
		editEnd = this.edit.getSelectionEnd();
//		if (temp.length() > 200) {
//			ToastUntil.makeText(this.context, R.string.input_character, 1);
//			s.delete(editStart - 1, editEnd);
//			int tempSelection = editStart; // 重新定位光标的位置
//			this.edit.setText(s);
//			this.edit.setSelection(tempSelection);
//		}
	}

	private boolean isHex;
	private boolean mFormat;
	private boolean mInvalid;
	private int mSelection;
	private String mLastText;
	public void inputHex(boolean isHex) {

		this.isHex = isHex;

		if (isHex) {

			String str = stringToHex(getEdit());
			String str2 = repairHexString(str);
			edit.setText(formatHexString(str2));

			mFormat = false;
			mInvalid = false;
			mLastText = "";

			edit.addTextChangedListener(myTextWatcher);

		} else {

			edit.removeTextChangedListener(myTextWatcher);

			String str = getEdit();
			String str2 = str.replace(" ", "");
			String str3 = repairHexString(str2);
			String str4 = hexToString(str3);
			edit.setText(str4);
		}
	}

	private TextWatcher myTextWatcher = new TextWatcher() {
		@Override
		public void beforeTextChanged(CharSequence s, int start, int count, int after) {

		}

		@Override
		public void onTextChanged(CharSequence charSequence, int start, int before, int count) {

			try {

				String temp = charSequence.toString();

				// Set selection.
				if (mLastText.equals(temp)) {
					if (mInvalid) {
						mSelection -= 1;
					} else {
						if ((mSelection >= 1) && (temp.length() > mSelection - 1)
								&& (temp.charAt(mSelection - 1)) == ' ') {
							mSelection += 1;
						}
					}
					int length = mLastText.length();
					if (mSelection > length) {

						edit.setSelection(length);
					} else {

						edit.setSelection(mSelection);
					}
					mFormat = false;
					mInvalid = false;
					return;
				}

				mFormat = true;
				mSelection = start;

				// Delete operation.
				if (count == 0) {
					if ((mSelection >= 1) && (temp.length() > mSelection - 1)
							&& (temp.charAt(mSelection - 1)) == ' ') {
						mSelection -= 1;
					}

					return;
				}

				// Input operation.
				mSelection += count;
				char[] lastChar = (temp.substring(start, start + count))
						.toCharArray();
				int mid = lastChar[0];
				if (mid >= 48 && mid <= 57) {
                		/* 1-9. */
				} else if (mid >= 65 && mid <= 70) {
						/* A-F. */
				} else if (mid >= 97 && mid <= 102) {
                		/* a-f. */
				} else {
                			/* Invalid input. */
					mInvalid = true;
					temp = temp.substring(0, start)
							+ temp.substring(start + count, temp.length());
					edit.setText(temp);
					return;
				}

			} catch (Exception e) {
				Log.i("---", e.toString());
			}

		}

		@Override
		public void afterTextChanged(Editable editable) {

			try {

            	/* Format input. */
				if (mFormat) {
					edit.setText(formatHexString(editable.toString()));
				}
			} catch (Exception e) {
				Log.i("---", e.toString());
			}
		}
	};

	//ASCII转换为十六进制
	private String stringToHex(String str){

		char[] chars = str.toCharArray();

		StringBuffer hex = new StringBuffer();
		for(int i = 0; i < chars.length; i++){
			hex.append(Integer.toHexString((int)chars[i]));
		}

		return hex.toString();
	}

	//十六进制转换为ASCII
	private String hexToString(String hex){

		StringBuilder sb = new StringBuilder();
		StringBuilder temp = new StringBuilder();

		//49204c6f7665204a617661 split into two characters 49, 20, 4c...
		for( int i=0; i<hex.length()-1; i+=2 ){

			try {

				//grab the hex in pairs
				String output = hex.substring(i, (i + 2));
				//convert hex to decimal
				int decimal = Integer.parseInt(output, 16);
				//convert the decimal to character
				sb.append((char)decimal);

				temp.append(decimal);

			} catch (Exception e) {
				Log.i("---", e.toString());
			}
		}

		return sb.toString();
	}

	//格式化字符串（每8位添加一个空格）
	private String formatHexString(String str) {

		StringBuilder text = new StringBuilder();
		text.append(str.replace(" ", ""));
		int length = text.length();
		int sum = (length % 8 == 0) ? (length / 8) - 1 : (length / 8);
		for (int offset = 8, index = 0; index < sum; offset += 9, index++) {

			text.insert(offset, " ");
		}
		mLastText = text.toString();

		return text.toString();
	}

	//当长度为单数是不为双数（补0）
	private String repairHexString(String str) {

		if (str.length()%2 == 1) {
			String str1 = str.substring(0, str.length()-1);
			String str2 = str.substring(str.length()-1, str.length());
			str = str1 + 0 + str2;
		}

		return str;
	}
}
