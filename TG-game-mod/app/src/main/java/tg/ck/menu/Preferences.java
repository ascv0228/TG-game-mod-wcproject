package tg.ck.menu;

import android.content.Context;
import android.content.SharedPreferences;

//TODO
//Write up android logcat on readme

public class Preferences {
    private static SharedPreferences.Editor editor;
    public static Context context;
    public static boolean isSoundEnabled = false, savePref = false, animation = false, expanded = false;

    public static native void Changes(Context context, int feature, int value, boolean bool, String str);

    public static native String Export();

    public static void changeFeatureInt(String feature, int featureNum, int value) {
        Changes(context, featureNum, value, false, feature);
        editor.putInt(String.valueOf(featureNum), value).apply();
    }

    public static void changeFeatureBoolean(String feature, int featureNum, boolean value) {
        if (featureNum == 1000)
            isSoundEnabled = value;
        if (featureNum == 1001)
            animation = value;
        if (featureNum == 1002)
            expanded = value;
        if (featureNum == 9998)
            savePref = value;
        Changes(context, featureNum, 0, value, feature);

        editor.putBoolean(String.valueOf(featureNum), value).apply();
    }

    //TODO: changeFeatureString

    public static int loadPrefInt(int featureNum, int val) {
        if (savePref) {
            try {
                SharedPreferences preferences = context.getSharedPreferences("TechGG_Mod", 0);
                editor = preferences.edit();
                return preferences.getInt(String.valueOf(featureNum), val);
            } catch (ClassCastException ex) {
                return val;
            }
        }
        return val;
    }

    public static boolean loadPrefBoolean(int featureNum) {
        if (savePref || featureNum >= 1000 && featureNum < 7878 || featureNum > 8787) {
            try {
                SharedPreferences preferences = context.getSharedPreferences("TechGG_Mod", 0);
                editor = preferences.edit();
                if (featureNum == 9998 && !preferences.contains("9998"))
                    return true;
                return preferences.getBoolean(String.valueOf(featureNum), false);
            } catch (ClassCastException ex) {
                return false;
            }
        }
        return false;
    }
}
