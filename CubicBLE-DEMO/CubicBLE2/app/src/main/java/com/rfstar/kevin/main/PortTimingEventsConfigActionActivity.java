package com.rfstar.kevin.main;

import java.text.SimpleDateFormat;
import com.rfstar.kevin.main.R;
import com.rfstar.kevin.params.BLEDevice.RFStarBLEBroadcastReceiver;
import com.rfstar.kevin.service.RFStarBLEService;
import com.rfstar.kevin.tools.Tools;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.CompoundButton.OnCheckedChangeListener;
import android.widget.TimePicker;
import android.widget.TimePicker.OnTimeChangedListener;

public class PortTimingEventsConfigActionActivity extends BaseActivity implements RFStarBLEBroadcastReceiver {

    private final static String TAG = "lee";

    private int portNum = -1;
    private int eventIndex = -1;
    private String executeTime;
    private String currentTime;
    private int actionType = 0;
    private String operatePara;

    private String eventBit;// 4 byte

    TimePicker tp;
    CheckBox cbHighLevel;
    CheckBox cbLowLevel;
    CheckBox cbTurnoverLevel;

    CheckBox cbPwmMutate;
    CheckBox cbPwmGraduate;

    Button btSet;

    private String hourSet = "00";
    private String minuteSet = "00";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        // TODO Auto-generated method stub
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_action_time);
        initView();
        // 开启通知使能
        openIoNotify(true);
    }

    @Override
    protected void onResume() {
        Log.d(TAG, "==lee onResume==portNum " + portNum);
        // TODO Auto-generated method stub
        if (app.manager.cubicBLEDevice != null) {
            app.manager.cubicBLEDevice.setBLEBroadcastDelegate(this);
        }
        // this.getDeviceMessage(null);
        if (portNum >= 0 && portNum <= 5) {
            cbHighLevel.setVisibility(View.VISIBLE);
            cbLowLevel.setVisibility(View.VISIBLE);
            cbTurnoverLevel.setVisibility(View.VISIBLE);
            cbPwmMutate.setVisibility(View.GONE);
            cbPwmGraduate.setVisibility(View.GONE);
        } else if (portNum >= 6 && portNum <= 9) {
            cbHighLevel.setVisibility(View.GONE);
            cbLowLevel.setVisibility(View.GONE);
            cbTurnoverLevel.setVisibility(View.GONE);
            cbPwmMutate.setVisibility(View.VISIBLE);
            cbPwmGraduate.setVisibility(View.VISIBLE);
        }

        super.onResume();

    }

    @SuppressWarnings("static-access")
    private void initView() {
        Intent intent = this.getIntent();
        portNum = intent.getIntExtra("port_bumber", 0);
        eventIndex = intent.getIntExtra("event_index", 0);

        Log.d(TAG, "==lee initView select port/event ==" + portNum + "\\" + eventIndex);

        this.initNavigation(getString(R.string.time_and_action));

        tp = (TimePicker) findViewById(R.id.time_picker);
        tp.setIs24HourView(true);

        cbHighLevel = (CheckBox) findViewById(R.id.high_level);
        cbLowLevel = (CheckBox) findViewById(R.id.low_level);
        cbTurnoverLevel = (CheckBox) findViewById(R.id.turnover_level);
        cbPwmMutate = (CheckBox) findViewById(R.id.pwm_mutate);
        cbPwmGraduate = (CheckBox) findViewById(R.id.pwm_graduate);

        btSet = (Button) findViewById(R.id.bt_commit);

        tp.setOnTimeChangedListener(new OnTimeChangedListener() {

            @Override
            public void onTimeChanged(TimePicker view, int hourOfDay, int minute) {
                // TODO Auto-generated method stub
                Log.d(TAG, "==lee onTimeChanged==" + hourOfDay + ":" + minute);
                if (hourOfDay < 10) {
                    hourSet = "0" + hourOfDay;
                } else {
                    hourSet = Integer.toString(hourOfDay);
                }
                if (minute < 10) {
                    minuteSet = "0" + minute;
                } else {
                    minuteSet = Integer.toString(minute);
                }
            }

        });

        cbHighLevel.setOnCheckedChangeListener(new OnCheckedChangeListener() {

            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                // TODO Auto-generated method stub
                if (isChecked) {
                    actionType = 1;
                } else {
                    actionType = 0;
                }
                Log.d(TAG, "==lee cbHighLevel onCheckedChanged actionType==" + actionType);

            }
        });
        cbLowLevel.setOnCheckedChangeListener(new OnCheckedChangeListener() {

            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                // TODO Auto-generated method stub
                if (isChecked) {
                    actionType = 2;
                } else {
                    actionType = 0;
                }
                Log.d(TAG, "==lee cbLowLevel onCheckedChanged actionType==" + actionType);

            }
        });
        cbTurnoverLevel.setOnCheckedChangeListener(new OnCheckedChangeListener() {

            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                // TODO Auto-generated method stub
                if (isChecked) {
                    actionType = 3;
                } else {
                    actionType = 0;
                }
                Log.d(TAG, "==lee cbTurnoverLevel onCheckedChanged actionType==" + actionType);
            }
        });
        cbPwmMutate.setOnCheckedChangeListener(new OnCheckedChangeListener() {

            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                // TODO Auto-generated method stub
                if (isChecked) {
                    actionType = 4;
                } else {
                    actionType = 0;
                }
                Log.d(TAG, "==lee cbPwmMutate onCheckedChanged actionType==" + actionType);
            }
        });
        cbPwmGraduate.setOnCheckedChangeListener(new OnCheckedChangeListener() {

            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                // TODO Auto-generated method stub
                if (isChecked) {
                    actionType = 5;
                } else {
                    actionType = 0;
                }
                Log.d(TAG, "==lee cbPwmGraduate onCheckedChanged actionType==" + actionType);
            }
        });

        btSet.setOnClickListener(new OnClickListener() {

            @Override
            public void onClick(View v) {
                // TODO Auto-generated method stub
                Log.d(TAG, "==lee btSet onClick==");

                // get currentTime
                currentTime = getCurrentTime();
                Log.d(TAG, "==lee currentTime ==" + currentTime);

                // get timing time
                executeTime = getTimerString(currentTime);
                Log.d(TAG, "==lee executeTime ==" + executeTime);
                portNum = 2;
                eventIndex = 0;
                actionType = 3;

                // 开启对应端口的事件 pass
                setPortoutput(portNum);

                // set timer event pass
                setTimerEvent(eventIndex, executeTime, actionType);

                // 开启定时端口的定时事件 pass
                openIoTimerEvent(portNum, eventIndex);

                // 打开端口定时任务使能开关和定时任务总使能开关
                openIoAndAll(portNum);

                // 更新rtc时间 pass
                updateRtc(currentTime);

            }

        });

    }

    private String getTimerString(String oldTime) {
        StringBuffer sb = new StringBuffer(oldTime);
        Log.d(TAG, "==lee getTimerString ==" + hourSet + "\\" + minuteSet);
        sb.replace(8, 10, hourSet);
        Log.d(TAG, "==lee getTimerString ==" + sb.toString());
        sb.replace(10, 12, minuteSet);
        Log.d(TAG, "==lee getTimerString ==" + sb.toString());
        return sb.toString();
    }

    private String getCurrentTime() {
        SimpleDateFormat sDateFormat = new SimpleDateFormat("yyyyMMddHHmmss");
        String date = sDateFormat.format(new java.util.Date());
        Log.d(TAG, "==lee currentTime ==" + date);

        return date;
    }

    private void openIoNotify(boolean enable) {
        Log.d(TAG, "==lee openIoNotify==" + enable);

        if (app.manager.cubicBLEDevice != null) {
            app.manager.cubicBLEDevice.setNotification("fff0", "fff3", enable);
        }

    }

    // config output port
    void setPortoutput(int port) {
        if (port < 8) {
            byte[] bt = new byte[8];
            for (int i = 0; i <= 7; i++) {
                if (i == (7 - port)) {
                    bt[i] = 1;
                } else {
                    bt[i] = 0;
                }
            }

            byte data = Tools.getByteFromBooleanArray(bt);
            Log.d(TAG, "==lee setPortoutput ==" + data);
            if (app.manager.cubicBLEDevice != null) {
                app.manager.cubicBLEDevice.writeValue("fff0", "fff1", new byte[] { data });
            }
        }
    }

    // set timer event
    void setTimerEvent(int eventIndex, String time, int actionType) {
        StringBuffer sb = new StringBuffer();

        int second = Integer.parseInt(time.substring(12, 14));
        int minute = Integer.parseInt(time.substring(10, 12));
        int hour = Integer.parseInt(time.substring(8, 10));
        int day = Integer.parseInt(time.substring(6, 8));
        int month = Integer.parseInt(time.substring(4, 6));
        int year = Integer.parseInt(time.substring(0, 4));
        int yearLow = (year & 0xff);
        int yearHigh = ((year >> 8) & 0xff);

        Log.d("lee", "==setTimerEvent==" + year + "\\" + month + "\\" + day + "\\" + hour + "\\" + minute + "\\"
                + second);
        String eventIndexStr = dec2HexStr(eventIndex);

        String secondStr = dec2HexStr(second);
        String minuteStr = dec2HexStr(minute);
        String hourStr = dec2HexStr(hour);
        String dayStr = dec2HexStr(day);
        String monthStr = dec2HexStr(month);

        String yearLowStr = dec2HexStr(yearLow);
        String yearHighStr = dec2HexStr(yearHigh);

        Log.d("lee", "==setTimerEvent==" + secondStr + "\\" + minuteStr + "\\" + hourStr + "\\" + dayStr + "\\"
                + monthStr + "\\" + yearLowStr + "\\" + yearHighStr);

        String actionTypeStr = dec2HexStr(actionType);

        String bt9Str;
        if ((actionType == 4) || (actionType == 5)) {
            bt9Str = dec2HexStr(100);
        } else {
            bt9Str = "00";
        }

        int pwmMutateTime = 3000;
        String bt10Str;
        String bt11Str;
        if (actionType == 5) {
            bt10Str = dec2HexStr(pwmMutateTime & 0xff);
            bt11Str = dec2HexStr((pwmMutateTime >> 16) & 0xff);
        } else {
            bt10Str = "00";
            bt11Str = "00";
        }
        sb.append(eventIndexStr).append(secondStr).append(minuteStr).append(hourStr).append(dayStr).append(monthStr)
                .append(yearLowStr).append(yearHighStr).append(actionTypeStr).append(bt9Str).append(bt10Str)
                .append(bt11Str);

        Log.d("lee", "==setTimerEvent==" + sb.toString());

        if (app.manager.cubicBLEDevice != null) {
            app.manager.cubicBLEDevice.writeValue("fe00", "fe03", sb.toString());
        }
    }

    // 十进制转十六进制字符串 前面 补0
    String dec2HexStr(int num) {
        String ret = Integer.toHexString(num);

        Log.d(TAG, "==dec2HexStr==" + ret);
        if (ret.length() == 2) {
            return ret;
        } else if (ret.length() == 1) {
            return "0" + ret;
        } else
            return "00";

    }

    // 开启指定端口的定时事件
    private void openIoTimerEvent(int port, int eventIndex) {

        Log.d(TAG, "==lee openIoTimerEvent==" + port + "\\" + eventIndex);
        StringBuffer sb = new StringBuffer();
        int tmpIndex = eventIndex % 8;

        int bt0, bt1, bt2, bt3, bt4;
        bt0 = 0;
        bt1 = 0;
        bt2 = 0;
        bt3 = 0;
        bt4 = 0;

        bt0 = port;

        if (eventIndex < 8) {
            bt1 = byteArray2Byte(tmpIndex);
            bt2 = 0;
            bt3 = 0;
            bt4 = 0;
        } else if (eventIndex < 16) {
            bt1 = 0;
            bt2 = byteArray2Byte(tmpIndex);
            bt3 = 0;
            bt4 = 0;

        } else if (eventIndex < 24) {
            bt1 = 0;
            bt2 = 0;
            bt3 = byteArray2Byte(tmpIndex);
            bt4 = 0;

        } else if (eventIndex < 32) {
            bt1 = 0;
            bt2 = 0;
            bt3 = 0;
            bt4 = byteArray2Byte(tmpIndex);
        }

        String bt0Str = dec2HexStr(bt0);
        String bt1Str = dec2HexStr(bt1);
        String bt2Str = dec2HexStr(bt2);
        String bt3Str = dec2HexStr(bt3);
        String bt4Str = dec2HexStr(bt4);
        Log.d(TAG, "==lee openIoTimerEvent==" + bt0 + "\\" + bt1 + "\\" + bt2 + "\\" + bt3 + "\\" + bt4);

        sb.append(bt0Str).append(bt1Str).append(bt2Str).append(bt3Str).append(bt4Str);
        Log.d(TAG, "==lee openIoTimerEvent==" + sb.toString());
        if (app.manager.cubicBLEDevice != null) {
            app.manager.cubicBLEDevice.writeValue("fe00", "fe05", sb.toString());
        }
    }

    private int byteArray2Byte(int index) {
        if (index < 8) {
            byte[] bt = new byte[8];
            for (int i = 0; i <= 7; i++) {
                if (i == (7 - index)) {
                    bt[i] = 1;
                } else {
                    bt[i] = 0;
                }
            }

            int data = Tools.getIntFromBooleanArray(bt);
            Log.d(TAG, "==lee byteArray2Byte ==" + data);
            return data;
        }
        return 0;
    }

    // 使能端口定时事件总使能位和 端口使能位 和 清除所有定时事件
    private void openIoAndAll(int port) {
        Log.d(TAG, "==lee openIoAndAll==" + port + "\\" + port);

        StringBuffer sb = new StringBuffer();
        int bt0, bt1;
        bt0 = 0;
        bt1 = 0;

        byte[] low = new byte[8];
        // byte[] high = new byte[8];

        if ((port >= 0) && (port < 7)) {
            low[0] = 1; // 总使能开关
            for (int i = 0; i <= 7; i++) {
                if (i == 6 - port) {
                    low[i] = 1;
                } else {
                    low[i] = 0;
                }
            }
            low[7] = 1; // 总使能开关
            bt0 = Tools.getIntFromBooleanArray(low);
            bt1 = 0;
        } else if (port >= 7 && (port <= 9)) {
            bt0 = 1;
            if (port == 7) {
                bt1 = 1;
            } else if (port == 8) {
                bt1 = 2;
            } else if (port == 9) {
                bt1 = 4;
            }

        }

        Log.d(TAG, "==openIoAndAll bt0==" + bt0);
        Log.d(TAG, "==openIoAndAll bt1==" + bt1);

        sb.append(dec2HexStr(bt0)).append(dec2HexStr(bt1));
        Log.d(TAG, "==openIoAndAll string==" + sb.toString());

        if (app.manager.cubicBLEDevice != null) {
            app.manager.cubicBLEDevice.writeValue("fe00", "fe06", sb.toString());
        }
    }

    // 清除所有定时事件和定时任务
    private void removeIoAndAll() {
        Log.d(TAG, "==lee removeIoAndAll==");

        StringBuffer sb = new StringBuffer();
        String bt0, bt1;

        bt0 = "00";
        bt1 = dec2HexStr(24);
        Log.d(TAG, "==openIoAndAll bt0==" + bt0);
        Log.d(TAG, "==openIoAndAll bt1==" + bt1);

        sb.append(bt0).append(bt1);
        Log.d(TAG, "==openIoAndAll string==" + sb.toString());

        if (app.manager.cubicBLEDevice != null) {
            app.manager.cubicBLEDevice.writeValue("fe00", "fe06", sb.toString());
        }
    }

    // Update RTC
    void updateRtc(String time) {
        StringBuffer sb = new StringBuffer();

        int second = Integer.parseInt(time.substring(12, 14));
        int minute = Integer.parseInt(time.substring(10, 12));
        int hour = Integer.parseInt(time.substring(8, 10));
        int day = Integer.parseInt(time.substring(6, 8));
        int month = Integer.parseInt(time.substring(4, 6));
        int year = Integer.parseInt(time.substring(0, 4));
        int yearLow = (year & 0xff);
        int yearHigh = ((year >> 8) & 0xff);

        Log.d("lee", "==updateRtc==" + year + "\\" + month + "\\" + day + "\\" + hour + "\\" + minute + "\\" + second);

        String secondStr = dec2HexStr(second);
        String minuteStr = dec2HexStr(minute);
        String hourStr = dec2HexStr(hour);
        String dayStr = dec2HexStr(day);
        String monthStr = dec2HexStr(month);

        String yearLowStr = dec2HexStr(yearLow);
        String yearHighStr = dec2HexStr(yearHigh);

        Log.d("lee", "==updateRtc==" + secondStr + "\\" + minuteStr + "\\" + hourStr + "\\" + dayStr + "\\" + monthStr
                + "\\" + yearLowStr + "\\" + yearHighStr);

        sb.append(secondStr).append(minuteStr).append(hourStr).append(dayStr).append(monthStr).append(yearLowStr)
                .append(yearHighStr);

        Log.d("lee", "==updateRtc==" + sb.toString());

        if (app.manager.cubicBLEDevice != null) {
            app.manager.cubicBLEDevice.writeValue("fe00", "fe01", sb.toString());
        }
    }

    @Override
    public void onReceive(Context context, Intent intent, String macData, String uuid) {

        Log.d(TAG, "lee 有数据返回");
        String action = intent.getAction();
        // int ret = 0;
        this.connectedOrDis(action);
        if (RFStarBLEService.ACTION_DATA_AVAILABLE.equals(action)) {
            byte[] data = intent.getByteArrayExtra(RFStarBLEService.EXTRA_DATA);
            if (uuid.contains("fff3")) {
                // rssiTxt.setText(""+(int) data[0]);
                // ret = data[0];
                Log.d(TAG, "lee 有数据返回" + data);

            }
        } else if (RFStarBLEService.ACTION_GATT_SERVICES_DISCOVERED.equals(action)) {
        }

    }

}
