package name.liushiyao.emptynester;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.text.method.ScrollingMovementMethod;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.zihao.radar.view.RadarView;

import org.java_websocket.client.WebSocketClient;
import org.java_websocket.drafts.Draft_17;
import org.java_websocket.handshake.ServerHandshake;

import java.net.URI;

import name.liushiyao.User;

/**
 * BUG ：1.当有设备在断开连接时，不能从雷达中删除点（可以从列表中删除），比较难解决
 */
public class FindDeviceActivity extends Activity {
    RadarView mRadarView;
    //	Button addDeviceButton;
    Button linkDeviceButton;
    private TextView device_list;
    private WebSocketClient client;//连接客户端
    private static final String address = "ws://myxtc910839.java.jspee.cn/websocket?check";
    private boolean flag = false;
    private TextView finddevicetext;

    private Button smartButton;

    private Handler handler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            String device_name = (String) msg.obj;
            device_list.setVisibility(View.VISIBLE);
            System.out.println(mRadarView.getmPointCount());
            if (device_name.contains("@delete"))
            {

                String temp = device_name.replace("@delete","");
                System.out.println("@delete"+temp);
                if (user.getDevice().equals(temp))
                {
                    flag = false;
                    device_list.setText(device_list.getText().toString().replace(temp+"(已绑定设备)\n",""));
//                    System.out.println(device_list.getText().toString().replace(temp+"(已绑定设备)",""));
                }
                else
                {
                    device_list.setText(device_list.getText().toString().
                            replace(temp+"\n",""));
                }
                mRadarView.deletePoint();

            }
            else if (user.getDevice().equals(device_name)) {
                mRadarView.addPoint();
                System.out.println("message running");
                flag = true;
                device_list.setText(device_list.getText() + device_name + "(已绑定设备)" + "\n");
            } else {
                mRadarView.addPoint();
                device_list.setText(device_list.getText() + device_name + "\n");
            }

        }
    };
    private User user;

    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_find_device);
        initView();
        initEvent();
        //开启扫描
        mRadarView.setSearching(true);
        Intent intent = getIntent();
        Bundle bundle = intent.getExtras();
        user = (User) bundle.getSerializable("user");
        System.out.println("FindDevice Activity:" + user.getDevice());
        new Thread(new WebSocketThread()).start();

    }

		


	/*public class FindDeviceThread implements Runnable{
        @Override
		public void run() {

	}*/

    private void initEvent() {


        linkDeviceButton.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                if (flag) {
                    Intent intent = new Intent();
//                    intent.putExtra("device_name", "Xiaoxin No.1");
                    Bundle bundle = new Bundle();
                    bundle.putSerializable("user", user);
                    intent.putExtras(bundle);
                    intent.setClass(FindDeviceActivity.this, MainActivity.class);
                    startActivity(intent);
                    finish();
                    //关闭连接
                    client.close();

                } else {
                    Toast.makeText(FindDeviceActivity.this, "未检测到在线的已绑定设备，\n请将设备进行联网操作", Toast.LENGTH_SHORT).show();
                }


            }
        });
        //跳过连接设备
        finddevicetext.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent();
                intent.putExtra("device_name", "Xiaoxin No.1");
                Bundle bundle = new Bundle();
                bundle.putSerializable("user", user);
                intent.putExtras(bundle);
                intent.setClass(FindDeviceActivity.this, MainActivity.class);
                startActivity(intent);
                finish();
                //关闭连接
                client.close();
            }
        });

        smartButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent();
                intent.setClass(FindDeviceActivity.this, WifiActivity.class);
                startActivity(intent);
            }
        });
    }

    private void initView() {
        mRadarView = (RadarView) findViewById(R.id.radar_view);

        FindDeviceActivity.this.setFinishOnTouchOutside(false);
//		addDeviceButton = (Button) findViewById(R.id.add_device_button);
        linkDeviceButton = (Button) findViewById(R.id.link_device_button);
        device_list = (TextView) findViewById(R.id.devcie_list);
        device_list.setMovementMethod(ScrollingMovementMethod.getInstance());
        finddevicetext = (TextView) findViewById(R.id.finddevicetext);
        smartButton = (Button) findViewById(R.id.smart_button);

    }

    private class WebSocketThread implements Runnable {
        @Override
        public void run() {
            try {
                System.out.println("check connect WebSocket");
                Log.e("wsDbug", "连接地址：" + address);
                client = new WebSocketClient(new URI(address), new Draft_17()) {
                    @Override
                    public void onClose(final int code, String arg1, boolean arg2) {
                        System.out.println("与服务器[" + getURI() + "]断开连接" + "]");
                        System.out.println("状态吗[" + code + "]\n");
                        Log.e("wsdDbug", "与服务器[" + getURI() + "]断开连接" + "]");
                        Log.e("wsDbug", "状态吗[" + code + "]\n");
                    }

                    @Override
                    public void onError(Exception e) {
                        System.out.println("连接[" + getURI() + "]服务器发生异常" + "]\n");
                        System.out.println("异常原因：[" + e + "]\n");
                    }

                    @Override
                    public void onMessage(final String message) {

                        System.out.println("获取服务器的信息[" + message + "]\n");
                        Log.e("wsDbug", "获取服务器的信息[" + message + "]\n");
                        Message msg = Message.obtain();
                        msg.obj = message;
                        handler.sendMessage(msg);
                    }

                    @Override
                    public void onOpen(ServerHandshake arg0) {
                        System.out.println("成功连接到服务器[" + getURI() + "]\n");
                        Log.e("webDbug", "成功连接到服务器[" + getURI() + "]\n");
                    }
                };

                client.connect();
//			client.connectBlocking();//堵塞式
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

}
