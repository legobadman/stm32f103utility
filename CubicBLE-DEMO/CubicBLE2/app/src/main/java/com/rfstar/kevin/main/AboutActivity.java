package com.rfstar.kevin.main;

import java.util.ArrayList;

import com.rfstar.kevin.adapter.SetAdapter;
import com.rfstar.kevin.main.R;
import com.rfstar.kevin.params.SetItem;
import com.rfstar.kevin.tools.Tools;

import android.content.Intent;
import android.graphics.Color;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.net.Uri;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.LinearLayout.LayoutParams;
import android.widget.ListView;

/**
 * 
 * @author Kevin.wu E-mail:wh19575782@163.com
 * 
 */
public class AboutActivity extends BaseActivity implements OnItemClickListener {
	private ArrayList<SetItem> arraySource = null;
	private ListView list = null;
	private SetAdapter adapter = null;

	@SuppressWarnings("static-access")
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_about);
		initView();
		this.initNavigation(getString(R.string.About_About));
		navigateView.setRightHideBtn(true);
	}

	@SuppressWarnings({ "static-access", "unused" })
	private void initView() {

		initArraySource();
		adapter = new SetAdapter(this, arraySource);

		this.list = (ListView) this.findViewById(R.id.list);
		this.list.setCacheColorHint(Color.TRANSPARENT);
		this.list.setDividerHeight(1);
		this.list.addHeaderView(this.headerView());
		this.list.setAdapter(adapter);
		this.list.setOnItemClickListener(this);

		this.list.setSelector(R.color.clear);

	}

	/**
	 * 表头
	 * 
	 * @return
	 */
	@SuppressWarnings("static-access")
	private View headerView() {
		LayoutInflater factory = LayoutInflater.from(this);
		View view = factory.inflate(R.layout.about_list_header_item, null);
		TextView textView = (TextView) view.findViewById(R.id.versionTxt);
		textView.setText(Tools.getVersion(this));
		ImageView logoImg = (ImageView) view.findViewById(R.id.logo);
		DisplayMetrics display = Tools.getDisplayMetrics(this);

		logoImg.setImageResource(R.drawable.rfstar_logo);
		return view;
	}

	/**
	 * 初始化source
	 */
	private void initArraySource() {
		arraySource = new ArrayList<SetItem>();
		SetItem item1 = new SetItem();
		SetItem item2 = new SetItem();

		item1.name = getString(R.string.user_feedback);
		item2.name = getString(R.string.about_us);// Tools.getVersion(this);
		arraySource.add(item1);
		arraySource.add(item2);
	}

	@Override
	public void onItemClick(AdapterView<?> arg0, View arg1, int arg2, long arg3) {
		// TODO Auto-generated method stub
		switch (arg2) {
		case 1:

			break;
		case 2:
			Intent intent = new Intent(Intent.ACTION_VIEW,
					Uri.parse("http://www.szrfstar.com"));
			startActivity(intent);
			break;
		default:
			break;
		}
	}
}
