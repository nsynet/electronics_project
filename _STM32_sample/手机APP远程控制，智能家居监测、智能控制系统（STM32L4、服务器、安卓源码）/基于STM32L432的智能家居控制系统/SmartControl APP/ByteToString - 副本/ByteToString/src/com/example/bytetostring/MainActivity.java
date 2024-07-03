package com.example.bytetostring;

import java.io.ByteArrayOutputStream;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.TextView;
import android.app.Activity;

public class MainActivity extends Activity implements OnClickListener{

	private byte[] mDataByte = new byte[] { 11, 38, 90, 72, 78 ,46,8,99,102};
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		findViewById(R.id.button1).setOnClickListener(this);
		findViewById(R.id.button2).setOnClickListener(this);
		findViewById(R.id.button3).setOnClickListener(this);
		findViewById(R.id.button4).setOnClickListener(this);
		findViewById(R.id.button5).setOnClickListener(this);
		
		TextView textviewData = (TextView)findViewById(R.id.textviewData);
//		String string = new String(mDataByte,0,mDataByte.length);
		textviewData.setText("byte[] { 11, 38, 90, 72, 78 ,46,8,99,102}");
	}

	//  byte����ת����ʮ��������ʾ
	private String getHexString(byte[] data, int nLength) {
		String strTemString = "";
		for (int i = 0; i < nLength; i++) {
			strTemString += String.format("%02X ", data[i]);
		}
		return strTemString;
	}

	// ʮ�����Ƶ��ַ���ת��ΪString
	private String hexString = "0123456789ABCDEF";
	public String decode(String bytes) {
		ByteArrayOutputStream baos = new ByteArrayOutputStream(
				bytes.length() / 2);
		// ��ÿ2λ16����������װ��һ���ֽ�
		for (int i = 0; i < bytes.length(); i += 2)
			baos.write((hexString.indexOf(bytes.charAt(i)) << 4 | hexString
					.indexOf(bytes.charAt(i + 1))));
		return new String(baos.toByteArray());
	}

	/**
	 * byte����ת����16�����ַ���
	 * 
	 * @param src
	 * @return
	 */
	public static String bytesToHexString(byte[] src) {
		StringBuilder stringBuilder = new StringBuilder();
		if (src == null || src.length <= 0) {
			return null;
		}
		for (int i = 0; i < src.length; i++) {
			int v = src[i] & 0xFF;
			String hv = Integer.toHexString(v);
			if (hv.length() < 2) {
				stringBuilder.append(0);
			}
			stringBuilder.append(hv);
		}
		return stringBuilder.toString();
	}

	/**
	 * byte����ת����16�����ַ�����
	 * 
	 * @param src
	 * @return
	 */
	public static String[] bytesToHexStrings(byte[] src) {
		if (src == null || src.length <= 0) {
			return null;
		}
		
		String[] str = new String[src.length];

		for (int i = 0; i < src.length; i++) {
			StringBuilder stringBuilder = new StringBuilder();
			int v = src[i] & 0xFF;
			String hv = Integer.toHexString(v);
			
			if (hv.length() < 2) {
				stringBuilder.append(0);
			}
			stringBuilder.append(hv);
			str[i] = stringBuilder.toString();
		}
		return str;
	}
	
    //  ʮ�����Ƶ��ַ���ת����byte����	
	public byte[] HexCommandtoByte(byte[] data) {
		if (data == null) {
			return null;
		}
		int nLength = data.length; 
		if (nLength < 10 || data[2] != ' ' 
				|| data[5] != ' ' || data[8] != ' ') {
			return data;
		}
		
		//ʮ�����Ʒ���
		//ȥ��\r\n;
		while (true) {
			if (data[nLength-1] == '\n')
				nLength--;
			else if (data[nLength-1] == '\r')
				nLength--;
			else if (data[nLength-1] == ' ')
				nLength--;
			else 
				break;
			if (nLength < 8) {
				return null;
			}
		}
		
		String strTemString = new String(data, 0, nLength);
		String[] strings = strTemString.split(" ");
		nLength = strings.length;
		data = new byte[nLength];			
		for (int i = 0; i < nLength; i++) {
			if (strings[i].length() != 2) {
				return null;
			}
			try {
				data[i] = (byte)Integer.parseInt(strings[i], 16);
			} catch (Exception e) {
				return null;
			}
		}
	
		return data;
	}

	@Override
	public void onClick(View v) {
		TextView textviewShow1 = (TextView)findViewById(R.id.textviewShow1);
		String strResult = null;
		String[] Result = null;
		switch (v.getId()) {
		case R.id.button1:  //byte����ת����16�����ַ�����
			textviewShow1.setText("");
			Result = bytesToHexStrings(mDataByte);
			for (int i = 0; i < Result.length; i++) {
				String string = Result[i];
				textviewShow1.append(string);
			}
			break;
		case R.id.button2: //byte����ת����16�����ַ���
			strResult  = bytesToHexString(mDataByte);
			textviewShow1.setText(strResult);
			break;
		case R.id.button3: // ʮ�����Ƶ��ַ���ת��ΪString
			strResult  = decode(bytesToHexString(mDataByte));
			textviewShow1.setText(strResult);
			break;	
		case R.id.button4: //  byte����ת����ʮ��������ʾ
			strResult  = getHexString(mDataByte,mDataByte.length);
			textviewShow1.setText(strResult);
			break;	
		case R.id.button5: //  ʮ�����Ƶ��ַ���ת����byte����	
			String strCommand = "68 04 04 68 35 FD 50 00 A0 16";
			byte [] byteData = HexCommandtoByte(strCommand.getBytes());
			textviewShow1.setText("���ת������������ѵ��Բ鿴��");
			break;
		default:
			break;
		}
	}

}
