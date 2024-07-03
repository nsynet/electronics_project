package name.liushiyao.emptynester;


import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.media.MediaPlayer;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.SystemClock;
import android.os.Vibrator;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.view.animation.AccelerateInterpolator;
import android.view.animation.AlphaAnimation;
import android.view.animation.Animation;
import android.view.animation.AnimationSet;
import android.view.animation.DecelerateInterpolator;
import android.view.animation.ScaleAnimation;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.Toast;

import com.example.scrolltextview.ScrollTextView;

import net.ting.sliding.SlideMenu;

import org.java_websocket.client.WebSocketClient;
import org.java_websocket.drafts.Draft_17;
import org.java_websocket.handshake.ServerHandshake;

import java.net.URI;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;

import lecho.lib.hellocharts.samples.PreviewLineChartActivity;
import name.liushiyao.User;
import name.liushiyao.arcmenu.ArcMenu;

/**
 * 功能模块设计：
 * arcmenu：
 * 1. smartlink（wifi连接）（已实现）
 * 2. QRcode（二维码绑定）
 * 3. 功能设定（已实现）
 * 4. about
 */

/**
 * 与设备之间的通信采用响应机制：
 *      当app发送getData和updateSetting是必须等待设备的响应（OK）(getData返回数据)才算传输成功
 *      当发送指令后，没有收到OK则视为设备断开（发送错误）
 */
public class MainActivity extends AppCompatActivity {

    private SlideMenu slideMenu;
    private ImageView menuImg;
    private ArcMenu arcMenu;
    private ImageView mainIcon;
    private ScrollTextView scrollTextView;
    private boolean isContinue;
    private Thread myThread;
    private RelativeLayout mainActivity;
    private final static int REQUESTCODE = 0;
    private Button calculatorBtn;
    private ProgressDialog progressDialog;

    private List<View> heartbeatViews;

    private boolean icon_flag = true;

    private WebSocketClient client;
    private User user;
    //    private static final String address = "ws://192.168.1.114:8080/WebSocket_1/websocket?getData";
    private static final String address = "ws://myxtc910839.java.jspee.cn/websocket?app";

    private TextView logout;
    private TextView userInfo;
    private TextView product;
    private TextView aboutSoftware;
    private TextView dataCount;
    private Button center;
    private MediaPlayer mPlayer = null;
    private View calculatorBg;
    private View calculatorAnima;
    private String showData = "正在获取数据，请稍后...";
    private String show = "获取数据";
    private boolean status = false;

    private Vibrator vibrator;

