package com.rfstar.kevin.main;

import com.rfstar.kevin.app.App;
import com.rfstar.kevin.main.R;
import com.rfstar.kevin.params.MemberItem;
import com.rfstar.kevin.params.BLEDevice.RFStarBLEBroadcastReceiver;
import com.rfstar.kevin.service.RFStarBLEService;
import com.rfstar.kevin.tools.Tools;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

public class LevelCountingPulseActivity extends BaseActivity implements RFStarBLEBroadcastReceiver {
    private TextView IO4 = null;
    private TextView IO5 = null;
    public static final String TAG = LevelCountingPulseActivity.class.getSimpleName();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        this.setContentView(R.layout.level_counting_pulse);
        initView();
    }

    private void initView() {
        Intent intent = this.getIntent();
        MemberItem member = (MemberItem) intent.getSerializableExtra(App.TAG);
        this.initNavigation(member.name);
        IO4 = (TextView) findViewById(R.id.tv_io4);
        IO5 = (TextView) findViewById(R.id.tv_io5);
    }

    @Override
    protected void onResume() {
        super.onResume();
        this.getPulseInfo(null);
    }

    public void getPulseInfo(View view) {
        if (app.manager.cubicBLEDevice == null) {
            return;
        }
        app.manager.cubicBLEDevice.setBLEBroadcastDelegate(this);
        app.manager.cubicBLEDevice.readValue("fff0", "fff8");
        app.manager.cubicBLEDevice.setNotification("fff0", "fff8", true);
        new Handler().postDelayed(new Runnable() { // 延时0.2秒后执行
                    @Override
                    public void run() {
                        app.manager.cubicBLEDevice.readValue("fff0", "fff9");
                        app.manager.cubicBLEDevice.setNotification("fff0", "fff9", true);
                    }
                }, 500);
    }

    @Override
    public void onReceive(Context context, Intent intent, String macData, String uuid) {
        String action = intent.getAction();
        Log.v(TAG, "action is " + action);
        this.connectedOrDis(action);
        if (RFStarBLEService.ACTION_GATT_CONNECTED.equals(action)) {
            this.setTitle(macData + getString(R.string.connected));

        } else if (RFStarBLEService.ACTION_GATT_DISCONNECTED.equals(action)) {
            this.setTitle(macData + getString(R.string.disconnected));

        } else if (RFStarBLEService.ACTION_DATA_AVAILABLE.equals(action)) {
            byte[] data = intent.getByteArrayExtra(RFStarBLEService.EXTRA_DATA);
            Log.v(TAG, "uuid is" + uuid);
            Log.v(TAG, "data is" + Tools.bytes2Int(data));
            if (uuid.toLowerCase().contains("fff8")) {
                IO4.setText(Tools.bytes2Int(data) + "ms");
            } else if (uuid.toLowerCase().contains("fff9")) {
                IO5.setText(Tools.bytes2Int(data) + "ms");
            }
        } else if (RFStarBLEService.ACTION_GATT_SERVICES_DISCOVERED.equals(action)) {

        }
    }

}
