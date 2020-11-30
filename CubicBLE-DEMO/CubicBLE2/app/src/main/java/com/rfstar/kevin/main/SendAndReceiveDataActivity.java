package com.rfstar.kevin.main;

import android.view.View;
import android.os.Bundle;
import android.os.AsyncTask;
import android.widget.Button;
import android.view.KeyEvent;
import android.widget.Spinner;
import android.content.Intent;
import android.content.Context;
import android.widget.CheckBox;
import com.rfstar.kevin.main.R;
import com.rfstar.kevin.app.App;
import android.widget.AdapterView;
import android.widget.CompoundButton;
import com.rfstar.kevin.params.MemberItem;
import com.rfstar.kevin.view.SendDataView;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.CompoundButton.OnCheckedChangeListener;
import com.rfstar.kevin.params.BLEDevice.RFStarBLEBroadcastReceiver;

/**
 * 发送数据的界面
 * 
 * @author kevin
 * 
 */
public class SendAndReceiveDataActivity extends BaseActivity implements
		android.view.View.OnClickListener, RFStarBLEBroadcastReceiver {
	private SendDataView sendView = null;

	private Button resetBtn = null, clearBtn = null, sendBtn = null;
	private CheckBox autoSend;
	private int sendtime = 10;
	private Spinner spinnerSendTime;
	@SuppressWarnings("static-access")
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		this.setContentView(R.layout.activity_senddata);
		this.initView();
	}

	@Override
	protected void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
		if (app.manager.cubicBLEDevice != null) {
			app.manager.cubicBLEDevice.setBLEBroadcastDelegate(this);
		}
	}

	@SuppressWarnings("static-access")
	private void initView() {
		// TODO Auto-generated method stub
		Intent intent = this.getIntent();
		MemberItem member = (MemberItem) intent.getSerializableExtra(App.TAG);
		this.initNavigation(member.name);
		sendView = (SendDataView) this.findViewById(R.id.sendDataView);
		sendView.setEdit("Hello");
		resetBtn = (Button) this.findViewById(R.id.resetBtn);
		clearBtn = (Button) this.findViewById(R.id.clearBtn);
		sendBtn = (Button) this.findViewById(R.id.sendBtn);
		autoSend = (CheckBox) this.findViewById(R.id.autoSend);
		spinnerSendTime = (Spinner) this.findViewById(R.id.spinnerSendTime);
		spinnerSendTime.setOnItemSelectedListener(new OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, 
                    int pos, long id) {
            	
                if(pos == 0){
                	sendtime = 500;
                }else if(pos == 1){
                	sendtime = 600;
                }else if(pos == 2){
                	sendtime = 700;
                }else if(pos == 3){
                	sendtime = 800;
                }else if(pos == 4){
                	sendtime = 900;
                }else if(pos == 5){
                	sendtime = 1000;
                }else if(pos == 6){
                	sendtime = 2000;
                }else if(pos == 7){
                	sendtime = 3000;
                }else if(pos == 8){
                	sendtime = 4000;
                }else if(pos == 9){
                	sendtime = 5000;
                }else if(pos == 10){
                	sendtime = 6000;
                }else if(pos == 11){
                	sendtime = 7000;
                }else if(pos == 12){
                	sendtime = 8000;
                }else if(pos == 13){
                	sendtime = 9000;
                }else if(pos == 14){
                	sendtime = 10000;
                }
            }
            @Override
            public void onNothingSelected(AdapterView<?> parent) {
            	sendtime = 10;
            }
        });
		autoSend.setOnCheckedChangeListener(new OnCheckedChangeListener() {

			@Override
			public void onCheckedChanged(CompoundButton arg0, boolean arg1) {
				if (app.manager.cubicBLEDevice != null) {
					if (arg1) {
							sendBtn.setFocusable(false);
							new AsyncSendEmptyMessageRight().execute();
					} else {
						sendBtn.setFocusable(true);
						iSend = true;
					}
				}
			}
		});

		resetBtn.setOnClickListener(this);
		clearBtn.setOnClickListener(this);
		sendBtn.setOnClickListener(this);
	}

	private boolean iSend = false;

	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
		if (v == resetBtn) {
			sendView.reset();
		} else if (v == clearBtn) {
			sendView.clear();
		} else if (v == sendBtn) {
			if (app.manager.cubicBLEDevice != null) {
				app.manager.cubicBLEDevice.writeValue("ffe5", "ffe9", sendView.getEdit().getBytes());
				sendView.setCountTimes();
			}
		}
	}

	@Override
	public void onReceive(Context context, Intent intent, String macData,
			String uuid) {
		// TODO Auto-generated method stub
		this.connectedOrDis(intent.getAction());
	}


	
	class AsyncSendEmptyMessageRight extends AsyncTask<Void, Integer, Integer>{ 
	    @Override 
	    protected Integer doInBackground(Void... params) {
	    	
	    	int count = 0;
	    	while (!iSend) {
				if (app.manager.cubicBLEDevice != null) {
					app.manager.cubicBLEDevice.writeValue("ffe5","ffe9", sendView.getEdit().getBytes());
					count++;
					publishProgress(count);
				}
				try {
					Thread.sleep(sendtime);
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
				if (iSend) {
					iSend = false;
					break;
				}
			}
	      return null; 
	    }

		@Override
		protected void onProgressUpdate(Integer... values) {
			super.onProgressUpdate(values);
			sendView.setCountTimes();
		} 
	    
	   
	  } 
	
	
	
	
	 @Override
	    public boolean dispatchKeyEvent(KeyEvent event) {
	        if (event.getKeyCode() == KeyEvent.KEYCODE_BACK && event.getRepeatCount() == 0
	                && event.getAction() == KeyEvent.ACTION_DOWN) { 
	        	
	        	sendBtn.setFocusable(true);
				iSend = true;
	        	finish();
	        }
	        return super.dispatchKeyEvent(event);
	    }
	
	
}
