package com.rfstar.kevin.main;

import com.rfstar.kevin.app.App;
import com.rfstar.kevin.main.R;
import com.rfstar.kevin.params.MemberItem;
import com.rfstar.kevin.params.BLEDevice.RFStarBLEBroadcastReceiver;
import com.rfstar.kevin.service.RFStarBLEService;
import com.rfstar.kevin.tools.Tools;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.text.TextUtils;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.RadioGroup;
import android.widget.TextView;
import android.widget.Toast;

public class TurnimingOutputActivity extends BaseActivity implements RFStarBLEBroadcastReceiver {
    public static final String TAG = TurnimingOutputActivity.class.getSimpleName();
    private LinearLayout highLevelLayout, lowLevelLayout;
    private TextView highLevelTextView, lowLevelTextView;
    
    private TextView fff5TextView, fff4TextView;

    private Button start;
    private Button end;

    private RadioGroup ioRadioGroup;
    
    private int fff5Value;
    private int fff4Value;
    private int currentCheckId;
    private int currentLevel;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        this.setContentView(R.layout.turniming_out_put);
        initView();
        
        initOnClick();
    }
    
    @Override
    protected void onResume() {
        super.onResume();
        if (app.manager.cubicBLEDevice != null) {
            app.manager.cubicBLEDevice.setBLEBroadcastDelegate(this);
        }
    }

    private void initOnClick() {
        highLevelLayout.setOnClickListener(highLevelOnClick);
        lowLevelLayout.setOnClickListener(lowLevelOnClick);
        
        start.setOnClickListener(startOnClick);
        end.setOnClickListener(endOnClick);
        
        ioRadioGroup.setOnCheckedChangeListener(new RadioGroup.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(RadioGroup group, int checkedId) {
                if(checkedId  != currentCheckId) {
                    endTurn();
                    currentCheckId = checkedId;
                }
            }
        });
    }

    private void initView() {
        Intent intent = this.getIntent();
        MemberItem member = (MemberItem) intent.getSerializableExtra(App.TAG);
        this.initNavigation(member.name);
        
        highLevelLayout  = (LinearLayout) this.findViewById(R.id.first_item);
        lowLevelLayout  = (LinearLayout) this.findViewById(R.id.second_item);

        start  = (Button) this.findViewById(R.id.btn_start);
        end  = (Button) this.findViewById(R.id.btn_end);
        
        ioRadioGroup = (RadioGroup) this.findViewById(R.id.io_choose);
        fff5TextView = (TextView) this.findViewById(R.id.fff5_tv);
        fff4TextView = (TextView) this.findViewById(R.id.fff4_tv);
        
        highLevelTextView = (TextView) this.findViewById(R.id.first_txt);
        lowLevelTextView = (TextView) this.findViewById(R.id.second_txt);
        
        currentCheckId = R.id.io6_rb;
        currentLevel = 0;
        
        selectHighLevel();
    }
    
    private boolean isHighLevel() {
        return currentLevel == R.id.first_item;
    }
    
    private OnClickListener highLevelOnClick = new OnClickListener() {

        @Override
        public void onClick(View arg0) {
            selectHighLevel();
        }
    };
    
    private void selectHighLevel() {
        currentLevel = R.id.first_item;
        highLevelTextView.setEnabled(false);
        highLevelLayout.setEnabled(false);
        lowLevelTextView.setEnabled(true);
        lowLevelLayout.setEnabled(true);
    }
    
    private void selectLowLevel() {
        currentLevel = R.id.second_item;
        highLevelTextView.setEnabled(true);
        highLevelLayout.setEnabled(true);
        lowLevelTextView.setEnabled(false);
        lowLevelLayout.setEnabled(false);
    }
    
    private OnClickListener lowLevelOnClick = new OnClickListener() {

        @Override
        public void onClick(View arg0) {
            selectLowLevel();
        }
    };
    
    private OnClickListener startOnClick = new OnClickListener() {
        
        @Override
        public void onClick(View v) {
            if (app.manager.cubicBLEDevice != null) {
                if(!checkInput()) {
                    return;
                }
                
                app.manager.cubicBLEDevice.setBLEBroadcastDelegate(TurnimingOutputActivity.this);
                
                prepareForLevel();
                
                secondTurn();
                
                firstTurn();
            }
        }
    };
    
    private void firstTurn() {
        new Handler().postDelayed(new Runnable() { // 延时0.2秒后执行
            @Override
            public void run() {
                if(currentCheckId == R.id.io6_rb) {
                    app.manager.cubicBLEDevice.writeValue("fff0", "fff4", Tools.int2Bytes(fff4Value));
                } else {
                    app.manager.cubicBLEDevice.writeValue("fff0", "fff6", Tools.int2Bytes(fff4Value));
                }
            }
        }, 600);
    }

    private void secondTurn() {
        new Handler().postDelayed(new Runnable() { // 延时0.2秒后执行
            @Override
            public void run() {
                if(currentCheckId == R.id.io6_rb) {
                    app.manager.cubicBLEDevice.writeValue("fff0", "fff5", Tools.int2Bytes(fff5Value));
                } else {
                    app.manager.cubicBLEDevice.writeValue("fff0", "fff7", Tools.int2Bytes(fff5Value));
                }
            }
        }, 400);
    }

    private void prepareForLevel() {
        final byte[] data = new byte[1];
        data[0] = 0x40;
        final byte[] zeroData = new byte[1];
        zeroData[0] = 0x0;
        
        app.manager.cubicBLEDevice.writeValue("fff0", "fff1", data);
        
        new Handler().postDelayed(new Runnable() { // 延时0.2秒后执行
            @Override
            public void run() {
                if(isHighLevel()) {
                    app.manager.cubicBLEDevice.writeValue("fff0", "fff2", data);
                } else {
                    app.manager.cubicBLEDevice.writeValue("fff0", "fff2", zeroData);
                }
            }

        }, 200);
    }
    
    private boolean checkInput() {
        if(TextUtils.isEmpty(fff5TextView.getText().toString()) || TextUtils.isEmpty(fff4TextView.getText().toString())) {
            Toast.makeText(this, R.string.invalid_hint, Toast.LENGTH_LONG);
            return false;
        }
        fff5Value = Integer.valueOf(fff5TextView.getText().toString());
        fff4Value = Integer.valueOf(fff4TextView.getText().toString());
        
        if(fff5Value == 0 || fff4Value == 0) {
            Toast.makeText(this, R.string.invalid_hint, Toast.LENGTH_LONG);
            return false;
        }
        
        return true;
    }
    
    private OnClickListener endOnClick = new OnClickListener() {
        
        @Override
        public void onClick(View v) {
            if (app.manager.cubicBLEDevice != null) {
                app.manager.cubicBLEDevice.setBLEBroadcastDelegate(TurnimingOutputActivity.this);
            }
            endTurn();
        }
    };
    
    private void endTurn() {
        if (app.manager.cubicBLEDevice != null) {
            if(currentCheckId == R.id.io6_rb) {
                app.manager.cubicBLEDevice.writeValue("fff0", "fff4", Tools.int2Bytes(0));
            } else {
                app.manager.cubicBLEDevice.writeValue("fff0", "fff6", Tools.int2Bytes(0));
            }
        }
    }
    
    @Override
    protected void onDestroy() {
        super.onDestroy();
        endTurn();
    }

    @Override
    public void onReceive(Context context, Intent intent, String macData, String uuid) {
        
    }

}
