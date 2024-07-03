package com.hiflying.smartlink3.demo;

import android.os.Bundle;
import android.support.v4.app.FragmentActivity;

import com.hiflying.smartlink.v3.SnifferSmartLinkerFragment;

import name.liushiyao.emptynester.R;

public class SnifferSmartLinkerFragmentActivity extends FragmentActivity {

	@Override
	protected void onCreate(Bundle arg0) {
		// TODO Auto-generated method stub
		super.onCreate(arg0);
		
		setContentView(R.layout.activity_sniffer_smart_linker_fragment);
		getSupportFragmentManager().beginTransaction().add(R.id.frameLayout_01, new SnifferSmartLinkerFragment(), null).commit();
	}
}
