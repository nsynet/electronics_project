package name.liushiyao.emptynester;


import android.app.Activity;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.text.TextWatcher;
import android.view.KeyEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.View.OnLongClickListener;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.Toast;

import com.example.administrator.myapplication.ExplosionField;
import com.example.administrator.myapplication.factory.FlyawayFactory;
import com.example.administrator.myapplication.factory.VerticalAscentFactory;

import org.apache.http.HttpResponse;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.util.EntityUtils;

import name.liushiyao.User;
import name.liushiyao.utils.JsonUtils;



/**
 * 注 册： ValidatePhoneNumActivity --> RegisterActivity
 * <p/>
 * 忘记密码 ForgetCodeActivity --> RepasswordActivity
 *
 * @author liubao.zeng
 */

/**
 * 网络功能
 * 从servlet获取User信息
 */
public class LoginActivity extends Activity implements OnClickListener, OnLongClickListener {
    // 声明控件对象
    private EditText et_name, et_pass;
    private Button mLoginButton, mLoginError, mRegister;


    int selectIndex = 1;
    int tempSelect = selectIndex;
    boolean isReLogin = false;
    private int SERVER_FLAG = 0;
    private RelativeLayout countryselect;
    private TextView coutry_phone_sn, coutryName;//
    // private String [] coutry_phone_sn_array,coutry_name_array;
    public final static int LOGIN_ENABLE = 0x01; // 注册完毕了
    public final static int LOGIN_UNABLE = 0x02; // 注册完毕了
    public final static int PASS_ERROR = 0x03; // 注册完毕了
    public final static int NAME_ERROR = 0x04; // 注册完毕了


    private CheckBox checkpass,auto_login =null;

    private SharedPreferences shared =null;
    private SharedPreferences.Editor editor =null;

    private int isCheck =0; //是否记住密码 0默认 不记住
    private int isAuto =0;

    private Button bt_username_clear;
    private Button bt_pwd_clear;
    private Button bt_pwd_eye;
    private TextWatcher username_watcher;
    private TextWatcher password_watcher;

    private LinearLayout main =null;
    private TextView auto =null;

    private String str_user,str_pass =null;

    private TextView ONLYTEST;
    private ProgressDialog progressDialog;

    //获取用户信息的服务器URL
//    private final static String SERVLET_URL = "http://192.168.1.114:8080/WebSocket_1/LoginServlet";
    private final static String SERVLET_URL = "http://myxtc910839.java.jspee.cn/LoginServlet";

