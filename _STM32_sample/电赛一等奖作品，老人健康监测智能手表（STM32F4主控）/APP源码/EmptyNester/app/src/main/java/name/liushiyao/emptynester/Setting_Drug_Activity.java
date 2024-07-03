package name.liushiyao.emptynester;

import android.annotation.TargetApi;
import android.app.Activity;
import android.content.Intent;
import android.os.Build;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TimePicker;

public class Setting_Drug_Activity extends Activity {

    private TimePicker timePicker;

    private Button save_button;



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_setting__drug);
        save_button = (Button) findViewById(R.id.setting_drug_save_button);
        timePicker = (TimePicker) findViewById(R.id.timePicker);
        timePicker.setIs24HourView(true);

        save_button.setOnClickListener(new View.OnClickListener() {
            @TargetApi(Build.VERSION_CODES.M)
            @Override
            public void onClick(View v) {
                    int hour = timePicker.getCurrentHour();
                    int min = timePicker.getCurrentMinute();
              /*  Toast.makeText(Setting_Drug_Activity.this, "保存设置时间 "+hour+":"+min
                        , Toast.LENGTH_SHORT).show();*/
                //返回数据给SettingActivity
                Intent intent = new Intent();
                intent.putExtra("hour",hour);
                intent.putExtra("min",min);
                setResult(2,intent);
                finish();               //结束该Activity
            }
        });
    }

}
