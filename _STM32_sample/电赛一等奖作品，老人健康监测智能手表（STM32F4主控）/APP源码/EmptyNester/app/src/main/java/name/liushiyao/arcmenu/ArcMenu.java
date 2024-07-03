package name.liushiyao.arcmenu;


import android.content.Context;
import android.content.res.TypedArray;
import android.util.AttributeSet;
import android.util.Log;
import android.util.TypedValue;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.view.animation.AlphaAnimation;
import android.view.animation.Animation;
import android.view.animation.Animation.AnimationListener;
import android.view.animation.AnimationSet;
import android.view.animation.RotateAnimation;
import android.view.animation.ScaleAnimation;
import android.view.animation.TranslateAnimation;
import name.liushiyao.emptynester.R;

/**
 * 
 * @author 北岭山下
 * @aim
 * @time 2016年7月24日 下午11:41:33
 * 
 * "卫星"菜单设计
 */
/*
 * 1.自定义属性
 * 	a。attr.xml
 * 	b.在布局文件中使用
 * 	3.
 * 
 */
public class ArcMenu extends ViewGroup implements OnClickListener
{
	
	@Override
	protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec)
	{
		int count = getChildCount();
		for (int i = 0; i < count; i++)
		{
			measureChild(getChildAt(i), widthMeasureSpec,heightMeasureSpec);
		}
		super.onMeasure(widthMeasureSpec, heightMeasureSpec);
		
	}


	private static final int POS_LEFT_TOP = 0;
	private static final int POS_LEFT_BUTTON = 1;
	private static final int POS_RIGHT_TOP = 2;
	private static final int POS_RIGHT_BUTTON = 3;
	
	public void setOnMenuItemClickListener(
			OnMenuItemClickListener onMenuItemClickListener)
	{
		this.onMenuItemClickListener = onMenuItemClickListener;
	}


	//整个菜单的显示半径，默认100dp
	private int myRadius = 100;
	//主按键的显示位置,默认右下
	private Position myPosition = Position.RIGHT_BUTTON;
	//主按键的位置
	public enum Position{
		LEFT_TOP,LEFT_BUTTON,RIGHT_TOP,RIGHT_BUTTON
	}
	//Menu的状态
	public enum Status{
		OPEN,CLOSE
	}
	//用户点击的按钮
	private View myButton;
	//主按键的状态 默认关闭
	private Status myCurrentStatus = Status.CLOSE;
	//回调接口
	private OnMenuItemClickListener onMenuItemClickListener;
	//
	public interface OnMenuItemClickListener
	{
		void onClick(View view, int pos);
	}
	
	
	public ArcMenu(Context context)
	{
		this(context, null);
		
	}
	public ArcMenu(Context context, AttributeSet attrs)
	{
		this(context, attrs,0);
		
	}
	
	/**
	 * 初始参数（获取layout.xml中的属性）
	 * 
	 * @param context
	 * @param attrs
	 * @param defStyleAttr
	 */
	public ArcMenu(Context context, AttributeSet attrs, int defStyleAttr)
	{
		super(context, attrs, defStyleAttr);
		//默认值 100
		myRadius = (int)TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP,
				100, getResources().getDisplayMetrics());
		TypedArray a = context.getTheme().obtainStyledAttributes(attrs, R.styleable.ArcMenu, defStyleAttr, 0);
		//主按键位置 默认值3
		try
		{
			int pos = a.getInt(R.styleable.ArcMenu_position,POS_RIGHT_BUTTON);
			switch (pos)
			{
				case POS_LEFT_TOP:
					myPosition = Position.LEFT_TOP;
					break;
				case POS_LEFT_BUTTON:
					myPosition = Position.LEFT_BUTTON;
					break;
				case POS_RIGHT_TOP:
					myPosition = Position.RIGHT_TOP;
					break;
				case POS_RIGHT_BUTTON:
					myPosition = Position.RIGHT_BUTTON;
					break;
			}
			myRadius = (int)a.getDimension(R.styleable.ArcMenu_radius, 
					TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP,100,
							getResources().getDisplayMetrics()));//?
			Log.e("TAG","pos="+myPosition+" radius="+myRadius);
			System.out.println("pos="+myPosition+" radius="+myRadius);
					
		} finally
		{
			a.recycle();
			
		}
	}

	
	@Override
	protected void onLayout(boolean changed, int l, int t, int r, int b)
	{

		if (changed)
		{
			layoutCButton();

			int count = getChildCount();

			for (int i = 0; i < count - 1; i++)
			{
				View child = getChildAt(i + 1);

				child.setVisibility(View.GONE);

				int cl = (int) (myRadius * Math.sin(Math.PI / 2 / (count - 2) * i));
				int ct = (int) (myRadius * Math.cos(Math.PI / 2 / (count - 2) * i));

				int cWidth = child.getMeasuredWidth();
				int cHeight = child.getMeasuredHeight();

				// 如果菜单位置在底部 左下，右下
				if (myPosition == Position.LEFT_BUTTON || myPosition == Position.RIGHT_BUTTON)
				{
					ct = getMeasuredHeight() - cHeight - ct;
				}
				// 右上，右下
				if (myPosition == Position.RIGHT_TOP || myPosition == Position.RIGHT_BUTTON)
				{
					cl = getMeasuredWidth() - cWidth - cl;
				}
				child.layout(cl, ct, cl + cWidth, ct + cHeight);

			}

		}

	}
	/**
	 * 定位主菜单按钮
	 */
	private void layoutCButton()
	{
		myButton = getChildAt(0);
		myButton.setOnClickListener(this);

		int l = 0;
		int t = 0;

		int width = myButton.getMeasuredWidth();
		int height = myButton.getMeasuredHeight();

		switch (myPosition)
		{
		case LEFT_TOP:
			l = 0;
			t = 0;
			break;
		case LEFT_BUTTON:
			l = 0;
			t = getMeasuredHeight() - height;
			break;
		case RIGHT_TOP:
			l = getMeasuredWidth() - width;
			t = 0;
			break;
		case RIGHT_BUTTON:
			l = getMeasuredWidth() - width;
			t = getMeasuredHeight() - height;
			break;
		}
		myButton.layout(l, t, l + width, t + width);
	}
	
	@Override
	public void onClick(View v)
	{
		// mCButton = findViewById(R.id.id_button);
		// if(mCButton == null)
		// {
		// mCButton = getChildAt(0);
		// }

		rotateCButton(v, 0f, 360f, 300);

		toggleMenu(290);

	}

	/**
	 * 切换菜单
	 */
	public void toggleMenu(int duration)
	{
		// 为menuItem添加平移动画和旋转动画
		int count = getChildCount();

		for (int i = 0; i < count - 1; i++)
		{
			final View childView = getChildAt(i + 1);
			childView.setVisibility(View.VISIBLE);

			// end 0 , 0
			// start
			int cl = (int) (myRadius * Math.sin(Math.PI / 2 / (count - 2) * i));
			int ct = (int) (myRadius * Math.cos(Math.PI / 2 / (count - 2) * i));

			int xflag = 1;
			int yflag = 1;

			if (myPosition == Position.LEFT_TOP
					|| myPosition == Position.LEFT_BUTTON)
			{
				xflag = -1;
			}

			if (myPosition == Position.LEFT_TOP
					|| myPosition == Position.RIGHT_TOP)
			{
				yflag = -1;
			}

			AnimationSet animset = new AnimationSet(true);
			Animation tranAnim = null;

			// to open
			if (myCurrentStatus == Status.CLOSE)
			{
				tranAnim = new TranslateAnimation(xflag * cl, 0, yflag * ct, 0);
				childView.setClickable(true);
				childView.setFocusable(true);

			} else
			// to close
			{
				tranAnim = new TranslateAnimation(0, xflag * cl, 0, yflag * ct);
				childView.setClickable(false);
				childView.setFocusable(false);
			}
			tranAnim.setFillAfter(true);
			tranAnim.setDuration(duration);
			//调整各个子图标的弹出时间
			tranAnim.setStartOffset((i * 285) / count);

			tranAnim.setAnimationListener(new AnimationListener()
			{

				@Override
				public void onAnimationStart(Animation animation)
				{

				}

				@Override
				public void onAnimationRepeat(Animation animation)
				{

				}

				@Override
				public void onAnimationEnd(Animation animation)
				{
					if (myCurrentStatus == Status.CLOSE)
					{
						childView.setVisibility(View.GONE);
					}
				}
			});
			// 旋转动画
			RotateAnimation rotateAnim = new RotateAnimation(0, 720,
					Animation.RELATIVE_TO_SELF, 0.5f,
					Animation.RELATIVE_TO_SELF, 0.5f);
			rotateAnim.setDuration(duration);
			rotateAnim.setFillAfter(true);

			animset.addAnimation(rotateAnim);
			animset.addAnimation(tranAnim);
			childView.startAnimation(animset);

			final int pos = i + 1;
			//子菜单点击监听器
			childView.setOnClickListener(new OnClickListener()
			{
				@Override
				public void onClick(View v)
				{
					if (onMenuItemClickListener != null)
						onMenuItemClickListener.onClick(childView, pos);

//					menuItemAnim(pos - 1);
//					changeStatus();
					

				}
			});
		}
		// 切换菜单状态
		changeStatus();
	}

	/**
	 * 添加menuItem的点击动画
	 * 
	 * @param i
	 */
	private void menuItemAnim(int pos)
	{
		for (int i = 0; i < getChildCount() - 1; i++)
		{

			View childView = getChildAt(i + 1);
			if (i == pos)
			{
				childView.startAnimation(scaleBigAnim(300));
			} else
			{

				childView.startAnimation(scaleSmallAnim(300));
			}

			childView.setClickable(false);
			childView.setFocusable(false);

		}

	}

	private Animation scaleSmallAnim(int duration)
	{

		AnimationSet animationSet = new AnimationSet(true);

		ScaleAnimation scaleAnim = new ScaleAnimation(1.0f, 0.0f, 1.0f, 0.0f,
				Animation.RELATIVE_TO_SELF, 0.5f, Animation.RELATIVE_TO_SELF,
				0.5f);
		AlphaAnimation alphaAnim = new AlphaAnimation(1f, 0.0f);
		animationSet.addAnimation(scaleAnim);
		animationSet.addAnimation(alphaAnim);
		animationSet.setDuration(duration);
		animationSet.setFillAfter(true);
		return animationSet;

	}

	/**
	 * 为当前点击的Item设置变大和透明度降低的动画
	 * 
	 * @param duration
	 * @return
	 */
	private Animation scaleBigAnim(int duration)
	{
		AnimationSet animationSet = new AnimationSet(true);

		ScaleAnimation scaleAnim = new ScaleAnimation(1.0f, 4.0f, 1.0f, 4.0f,
				Animation.RELATIVE_TO_SELF, 0.5f, Animation.RELATIVE_TO_SELF,
				0.5f);
		AlphaAnimation alphaAnim = new AlphaAnimation(1f, 0.0f);

		animationSet.addAnimation(scaleAnim);
		animationSet.addAnimation(alphaAnim);

		animationSet.setDuration(duration);
		animationSet.setFillAfter(true);
		return animationSet;

	}

	/**
	 * 切换菜单状态
	 */
	public void changeStatus()
	{
		myCurrentStatus = (myCurrentStatus == Status.CLOSE ? Status.OPEN
				: Status.CLOSE);
	}

	public boolean isOpen()
	{
		return myCurrentStatus == Status.OPEN;
	}
	
	
	private void rotateCButton(View v, float start, float end, int duration)
	{

		RotateAnimation anim = new RotateAnimation(start, end,
				Animation.RELATIVE_TO_SELF, 0.5f, Animation.RELATIVE_TO_SELF,
				0.5f);
		anim.setDuration(duration);
		anim.setFillAfter(true);
		v.startAnimation(anim);
	}
	
}
