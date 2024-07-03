package com.example.smartcontrol;

import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.Socket;
import java.nio.charset.Charset;










import com.example.smartcontrol_view.AddAndSubView;
import com.example.smartcontrol_view.AddAndSubView.OnNumChangeListener;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.CompoundButton.OnCheckedChangeListener;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.ToggleButton;

//���ý��棬ʵ�����ã��������úõĸ����������͵�MainActivity�����У�
//��MainActivity�ķ��Ͱ�ť������Ϣ

public class SettingsActivity extends Activity implements OnClickListener,OnCheckedChangeListener{
	
	private String final_manual="";
	private String final_auto = "";
	//10��Toggle
	private int m_1;
	private int m_2;
	private int m_3;
	private int m_4;
	private int m_5;
	
	private int a_1;
	private int a_2;
	private int a_3;
	private int a_4;
	private int a_5;
	
	
	AddAndSubView wenduAdd;
	AddAndSubView yuzhiAdd;
	AddAndSubView lightAdd;
	AddAndSubView senwenAdd;
	 AddAndSubView rongchaAdd;
	
	
	//�˿ں�IP
	private String ip;
	private String port;
	//�ؼ�
	private LinearLayout wenduSetting ;
	private LinearLayout yuzhiSetting;
	private LinearLayout lightSetting;
	private LinearLayout sewenSetting;
	private LinearLayout rongchaSetting;
	
	private ToggleButton manual_1;
	private ToggleButton manual_2;
	private ToggleButton manual_3;
	private ToggleButton manual_4;
	private ToggleButton manual_5;
	
	private ToggleButton auto_1;
	private ToggleButton auto_2;
	private ToggleButton auto_3;
	private ToggleButton auto_4;
	private ToggleButton auto_5;
	
	
	
	
	//��ť
	private Button back_Btn;
	private Button save_Btn;
	
	//�Ƿ�����
	private boolean isConnected = false;
	private Socket mSocket = null;
		
	//��ӡ������Ͷ�����
	private PrintWriter printWriter; 
	
	
	private TextView serverState;
	
	private Handler mHandler = new Handler(){
		
		@Override
		public void handleMessage(Message msg) {
			
			switch (msg.what) {
			
			case 2:

				serverState.setText("������������");
				isConnected = true;
				break;
			}
			
		};
	};

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setContentView(R.layout.settings);
		initsViews();

		//�����¼�
		back_Btn.setOnClickListener(this);
		save_Btn.setOnClickListener(this);
		manual_1.setOnCheckedChangeListener(this);
		manual_2.setOnCheckedChangeListener(this);
		manual_3.setOnCheckedChangeListener(this);
		manual_4.setOnCheckedChangeListener(this);
		manual_5.setOnCheckedChangeListener(this);
		
		auto_1.setOnCheckedChangeListener(this);
		auto_2.setOnCheckedChangeListener(this);
		auto_3.setOnCheckedChangeListener(this);
		auto_4.setOnCheckedChangeListener(this);
		auto_5.setOnCheckedChangeListener(this);
		
		
		
		
		
		//����������һ���������
//		Intent intent = getIntent();
		Bundle bundle = this.getIntent().getExtras();
		ip = bundle.getString("ip");
		port = bundle.getString("port");
		
