package com.example.smartcontrol;




import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.io.PrintStream;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.UnknownHostException;
import java.nio.charset.Charset;







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
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

//������ʵ�����ӣ����ݼ�صȹ���

public class MainActivity extends Activity implements OnClickListener{
	
	private Button settingsBtn ;
	
	//������ʾ�ؼ�
	private TextView p1;
	private TextView p2;
	private TextView p3;
	private TextView p4;
	private TextView p5;
	private TextView p6;
	private TextView p7;
	private TextView p8;
	private TextView p9;
	private TextView p10;
	private TextView p11;
	private TextView p12;
	private TextView p13;
	private TextView p14;
	private TextView p15;
	private TextView p16;
	
	
	
	private ImageView manual_1;
	private ImageView manual_2;
	private ImageView manual_3;
	private ImageView manual_4;
	private ImageView manual_5;
	
	
	private ImageView auto_1;
	private ImageView auto_2;
	private ImageView auto_3;
	private ImageView auto_4;
	private ImageView auto_5;
	
	
	//�ڶ��˿ں�IP
	private String  ip = "119.29.201.135";
	private String  port ="8001";
	
	//�Ƿ�����
	private boolean isConnected = false;
	private Socket mSocket = null;
	
	//��ӡ������Ͷ�����
	private InputStream in;
	private PrintWriter printWriter; 
	
	//�����߳�
	private Thread reciverThread;
	

	private TextView serverState;
	
