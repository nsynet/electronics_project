package name.liushiyao.emptynester;

import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import com.karics.library.zxing.android.CaptureActivity;

import org.apache.http.HttpResponse;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.util.EntityUtils;

import name.liushiyao.User;


/**
 * 完成用户绑定的操作（已完成）
 * xx2:
 * 解析码：868024027989566
 * 加密码：11000101010111011010010111110110000100011000111110
 * xx1：
 *
 * 加密码：‭0001100010101011101101001011111011000010001100011111‬
 *
 */

/**
 * 二维码扫描
 *      1. 新用户注册时，进行设备的绑定（目前只支持 xx2） BUG 扫描成功后，没有对服务端的数据进行修改(已解决)
 *      2. 绑定成功后，进行设备的更换（目前只支持xx2，所以没有必要修改服务端的数据）
 */
public class QRCodeActivity extends Activity {


    private static final int REQUEST_CODE_SCAN = 0x0000;

    private static final String DECODED_CONTENT_KEY = "codedContent";
    private static final String DECODED_BITMAP_KEY = "codedBitmap";
    private static final String XX2_CODE = "11000101010111011010010111110110000100011000111110";
    private static final String XX1_CODE = "‭0001100010101011101101001011111011000010001100011111‬";
    private String Device_Name;

    private User user;

    TextView qrCoded;
    ImageView qrCodeImage;
    Button creator, scanner;
    EditText qrCodeUrl;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.device_scan);

        initView();
        /*
        1. 接收来自MainActivity 的User对象（已经进行了设备绑定）
        2. 接收来自FindDeviceActivity 的对象（新用户，。未进行设备绑定）
         */
        Intent intent = getIntent();
        final Bundle bundle = intent.getExtras();
        user  = (User) bundle.getSerializable("user");
        System.out.println("QRCodeActivity 接收的User：user.getDevice="+user.getDevice());

        //自动生成二维码（暂时取消该功能）
        /*creator.setOnClickListener(new OnClickListener() {

            @Override
            public void onClick(View arg0) {

                String url = qrCodeUrl.getText().toString();
                try {
                    Bitmap bitmap = CodeCreator.createQRCode(url);
                    qrCodeImage.setImageBitmap(bitmap);
                } catch (WriterException e) {
                    e.printStackTrace();
                }

            }
        });*/
        qrCodeImage.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(QRCodeActivity.this,
                        CaptureActivity.class);
                startActivityForResult(intent, REQUEST_CODE_SCAN);
            }
        });

        creator.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
               /* Toast.makeText(QRCodeActivity.this, "绑定 "+Device_Name+" 成功", Toast.LENGTH_SHORT).show();
                user.setDevice("xx2");*/
                //启动更新线程
                new Thread(new UpdateDeviceThread()).start();
                //回传
                Intent intent = new Intent() ;
                Bundle bundle = new Bundle();
                bundle.putSerializable("user",user);
                intent.putExtras(bundle);
                intent.setClass(QRCodeActivity.this,MainActivity.class);
//                startActivity(intent);
                setResult(3, intent);
                finish();
            }
        });

        scanner.setOnClickListener(new OnClickListener() {

            @Override
            public void onClick(View arg0) {
                // TODO Auto-generated method stub
                Intent intent = new Intent(QRCodeActivity.this,
                        CaptureActivity.class);
                startActivityForResult(intent, REQUEST_CODE_SCAN);
            }
        });

    }

    private void initView(){
        qrCoded = (TextView) findViewById(R.id.ECoder_title);
        qrCodeImage = (ImageView) findViewById(R.id.ECoder_image);
        qrCodeImage.setImageResource(R.drawable.scaner);
        creator = (Button) findViewById(R.id.ECoder_creator);
        //隐藏添加按钮
        creator.setVisibility(View.GONE);
        scanner = (Button) findViewById(R.id.ECoder_scaning);
        qrCodeUrl = (EditText) findViewById(R.id.ECoder_input);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        // 扫描二维码/条码回传
        if (requestCode == REQUEST_CODE_SCAN && resultCode == RESULT_OK) {
            if (data != null) {

                String content = data.getStringExtra(DECODED_CONTENT_KEY);
                Bitmap bitmap = data.getParcelableExtra(DECODED_BITMAP_KEY);
                /**
                 * 设备解码
                 */
                if(content.equals(XX2_CODE))
                {
                    Device_Name = "小鑫二号（xx2）";
                    qrCoded.setTextColor(Color.RED);
                    qrCoded.setText("设备识别结果：\n"+ Device_Name);
                    System.out.println("QRCodeActivity "+"设备识别结果："+ Device_Name);
                    user.setDevice("xx2");
                    //打开添加按钮
                    qrCodeImage.setImageResource(R.drawable.xx2);
                    creator.setVisibility(View.VISIBLE);
                }
                else if (content.equals(XX1_CODE))
                {
                    Device_Name = "小鑫一号（xx1）";
                    qrCoded.setTextColor(Color.RED);
                    qrCoded.setText("设备识别结果：\n"+ Device_Name);
                    Toast.makeText(QRCodeActivity.this, "app不支持改设备\n请重新扫描", Toast.LENGTH_SHORT).show();
                    //打开添加按钮
                    qrCodeImage.setImageResource(R.drawable.xx1);
                    creator.setVisibility(View.GONE);
                }
                else{
                    qrCoded.setTextColor(Color.BLACK);
                    qrCoded.setText("很抱歉，不存在该机型\n请重新扫描");
                    creator.setVisibility(View.GONE);
                    qrCodeImage.setImageBitmap(bitmap);

                }
//                qrCoded.setText("设备识别结果： \n" + content);
            }
        }
    }

    private Handler handler = new Handler(){
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            String string = (String)msg.obj;
            if (string.equals("device succeed\r\n"))
            {
                Toast.makeText(QRCodeActivity.this, "绑定设备"+user.getDevice()+"成功", Toast.LENGTH_SHORT).show();
            }
            else if (string.equals("device unsucceed\r\n"))
            {
                Toast.makeText(QRCodeActivity.this,  "绑定设备"+user.getDevice()+"失败", Toast.LENGTH_SHORT).show();
            }
            else
            {
                Toast.makeText(QRCodeActivity.this,string, Toast.LENGTH_SHORT).show();
            }

        }
    };

    private static  final String DEVICE_URL = "http://myxtc910839.java.jspee.cn/UpdateDeviceServlet";
    private class UpdateDeviceThread implements Runnable{
        @Override
        public void run() {
            HttpClient httpClient = new DefaultHttpClient();

            HttpGet httpGet = new HttpGet(DEVICE_URL+"?account="+user.getAccount()+"&device="+user.getDevice());
            HttpResponse httpResponse = null;
            try{

                httpResponse = httpClient.execute(httpGet);
                int code =httpResponse.getStatusLine().getStatusCode();
                if (code == 200)
                {
                    System.out.println("QRCodeActivity 连接 UpdateDeviceServlet成功");
                    String data = EntityUtils.toString(httpResponse.getEntity());
                    Message msg = Message.obtain();
                    msg.obj = data;
                    handler.sendMessage(msg);
                    System.out.println("UpdateDeviceThread running......");

                }
                else{
                    System.out.println("UpdateDeviceThread 连接异常"+"错误代码："+code);
                    Message msg = Message.obtain();
                    msg.obj = "连接服务器异常，请检测网络\n错误代码："+code;
                    handler.sendMessage(msg);
                }
            }catch(Exception e){
                e.printStackTrace();
            }
        }
    }


}
