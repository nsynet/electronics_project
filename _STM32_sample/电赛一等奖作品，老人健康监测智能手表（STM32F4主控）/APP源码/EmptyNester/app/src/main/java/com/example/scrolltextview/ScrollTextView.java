package com.example.scrolltextview;

import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.Camera;
import android.graphics.Matrix;
import android.text.TextUtils.TruncateAt;
import android.util.AttributeSet;
import android.view.Gravity;
import android.view.View;
import android.view.animation.AccelerateInterpolator;
import android.view.animation.Animation;
import android.view.animation.Transformation;
import android.widget.TextSwitcher;
import android.widget.TextView;
import android.widget.ViewSwitcher;

public class ScrollTextView extends TextSwitcher implements
		ViewSwitcher.ViewFactory {

	private Context context;

	// inAnimation,outAnimation分别构成翻页的进出动画
	private ScrollAnimation inAnimation;
	private ScrollAnimation outAnimation;

	public ScrollTextView(Context context) {
		this(context, null);
	}

	public ScrollTextView(Context context, AttributeSet attrs) {
		super(context, attrs);
		TypedArray typedArray = context.obtainStyledAttributes(attrs,
				new int[] { 0x7f010000 });

		typedArray.recycle();
		this.context = context;
		init();
	}

	private void init() {
		setFactory(this);
		inAnimation = createAnim(-90, 0, true, true);
		outAnimation = createAnim(0, 90, false, true);
		setInAnimation(inAnimation);
		setOutAnimation(outAnimation);

	}

	private ScrollAnimation createAnim(float start, float end, boolean turnIn,
			boolean turnUp) {
		final ScrollAnimation rotation = new ScrollAnimation(start, end,
				turnIn, turnUp);
		rotation.setDuration(800);
		rotation.setFillAfter(false);
		rotation.setInterpolator(new AccelerateInterpolator());

		return rotation;
	}

	// 这里返回的TextView，就是我们看到的View
	@Override
	public View makeView() {
		TextView textView = new TextView(context);
		textView.setGravity(Gravity.LEFT);
		textView.setPadding(0, 21, 0, 21);
		textView.setTextSize(21);
		textView.setGravity(Gravity.CENTER_VERTICAL);// gravity center_vertical
		textView.setText("");
		textView.setSingleLine(false);
		textView.setEllipsize(TruncateAt.END);
		return textView;
	}

	public void next() {
		if (getInAnimation() != inAnimation) {
			setInAnimation(inAnimation);
		}
		if (getOutAnimation() != outAnimation) {
			setOutAnimation(outAnimation);
		}
	}

	class ScrollAnimation extends Animation {
		private final float mFromDegrees;
		private final float mToDegrees;
		private float mCenterX;
		private float mCenterY;
		private final boolean mTurnIn;
		private final boolean mTurnUp;
		private Camera mCamera;

		public ScrollAnimation(float fromDegrees, float toDegrees,
				boolean turnIn, boolean turnUp) {
			mFromDegrees = fromDegrees;
			mToDegrees = toDegrees;
			mTurnIn = turnIn;
			mTurnUp = turnUp;
		}

		@Override
		public void initialize(int width, int height, int parentWidth,
				int parentHeight) {
			super.initialize(width, height, parentWidth, parentHeight);
			mCamera = new Camera();
			mCenterY = getHeight() / 2;
			mCenterX = getWidth() / 2;
		}

		@Override
		protected void applyTransformation(float interpolatedTime,
				Transformation t) {
			final float fromDegrees = mFromDegrees;
			float degrees = fromDegrees
					+ ((mToDegrees - fromDegrees) * interpolatedTime);

			final float centerX = mCenterX;
			final float centerY = mCenterY;
			final Camera camera = mCamera;
			final int derection = mTurnUp ? 1 : -1;

			final Matrix matrix = t.getMatrix();

			camera.save();
			if (mTurnIn) {
				camera.translate(0.0f, derection * mCenterY
						* (interpolatedTime - 1.0f), 0.0f);
			} else {
				camera.translate(0.0f, derection * mCenterY
						* (interpolatedTime), 0.0f);
			}
			camera.getMatrix(matrix);
			camera.restore();

			matrix.preTranslate(-centerX, -centerY);
			matrix.postTranslate(centerX, centerY);
		}
	}
}