    private boolean xx2_con_flag = true;
    private Handler handler = new Handler() {
        public void handleMessage(android.os.Message msg) {
            String data = (String) msg.obj;

            System.out.println("showData:" + showData);
            System.out.println("data:" + data);
            /**
             * 数据处理：t333&h084&bp081&bd057
             * 体温：t333（*10）
             * 心率：h84
             * 收缩压：bp081
             * 舒张压：bd057
             *
             */


            if (data != null) {

               /* if (data.contains("danger")) {
                    String temp = data.replace("danger", "");
                    if (temp.equals("hh")) {
                        show = "心率异常";
                    } else if (temp.equals("ht")) {
                        show = "体温异常";
                    } else if (temp.equals("hg")) {
                        show = "摔倒";
                    } else {
                        show = "其他问题";
                    }

                    //报警设置
                    center.setBackground(getResources().getDrawable(R.drawable.btn_shape_studytool_red, getTheme()));
                    calculatorAnima.setBackground(getResources().getDrawable(R.drawable.btn_shape_studytool_red, getTheme()));
                    calculatorBg.setBackground(getResources().getDrawable(R.drawable.btn_shape_studytool_red, getTheme()));
                    status = true;
                    mPlayer.start();
                } else
                */

                if (data.contains("sendData")) {
                    String temp = data.replace("sendData", "");
                    String dataTemp[] = temp.split("&");
                    String temper = String.valueOf(Integer.valueOf(dataTemp[0].replace("t", "")) / 10.0);
                    String heart = String.valueOf(Integer.valueOf(dataTemp[1].replace("h", "")));
                    String bp = String.valueOf(Integer.valueOf(dataTemp[2].replace("bp", "")));
                    String bd = String.valueOf(Integer.valueOf(dataTemp[3].replace("bd", "")));
                    //没有异常信息
                    if (temp.contains("nu")){
                        System.out.println("正常");
                        center.setBackground(getResources().getDrawable(R.drawable.btn_shape_studytool_green, getTheme()));
                        calculatorAnima.setBackground(getResources().getDrawable(R.drawable.btn_shape_studytool_green, getTheme()));
                        calculatorBg.setBackground(getResources().getDrawable(R.drawable.btn_shape_studytool_green, getTheme()));
                        showData = "体温:" + temper + "℃" + "     心率:" + heart + "次/分钟" + "\n血压：" + bp + "/" + bd + " PB";
                        show = "一切正常";
                        if (status) {
                            mPlayer.pause();
                            vibrator.cancel();
                        }
                        status = false;
                    }
                    //存在异常信息
                    else{
                        System.out.println("异常");
                        if (dataTemp[4].equals("hh")) {
                            show = "心率异常";
                        } else if (dataTemp[4].equals("ht")) {
                            show = "体温异常";
                        } else if (dataTemp[4].equals("hg")) {
                            show = "摔倒";
                        } else {
                            show = "其他问题";
                        }
                        showData = "体温:" + temper + "℃" + "     心率:" + heart + "次/分钟" + "\n血压：" + bp + "/" + bd + " PB";

                        //报警设置
                        center.setBackground(getResources().getDrawable(R.drawable.btn_shape_studytool_red, getTheme()));
                        calculatorAnima.setBackground(getResources().getDrawable(R.drawable.btn_shape_studytool_red, getTheme()));
                        calculatorBg.setBackground(getResources().getDrawable(R.drawable.btn_shape_studytool_red, getTheme()));
                        status = true;
                        mPlayer.start();
                           /*
         * 想设置震动大小可以通过改变pattern来设定，如果开启时间太短，震动效果可能感觉不到
         * */

                        long [] pattern = {100,400,100,400};   // 停止 开启 停止 开启
                        vibrator.vibrate(pattern,2);           //重复两次上面的pattern 如果只想震动一次，index设为-1
                    }

                } 
                else if (data.equals("{OK}")){
                    Toast.makeText(MainActivity.this, "用户设置保存成功", Toast.LENGTH_SHORT).show();
                }
                else if(data.equals("警告，手表已断开连接，无法获取数据")){
                    xx2_con_flag = false;
                    showData = data;
                }
                else if(data.equals("xx2 已连入")){
                    showData = data;
                    xx2_con_flag = true;
                }


                else {
                    showData = data;
                    show = "文本数据";
                }
                scrollTextView.next();
                scrollTextView.setText(showData);

            }
            center.setText(show);


            switch (msg.what) {
                case 0:

                    break;

                default:
                    break;
            }
        }

        ;
    };

