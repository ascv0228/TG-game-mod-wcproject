package tg.ck.menu;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.net.Uri;
import android.os.Build;
import android.os.Handler;
import android.os.Looper;
import android.os.Process;
import android.provider.Settings;

public class StaticActivity {

    /*static{
        // When you change the lib name, change also on Android.mk file
        // Both must have same name
        System.loadLibrary("tg");
    }*/

    public static void Start(final Context context) {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M && !Settings.canDrawOverlays(context)) {
            AlertDialog alertDialog = new AlertDialog.Builder(context)
                    .setTitle("警告")
                    .setMessage("沒有懸浮權限選單將無法顯示, 請先進入系統設置裡開啟!")
                    .setPositiveButton("是", new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int which) {
                            context.startActivity(new Intent(Settings.ACTION_MANAGE_OVERLAY_PERMISSION,
                                    Uri.parse("package:" + context.getPackageName())));
                            Process.killProcess(Process.myPid());
                        }
                    })
                    .setNegativeButton("否", new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int which) {
                            Process.killProcess(Process.myPid());
                        }
                    }).setCancelable(false)
                    .create();
            alertDialog.show();
        } else {
            Handler handler = new Handler(Looper.getMainLooper());
            handler.postDelayed(new Runnable() {

                @Override
                public void run() {
                    context.startService(new Intent(context, FloatingModMenuService.class));
                }
            }, 500);
        }
    }
}
