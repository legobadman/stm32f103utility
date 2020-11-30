package com.rfstar.kevin.main;

import java.util.ArrayList;

import com.rfstar.kevin.adapter.EventsAdapter;
import com.rfstar.kevin.app.App;
import com.rfstar.kevin.main.R;
import com.rfstar.kevin.params.BLEDevice.RFStarBLEBroadcastReceiver;
import com.rfstar.kevin.params.EventItem;
import com.rfstar.kevin.params.PortItem;
import com.rfstar.kevin.service.RFStarBLEService;

import android.content.Context;
import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ListView;
import android.widget.AdapterView.OnItemClickListener;

public class PortTimingEventsConfigEventsActivity extends BaseActivity implements OnItemClickListener,
        RFStarBLEBroadcastReceiver {

    private final static String TAG = "lee";

    private ArrayList<EventItem> arraySource = null;
    private ListView list = null;
    private EventsAdapter adapter = null;

    private int portNum = -1;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        // TODO Auto-generated method stub
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_event_index);
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
        PortItem port = (PortItem) intent.getSerializableExtra(App.TAG);
        portNum = intent.getIntExtra("port_bumber", 0);

        Log.d(TAG, "==lee initView select port ==" + portNum);

        this.initNavigation(port.port);

        initArraySource();
        adapter = new EventsAdapter(this, arraySource);

        this.list = (ListView) this.findViewById(R.id.list_event_index);
        this.list.setCacheColorHint(Color.TRANSPARENT);
        this.list.setDivider(null);
        this.list.setAdapter(adapter);
        this.list.setOnItemClickListener(this);

        this.list.setSelector(R.color.clear);
    }

    private void initArraySource() {
        // TODO Auto-generated method stub
        this.arraySource = new ArrayList<EventItem>();

        EventItem item0 = new EventItem();
        item0.eventIndex = "Event index 0";
        EventItem item1 = new EventItem();
        item1.eventIndex = "Event index 1";
        EventItem item2 = new EventItem();
        item2.eventIndex = "Event index 2";
        EventItem item3 = new EventItem();
        item3.eventIndex = "Event index 3";
        EventItem item4 = new EventItem();
        item4.eventIndex = "Event index 4";
        EventItem item5 = new EventItem();
        item5.eventIndex = "Event index 5";

        EventItem item6 = new EventItem();
        item6.eventIndex = "Event index 6";
        EventItem item7 = new EventItem();
        item7.eventIndex = "Event index 7";
        EventItem item8 = new EventItem();
        item8.eventIndex = "Event index 8";
        EventItem item9 = new EventItem();
        item9.eventIndex = "Event index 9";

        EventItem item10 = new EventItem();
        item10.eventIndex = "Event index 10";
        EventItem item11 = new EventItem();
        item11.eventIndex = "Event index 11";
        EventItem item12 = new EventItem();
        item12.eventIndex = "Event index 12";
        EventItem item13 = new EventItem();
        item13.eventIndex = "Event index 13";
        EventItem item14 = new EventItem();
        item14.eventIndex = "Event index 14";
        EventItem item15 = new EventItem();
        item15.eventIndex = "Event index 15";

        EventItem item16 = new EventItem();
        item16.eventIndex = "Event index 16";
        EventItem item17 = new EventItem();
        item17.eventIndex = "Event index 17";
        EventItem item18 = new EventItem();
        item18.eventIndex = "Event index 18";
        EventItem item19 = new EventItem();
        item19.eventIndex = "Event index 19";

        EventItem item20 = new EventItem();
        item20.eventIndex = "Event index 20";
        EventItem item21 = new EventItem();
        item21.eventIndex = "Event index 21";
        EventItem item22 = new EventItem();
        item22.eventIndex = "Event index 22";
        EventItem item23 = new EventItem();
        item23.eventIndex = "Event index 23";
        EventItem item24 = new EventItem();
        item24.eventIndex = "Event index 24";
        EventItem item25 = new EventItem();
        item25.eventIndex = "Event index 25";

        EventItem item26 = new EventItem();
        item26.eventIndex = "Event index 26";
        EventItem item27 = new EventItem();
        item27.eventIndex = "Event index 27";
        EventItem item28 = new EventItem();
        item28.eventIndex = "Event index 28";
        EventItem item29 = new EventItem();
        item29.eventIndex = "Event index 29";

        EventItem item30 = new EventItem();
        item30.eventIndex = "Event index 30";
        EventItem item31 = new EventItem();
        item31.eventIndex = "Event index 31";

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
        this.arraySource.add(item15);
        this.arraySource.add(item16);
        this.arraySource.add(item17);
        this.arraySource.add(item18);
        this.arraySource.add(item19);
        this.arraySource.add(item20);
        this.arraySource.add(item21);
        this.arraySource.add(item22);
        this.arraySource.add(item23);
        this.arraySource.add(item24);
        this.arraySource.add(item25);
        this.arraySource.add(item26);
        this.arraySource.add(item27);
        this.arraySource.add(item28);
        this.arraySource.add(item29);
        this.arraySource.add(item30);
        this.arraySource.add(item31);
    }

    public void onItemClick(AdapterView<?> arg0, View arg1, int arg2, long arg3) {

        Log.d(TAG, "==lee onItemClick  portNum/eventIndex==" + portNum + "\\" + arg2);
        Intent intent = null;

        if ((arg2 >= 0) && (arg2 <= 31)) {
            intent = new Intent(this, PortTimingEventsConfigActionActivity.class);
        }

        if (intent != null) {
            intent.putExtra("port_bumber", portNum);
            intent.putExtra("event_index", arg2);
            startActivity(intent);
        }

    }

    @Override
    public void onReceive(Context context, Intent intent, String macData, String uuid) {
    }

}
