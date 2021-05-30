package tg.ck.reflection;

import android.util.Log;

import java.lang.reflect.Method;

import dalvik.system.DexFile;

public class ReflectionUtils {

    private static Method setHiddenApiExemptions;
    private static Object vmRuntime;

    static {
        try {
            Method getMethodMethod = Class.class.getDeclaredMethod("getDeclaredMethod", String.class, Class[].class);
            Method forNameMethod = Class.class.getDeclaredMethod("forName", String.class);

            Class vmRuntimeClass = (Class) forNameMethod.invoke(null, "dalvik.system.VMRuntime");
            setHiddenApiExemptions = (Method) getMethodMethod.invoke(vmRuntimeClass, "setHiddenApiExemptions", new Class[]{String[].class});

            Method getVMRuntimeMethod = (Method) getMethodMethod.invoke(vmRuntimeClass, "getRuntime", null);
            vmRuntime = getVMRuntimeMethod.invoke(null);
        } catch (Exception e) {
            Log.d("TechGG", "error get methods", e);
        }
    }

    public static boolean passByWhiteList(String... memberSigs) {
        if (vmRuntime == null || setHiddenApiExemptions == null) {
            return false;
        }

        try {
            setHiddenApiExemptions.invoke(vmRuntime, new Object[] {memberSigs});
            return true;
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }
    }

    public static boolean passAll()  {
        return passByWhiteList("L");
    }
}