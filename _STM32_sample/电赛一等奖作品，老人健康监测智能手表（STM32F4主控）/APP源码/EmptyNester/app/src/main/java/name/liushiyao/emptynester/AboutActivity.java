package name.liushiyao.emptynester;

import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.Toast;

import org.apache.http.HttpResponse;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.util.EntityUtils;

import name.liushiyao.User;

public class AboutActivity extends AppCompatActivity {

    private User user;
    private EditText account;
    private EditText username;
    private EditText age;
    private EditText sex;
    private EditText history;
    private EditText attention;

    private ImageButton Saving;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_about);
        account = (EditText) findViewById(R.id.account_editText);
        username = (EditText) findViewById(R.id.username_editText);
        age = (EditText) findViewById(R.id.age_edit);
        sex = (EditText) findViewById(R.id.sex_edit);
        history = (EditText) findViewById(R.id.history_edit);
        attention = (EditText) findViewById(R.id.attention_editText);
        account.setEnabled(false);
        Saving = (ImageButton) findViewById(R.id.imageButton_info);
        Saving.setBackgroundColor(200);

        Intent intent = getIntent();
        Bundle bundle = intent.getExtras();
        user = (User) bundle.getSerializable("user");
        account.setText(user.getAccount());
        username.setText(user.getName());
        age.setText(user.getAge());
        sex.setText(user.getSex());
        history.setText(user.getMedical_history());
        attention.setText(user.getAttention());

//        showAbout.setText(user.toString());
        System.out.println("About Activity == >>"+user.toString());
        Saving.setOnClickListener(new View.OnClickListener() {
                                      @Override
                                      public void onClick(View v) {
                                            user.setName(username.getText().toString());
                                            user.setAge(age.getText().toString());
                                            user.setSex(sex.getText().toString());
                                          user.setMedical_history(history.getText().toString());
                                          user.setAttention(attention.getText().toString());
                                          System.out.println(user.getName());
                                          System.out.println("UpdateThread running");
                                          //不往servlet中发数据，往websocket中发，避免乱码
//                                          new Thread(new UpdateThread()).start();

                                          Intent intent = new Intent();
                                          Bundle bundle = new Bundle();
                                          bundle.putSerializable("user", user);
                                          intent.putExtras(bundle);
                                          intent.setClass(AboutActivity.this, MainActivity.class);

                                          setResult(2, intent);
                                          finish();


                                      }

                                  }

        );


    }

    private class UpdateThread implements Runnable {
        @Override
        public void run() {

            HttpClient httpClient = new DefaultHttpClient();
            String get = UPDATE_URL + "?" + "name=" + user.getName() + "&" + "age=" + user.getAge() + "&"
                    + "sex=" + user.getSex() + "&" + "medical_history=" + user.getMedical_history() + "&" + "attention=" + user.getAttention() + "&"
                    + "account=" + user.getAccount();
            System.out.println("get请求为："+get);
            HttpPost httpGet = new HttpPost(get);
            HttpResponse httpResponse = null;
            try {
                httpResponse = httpClient.execute(httpGet);
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

    private final static String UPDATE_URL = "http://myxtc910839.java.jspee.cn/UpdateServlet2";

    private Handler handler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            String string = (String) msg.obj;
            Toast.makeText(AboutActivity.this, string, Toast.LENGTH_SHORT).show();
        }
    };
}