    private class WebSocketThread implements Runnable {
        @Override
        public void run() {
            try {
                System.out.println("chekc connect WebSocket");
                Log.e("wsDbug", "连接地址：" + address);
                client = new WebSocketClient(new URI(address), new Draft_17()) {
                    @Override
                    public void onClose(final int code, String arg1, boolean arg2) {
                        System.out.println("与服务器[" + getURI() + "]断开连接" + "]");
                        System.out.println("状态吗[" + code + "]\n");
                        Log.e("wsdDbug", "与服务器[" + getURI() + "]断开连接" + "]");
                        Log.e("wsDbug", "状态吗[" + code + "]\n");
                        Toast.makeText(MainActivity.this, "与服务器断开连接，请重新登录", Toast.LENGTH_LONG).show();
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

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                WindowManager.LayoutParams.FLAG_FULLSCREEN);
        setContentView(R.layout.activity_main);
        heartbeatViews = new ArrayList<View>();
        Intent intent = this.getIntent();
        Bundle bundle = intent.getExtras();
        user = (User) bundle.getSerializable("user");
        System.out.println("MainActivity:" + user.getDevice());
        System.out.println("MainActivity:" + user.getDrug_time());
        vibrator = (Vibrator)getSystemService(Context.VIBRATOR_SERVICE);
        initView();
        initEvent();
        init();
        mPlayer = MediaPlayer.create(this, R.raw.flyme_tune_synth);
        mPlayer.setLooping(true);
        new Thread(new WebSocketThread()).start();

    }

    private void init() {
        scrollTextView = (ScrollTextView) findViewById(R.id.switcher02);
        scrollTextView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                System.out.println("scrollTextView touch");
                if (status) {
                    mPlayer.pause();
                    vibrator.cancel();
                    center.setBackground(getResources().getDrawable(R.drawable.btn_shape_studytool_green, getTheme()));
                    calculatorAnima.setBackground(getResources().getDrawable(R.drawable.btn_shape_studytool_green, getTheme()));
                    calculatorBg.setBackground(getResources().getDrawable(R.drawable.btn_shape_studytool_green, getTheme()));
                    Toast.makeText(MainActivity.this, "解除警报", Toast.LENGTH_SHORT).show();
                }
            }
        });
        scrollTextView.setText("点击圆球边沿获取数据");// 设置初始值
        isContinue = true;
        myThread = new Thread() {
            public void run() {
                while (isContinue) {
                    SystemClock.sleep(3500);
                    handler.sendEmptyMessage(0);//每隔一秒  发一个空消息  滚动一次
                }
            }

            ;
        };
        myThread.start();

    }

    private void initView() {
        arcMenu = (ArcMenu) findViewById(R.id.aremenu);
        slideMenu = (SlideMenu) findViewById(R.id.slide_menu);
        menuImg = (ImageView) findViewById(R.id.title_bar_menu_btn);
        mainIcon = (ImageView) findViewById(R.id.main_ico);
        mainActivity = (RelativeLayout) findViewById(R.id.main_activity);


        calculatorBg = (View) findViewById(R.id.study_tool_calculator_backgroud);
        initAnimationBackground(calculatorBg);
        calculatorAnima = (View) findViewById(R.id.study_tool_calculator_animation);
        heartbeatViews.add(calculatorAnima);
        calculatorBtn = (Button) findViewById(R.id.study_tool_calculator);


        logout = (TextView) findViewById(R.id.logout);
        userInfo = (TextView) findViewById(R.id.user_info);
        product = (TextView) findViewById(R.id.product);
        aboutSoftware = (TextView) findViewById(R.id.about_software);
        dataCount = (TextView) findViewById(R.id.data_count);

        center = (Button) findViewById(R.id.study_tool_calculator);
        progressDialog = new ProgressDialog(MainActivity.this);
        progressDialog.setProgressStyle(ProgressDialog.STYLE_HORIZONTAL);

    }

