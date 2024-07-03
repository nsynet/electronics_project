package name.liushiyao.emptynester;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.widget.ImageView;
import android.widget.Switch;
import android.widget.TextView;

import com.example.administrator.myapplication.ExplosionField;
import com.example.administrator.myapplication.factory.FallingParticleFactory;

import org.apache.http.HttpResponse;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.util.EntityUtils;

import name.liushiyao.User;

/**
 * Created by 电子小孩 on 2016/8/5.
 * 需求：该类用于设置界面的Activity
 * 主要包括    1. 体温报警 --默认设置37度  开关
 * 2. 摔倒报警 --              开关
 * 3. 提醒吃药 --设置时间      开关
 * 4. 心率提醒 --正常水平范围  开关
 */
public class SettingActivity extends Activity {

    private ImageView save_img;
    private final int REQUESTCODE = 3;//返回的结果码

    private final int TEXT1 = 1;
    private final int TEXT2 = 2;
    private final int TEXT3 = 3;
    //    private TextView drug_time;
    private Switch temp_switch;
    private Switch drug_switch;
    private Switch heart_switch;
    private Switch fall_switch;

    private Switch time1_switch;
    private Switch time2_switch;
    private Switch time3_switch;

    private TextView time1_Text;
    private TextView time2_Text;
    private TextView time3_Text;

    private User user;