	//UI�̴߳���
	private Handler mHandler = new Handler(){
		
		@Override
		public void handleMessage(Message msg) {
			
			switch (msg.what) {
			
			case 1:
				//���մ���strΪ���յ�������ʾ
				String str = (String) msg.obj;
				Log.e("str", str);
				//��������
				if(str.contains("{")){
					phrasestr(str);
				}
				
				break;
				
			case 2:
				//�������������߳�
				reciverThread = new Thread(new MyReceiverRunnale());
				reciverThread.start();
			
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
		setContentView(R.layout.main_activity);
		initsViews();
		
		connectThread();
		settingsBtn.setOnClickListener(this);
	
	}
	
	//����
	protected void phrasestr(String str) {
		// TODO Auto-generated method stub
		

		String st = str.substring(1,str.length()-1);
		Log.e("st", st);
		
		
		String[] s = st.split(",");
	
//		//������1������
		p1.setText(s[0]);
		//������2������
		p2.setText(s[1]);
		//������3������
		p3.setText(s[2]);
		//������4������
		p4.setText(s[3]);
		//������5������
		p5.setText(s[4]);
		//������6������
		p6.setText(s[5]);
		
		//������7������
		p7phrase(s[6]);
		//������8������
		p8phrase(s[7]);
		//������9������
		p9phrase(s[8]);
		//������10������
		p10phrase(s[9]);
		//������11������
		p11phrase(s[10]);
		//������12������
		p12phrase(s[11]);
		//������13������
		p8.setText(s[12]);
		//������14������
		p16.setText(s[13]);
				
	}
	
	
	//���ڲ�ȫ8λ
	private String getEightBit(String s){
		
		String st ="";
		int length =8-s.length();
		for(int i=0;i<length;i++){
			st+="0";
		}
		
		s = st+s;
		
		return s;
		
	}
	
	private void p11phrase(String s){
		char[] ch = s.toCharArray();
		if(ch[0]=='1'){
			p15.setText("��");
		}else{
			p15.setText("��");
		}
	}
	
	
	private void p10phrase(String s){
		char[] ch = s.toCharArray();
		if(ch[0]=='1'){
			p14.setText("��");
		}else{
			p14.setText("��");
		}
	}
	
	
	private void p12phrase(String s) {
		// TODO Auto-generated method stub
		String st1 = Integer.toBinaryString(Integer.valueOf(s));
		
		st1 = getEightBit(st1);
		Log.e("12", st1);
		
		char[] st1Char = st1.toCharArray();
		
		if(st1Char[0]=='1'){
			p12.setText("��");
			
		}else{
			p12.setText("��");
		}
		
		String st2 = st1.substring(1, 4);
		Log.e("12", st2);
		p10.setText(Integer.valueOf(st2,2)+"");
		String st3 = st1.substring(4);
		Log.e("12", st3);
		p9.setText(Integer.valueOf(st3, 2)+"");
		
	}

	public void p7phrase(String s){
		
		String st1 = Integer.toBinaryString(Integer.valueOf(s));
		
		
		st1 = getEightBit(st1);
		Log.e("7", st1);
		
		char[] st1Char = st1.toCharArray();
		
		//�յ�
		if(st1Char[0]=='1'){
			auto_1.setImageResource(R.drawable.on1);
		}else{
			auto_1.setImageResource(R.drawable.off1);
		}
		
		if(st1Char[1]=='1'){
			auto_2.setImageResource(R.drawable.on1);
		}else{
			auto_2.setImageResource(R.drawable.off1);
		}
		
		if(st1Char[2]=='1'){
			auto_3.setImageResource(R.drawable.on1);
		}else{
			auto_3.setImageResource(R.drawable.off1);
		}
		
		if(st1Char[3]=='1'){
			auto_4.setImageResource(R.drawable.on1);
		}else{
			auto_4.setImageResource(R.drawable.off1);
		}
		
		if(st1Char[4]=='1'){
			auto_5.setImageResource(R.drawable.on1);
		}else{
			auto_5.setImageResource(R.drawable.off1);
		}
		
		if(st1Char[5]=='1'){
			p11.setText("��");
			
		}else{
			p11.setText("��");
		}
	}
	
	
	public void p8phrase(String s){
		
		String st1 = Integer.toBinaryString(Integer.valueOf(s));
		
		st1 = getEightBit(st1);
		
		Log.e("8", st1);
		
		char[] st1Char = st1.toCharArray();
		
		//�յ�
		if(st1Char[0]=='1'){
			manual_1.setImageResource(R.drawable.on1);
		}else{
			manual_1.setImageResource(R.drawable.off1);
		}
		
		if(st1Char[1]=='1'){
			manual_2.setImageResource(R.drawable.on1);
		}else{
			manual_2.setImageResource(R.drawable.off1);
		}
		
		if(st1Char[2]=='1'){
			manual_3.setImageResource(R.drawable.on1);
		}else{
			manual_3.setImageResource(R.drawable.off1);
		}
		
		if(st1Char[3]=='1'){
			manual_4.setImageResource(R.drawable.on1);
		}else{
			manual_4.setImageResource(R.drawable.off1);
		}
		
		if(st1Char[4]=='1'){
			manual_5.setImageResource(R.drawable.on1);
		}else{
			manual_5.setImageResource(R.drawable.off1);
		}
		
	}
	
	
	public void p9phrase(String s){
		String st1 = Integer.toBinaryString(Integer.valueOf(s));
		
		st1 = getEightBit(st1);
		
		char[] ch = st1.toCharArray();
		
		Log.e("9", st1);
		if(ch[0]=='1'){
			p13.setText("��");
		}else{
			p13.setText("��");
		}
		String st2 = st1.substring(1);
		Log.e("9", st2);
		p7.setText(Integer.valueOf(st2, 2)+"");
	}
	
	
	public void initsViews(){
		
		settingsBtn = (Button)findViewById(R.id.id_settings_btn);	
		serverState = (TextView)findViewById(R.id.id_servers_text);
		p1= (TextView)findViewById(R.id.id_pm2_5_text);
		p2= (TextView)findViewById(R.id.id_yanwu_text);
		p3= (TextView)findViewById(R.id.id_keranqiti_text);
		p4= (TextView)findViewById(R.id.id_pa_text);
		p5= (TextView)findViewById(R.id.id_shiwen_text);
		p6= (TextView)findViewById(R.id.id_shiwaiwen_text);
		p7= (TextView)findViewById(R.id.id_airwendu_text);
		//���˸��ݲ�
		p16 = (TextView)findViewById(R.id.id_rongcha_text);
		
		p8= (TextView)findViewById(R.id.id_yuzhi_text);
		p9= (TextView)findViewById(R.id.id_lamp_light_text);
		p10= (TextView)findViewById(R.id.id_lampsewen_text);
		p11= (TextView)findViewById(R.id.id_window_text);
		p12= (TextView)findViewById(R.id.id_light_text);
		p13= (TextView)findViewById(R.id.id_air_text);
		p14= (TextView)findViewById(R.id.id_clean_text);
		p15= (TextView)findViewById(R.id.id_exh_text);
		
		
		
		
		manual_1 = (ImageView)findViewById(R.id.manual_air_state);
		manual_2 = (ImageView)findViewById(R.id.manual_clean_state);
		manual_3 = (ImageView)findViewById(R.id.manual_exh_state);
		manual_4 = (ImageView)findViewById(R.id.manual_window_state);
		manual_5 = (ImageView)findViewById(R.id.manual_light_state);
		
		
		auto_1 = (ImageView)findViewById(R.id.auto_air_state);
		auto_2 = (ImageView)findViewById(R.id.auto_clean_state);
		auto_3 = (ImageView)findViewById(R.id.auto_exh_state);
		auto_4 = (ImageView)findViewById(R.id.auto_window_state);
		auto_5 = (ImageView)findViewById(R.id.auto_light_state);
			
	}
	
	
	


	//��ť����
	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
		switch (v.getId()) {
		case R.id.id_settings_btn:
			
			if(isConnected){
				
				Intent intent = new Intent(MainActivity.this,SettingsActivity.class);
				Bundle bundle = new Bundle();
				bundle.putString("ip", ip);
				bundle.putString("port", port);
				intent.putExtras(bundle);
				startActivity(intent);
				this.finish();
				
				try {
					mSocket.close();
					reciverThread.interrupt();

					serverState.setText("δ���ӷ�����");
					
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}else{
				Toast.makeText(this, "�������ӷ�����", Toast.LENGTH_SHORT).show();
			}
			//���ý���
			
			break;
		}
	}
	
	
	//��������
	private void sendData() {
		// TODO Auto-generated method stub
		
		try {
			
			String context = "hello world";
			
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


	//�����߳�
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
			

			serverState.setText("δ���ӷ�����");
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
			//listener.setOutStream(printWriter);�����Ƿ�Ҫ�ӳ�
			
			//��ȡ������
			in = mSocket.getInputStream();
			//���͵��̴߳����򿪽��������߳�
			mHandler.sendEmptyMessage(2);
			showInfo("���ӳɹ�");	
			
		} catch (Exception e) {
			// TODO Auto-generated catch block
			
			isConnected = false;
			Toast.makeText(getApplicationContext(), "���ӷ�����ʧ��", Toast.LENGTH_SHORT).show();
			
			
			
		}
		
		
	}
	
	private class MyReceiverRunnale implements Runnable {

		public void run() {

			while (true) {
				
				if (isConnected) {
					if (mSocket != null && mSocket.isConnected()) {

						String result = "";
						//��ȡ����
						int count = 0;
						byte[] inDatas = null;
						try {
							while (count == 0) {
								count = in.available();
							}
							inDatas = new byte[count];
							in.read(inDatas);
							//���������뷽ʽΪgb2312,������������뷽ʽ����
							result =  new String(inDatas, "gb2312");
							
							
							//����UI�̴߳���
							if (!result.equals("")) {

								Message msg = new Message();
								msg.what = 1;
								msg.obj = result;
								mHandler.sendMessage(msg);
							}

						} catch (Exception e) {
							Log.e("e", e.getMessage());
							e.printStackTrace();
						}
					}
				}
				//�ӳٽ���
				try {
					Thread.sleep(100L);
				} catch (InterruptedException e) {
					e.printStackTrace();
				}

			}
		}
	}


	private void showInfo(String msg) {
		Toast.makeText(MainActivity.this, msg, Toast.LENGTH_SHORT).show();

	}

}
