package com.rfstar.kevin.main;

import android.content.Context;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.View;
import android.view.WindowManager;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.Spinner;
import android.widget.ToggleButton;

import com.rfstar.kevin.main.R;
import com.rfstar.kevin.params.BLEDevice;
import com.rfstar.kevin.service.RFStarBLEService;
import com.rfstar.kevin.tools.Tools;
import com.rfstar.kevin.view.NavigateView;
import com.rfstar.kevin.view.ReceiveDataView;
import com.rfstar.kevin.view.SegmentView;
import com.rfstar.kevin.view.SendDataView;

import java.io.UnsupportedEncodingException;

public class PassthroughActivity extends BaseActivity implements
        View.OnClickListener, BLEDevice.RFStarBLEBroadcastReceiver {

    private ReceiveDataView receiveDataView;
    private SendDataView sendDataView;

    private SegmentView segmentView;
    private Spinner intervalSpinner;

    private ToggleButton autoSendSwitch;
    private ToggleButton recvDataSwitch;

    private Button resetButton;
    private Button clearButton;
    private Button sendButton;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_passthrough);

        initView();
        initNavigation(R.string.twoway_data_channe);
        getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_ADJUST_PAN);
    }

    @Override
    protected void onResume() {
        // TODO Auto-generated method stub
        super.onResume();
        if (app.manager.cubicBLEDevice != null)
            app.manager.cubicBLEDevice.setBLEBroadcastDelegate(this);
    }

    @Override
    protected void onDestroy() {
        // TODO Auto-generated method stub
        super.onDestroy();
        if (app.manager.cubicBLEDevice != null)
            app.manager.cubicBLEDevice.setNotification("ffe0", "ffe4",
                    false);
    }

    protected void initNavigation(int title) {
        navigateView = (NavigateView) this.findViewById(R.id.navigateView);
        navigateView.setTitle(title);
        navigateView.setRightHideBtn(false);
        navigateView.rightBtn.setText(getString(R.string.search));
        navigateView.rightBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // TODO Auto-generated method stub
                Intent intent = new Intent(PassthroughActivity.this,
                        SearchActivity.class);
                startActivity(intent);
            }
        });
    }

    private void initView() {

        receiveDataView = (ReceiveDataView) findViewById(R.id.passthrough_recv_view);
        sendDataView = (SendDataView) findViewById(R.id.passthrough_send_view);

        segmentView = (SegmentView) findViewById(R.id.passthrough_segmentview);
        intervalSpinner = (Spinner) findViewById(R.id.passthrough_interval);

        autoSendSwitch = (ToggleButton) findViewById(R.id.passthrough_switch_1);
        recvDataSwitch = (ToggleButton) findViewById(R.id.passthrough_switch_2);

        resetButton = (Button) findViewById(R.id.passthrough_reset_btn);
        clearButton = (Button) findViewById(R.id.passthrough_clear_btn);
        sendButton = (Button) findViewById(R.id.passthrough_send_btn);

        sendDataView.setEdit("Hello");
        resetButton.setOnClickListener(this);
        clearButton.setOnClickListener(this);
        sendButton.setOnClickListener(this);

        autoSendSwitch.setOnCheckedChangeListener(myOnCheckedChangeListener);
        recvDataSwitch.setOnCheckedChangeListener(myOnCheckedChangeListener);

        segmentView.setOnSegmentViewClickListener(myOnSegmentViewClickListener);
        intervalSpinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int pos, long id) {

                if(pos == 0){
                    sendtime = 100;
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
    }

    private SegmentView.onSegmentViewClickListener myOnSegmentViewClickListener =
            new SegmentView.onSegmentViewClickListener() {
                @Override
                public void onSegmentViewClick(View v, int position) {

                    switch (position) {

                        case 0:
                        {
                            sendDataView.inputHex(false);
                        }
                            break;

                        case 1:
                        {
                            sendDataView.inputHex(true);
                        }
                            break;

                        default:
                            break;

                    }

                }
            };

    private CompoundButton.OnCheckedChangeListener myOnCheckedChangeListener =
            new CompoundButton.OnCheckedChangeListener(){

        @Override
        public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {

            switch (buttonView.getId()) {

                case R.id.passthrough_switch_1:
                {
                    intervalSpinner.setEnabled(!isChecked);

                    if (app.manager.cubicBLEDevice != null) {
                        if (isChecked) {
                            new AsyncSendEmptyMessageRight().execute();
                        } else {
                            iSend = true;
                        }
                    }
                }
                    break;

                case R.id.passthrough_switch_2:
                {
                    receiveDataView.changeEditBackground(isChecked);
                    if (app.manager.cubicBLEDevice != null)
                        app.manager.cubicBLEDevice.setNotification("ffe0", "ffe4",
                                isChecked);
                }
                    break;

                default:
                    break;

            }
        }
    };

    @Override
    public void onClick(View v) {

        switch (v.getId()) {

            case R.id.passthrough_reset_btn:
            {
                receiveDataView.reset();
                sendDataView.reset();
            }
                break;

            case R.id.passthrough_clear_btn:
            {
                receiveDataView.reset();
                sendDataView.clear();
            }
                break;

            case R.id.passthrough_send_btn:
            {
                if (app.manager.cubicBLEDevice != null) {
                    app.manager.cubicBLEDevice.writeValue("ffe5", "ffe9", Tools.getBytes(sendDataView.getHexEdit()));
                    sendDataView.setCountTimes();
                }
            }
                break;

            default:
                break;
        }

    }

    @Override
    public void onReceive(Context context, Intent intent, String macData, String uuid) {
        String action = intent.getAction();
        this.connectedOrDis(intent.getAction());
        if (RFStarBLEService.ACTION_DATA_AVAILABLE.equals(action)) {
            if (uuid.contains("ffe4")) {
                byte[] data = intent
                        .getByteArrayExtra(RFStarBLEService.EXTRA_DATA);
                try {
                    receiveDataView.setCountTimesTxt();
                    receiveDataView.appendString(new String(data, "GB2312"));
                } catch (UnsupportedEncodingException e) {
                    // TODO Auto-generated catch block
                    e.printStackTrace();
                }
            }
        } else if (RFStarBLEService.ACTION_GATT_SERVICES_DISCOVERED
                .equals(action)) {

        }
    }

    private boolean iSend = false;
    private int sendtime = 10;
    class AsyncSendEmptyMessageRight extends AsyncTask<Void, Integer, Integer> {
        @Override
        protected Integer doInBackground(Void... params) {

            int count = 0;
            while (!iSend) {
                if (app.manager.cubicBLEDevice != null) {
                    app.manager.cubicBLEDevice.writeValue("ffe5","ffe9", Tools.getBytes(sendDataView.getHexEdit()));
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
            sendDataView.setCountTimes();
        }


    }

    @Override
    public boolean dispatchKeyEvent(KeyEvent event) {
        if (event.getKeyCode() == KeyEvent.KEYCODE_BACK && event.getRepeatCount() == 0
                && event.getAction() == KeyEvent.ACTION_DOWN) {

            iSend = true;
            finish();
        }
        return super.dispatchKeyEvent(event);
    }
}
