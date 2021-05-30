package tg.ck;

import android.content.Context;
import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Method;

public class MApplication extends
        //tw.sonet.wcp.ColoplApplication
        jp.colopl.wcat.ColoplApplication
        implements InvocationHandler {

    //public static String GameActivity = "tw.sonet.wcp.StartActivity";
    public static String GameActivity = "jp.colopl.wcat.StartActivity";

    static{
        System.loadLibrary("TechGG");
    }

    @Override
    public native void attachBaseContext(Context base);

    @Override
    public native Object invoke(Object proxy, Method method, Object[] args);
}