    private Handler handler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            if (msg.what == 1) {
//                User user = (User)msg.getData().getSerializable("user");
                Intent intent = new Intent();
                Bundle bundle = new Bundle();

                bundle.putSerializable("user", msg.getData().getSerializable("user"));
                intent.putExtras(bundle);

                //没有进行设备绑定
                if (((String) msg.obj).equals("null")) {
                    intent.setClass(LoginActivity.this, QRCodeActivity.class);
                    Toast.makeText(LoginActivity.this, "登录成功！！！\n但检测到该账号未进行设备绑定，\n请扫描二维码进行绑定操作", Toast.LENGTH_LONG).show();
                    startActivity(intent);
                }
                //将user对象传递给MainActivity
                else {
                    Toast.makeText(LoginActivity.this, "登录成功，进行设备连接", Toast.LENGTH_SHORT).show();

                    intent.setClass(LoginActivity.this,
                            FindDeviceActivity.class);
                    startActivity(intent);
                    finish();
                }
            } else {
                System.out.println((String) msg.obj);
                Toast.makeText(LoginActivity.this, (String) msg.obj, Toast.LENGTH_SHORT).show();
            }
            System.out.println("取消显示进度条");
            progressDialog.dismiss();
        }
    };

    /**
     * 进行联网获取User信息，并进行正确性的判断
     */
    private class GetUserThread implements Runnable {
        @Override
        public void run() {
            HttpClient httpClient = new DefaultHttpClient();
//            HttpGet httpGet = new HttpGet(SERVLET_URL);
            HttpGet httpGet = new HttpGet(SERVLET_URL + "?account=" + et_name.getText()
                    + "&password=" + et_pass.getText());
            try {
                HttpResponse httpResponse = null;
                httpResponse = httpClient.execute(httpGet);
                System.out.println("GetUserThread running...");
                System.out.println(httpResponse.getStatusLine().getStatusCode());
                int code = httpResponse.getStatusLine().getStatusCode();
                if (code== 200) {
                    System.out.println("entry===============");
//                    String data = EntityUtils.toString(httpResponse.getEntity());
                    String data = EntityUtils.toString(httpResponse.getEntity());
                    System.out.println("接收到的数据 data==>" + data);

                    Message msg = Message.obtain();
                    //坑爹，既然要加 \r\n
                    if (data.equals("no account\r\n")) {
                        msg.obj = "不存在该账号，请重试！";
                        msg.what = 0;
                    } else if (data.equals("no password\r\n")) {
                        msg.obj = "密码不正确，请重新输入！";
                        msg.what = 0;
                    } else {
                        User user = JsonUtils.parseUserFromJson(data);
                        Bundle bundle = new Bundle();
                        bundle.putSerializable("user", user);
                        msg.setData(bundle);
//                        msg.obj = user;
                        msg.what = 1;
                        //判断是否绑定了设备
                        if (user.getDevice().equals("null")) {
                            msg.obj = "null";
                        } else {
                            msg.obj = user.getDevice();
                        }

                    }

                    handler.sendMessage(msg);
                }
                else
                {
                    System.out.println("GetUserThread 连接异常"+"错误代码："+code);
                    Message msg = Message.obtain();
                    msg.obj = "连接服务器异常，请检测网络\n错误代码："+code;
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
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        // //不显示系统的标题栏
        getWindow().setFlags( WindowManager.LayoutParams.FLAG_FULLSCREEN,
        WindowManager.LayoutParams.FLAG_FULLSCREEN );

        setContentView(R.layout.activity_login);
        et_name = (EditText) findViewById(R.id.user_info);
        et_pass = (EditText) findViewById(R.id.password);

//        bt_username_clear = (Button) findViewById(R.id.bt_username_clear);
//        bt_pwd_clear = (Button) findViewById(R.id.bt_pwd_clear);
//        bt_pwd_eye = (Button) findViewById(R.id.bt_pwd_eye);
//        bt_username_clear.setOnClickListener(this);
//        bt_pwd_clear.setOnClickListener(this);
//        bt_pwd_eye.setOnClickListener(this);
//        initWatcher();
       /* et_name.addTextChangedListener(username_watcher);
        et_pass.addTextChangedListener(password_watcher);*/

        mLoginButton = (Button) findViewById(R.id.login);
        mLoginError = (Button) findViewById(R.id.login_error);
        mRegister = (Button) findViewById(R.id.register);
        ONLYTEST = (TextView) findViewById(R.id.registfer);

        mLoginButton.setOnClickListener(this);
        mLoginError.setOnClickListener(this);
        mRegister.setOnClickListener(this);



        ExplosionField explosionField5 = new ExplosionField(this,new VerticalAscentFactory());
        explosionField5.addListener(findViewById(R.id.logo));

        ExplosionField explosionField2 = new ExplosionField(this,new FlyawayFactory());
        explosionField2.addListener(findViewById(R.id.registfer));


        shared =this.getSharedPreferences("login", Context.MODE_PRIVATE);
        editor =shared.edit();

        isAuto =shared.getInt("auto", 0);
        isCheck =shared.getInt("check",0); //刚进入界面获取 是否记住密码的状态

        checkpass =(CheckBox)findViewById(R.id.checkpass);
        auto_login =(CheckBox)findViewById(R.id.auto_login);
        auto =(TextView)findViewById(R.id.auto);

        progressDialog = new ProgressDialog(LoginActivity.this);
        progressDialog .setProgressStyle(ProgressDialog.STYLE_SPINNER);

        Intent intent = getIntent();
        System.out.println("intent == null?"+intent == null);
        System.out.println(intent.getIntExtra("login",0));
        if (intent.getIntExtra("login",0) == 1)
        {
            editor.putInt("auto", 0);
            isAuto = 0;
        }

        //记住密码
        if(isCheck==1){
            System.out.println("记住密码。。。");
            //记住密码，从SharedPreference中就获取账号密码
            str_user =shared.getString("user", "");
            str_pass =shared.getString("pass", "");
            System.out.println(str_user+"----"+str_pass);
            //设置给控件
            et_name.setText(str_user);
            et_pass.setText(str_pass);
            //设置控件为选中状态
            checkpass.setChecked(true);
        }else {
            //不记住密码
            checkpass.setChecked(false);
        }
        //自动登录
       if(isAuto == 1){
           System.out.println("自动登录中。。。");
           new Thread(new GetUserThread()).start();
            /*new Thread(new Runnable() {
                @Override
                public void run() {
                    // TODO Auto-generated method stub
                    try {
                        main.setVisibility(View.GONE);
                        auto.setVisibility(View.VISIBLE);
                        Thread.sleep(2000);
                        Intent intent =new Intent(LoginActivity.this,TwoActivity.class);
                        startActivity(intent);
                    } catch (InterruptedException e) {
                        // TODO Auto-generated catch block
                        e.printStackTrace();
                    }
                }
            }).start();*/
        }


        //自动登录监听器
        auto_login.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {

            @Override
            public void onCheckedChanged(CompoundButton arg0, boolean bool) {
                // TODO Auto-generated method stub
                if(bool){//自动登录选中，记住密码也选中。
                    checkpass.setChecked(true);
                }
            }
        });
        //记住密码监听器
        checkpass.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {

            @Override
            public void onCheckedChanged(CompoundButton arg0, boolean bool) {
                // TODO Auto-generated method stub
                if(!bool){
                    auto_login.setChecked(false);
                    //防止 在正常登录后，在次进入登录界面时，只做了取消记住密码操作，而没有登录的情况。
                    editor.putInt("check",0);
                    editor.putInt("auto", 0);
                    editor.commit();
                }
            }
        });
    }

    /**
     * 手机号，密码输入控件公用这一个watcher
     */
   /* private void initWatcher() {
        username_watcher = new TextWatcher() {
            public void onTextChanged(CharSequence s, int start, int before, int count) {
            }

            public void beforeTextChanged(CharSequence s, int start, int count, int after) {
            }

            public void afterTextChanged(Editable s) {
                et_pass.setText("");
                if (s.toString().length() > 0) {
                    bt_username_clear.setVisibility(View.VISIBLE);
                } else {
                    bt_username_clear.setVisibility(View.INVISIBLE);
                }
            }
        };

        password_watcher = new TextWatcher() {
            public void onTextChanged(CharSequence s, int start, int before, int count) {
            }

            public void beforeTextChanged(CharSequence s, int start, int count, int after) {
            }

            public void afterTextChanged(Editable s) {
                if (s.toString().length() > 0) {
                    bt_pwd_clear.setVisibility(View.VISIBLE);
                } else {
                    bt_pwd_clear.setVisibility(View.INVISIBLE);
                }
            }
        };
    }
*/
    @Override
    public void onClick(View arg0) {
        switch (arg0.getId()) {
            case R.id.login: // 登陆
                boolean netStatus = isNetworkAvailable(LoginActivity.this);
                if ( netStatus == true) {
                    if(auto_login.isChecked()){
                        System.out.println("auto_login.isChecked()");
                        editor.putInt("check",1);
                        editor.putInt("auto", 1);
                        editor.putString("user", String.valueOf(et_name.getText()));
                        editor.putString("pass",String.valueOf(et_pass.getText()));
                    }else if(checkpass.isChecked()){
                        System.out.println("checkpass.isChecked()");
                        editor.putInt("auto", 0);
                        editor.putInt("check",1);
                        editor.putString("user", String.valueOf(et_name.getText()));
                        editor.putString("pass", String.valueOf(et_pass.getText()));
                    }else {
                        editor.putInt("check",0);
                        editor.putInt("auto", 0);
                    }
                    editor.commit();
                    progressDialog.show();
                    progressDialog.setTitle("正在登陆，请稍后");
                    progressDialog.setMessage("正在登陆，请稍后");
                    str_user =shared.getString("user", "");
                    str_pass =shared.getString("pass", "");
                    System.out.println(str_user+"----"+str_pass);
                    new Thread(new GetUserThread()).start();
                }
                else {
                    Toast.makeText(LoginActivity.this, "网络连接异常，请检查你的网络连接", Toast.LENGTH_SHORT).show();
                }
                

                //结束
//                finish();
                break;
            case R.id.login_error: // 无法登陆(忘记密码了吧)

                Toast.makeText(this, "忘记密码？请重新注册", Toast.LENGTH_SHORT).show();
                Intent intent = new Intent();
                intent.setClass(LoginActivity.this,
                        RegeditActivity.class);
                startActivity(intent);

                break;
            case R.id.register: // 注册新的用户
                Intent intent2 = new Intent();
                intent2.setClass(LoginActivity.this,
                        RegeditActivity.class);
                startActivity(intent2);

                break;

			/*case R.id.registfer:
                if (SERVER_FLAG > 10)
				{
					Toast.makeText(this, "[内部测试--谨慎操作]", Toast.LENGTH_SHORT).show();
				}
				SERVER_FLAG++;
				break;*/
           /* case R.id.bt_username_clear:
                et_name.setText("");
                et_pass.setText("");
                break;
            case R.id.bt_pwd_clear:
                et_pass.setText("");
                break;
            case R.id.bt_pwd_eye:
                if (et_pass.getInputType() == (InputType.TYPE_CLASS_TEXT | InputType.TYPE_TEXT_VARIATION_PASSWORD)) {
                    bt_pwd_eye.setBackgroundResource(R.drawable.eye);
                    et_pass.setInputType(InputType.TYPE_CLASS_TEXT | InputType.TYPE_TEXT_VARIATION_NORMAL);
                } else                    //隐藏密码
                {
                    bt_pwd_eye.setBackgroundResource(R.drawable.eye_c);
                    et_pass.setInputType(InputType.TYPE_CLASS_TEXT | InputType.TYPE_TEXT_VARIATION_PASSWORD);
                }
                et_pass.setSelection(et_pass.getText().toString().length());
                break;*/
        }
    }

    /**
     * 登陆
     */
    private void login() {
    }

    @Override
    public boolean onLongClick(View v) {
        // TODO Auto-generated method stub
        switch (v.getId()) {
            case R.id.registfer:
                if (SERVER_FLAG > 9) {

                }
                // SERVER_FLAG++;
                break;
        }
        return true;
    }

    /**
     * 监听Back键按下事件,方法2: 注意: 返回值表示:是否能完全处理该事件 在此处返回false,所以会继续传播该事件.
     * 在具体项目中此处的返回值视情况而定.
     */
    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        if ((keyCode == KeyEvent.KEYCODE_BACK)) {
            if (isReLogin) {
                Intent mHomeIntent = new Intent(Intent.ACTION_MAIN);
                mHomeIntent.addCategory(Intent.CATEGORY_HOME);
                mHomeIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_RESET_TASK_IF_NEEDED);
                LoginActivity.this.startActivity(mHomeIntent);
            } else {
                LoginActivity.this.finish();
            }
            return false;
        } else {
            return super.onKeyDown(keyCode, event);
        }

    }
    //检测网络是否可用
    public static boolean isNetworkAvailable( Activity mActivity ) {
        Context context = mActivity.getApplicationContext();
        ConnectivityManager connectivity = (ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);
        if (connectivity == null) {
            return false;
        } else {
            NetworkInfo[] info = connectivity.getAllNetworkInfo();
            if (info != null) {
                for (int i = 0; i < info.length; i++) {
                    if (info[i].getState() == NetworkInfo.State.CONNECTED) {
                        return true;
                    }
                }
            }
        }
        return false;
    }
}

