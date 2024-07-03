package name.liushiyao.emptynester;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.view.Window;
import android.view.WindowManager;

/**
 * app 启动显示界面（已完成）
 *      启动后显示3s的图片，然后跳转至LoginActivity
 */
/*
        实现原理：
            开启一个Handler，利用Handler.postDelay();达到延时跳转的效果

 */
public class LoadActivity extends Activity {

    //time for picture display
    private static final int LOAD_DISPLAY_TIME = 3500;

    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        //取消标题栏和状态栏
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setFlags( WindowManager.LayoutParams.FLAG_FULLSCREEN,
                WindowManager.LayoutParams.FLAG_FULLSCREEN );

        setContentView(R.layout.activity_load);
        //开启Handler线程，到达持续显示的效果
        new Handler().postDelayed(new Runnable() {
            public void run() {
                //Go to main activity, and finish load activity
                Intent intent = new Intent(LoadActivity.this, LoginActivity.class);
                LoadActivity.this.startActivity(intent);
                //结束当前的Activity
                LoadActivity.this.finish();
            }
        }, LOAD_DISPLAY_TIME);
    }
}
