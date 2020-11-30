package com.rfstar.kevin.main;

import java.util.ArrayList;
import com.rfstar.kevin.adapter.PortsAdapter;
import com.rfstar.kevin.app.App;
import com.rfstar.kevin.main.R;
import com.rfstar.kevin.params.MemberItem;
import com.rfstar.kevin.params.BLEDevice.RFStarBLEBroadcastReceiver;
import com.rfstar.kevin.params.PortItem;
import android.content.Context;
import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ListView;
import android.widget.AdapterView.OnItemClickListener;

public class PortTimingEventsConfigPortsActivity extends BaseActivity implements OnItemClickListener,
        RFStarBLEBroadcastReceiver {

    private ArrayList<PortItem> arraySource = null;
    private ListView list = null;
    private PortsAdapter adapter = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        // TODO Auto-generated method stub
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_port_num);
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

        initArraySource();
        adapter = new PortsAdapter(this, arraySource);

        this.list = (ListView) this.findViewById(R.id.list_port_num);
        this.list.setCacheColorHint(Color.TRANSPARENT);
        this.list.setDivider(null);
        this.list.setAdapter(adapter);
        this.list.setOnItemClickListener(this);

        this.list.setSelector(R.color.clear);

        // deviceIDtxt = (TextView) findViewById(R.id.deviceIDTxt);
        // versionTxt = (TextView) findViewById(R.id.deviceVersionTxt);
    }

    private void initArraySource() {
        // TODO Auto-generated method stub
        this.arraySource = new ArrayList<PortItem>();

        PortItem item0 = new PortItem();
        item0.port = "IO Port 0";
        PortItem item1 = new PortItem();
        item1.port = "IO Port 1";
        PortItem item2 = new PortItem();
        item2.port = "IO Port 2";
        PortItem item3 = new PortItem();
        item3.port = "IO Port 3";
        PortItem item4 = new PortItem();
        item4.port = "IO Port 4";
        PortItem item5 = new PortItem();
        item5.port = "IO Port 5";

        PortItem item6 = new PortItem();
        item6.port = "PWM Port 0";
        PortItem item7 = new PortItem();
        item7.port = "PWM Port 1";
        PortItem item8 = new PortItem();
        item8.port = "PWM Port 2";
        PortItem item9 = new PortItem();
        item9.port = "PWM Port 3";

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
    }

    public void onItemClick(AdapterView<?> arg0, View arg1, int arg2, long arg3) {
        // TODO Auto-generated method stub
        Intent intent = null;

        if ((arg2 >= 0) && (arg2 <= 9)) {
            intent = new Intent(this, PortTimingEventsConfigEventsActivity.class);
        }

        if (intent != null) {
            intent.putExtra(App.TAG, arraySource.get(arg2));
            intent.putExtra("port_bumber",arg2);
            startActivity(intent);
        }
    }

    @Override
    public void onReceive(Context context, Intent intent, String macData, String uuid) {
        // TODO Auto-generated method stub

    }

}
