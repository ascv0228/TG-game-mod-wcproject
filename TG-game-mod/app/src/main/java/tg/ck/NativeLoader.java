package tg.ck;

import android.content.Context;
import android.os.Environment;
import android.util.Log;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

import tg.ck.tinker.TinkerLoadLibrary;

public final class NativeLoader {
    private static final String HOTFIX_FOLDER = "tg_hotfix";

    public static boolean install(Context context, String libName) {
        File tmpPath = new File(
                Environment.getExternalStorageDirectory(), HOTFIX_FOLDER);
        File libsPath = new File(context.getFilesDir(), HOTFIX_FOLDER);

        if(!tmpPath.exists() && !tmpPath.mkdirs()) {
            return false;
        }

        if(!isLibraryExists(tmpPath, libName)) {
            return false;
        }

        if(!libsPath.exists() && !libsPath.mkdirs()) {
            return false;
        }

        try {
            copyLibrary(toLibFile(tmpPath, libName), toLibFile(libsPath, libName));
            TinkerLoadLibrary.installNativeLibraryPath(context.getClassLoader(), libsPath);
            System.loadLibrary(resolveLibraryName(libName, true));
            deleteLibs(libsPath);

            return true;
        } catch (Throwable th) {
            Log.d("TechGG", th.toString());
        }

        return false;
    }

    /**
     *
     * @param context
     * @param libName
     * @param isForceLoad 設置為true將會載入apk內的備份so
     * @return
     */
    public static boolean install(Context context, String libName, boolean isForceLoad) {
        boolean status = install(context, libName);

        if(!status && isForceLoad) {
            System.loadLibrary(resolveLibraryName(libName, true).concat("_bk"));
            Log.d("TechGG", "Loaded backup library -> ".concat(libName));
            return true;
        }

        return status;
    }

    private static void copyLibrary(File from, File to) throws IOException {
        byte[] buffer = new byte[0xffff];
        FileInputStream fin = new FileInputStream(from);
        ByteArrayOutputStream bArray = new ByteArrayOutputStream();
        int counter = 0;
        while((counter = fin.read(buffer)) >= 0) {
            bArray.write(buffer, 0, counter);
        }
        bArray.close();
        fin.close();

        FileOutputStream fout = new FileOutputStream(to);
        fout.write(bArray.toByteArray());
        fout.close();
    }

    private static File toLibFile(File path, String libName) {
        return new File(path, resolveLibraryName(libName));
    }

    private static boolean isLibraryExists(File path, String libName) {
        return toLibFile(path, libName).exists();
    }

    private static String resolveLibraryName(String libName) {
        if(libName.startsWith("lib") && libName.endsWith(".so")) {
            return libName;
        }

        return "lib".concat(libName).concat(".so");
    }

    private static String resolveLibraryName(String libName, boolean isTrim) {
        if(isTrim) {
            if(libName.startsWith("lib") && libName.endsWith(".so")) {
                return libName.substring(3, libName.length() - 3);
            }

            return libName;
        }

        return resolveLibraryName(libName);
    }

    private static boolean deleteLibs(File path) throws IOException {
        if(path.exists()) {
            Runtime.getRuntime().exec(new String[] {"rm", "-rf", path.getAbsolutePath()});
            return true;
        }

        return false;
    }
}