    private void initEvent() {




        calculatorBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (xx2_con_flag){
                    Toast.makeText(MainActivity.this, "获取数据中...", Toast.LENGTH_SHORT).show();

                    client.send("@xx2{getData}");
                }
            }
        });

        calculatorAnima.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (xx2_con_flag){
                    Toast.makeText(MainActivity.this, "获取数据中...", Toast.LENGTH_SHORT).show();

                    client.send("@xx2{getData}");
                }
            }
        });

        calculatorBg.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (xx2_con_flag){
                    Toast.makeText(MainActivity.this, "获取数据中...", Toast.LENGTH_SHORT).show();

                    client.send("@xx2{getData}");
                }
            }
        });


        logout.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent();
                intent.setClass(MainActivity.this, LoginActivity.class);
                intent.putExtra("login", 1);
                startActivity(intent);
                Toast.makeText(MainActivity.this, "退出登录", Toast.LENGTH_SHORT).show();
                //关闭连接
                client.close();
                finish();
            }
        });

        product.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent();
                intent.setClass(MainActivity.this, Production.class);
                startActivity(intent);
            }
        });

        dataCount.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent();
                intent.setClass(MainActivity.this, PreviewLineChartActivity.class);
                MainActivity.this.startActivity(intent);
            }
        });

        userInfo.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent();
                intent.setClass(MainActivity.this, AboutActivity.class);
                Bundle bundle = new Bundle();
                bundle.putSerializable("user", user);
                intent.putExtras(bundle);
                startActivityForResult(intent, REQUESTCODE);
            }
        });

        calculatorBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (arcMenu.isOpen())
                    arcMenu.toggleMenu(600);
            }
        });

        mainActivity.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (arcMenu.isOpen())
                    arcMenu.toggleMenu(600);

            }
        });


        mainIcon.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Toast.makeText(MainActivity.this, "chang icon", Toast.LENGTH_SHORT).show();
                if (icon_flag) {
                    mainIcon.setImageResource(R.drawable.main_ico_weman);
                } else {

                    mainIcon.setImageResource(R.drawable.main_ico_man);
                }
                icon_flag = !icon_flag;


            }
        });

        menuImg.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (arcMenu.isOpen())
                    arcMenu.toggleMenu(600);
                if (slideMenu.isMainScreenShowing()) {
                    slideMenu.openMenu();

                } else {
                    slideMenu.closeMenu();
                }


            }
        });

        arcMenu.setOnMenuItemClickListener(new name.liushiyao.arcmenu.ArcMenu.OnMenuItemClickListener() {
            @Override
            public void onClick(View view, int pos) {
                /*if (arcMenu.isOpen())
                    arcMenu.changeStatus();*/
                //跳转至新的Activity是关闭ARCMenu显示
                arcMenu.setVisibility(View.GONE);

                Toast.makeText(MainActivity.this, pos + ":" + view.getTag(), Toast.LENGTH_SHORT).show();
                if (view.getTag().toString().equals("Setting")) {


                    Intent intent = new Intent();
                    intent.setClass(MainActivity.this, SettingActivity.class);
                    Bundle bundle = new Bundle();
                    bundle.putSerializable("user", user);
                    intent.putExtras(bundle);
                    startActivityForResult(intent, REQUESTCODE);
//                    MainActivity.this.startActivity(intent);
                } else if (view.getTag().toString().equals("SmartWifi")) {

                    Intent intent = new Intent();
                    intent.setClass(MainActivity.this, WifiActivity.class);
                    MainActivity.this.startActivity(intent);
                } else if (view.getTag().toString().equals("About")) {

                    Intent intent = new Intent();
                    intent.setClass(MainActivity.this, AboutActivity.class);
                    Bundle bundle = new Bundle();
                    bundle.putSerializable("user", user);
                    intent.putExtras(bundle);
                    startActivityForResult(intent, REQUESTCODE);
                } else if (view.getTag().toString().equals("QR_Code")) {
                    Intent intent = new Intent();
                    Bundle bundle = new Bundle();
                    bundle.putSerializable("user", user);
                    intent.putExtras(bundle);
                    intent.setClass(MainActivity.this, QRCodeActivity.class);
                    startActivityForResult(intent, REQUESTCODE);
//                    MainActivity.this.startActivity(intent);
                }

            }
        });

    }

    @Override
    protected void onResume() {
        super.onResume();
        //返回MainActivity时重新显示ArcMenu
        arcMenu.setVisibility(View.VISIBLE);
        startHeartBeat();
    }

    /**
     * 在销毁的时候干掉线程
     */
    @Override
    protected void onDestroy() {
        super.onDestroy();
        isContinue = false;
        if (myThread != null && myThread.isAlive()) {
            myThread.interrupt();
        }
    }

    private class HeatbeatThread extends Thread {
        public void run() {
            try {
                sleep(100);
            } catch (InterruptedException e1) {
                e1.printStackTrace();
            }
            while (true) {
                runOnUiThread(new Runnable() {
                    public void run() {
                        for (View view : heartbeatViews) {
                            playHeartbeatAnimation(view);
                        }
                    }
                });
                try {
                    Thread.sleep(1500);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }

        ;
    }

    private Thread heartbeatThread;

    /**
     * 开始心跳
     */
    private void startHeartBeat() {
        if (heartbeatThread == null) {
            heartbeatThread = new HeatbeatThread();
        }
        if (!heartbeatThread.isAlive()) {
            heartbeatThread.start();
        }
    }

    /**
     * 停止心跳
     */
    private void stopHeartBeat() {
        if (heartbeatThread != null && heartbeatThread.isInterrupted()) {
            heartbeatThread.interrupt();
            heartbeatThread = null;
            System.gc();
        }
    }


    @Override
    protected void onPause() {
        super.onPause();
        stopHeartBeat();
    }

    private void initAnimationBackground(View view) {
        view.setAlpha(0.2f);
        view.setScaleX(1.4f);
        view.setScaleY(1.4f);

    }

    // 按钮模拟心脏跳动
    private void playHeartbeatAnimation(final View heartbeatView) {
        AnimationSet swellAnimationSet = new AnimationSet(true);
        swellAnimationSet.addAnimation(new ScaleAnimation(1.0f, 1.8f, 1.0f, 1.8f, Animation.RELATIVE_TO_SELF, 0.5f,
                Animation.RELATIVE_TO_SELF, 0.5f));
        swellAnimationSet.addAnimation(new AlphaAnimation(1.0f, 0.3f));

        swellAnimationSet.setDuration(500);
        swellAnimationSet.setInterpolator(new AccelerateInterpolator());
        swellAnimationSet.setFillAfter(true);

        swellAnimationSet.setAnimationListener(new Animation.AnimationListener() {

            @Override
            public void onAnimationStart(Animation animation) {
            }

            @Override
            public void onAnimationRepeat(Animation animation) {
            }

            @Override
            public void onAnimationEnd(Animation animation) {
                AnimationSet shrinkAnimationSet = new AnimationSet(true);
                shrinkAnimationSet.addAnimation(new ScaleAnimation(1.8f, 1.0f, 1.8f, 1.0f, Animation.RELATIVE_TO_SELF,
                        0.5f, Animation.RELATIVE_TO_SELF, 0.5f));
                shrinkAnimationSet.addAnimation(new AlphaAnimation(0.3f, 1.0f));
                shrinkAnimationSet.setDuration(1000);
                shrinkAnimationSet.setInterpolator(new DecelerateInterpolator());
                shrinkAnimationSet.setFillAfter(false);
                heartbeatView.startAnimation(shrinkAnimationSet);// 动画结束时重新开始，实现心跳的View
            }
        });

        heartbeatView.startAnimation(swellAnimationSet);
    }

    private class MyView extends View {
        private Handler mHandler;

        private int mColor;
        private float cx;
        private float cy;
        private float radius;

        public MyView(Context context) {
            super(context);
            mHandler = new Handler(getMainLooper());
            setBackgroundColor(Color.WHITE);
            Thread mThread = new Thread(new Runnable() {

                @Override
                public void run() {
                    MyView.this.invalidate();
                    mHandler.postDelayed(this, 300);
                }
            });
            mThread.start();
        }

        @Override
        protected void onDraw(Canvas canvas) {
            // TODO Auto-generated method stub
            super.onDraw(canvas);
            update();
            Paint p = new Paint();
            p.setColor(mColor);
            canvas.drawCircle(cx, cy, radius, p);
        }

        private void update() {
            Random random = new Random();
            cx = (float) (random.nextInt(720)); // 随机生成圆心横坐标（0至200）
            cy = (float) (random.nextInt(1180)); // 随机生成圆心纵坐标（0至400）
            radius = (float) (10 + random.nextInt(90)); // 随机生成圆的半径（10至100）

            int r = random.nextInt(256);
            int g = random.nextInt(256);
            int b = random.nextInt(256);
            mColor = Color.rgb(r, g, b); // 随机生成颜色
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        System.out.println(resultCode + "    " + requestCode);
        if (requestCode == REQUESTCODE) {
            //Settiong Activity 返回
            if (resultCode == 1) {
                if (data.getExtras() != null) {
                    System.out.println("用户进行了重新设置");
                    Bundle bundle = data.getExtras();
                    user = (User) bundle.getSerializable("user");
                    System.out.println("Setting Activity" + user.getDrug_time());
                    String temper = "";
                    String heart = "";
                    String fall = "";
                    if (user.getTemper().equals("on"))
                    {
                        temper = "SPO";
                    }
                    else
                    {
                        temper = "SPC";
                    }
                    if (user.getHeart().equals("on")){
                        heart = "SHO";
                    }else
                    {
                        heart = "SHC";
                    }
                    if (user.getFall().equals("on"))
                    {
                        fall = "SGO";
                    }else
                    {
                        fall = "SGC";
                    }
                    String time1 = "";
                    String time2 = "";
                    String time3 = "";

                   /* String sw1 = "";
                    String sw2 = "";
                    String sw3 = "";*/
                    String sw[] = user.getDrug().split("_");
                    String time[] = user.getDrug_time().split("_");
                    //总开关关闭
                    if (sw[0].equals("off"))
                    {
                        time1 = "60:00";
                        time2 = "60:00";
                        time3 = "60:00";
                    }
                    else
                    {
                        if (sw[1].equals("on")){
                            time1 = time[0];
                        }else
                        {
                            time1 = "60:00";
                        }
                        if (sw[2].equals("on")){
                            time2 = time[1];
                        }else
                        {
                            time2 = "60:00";
                        }
                        if (sw[3].equals("on")){
                            time3 = time[2];
                        }else
                        {
                            time3 = "60:00";
                        }
                    }

                    if (xx2_con_flag){
                        //发送设置保存数据
                        client.send("@xx2"+"updateSetting"+"{"+temper+"&"+
                                fall+"&"+heart+"&SE"+time1+"&"+time2+"&"+time3+"}");
                        System.out.println("@xx2"+"updateSetting"+"{"+temper+"&"+
                                fall+"&"+heart+"&SE"+time1+"&"+time2+"&"+time3+"}");
                    }
                }
                else{
                    System.out.println("用户未进行重新设置");
                }

            }
            //About Activity 返回
            else if (resultCode == 2) {
                Bundle bundle = data.getExtras();
                user = (User) bundle.getSerializable("user");
                System.out.println("About Activity=" + user.getAge());

                    client.send("@update" + "[{" + "\"account\":" + "\"" + user.getAccount() + "\"" + "," +
                            "\"name\":" + "\"" + user.getName() + "\"" + "," +
                            "\"age\":" + "\"" + user.getAge() + "\"" + "," +
                            "\"sex\":" + "\"" + user.getSex() + "\"" + "," +
                            "\"medical_history\":" + "\"" + user.getMedical_history() + "\"" + "," +
                            "\"attention\":" + "\"" + user.getAttention() + "\"" +
                            "}]");
                    System.out.println("Json ====" + "@update" + "[{" + "\"account\":" + "\"" + user.getAccount() + "\"" + "," +
                            "\"name\":" + "\"" + user.getName() + "\"" + "," +
                            "\"age\":" + "\"" + user.getAge() + "\"" + "," +
                            "\"sex\":" + "\"" + user.getSex() + "\"" + "," +
                            "\"medical_history\":" + "\"" + user.getMedical_history() + "\"" + "," +
                            "\"attention\":" + "\"" + user.getAttention() + "\"" +
                            "}]");

            }else if (resultCode == 3){
                Bundle bundle = data.getExtras();
                user = (User) bundle.getSerializable("user");
                System.out.println("QR_CodeActivity=" + user.getDevice());
            }
        }
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        if (keyCode == KeyEvent.KEYCODE_BACK) {
            // 创建退出对话框
            AlertDialog isExit = new AlertDialog.Builder(this).create();
            // 设置对话框标题
            isExit.setTitle("系统提示");
            // 设置对话框消息
            isExit.setMessage("确定要退出吗");
            // 添加选择按钮并注册监听
            isExit.setButton("确定", listener);
            isExit.setButton2("取消", listener);
            // 显示对话框
            isExit.show();

        }

        return false;

    }

    /**
     * 监听对话框里面的button点击事件
     */
    DialogInterface.OnClickListener listener = new DialogInterface.OnClickListener() {
        public void onClick(DialogInterface dialog, int which) {
            switch (which) {
                case AlertDialog.BUTTON_POSITIVE:// "确认"按钮退出程序
                    finish();
                    break;
                case AlertDialog.BUTTON_NEGATIVE:// "取消"第二个按钮取消对话框
                    break;
                default:
                    break;
            }
        }
    };
}
