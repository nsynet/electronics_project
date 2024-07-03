package name.liushiyao.emptynester;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import org.apache.http.HttpResponse;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.util.EntityUtils;

/**
 * 实现账号注册
 */
public class RegeditActivity extends Activity {

    private Button regeditButton;
    private EditText newUserCode;
    private EditText newUserPassword1;
    private EditText newUserPassword2;

    private String account = "";
    private String password = "";
    //	private final static String SERVLET_URL = "http://192.168.1.114:8080/WebSocket_1/RegeditServlet";
    private final static String SERVLET_URL = "http://myxtc910839.java.jspee.cn/RegeditServlet";

    private Handler handler = new Handler() {

        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            if (((String) msg.obj).equals("succeed\r\n")) {
                Toast.makeText(RegeditActivity.this, "注册成功", Toast.LENGTH_SHORT).show();
                finish();
            } else {
                Toast.makeText(RegeditActivity.this, "注册失败，请重新注册", Toast.LENGTH_SHORT).show();
            }


        }
    };

    private class Regedit implements Runnable {
        @Override
        public void run() {
            HttpClient httpClient = new DefaultHttpClient();
//            HttpGet httpGet = new HttpGet(SERVLET_URL);
            HttpPost httpPost = new HttpPost(SERVLET_URL + "?account=" + account
                    + "&password=" + password);
            try {
                HttpResponse httpResponse = null;
                httpResponse = httpClient.execute(httpPost);
                if (httpResponse.getStatusLine().getStatusCode() == 200) {
                    String data = EntityUtils.toString(httpResponse.getEntity());
                    Message msg = Message.obtain();
                    msg.obj = data;
                    handler.sendMessage(msg);
                }
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_regedit);
        initView();
        initEvent();
    }

    private void initEvent() {
        regeditButton.setOnClickListener(new OnClickListener() {

            @Override
            public void onClick(View v) {
                if (newUserCode.getText().toString().equals("")) {
                    Toast.makeText(RegeditActivity.this, "用户账号不能为空", Toast.LENGTH_SHORT).show();
                } else if (newUserPassword1.getText().toString().equals("")) {
                    Toast.makeText(RegeditActivity.this, "用户密码不能为空", Toast.LENGTH_SHORT).show();
                } else if (!newUserPassword1.getText().toString().equals(newUserPassword2.getText().toString())) {
                    Toast.makeText(RegeditActivity.this, "两次输入的密码不一致，请重新输入", Toast.LENGTH_SHORT).show();
                } else {
                    account = newUserCode.getText().toString();
                    password = newUserPassword1.getText().toString();
//					Toast.makeText(RegeditActivity.this, "服务器异常，无法完成注册，请联系管理员",Toast.LENGTH_SHORT ).show();
                    new Thread(new Regedit()).start();
                    System.out.println("注册account：" + account + "密码：" + password);
                }
            }
        });
    }

    private void initView() {
        regeditButton = (Button) findViewById(R.id.regedit_button);
        newUserCode = (EditText) findViewById(R.id.new_user_code);
        newUserPassword1 = (EditText) findViewById(R.id.new_user_password1);
        newUserPassword2 = (EditText) findViewById(R.id.new_user_password2);
    }

}