		connectThread();
		
		
		
		
		
		
		
		
			
	}
	
	//��ʼ���ؼ�
	public void initsViews(){
		
		//��ʼ���ؼ�
		wenduSetting = (LinearLayout)findViewById(R.id.wendu_linearLayout);
		yuzhiSetting = (LinearLayout)findViewById(R.id.wenduyuzhi_linearLayout);
		lightSetting = (LinearLayout)findViewById(R.id.light_linearLayout);
		sewenSetting = (LinearLayout)findViewById(R.id.sewen_linearLayout);
		rongchaSetting = (LinearLayout)findViewById(R.id.rongcha_linearLayout);
		
		
		 wenduAdd = new AddAndSubView(this, 24);
		 yuzhiAdd = new AddAndSubView(this, 25);
		 lightAdd = new AddAndSubView(this, 2);
		 senwenAdd = new AddAndSubView(this, 2);
		 rongchaAdd = new AddAndSubView(this,5);
		
		wenduAdd.setOnNumChangeListener(new OnNumChangeListener() {
			
			@Override
			public void onNumChange(View view, int num) {
				// TODO Auto-generated method stub
				if(num<14 || num >32){
					Toast.makeText(getApplicationContext(), "�����뷶ΧΪ14-32����ֵ", Toast.LENGTH_SHORT).show();
				}
			}
		});
		yuzhiAdd.setOnNumChangeListener(new OnNumChangeListener() {
			
			@Override
			public void onNumChange(View view, int num) {
				// TODO Auto-generated method stub
				if(num<20 || num >45){
					Toast.makeText(getApplicationContext(), "�����뷶ΧΪ20-45����ֵ", Toast.LENGTH_SHORT).show();
				}
			}
		});
		lightAdd.setOnNumChangeListener(new OnNumChangeListener() {
			
			@Override
			public void onNumChange(View view, int num) {
				// TODO Auto-generated method stub
				if(num > 9){
					Toast.makeText(getApplicationContext(), "�����뷶ΧΪ0-9����ֵ", Toast.LENGTH_SHORT).show();
				}
			}
		});
		senwenAdd.setOnNumChangeListener(new OnNumChangeListener() {
			
			@Override
			public void onNumChange(View view, int num) {
				// TODO Auto-generated method stub
				if(num >5){
					Toast.makeText(getApplicationContext(), "�����뷶ΧΪ0-5����ֵ", Toast.LENGTH_SHORT).show();
				}
			}
		});
		rongchaAdd.setOnNumChangeListener(new OnNumChangeListener() {
			
			@Override
			public void onNumChange(View view, int num) {
				// TODO Auto-generated method stub
				if(num >10){
					Toast.makeText(getApplicationContext(), "�����뷶ΧΪ0-10����ֵ", Toast.LENGTH_SHORT).show();
				}
			}
		});
		
//		Log.e("�¶�", wenduAdd.getNum()+"");getNum()�ɻ�ȡֵ
		
		wenduSetting.addView(wenduAdd);
		yuzhiSetting.addView(yuzhiAdd);
		lightSetting.addView(lightAdd);
		sewenSetting.addView(senwenAdd);
		rongchaSetting.addView(rongchaAdd);
		
		
		//��ť
		back_Btn = (Button)findViewById(R.id.settings_back_btn);
	
		save_Btn = (Button)findViewById(R.id.save_settings_btn);
		
		//textView
		
		serverState = (TextView)findViewById(R.id.setting_server_text);
		
		//toggleButton
		manual_1 = (ToggleButton)findViewById(R.id.manual_air_switch_btn_setting);
		manual_2 = (ToggleButton)findViewById(R.id.manual_clean_switch_btn_setting);
		manual_3 = (ToggleButton)findViewById(R.id.manual_exh_switch_btn_setting);
		manual_4 = (ToggleButton)findViewById(R.id.manual_window_switch_btn_setting);
		manual_5 = (ToggleButton)findViewById(R.id.manual_liht_switch_btn_setting);
		
		auto_1 = (ToggleButton)findViewById(R.id.auto_air_switch_btn_setting);
		auto_2 = (ToggleButton)findViewById(R.id.auto_clean_switch_btn_setting);
		auto_3 = (ToggleButton)findViewById(R.id.auto_exh_switch_btn_setting);
		auto_4 = (ToggleButton)findViewById(R.id.auto_window_switch_btn_setting);
		auto_5 = (ToggleButton)findViewById(R.id.auto_light_switch_btn_setting);
		
	}

	//����
	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
		switch (v.getId()) {
		case R.id.settings_back_btn:
			//���ý���
			Intent intent = new Intent(SettingsActivity.this,MainActivity.class);
			
			startActivity(intent);
			try {
				mSocket.close();
				
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			this.finish();
			break;
			

			
		case R.id.save_settings_btn:
			//�������úͷ���
			sendData();
			break;
		}
	}

	//����
	private void sendData() {
		// TODO Auto-generated method stub
		try {
			
			final_manual = ""+m_1+m_2+m_3+m_4+m_5+"000";
			
			final_auto = ""+a_1+a_2+a_3+a_4+a_5+"000";
			
			String context = "{"
					
			+wenduAdd.getNum()+","
					+yuzhiAdd.getNum()+","
			+rongchaAdd.getNum()+","
					+lightAdd.getNum()+","
			+senwenAdd.getNum()+","
					+Integer.valueOf(final_manual,2)+","
			+Integer.valueOf(final_auto,2)+"}";
			
			Log.e("context", context);
			
			if (!context.equals("")) {

				if (printWriter == null) {
					showInfo("����ʧ��!");
					return;
				}
				
				printWriter.print(context);
				printWriter.flush();
				
			}else{
				showInfo("���͵����ݲ���Ϊ��");
			}

			
			
		} catch (Exception e) {
			// TODO: handle exception
			Log.e("����ʧ��", e.getMessage());
		}
	}

	//����
	private void connectThread() {
		// TODO Auto-generated method stub
		if(!isConnected){
			//û���ӾͿ���һ�������߳�
			
			new Thread(new Runnable() {
				
				@Override
				public void run() {
					// TODO Auto-generated method stub
					Looper.prepare();
					connectServer(ip,port);	
				}
			}).start();
			
			
		}else{
			//�ٴμ����ɶϿ�
			if(mSocket!=null){
				try {
					mSocket.close();
					mSocket =null;
					Log.e("mSocket", "�Ͽ�����");
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				
			}
			

			isConnected = false;
			
			
			
		}
	}
	
	//���ӵ�������
		protected void connectServer(String ip, String port) {
			// TODO Auto-generated method stub
			try {
				
				mSocket = new Socket(ip,Integer.parseInt(port));
				
				OutputStream outputStream = mSocket.getOutputStream();
				
				//��ȡ��������ô�ӡ����Ч
				printWriter = new PrintWriter(new BufferedWriter(
						new OutputStreamWriter(outputStream,
								Charset.forName("gb2312"))));
				
				mHandler.sendEmptyMessage(2);
				
				
			} catch (Exception e) {
				// TODO Auto-generated catch block
				Toast.makeText(getApplicationContext(), "���ӷ�����ʧ��", Toast.LENGTH_SHORT).show();
				isConnected = false;	
			}
			
			
		}
		
		
		private void showInfo(String msg) {
			Toast.makeText(SettingsActivity.this, msg, Toast.LENGTH_SHORT).show();

		}

		
		//toggleButton��������
		@Override
		public void onCheckedChanged(CompoundButton buttonView,
				boolean isChecked) {
			// TODO Auto-generated method stub
			switch (buttonView.getId()) {
			case R.id.manual_air_switch_btn_setting:
				if(isChecked){
					m_1 = 1;
					Log.e("m_1", m_1+"");
				}else{
					m_1 = 0;
					Log.e("m_1", m_1+"");
				}
				break;
			case R.id.manual_clean_switch_btn_setting:
				if(isChecked){
					m_2 = 1;
				}else{
					m_2 = 0;
				}
				break;
			case R.id.manual_exh_switch_btn_setting:
				if(isChecked){
					m_3 = 1;
				
				}else{
					m_3 = 0;
				}
				break;
			case R.id.manual_liht_switch_btn_setting:
				if(isChecked){
					m_4 = 1;
					
				}else{
					m_4 = 0;
				}
				break;
			case R.id.manual_window_switch_btn_setting:
				if(isChecked){
					m_5 = 1;
					
				}else{
					m_5 = 0;
				}
				
				break;
			case R.id.auto_air_switch_btn_setting:
				if(isChecked){
					a_1 = 1;
					
				}else{
					a_1 = 0;
				}
				
				break;
			case R.id.auto_clean_switch_btn_setting:
				
				if(isChecked){
					a_2 = 1;
					
				}else{
					a_2 = 0;
				}
				
				break;
			case R.id.auto_exh_switch_btn_setting:
				if(isChecked){
					a_3 = 1;
					
				}else{
					a_3 = 0;
				}
				
				break;
			case R.id.auto_light_switch_btn_setting:
				if(isChecked){
					a_4 = 1;
					
				}else{
					a_4 = 0;
				}
				break;
			case R.id.auto_window_switch_btn_setting:
				if(isChecked){
					a_5 = 1;
					
				}else{
					a_5 = 0;
				}
				break;

			
			}
			
		}

		
	
	
}
