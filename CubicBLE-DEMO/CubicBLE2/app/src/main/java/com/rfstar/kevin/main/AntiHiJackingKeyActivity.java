package com.rfstar.kevin.main;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;

import com.rfstar.kevin.app.App;
import com.rfstar.kevin.main.R;
import com.rfstar.kevin.params.BLEDevice.RFStarBLEBroadcastReceiver;
import com.rfstar.kevin.params.MemberItem;
import com.rfstar.kevin.service.RFStarBLEService;
import com.rfstar.kevin.tools.PreferenceHelper;

public class AntiHiJackingKeyActivity extends BaseActivity implements RFStarBLEBroadcastReceiver, OnClickListener {

    private final static String TAG = "lee";
    private EditText etPwdStatus;
    private EditText etOldPwd;
    private EditText etNewPwd;
    private Button btOk;
    private Button btCancel;

    private String pwdStatus;
    private String oldPwd;
    private String newPwd;

    private String BlePwd;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        // TODO Auto-generated method stub
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_ajk);
        initView();
    }

    @Override
    protected void onResume() {
        // TODO Auto-generated method stub
        super.onResume();
        if (app.manager.cubicBLEDevice != null) {
            app.manager.cubicBLEDevice.setBLEBroadcastDelegate(this);
            // this.getDeviceMessage(null);
        }
    }

    @SuppressWarnings("static-access")
    private void initView() {
        // TODO Auto-generated method stub
        Intent intent = this.getIntent();
        MemberItem member = (MemberItem) intent.getSerializableExtra(App.TAG);
        this.initNavigation(member.name);
        pwdStatus = "";
        BlePwd = "000000";

        etPwdStatus = (EditText) findViewById(R.id.et_pwd_status);
        etPwdStatus.setText(getString(R.string.init_pwd));
        etOldPwd = (EditText) findViewById(R.id.et_old_pwd);
        //etOldPwd.setText(BlePwd);
        etNewPwd = (EditText) findViewById(R.id.et_new_pwd);

        btOk = (Button) findViewById(R.id.bt_ok);
        btCancel = (Button) findViewById(R.id.bt_cancel);

        btOk.setOnClickListener(this);
        btCancel.setOnClickListener(this);
        // open notify
        openNotify(true);
    }

    @Override
    public void onClick(View v) {
        // TODO Auto-generated method stub
        switch (v.getId()) {
        case R.id.bt_ok:
            Log.d(TAG, "==lee onClick  ok==");
            // commit pwd
            oldPwd = etOldPwd.getText().toString();
            newPwd = etNewPwd.getText().toString();
            StringBuilder sb = new StringBuilder(oldPwd);
            sb.append(newPwd);
            Log.d(TAG, "==lee onClick  ok==" + sb.toString());
            setPwd(sb.toString().getBytes());
            break;
        case R.id.bt_cancel:
            Log.d(TAG, "==lee onClick  cancel==");
            // cancel pwd
            StringBuilder sbCancel = new StringBuilder(BlePwd);
            sbCancel.append("000000");
            Log.d(TAG, "==lee onClick  cancel==" + sbCancel.toString());
            setPwd(sbCancel.toString().getBytes());
            break;
        }

    }

    @Override
    public void onBackPressed() {
        // TODO Auto-generated method stub
        StringBuilder sbCancel = new StringBuilder(BlePwd);
        sbCancel.append("000000");
        Log.d(TAG, "==lee onBackPressed==" + sbCancel.toString());
        //setPwd(sbCancel.toString().getBytes());
        super.onBackPressed();
    }

    private void setPwd(byte[] data) {
        if (app.manager.cubicBLEDevice != null) {
            app.manager.cubicBLEDevice.writeValue("ffc0", "ffc1", data);
        }
    }

    private void openNotify(boolean enable) {
        Log.d(TAG, "==lee openNotify==" + enable);
        if (app.manager.cubicBLEDevice != null) {
            app.manager.cubicBLEDevice.setNotification("ffc0", "ffc2", enable);
        }

    }

    @Override
    public void onReceive(Context context, Intent intent, String macData, String uuid) {

        Log.d(TAG, "lee 有数据返回");
        String action = intent.getAction();
        int ret = 0;
        this.connectedOrDis(action);
        if (RFStarBLEService.ACTION_DATA_AVAILABLE.equals(action)) {
            byte[] data = intent.getByteArrayExtra(RFStarBLEService.EXTRA_DATA);
            if (uuid.contains("ffc2")) {
                // rssiTxt.setText(""+(int) data[0]);
                ret = data[0];
                Log.d(TAG, "lee 有数据返回" + data[0]);
                if (ret == 1) {
                    Log.d(TAG, "commit error");
                    pwdStatus = getString(R.string.set_pwd_error);
                    etPwdStatus.setText(pwdStatus);
                } else if (ret == 2) {
                    BlePwd = newPwd;
                    pwdStatus =  getString(R.string.set_pwd_success);
                    etPwdStatus.setText(pwdStatus);
                    PreferenceHelper.setParam(AntiHiJackingKeyActivity.this, app.manager.cubicBLEDevice.device.getAddress(), newPwd);
                    Log.e("wang3","address:"+app.manager.cubicBLEDevice.device.getAddress());
                } else if (ret == 3) {
                    BlePwd = "000000";
                    pwdStatus =  getString(R.string.cancel_pwd_success);
                    etPwdStatus.setText(pwdStatus);
                }
            }
        } else if (RFStarBLEService.ACTION_GATT_SERVICES_DISCOVERED.equals(action)) {
        }else if(RFStarBLEService.ACTION_GATT_SERVICES_DISCOVERED.equals(action)){
            openNotify(true);
        }


    }

}
