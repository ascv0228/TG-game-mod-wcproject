## 流程
___
1. 反編譯遊戲apk
2. 將此專案編譯成apk後使用工具反編譯此apk
3. 確保反編譯出的遊戲和此專案反編譯出的`lib`資料夾內具有完全相同的資料夾
4. 將此專案反編譯出的`smali`和`lib`資料夾直接拉入反編譯出的遊戲目錄中
5. 修改xml
6. 重新編譯遊戲

## 修改xml
___
1. 添加懸浮權限
```xml
<uses-permission android:name="android.permission.SYSTEM_ALERT_WINDOW"/>
```

2. 添加讀寫權限
```xml
<uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE"/>
<uses-permission android:name="android.permission.READ_EXTERNAL_STORAGE"/>
```

3. 添加 Android 9 以上的讀寫權限
```xml
<!--在 application 中加入 android:requestLegacyExternalStorage="true" -->
<application android:requestLegacyExternalStorage="true" ...
```

4. 添加懸浮菜單和啟動頁
```xml
<service android:enabled="true" android:exported="false" android:name="tg.ck.menu.FloatingModMenuService" android:stopWithTask="true"/>
<activity android:configChanges="keyboardHidden|orientation|screenSize" android:name="tg.ck.menu.MainActivity">
    <intent-filter>
        <action android:name="android.intent.action.MAIN"/>
        <category android:name="android.intent.category.LAUNCHER"/>
    </intent-filter>
</activity>
```