    private String userTemp = "";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.setting_layout);
        //接收传递的User对象
        Intent intent = this.getIntent();
        Bundle bundle = intent.getExtras();
        user = (User) bundle.getSerializable("user");
        userTemp = user.toString();
        initView();
        initEvent();
    }

    private void initView() {
        save_img = (ImageView) findViewById(R.id.imageButton);
        save_img.setBackgroundColor(200);
//        drug_time = (TextView) findViewById(R.id.drug_text);
        //swicht
        temp_switch = (Switch) findViewById(R.id.temp_switch);
        drug_switch = (Switch) findViewById(R.id.drug_switch);
        heart_switch = (Switch) findViewById(R.id.heart_switch);
        fall_switch = (Switch) findViewById(R.id.fall_switch);

        time1_switch = (Switch) findViewById(R.id.time1);
        time2_switch = (Switch) findViewById(R.id.time2);
        time3_switch = (Switch) findViewById(R.id.time3);

        time1_Text = (TextView) findViewById(R.id.textView1);
        time2_Text = (TextView) findViewById(R.id.textView2);
        time3_Text = (TextView) findViewById(R.id.textView3);

        temp_switch.setChecked(user.getTemper().equals("on"));
        heart_switch.setChecked(user.getHeart().equals("on"));
        fall_switch.setChecked(user.getFall().equals("on"));
        System.out.println(user.getDrug());
        String time_switch[] = user.getDrug().split("_");
       /* System.out.println(time_switch[0].equals("no"));
        System.out.println(time_switch[1].equals("no"));
        System.out.println(time_switch[2].equals("no"));
        System.out.println(time_switch[3].equals("no"));
*/

        drug_switch.setChecked(time_switch[0].equals("on"));
        time1_switch.setChecked(time_switch[1].equals("on"));
        time2_switch.setChecked(time_switch[2].equals("on"));
        time3_switch.setChecked(time_switch[3].equals("on"));

//        drug_time.setText("       提醒每天" + user.getDrug_time() + " 吃药（点击进行设置）");


        String timeTemp[] = user.getDrug_time().split("_");
        time1_Text.setText("提醒" + timeTemp[0] + "吃药");
        time2_Text.setText("提醒" + timeTemp[1] + "吃药");
        time3_Text.setText("提醒" + timeTemp[2] + "吃药");

        if (drug_switch.isChecked()) {
            time1_switch.setEnabled(true);
            time2_switch.setEnabled(true);
            time3_switch.setEnabled(true);

        } else {
            time1_switch.setEnabled(false);
            time2_switch.setEnabled(false);
            time3_switch.setEnabled(false);
        }
        if (time1_switch.isChecked()) {
            time1_Text.setEnabled(true);
        } else {
            time1_Text.setEnabled(false);
        }
        if (time2_switch.isChecked()) {
            time2_Text.setEnabled(true);
        } else {
            time2_Text.setEnabled(false);
        }
        if (time3_switch.isChecked()) {
            time3_Text.setEnabled(true);
        } else {
            time3_Text.setEnabled(false);
        }


    }

    private void initEvent() {
        ExplosionField explosionField = new ExplosionField(this, new FallingParticleFactory());
        explosionField.addListener(findViewById(R.id.setting_title));

        time1_switch.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (time1_switch.isChecked()) {
                    time1_Text.setEnabled(true);
                } else {
                    time1_Text.setEnabled(false);
                }
            }
        });

        time2_switch.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (time2_switch.isChecked()) {
                    time2_Text.setEnabled(true);
                } else {
                    time2_Text.setEnabled(false);
                }
            }
        });

        time3_switch.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (time3_switch.isChecked()) {
                    time3_Text.setEnabled(true);
                } else {
                    time3_Text.setEnabled(false);
                }
            }
        });

        drug_switch.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (drug_switch.isChecked()) {
                    time1_switch.setEnabled(true);
                    time2_switch.setEnabled(true);
                    time3_switch.setEnabled(true);
                } else {
                    time1_switch.setEnabled(false);
                    time2_switch.setEnabled(false);
                    time3_switch.setEnabled(false);
                }
            }
        });

        save_img.setOnClickListener(new View.OnClickListener() {
                                        @Override
                                        public void onClick(View v) {


                                            if (temp_switch.isChecked()) {
                                                user.setTemper("on");
                                            } else {
                                                user.setTemper("off");
                                            }

                                            String time_switch[] = user.getDrug().split("_");

                                            if (drug_switch.isChecked()) {
                                                time_switch[0] = "on";
                                            } else {
                                                time_switch[0] = "off";
                                            }

                                            if (time1_switch.isChecked()) {
                                                time_switch[1] = "on";
                                            } else {
                                                time_switch[1] = "off";
                                            }
                                            if (time2_switch.isChecked()) {
                                                time_switch[2] = "on";
                                            } else {
                                                time_switch[2] = "off";
                                            }
                                            if (time3_switch.isChecked()) {
                                                time_switch[3] = "on";
                                            } else {
                                                time_switch[3] = "off";
                                            }
                                            user.setDrug(time_switch[0]+"_"+time_switch[1]+"_"+time_switch[2]+"_"+time_switch[3]);
//                                            System.out.println("保存设置----》"+time_switch[0]+"_"+time_switch[1]+"_"+time_switch[2]+"_"+time_switch[3]);
                                            if (heart_switch.isChecked()) {
                                                user.setHeart("on");
                                            } else {
                                                user.setHeart("off");
                                            }
                                            if (fall_switch.isChecked()) {
                                                user.setFall("on");
                                            } else {
                                                user.setFall("off");
                                            }
                                            System.out.println("UpdateThread running");
//                                            Toast.makeText(SettingActivity.this, "用户设置已保存", Toast.LENGTH_SHORT).show();
                                            //如果数据发生变化
                                            Intent intent = new Intent();
                                            if (!(user.toString().equals(userTemp)))
                                            {
                                                Bundle bundle = new Bundle();
                                                new Thread(new UpdateThread()).start();
                                                bundle.putSerializable("user", user);
                                                intent.putExtras(bundle);

                                            }
                                            intent.setClass(SettingActivity.this, MainActivity.class);

                                            setResult(1, intent);
                                            finish();


                                        }

                                    }

        );

        /*drug_time.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent();

                intent.setClass(SettingActivity.this, Setting_Time_Activity.class);
                startActivityForResult(intent, REQUESTCODE);

            }
        });*/
        time1_Text.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent();

                intent.setClass(SettingActivity.this, Setting_Drug_Activity.class);
                startActivityForResult(intent, REQUESTCODE + TEXT1);

            }
        });

        time2_Text.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent();

                intent.setClass(SettingActivity.this, Setting_Drug_Activity.class);
                startActivityForResult(intent, REQUESTCODE + TEXT2);

            }
        });

        time3_Text.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent();

                intent.setClass(SettingActivity.this, Setting_Drug_Activity.class);
                startActivityForResult(intent, REQUESTCODE + TEXT3);

            }
        });

    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        System.out.println(resultCode + "    " + requestCode);
        String time[] = user.getDrug_time().split("_");

        if (requestCode == REQUESTCODE + TEXT1) {
            if (resultCode == 2) {
                int hour = data.getIntExtra("hour", 0);
                int min = data.getIntExtra("min", 0);
//                drug_time.setText("       提醒每天" + hour + ":" + min + " 吃药（点击进行设置）");
                time1_Text.setText("提醒 " + hour + ":" + min + " 吃药");
//                Toast.makeText(SettingActivity.this, "提醒每天" + hour + ":" + min + " 吃药", Toast.LENGTH_SHORT).show();
                System.out.println("提醒每天" + hour + ":" + min + " 吃药");
                time[0] = changeNum(hour) + ":" + changeNum(min);
//                user.setDrug_time(hour + ":" + min);
            }
        } else if (requestCode == REQUESTCODE + TEXT2) {
            if (resultCode == 2) {
                int hour = data.getIntExtra("hour", 0);
                int min = data.getIntExtra("min", 0);
                time2_Text.setText("提醒 " + hour + ":" + min + " 吃药");
//                drug_time.setText("       提醒每天" + hour + ":" + min + " 吃药（点击进行设置）");
//                Toast.makeText(SettingActivity.this, "提醒每天" + hour + ":" + min + " 吃药", Toast.LENGTH_SHORT).show();
//                System.out.println("提醒每天" + hour + ":" + min + " 吃药");
                time[1] = changeNum(hour) + ":" + changeNum(min);
//                user.setDrug_time(hour + ":" + min);
            }
        } else if (requestCode == REQUESTCODE + TEXT3) {
            if (resultCode == 2) {
                int hour = data.getIntExtra("hour", 0);
                int min = data.getIntExtra("min", 0);
                time3_Text.setText("提醒 " + hour + ":" + min + " 吃药");
//                drug_time.setText("       提醒每天" + hour + ":" + min + " 吃药（点击进行设置）");
//                Toast.makeText(SettingActivity.this, "提醒每天" + hour + ":" + min + " 吃药", Toast.LENGTH_SHORT).show();
//                System.out.println("提醒每天" + hour + ":" + min + " 吃药");
                time[2] = changeNum(hour) + ":" + changeNum(min);
//                user.setDrug_time(hour + ":" + min);
            }
        }
        String sendData = time[0] + "_" + time[1] + "_" + time[2];
        System.out.println("更新数据---->" + sendData);
        user.setDrug_time(sendData);

    }

    private class UpdateThread implements Runnable {
        @Override
        public void run() {

            HttpClient httpClient = new DefaultHttpClient();
            String get = UPDATE_URL + "?" + "temper=" + user.getTemper() + "&" + "drug=" + user.getDrug() + "&"
                    + "drug_time=" + user.getDrug_time() + "&" + "heart=" + user.getHeart() + "&" + "fall=" + user.getFall() + "&"
                    + "account=" + user.getAccount();
            System.out.println(get);
            HttpGet httpGet = new HttpGet(get);
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

    private final static String UPDATE_URL = "http://myxtc910839.java.jspee.cn/UpdateServlet";

    private Handler handler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            String string = (String) msg.obj;
            System.out.println(string);
//            Toast.makeText(SettingActivity.this, string, Toast.LENGTH_SHORT).show();
        }
    };
    private String changeNum(int num)
    {
        if (num > 9) {
            return num+"";
        }
        else {
            return "0"+num;
        }

    }
}
