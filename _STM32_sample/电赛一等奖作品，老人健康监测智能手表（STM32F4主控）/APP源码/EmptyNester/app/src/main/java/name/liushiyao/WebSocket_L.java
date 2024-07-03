package name.liushiyao;

import android.util.Log;

import org.java_websocket.client.WebSocketClient;
import org.java_websocket.drafts.Draft;
import org.java_websocket.handshake.ServerHandshake;

import java.io.Serializable;
import java.net.URI;

/**
 * Created by 电子小孩 on 2016/8/10.
 */
public class WebSocket_L extends WebSocketClient implements Serializable{

    public WebSocket_L(URI serverURI) {
        super(serverURI);
    }

    public WebSocket_L(URI serverUri, Draft draft) {
        super(serverUri, draft);
    }


    @Override
    public void onClose(final int code, String arg1, boolean arg2)
    {
                System.out.println("与服务器[" + getURI() + "]断开连接" + "]n");
                System.out.println("状态吗[" + code + "]\n");
                Log.e("wsdDbug", "与服务器[" + getURI() + "]断开连接" + "]n");
                Log.e("wsDbug", "状态吗[" + code + "]\n");
    }

    @Override
    public void onError(Exception e)
    {
        System.out.println("连接["+getURI()+"]服务器发生异常"+"]\n");
        System.out.println("异常原因：["+e+"]\n");

    }

    @Override
    public void onMessage(final String message)
    {

                System.out.println("获取服务器的信息["+message+"]\n");
                Log.e("wsDbug", "获取服务器的信息["+message+"]\n");

    }

    @Override
    public void onOpen(ServerHandshake arg0)
    {
                System.out.println("成功连接到服务器["+getURI()+"]\n");
                Log.e("webDbug", "成功连接到服务器["+getURI()+"]\n");
    }

}
