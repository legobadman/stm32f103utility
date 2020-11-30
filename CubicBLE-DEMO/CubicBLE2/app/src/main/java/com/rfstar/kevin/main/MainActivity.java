package com.rfstar.kevin.main;

import java.util.ArrayList;

import com.rfstar.kevin.main.R;
import com.rfstar.kevin.params.BLEDevice.RFStarBLEBroadcastReceiver;
import com.rfstar.kevin.params.MemberItem;
import com.rfstar.kevin.adapter.MemberAdapter;
import com.rfstar.kevin.app.App;
import com.rfstar.kevin.main.R;

import android.Manifest;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Bundle;
import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.graphics.Color;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemLongClickListener;
import android.widget.ListView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.TextView;

/**
 * 
 * @author Kevin.wu E-mail:wh19575782@163.com
 * 
 */
public class MainActivity extends BaseActivity implements OnItemClickListener, RFStarBLEBroadcastReceiver {
    private ArrayList<MemberItem> arraySource = null;
    private ListView list = null;
    private MemberAdapter adapter = null;
    private final  int ACCESS_COARSE_LOCATION_REQUEST_CODE = 1;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        initView();
        initNavigation(R.string.service_list);
        initPermissions();
    }


    private void initPermissions(){

        if(Build.VERSION.SDK_INT>= Build.VERSION_CODES.M){

            if (ContextCompat.checkSelfPermission(this, Manifest.permission.ACCESS_COARSE_LOCATION)
                    != PackageManager.PERMISSION_GRANTED) {
                //申请WRITE_EXTERNAL_STORAGE权限
                ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.ACCESS_COARSE_LOCATION, Manifest.permission.READ_EXTERNAL_STORAGE},
                        ACCESS_COARSE_LOCATION_REQUEST_CODE);
            }
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);

        if (requestCode == ACCESS_COARSE_LOCATION_REQUEST_CODE) {
            if(grantResults.length >0) {
                if (grantResults[0] == PackageManager.PERMISSION_GRANTED) {

                    Log.d("ooo", "==lee onRequestPermissionsResult READ_EXTERNAL_STORAGE_REQUEST_CODE==" + grantResults[0]);
                    // Permission Granted
                } else {
                    // Permission Denied
                    Log.d("ooo", "==lee onRequestPermissionsResult READ_EXTERNAL_STORAGE_REQUEST_CODE !=" + grantResults[0]);
                }
            }
        }

    }


    @Override
    protected void initNavigation(int title) {
        // TODO Auto-generated method stub
        super.initNavigation(title);
        navigateView.setLeftHideBtn(false);
        navigateView.leftBtn.setText(getString(R.string.About_About));
        navigateView.leftBtn.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                // TODO Auto-generated method stub
                Intent intent = new Intent(MainActivity.this, AboutActivity.class);
                startActivity(intent);
            }
        });
        navigateView.setRightHideBtn(false);
        navigateView.rightBtn.setText(getString(R.string.search));
        navigateView.rightBtn.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                // TODO Auto-generated method stub
                Intent intent = new Intent(MainActivity.this, SearchActivity.class);
                startActivity(intent);
            }
        });
    }

    @SuppressWarnings("static-access")
    private void initView() {

        initArraySource();
        adapter = new MemberAdapter(this, arraySource);

        this.list = (ListView) this.findViewById(R.id.list);
        this.list.setCacheColorHint(Color.TRANSPARENT);
        this.list.setDivider(null);
        this.list.setAdapter(adapter);
        this.list.setOnItemClickListener(this);

        this.list.setSelector(R.color.clear);
    }

    /**
     * 初始化source
     */
    private void initArraySource() {
        // TODO Auto-generated method stub
        this.arraySource = new ArrayList<MemberItem>();
        MemberItem item0 = new MemberItem();
        item0.name = getString(R.string.bluetooth_data_channel);
        item0.nameEnglish = "Bluetooth Data channel service";
        MemberItem item1 = new MemberItem();
        item1.name = getString(R.string.serial_data_channel);
        item1.nameEnglish = "Serial Data channel service";
        MemberItem item2 = new MemberItem();
        item2.name = getString(R.string.adc_input);
        item2.nameEnglish = "ADC input(2) service";
        MemberItem item3 = new MemberItem();
        item3.name = getString(R.string.rssi_reports);
        item3.nameEnglish = "RSSI report service";
        MemberItem item4 = new MemberItem();
        item4.name = getString(R.string.pwm_output);
        item4.nameEnglish = "PWM Output(4) service";
        MemberItem item5 = new MemberItem();
        item5.name = getString(R.string.batter_charge_reports);
        item5.nameEnglish = "Battery report service";

        MemberItem item6 = new MemberItem();
        item6.name = getString(R.string.time_flip_output);
        item6.nameEnglish = "Turn timing output(2) service";
        MemberItem item7 = new MemberItem();
        item7.name = getString(R.string.level_Width);
        item7.nameEnglish = "Level counting pulse width(2) service";
        MemberItem item8 = new MemberItem();
        item8.name = getString(R.string.port_configuration);
        item8.nameEnglish = "Port timing events configuration service";
        MemberItem item9 = new MemberItem();
        item9.name = getString(R.string.programmable_io);
        item9.nameEnglish = "Programmable IO(8) service";
        MemberItem item10 = new MemberItem();
        item10.name = getString(R.string.device_information);
        item10.nameEnglish = "Device information service";
        MemberItem item11 = new MemberItem();
        item11.name = getString(R.string.module_parameter_settings);
        item11.nameEnglish = "Module parameter Settings service";
        MemberItem item12 = new MemberItem();
        item12.name = getString(R.string.anti_hijack_key);
        item12.nameEnglish = "Anti-hijacking key service";
        MemberItem item13 = new MemberItem();
        item13.name = getString(R.string.gps_locate);
        item13.nameEnglish = "GPS Locate service";
        MemberItem item14 = new MemberItem();
        item14.name = getString(R.string.twoway_data_channe);
        item14.nameEnglish = "Two-way passthrough";
        
        this.arraySource.add(item0);
        this.arraySource.add(item1);
        this.arraySource.add(item2);
        this.arraySource.add(item3);
        this.arraySource.add(item4);
        this.arraySource.add(item5);
        this.arraySource.add(item6);
        this.arraySource.add(item7);
        this.arraySource.add(item8);
        this.arraySource.add(item9);
        this.arraySource.add(item10);
        this.arraySource.add(item11);
        this.arraySource.add(item12);
        this.arraySource.add(item13);
        this.arraySource.add(item14);
    }

    @Override
    protected void onResume() {
        // TODO Auto-generated method stub
        super.onResume();
        this.adapter.notifyDataSetChanged();
        if (app.manager.cubicBLEDevice != null) {
            app.manager.cubicBLEDevice.setBLEBroadcastDelegate(this);
        }
    }

    @Override
    public void onItemClick(AdapterView<?> arg0, View arg1, int arg2, long arg3) {
        // TODO Auto-generated method stub
        Intent intent = null;
        switch (arg2) {
        case App.BLUETOOTH_DATA_CHANNEL:
            intent = new Intent(this, SendDataActivity.class);
            break;
        case App.SERIAL_DATA_CHANNEL:
            intent = new Intent(this, ReceivedataActivity.class);
            break;
        case App.ADC_INPUT:
            intent = new Intent(this, ADC_Activity.class);
            break;
        case App.RSSI_REPORT:
            intent = new Intent(this, RssiActivity.class);
            break;
        case App.PWM_OUTPUT:
            intent = new Intent(this, PWMActivity.class);
            break;
        case App.BATTERY_REPORT:
            intent = new Intent(this, BatteryActivity.class);
            break;

        case App.TURNIMING_OUTPUT:
            intent = new Intent(this, TurnimingOutputActivity.class);
            break;
        case App.LEVEL_COUNTING_PULSE:
            intent = new Intent(this, LevelCountingPulseActivity.class);
            break;
        case App.PORT_TIMING_EVENTS_CONFIG:
            intent = new Intent(this, PortTimingEventsConfigPortsActivity.class);
            break;
        case App.PROGRAMM_ABLEIO:
            intent = new Intent(this, Programmable8Activity.class);
            break;
        case App.DEVICE_INFORMATION:
            intent = new Intent(this, DeviceInformationActivity.class);
            break;
        case App.MODULE_PARAMETER:
            intent = new Intent(this, ModuleParameterActivity.class);
            break;
        case App.ANTI_HIJACKINGKEY:
            intent = new Intent(this, AntiHiJackingKeyActivity.class);
            break;
        case App.GPS_SERVER:
            intent = new Intent(this, LocationAutoNotify.class);
            break;
        case App.PASSTHROUGH_CHANNEL:
            intent = new Intent(this, PassthroughActivity.class);
            break;
        default:
            break;
        }
        if (intent != null) {
            intent.putExtra(App.TAG, arraySource.get(arg2));
            startActivity(intent);
        }
    }

    @Override
    protected void onDestroy() {
        // TODO Auto-generated method stub
        super.onDestroy();
        if(app.manager.cubicBLEDevice != null) {
        //app.manager.cubicBLEDevice.disconnectedDevice();
        }
    }

    @Override
    public void onReceive(Context context, Intent intent, String macData, String uuid) {
        // TODO Auto-generated method stub
        connectedOrDis(intent.getAction());
    }

}
