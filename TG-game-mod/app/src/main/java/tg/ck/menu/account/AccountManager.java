package tg.ck.menu.account;

import android.annotation.SuppressLint;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.graphics.Color;
import android.graphics.PorterDuff;
import android.graphics.PorterDuffColorFilter;
import android.graphics.drawable.Drawable;
import android.graphics.drawable.GradientDrawable;
import android.os.Build;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import java.lang.reflect.Field;
import java.util.ArrayList;
import java.util.List;
import java.util.Objects;

import tg.ck.Utils;

public class AccountManager implements AccountAdapter.IAccountCallback {

    enum ManagerStatus {
        NORMAL,
        DELETE
    }

    enum CheckerStatus {
        NEW_ACCOUNT,
        LOAD_ACCOUNT
    }

    enum ActionStatus {
        SUCCESS,
        ADD_ERROR,
        NATIVE_ADD_ERROR,
        NATIVE_SAVE_ERROR,
        NATIVE_ACCESS_ERROR,
        NATIVE_UNKOWN_ERROR
    }

    private ManagerStatus mstatus;
    private AlertDialog manager;
    private AlertDialog saver;
    private AlertDialog checker;
    private List<AccountViewData> accounts;
    private String maccountTag;

    private void initAccountData() {
        String[] accs = loadAccountData();
        accounts = new ArrayList<>();
        for (int i = 0; i < accs.length; ++i) {
            accounts.add(new AccountViewData(accs[i]));
        }
    }

    private void initManager(final Context context) {
        initAccountData();

        TextView title = new TextView(context);
        title.setText("帳戶清單");
        title.setPadding(20, 20, 20, 20);
        title.setTextSize(20);
        title.setBackgroundColor(Color.parseColor("#FCFCFC"));
        title.setTextColor(Color.parseColor("#66B3FF"));

        manager = new AlertDialog.Builder(context)
                .setAdapter(new AccountAdapter(accounts, this), null)
                .setNeutralButton("刪除帳戶", null)
                .setPositiveButton("保存當前帳戶", null)
                .setCustomTitle(title)
                .create();

        final ListView listView = manager.getListView();

        Drawable verticalTrack = null;
        Drawable verticalThumb = null;

        // 低版本使用反射設置ListView的滾動條顏色
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.Q) {
            verticalTrack = listView.getVerticalScrollbarTrackDrawable();
            verticalThumb = listView.getVerticalScrollbarThumbDrawable();
        } else {
            try {
                Field mScrollCacheField = View.class.getDeclaredField("mScrollCache");
                mScrollCacheField.setAccessible(true);
                Object mScrollCache = mScrollCacheField.get(listView);

                Field scrollBarField = mScrollCache.getClass().getDeclaredField("scrollBar");
                scrollBarField.setAccessible(true);
                Object scrollBar = scrollBarField.get(mScrollCache);

                Field verticalTrackField = scrollBar.getClass().getDeclaredField("mVerticalTrack");
                verticalTrackField.setAccessible(true);
                verticalTrack = (Drawable) verticalTrackField.get(scrollBar);

                Field mVerticalThumbField = scrollBar.getClass().getDeclaredField("mVerticalThumb");
                mVerticalThumbField.setAccessible(true);
                verticalThumb = (Drawable) mVerticalThumbField.get(scrollBar);
            } catch (Exception ex) {
                ex.printStackTrace();
            }
        }

        if(verticalTrack != null) {
            verticalTrack.setColorFilter(new PorterDuffColorFilter(Color.parseColor("#66B3FF"), PorterDuff.Mode.SRC_IN));
        }

        if(verticalThumb != null) {
            verticalThumb.setColorFilter(new PorterDuffColorFilter(Color.parseColor("#66B3FF"), PorterDuff.Mode.SRC_IN));
        }

        listView.addOnLayoutChangeListener(new View.OnLayoutChangeListener() {
            private final int MAX_HEIGHT = Utils.dpToPx(215, context);

            @Override
            public void onLayoutChange(View v, int left, int top, int right, int bottom, int oldLeft, int oldTop, int oldRight, int oldBottom) {
                int height = v.getHeight();
                if(height > MAX_HEIGHT){
                    listView.setLayoutParams(
                            new FrameLayout.LayoutParams(FrameLayout.LayoutParams.MATCH_PARENT, MAX_HEIGHT));
                }
            }
        });

        GradientDrawable drawable = new GradientDrawable();
        drawable.setCornerRadius(10);
        drawable.setColor(Color.parseColor("#EEEDED"));
        manager.getWindow().setBackgroundDrawable(drawable);

        manager.setOnShowListener(new DialogInterface.OnShowListener() {
            private AccountAdapter accAdapter;
            private Button neutral_btn;
            private Button positive_btn;

            private void SwitchManagerStatus(ManagerStatus status) {
                switch(status) {
                    case NORMAL:
                        accAdapter.setStatus(false);
                        for(AccountViewData acc : accounts) {
                            acc.isChecked = false;
                        }
                        positive_btn.setText("保存當前帳戶");
                        neutral_btn.setText("刪除帳戶");
                        break;
                    case DELETE:
                        accAdapter.setStatus(true);
                        positive_btn.setText("刪除");
                        neutral_btn.setText("取消操作");
                        break;
                }
                mstatus = status;

                accAdapter.notifyDataSetChanged();
            }

            @Override
            public void onShow(DialogInterface dialog) {
                AlertDialog alertDialog = (AlertDialog) dialog;

                accAdapter = (AccountAdapter)listView.getAdapter();
                neutral_btn = alertDialog.getButton(AlertDialog.BUTTON_NEUTRAL);
                neutral_btn.setTextColor(Color.parseColor("#84C1FF"));
                positive_btn = alertDialog.getButton(AlertDialog.BUTTON_POSITIVE);
                positive_btn.setTextColor(Color.parseColor("#84C1FF"));

                SwitchManagerStatus(ManagerStatus.NORMAL);

                neutral_btn.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        if(mstatus == ManagerStatus.NORMAL) {
                            SwitchManagerStatus(ManagerStatus.DELETE);
                        } else if(mstatus == ManagerStatus.DELETE) {
                            SwitchManagerStatus(ManagerStatus.NORMAL);
                        }
                    }
                });

                positive_btn.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        if(mstatus == ManagerStatus.NORMAL) {
                            saver.show();
                        } else if(mstatus == ManagerStatus.DELETE) {
                            List<AccountViewData> tempData = new ArrayList<>();
                            for(AccountViewData acc : accounts) {
                                if(acc.isChecked) tempData.add(acc);
                            }

                            String tags[] = new String[tempData.size()];
                            for(int i = 0; i < tags.length; ++i){
                                tags[i] = tempData.get(i).tag;
                            }

                            if(tags.length != 0) {
                                int deleted = deleteSelected(tags);

                                String msg = "發生未知錯誤!";

                                if(deleted == tags.length) {
                                    msg = "已刪除所有已選帳戶!";
                                    accounts.removeAll(tempData);
                                } else if (deleted >= 0 && deleted < tags.length) {
                                    msg = "發生一些錯誤, 只刪除了前" + deleted + "個已選帳戶!";
                                    accounts.removeAll(tempData.subList(0, deleted));
                                } else if (deleted < 0) {
                                    msg = "保存失敗, 未知錯誤!";
                                }

                                Toast.makeText(context, msg, Toast.LENGTH_SHORT).show();
                            }
                        }
                        accAdapter.notifyDataSetChanged();
                    }
                });
            }
        });

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT) {
            Objects.requireNonNull(manager.getWindow()).setType(Build.VERSION.SDK_INT >= 26 ? 2038 : 2002);
        }
    }

    private void initSaver(final Context context) {
        final LinearLayout linearLayout = new LinearLayout(context);

        final EditText editText = new EditText(context);
        editText.setHint("輸入保存名稱");
        editText.setHintTextColor(Color.parseColor("#B4D8FC"));
        editText.setTextColor(Color.parseColor("#84C1FF"));
        editText.getBackground().setColorFilter(new PorterDuffColorFilter(Color.parseColor("#ACD6FF"), PorterDuff.Mode.SRC_ATOP));

        // 低版本使用反射設置EditText的光標顏色
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.Q) {
            Drawable cursorDrawable = editText.getTextCursorDrawable();
            cursorDrawable.setColorFilter(new PorterDuffColorFilter(Color.parseColor("#84C1FF"), PorterDuff.Mode.SRC_IN));
        } else {
            try {
                Field mCursorDrawableResField = TextView.class.getDeclaredField("mCursorDrawableRes");
                mCursorDrawableResField.setAccessible(true);
                int cursorDrawableResId = mCursorDrawableResField.getInt(editText);

                Field mEditorField = TextView.class.getDeclaredField("mEditor");
                mEditorField.setAccessible(true);
                Object editor = mEditorField.get(editText);

                Drawable tintDrawable;
                Context editTextContext = editText.getContext();
                if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
                    tintDrawable = editTextContext.getDrawable(cursorDrawableResId);
                } else {
                    tintDrawable = editTextContext.getResources().getDrawable(cursorDrawableResId);
                }

                tintDrawable.setColorFilter(new PorterDuffColorFilter(Color.parseColor("#84C1FF"), PorterDuff.Mode.SRC_IN));
                Drawable[] drawables = {tintDrawable, tintDrawable};

                Field mCursorDrawableField;
                if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
                    mCursorDrawableField = editor.getClass().getDeclaredField("mDrawableForCursor");
                    mCursorDrawableField.setAccessible(true);
                    mCursorDrawableField.set(editor, tintDrawable);
                } else {
                    mCursorDrawableField = editor.getClass().getDeclaredField("mCursorDrawable");
                    mCursorDrawableField.setAccessible(true);
                    mCursorDrawableField.set(editor, drawables);
                }
            } catch (Exception ex) {
                ex.printStackTrace();
            }
        }

        editText.setMaxLines(1);
        editText.setTextSize(20.0f);
        editText.setLayoutParams(
                new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.MATCH_PARENT));

        linearLayout.addView(editText);
        saver = new AlertDialog.Builder(context)
                .setCancelable(false)
                .setNeutralButton("取消", null)
                .setPositiveButton("保存", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        String text = editText.getText().toString().trim();
                        if(text.isEmpty()) {
                            Toast.makeText(context, "請輸入非空名稱!", Toast.LENGTH_SHORT).show();
                        } else {
                            String msg = "發生未知錯誤!";
                            switch (addAccount(text)) {
                                case SUCCESS:
                                    msg = "保存完畢!";
                                    ((AccountAdapter)manager.getListView().getAdapter()).notifyDataSetChanged();
                                    break;
                                case ADD_ERROR:
                                    msg = "新增失敗, 發現相同名稱!";
                                    break;
                                case NATIVE_ADD_ERROR:
                                    msg = "新增失敗, 未知錯誤!";
                                    break;
                                case NATIVE_SAVE_ERROR:
                                    msg = "保存失敗, 未知錯誤!";
                                    break;
                                case NATIVE_ACCESS_ERROR:
                                    msg = "操作出現問題, 請稍後再試!";
                                    break;
                            }
                            Toast.makeText(context, msg, Toast.LENGTH_SHORT).show();
                        }
                    }
                })
                .setView(linearLayout)
                .create();

        GradientDrawable drawable = new GradientDrawable();
        drawable.setCornerRadius(10);
        drawable.setColor(Color.parseColor("#EEEDED"));
        saver.getWindow().setBackgroundDrawable(drawable);

        saver.setOnShowListener(new DialogInterface.OnShowListener() {
            private Button neutral_btn;
            private Button positive_btn;

            @Override
            public void onShow(DialogInterface dialog) {
                AlertDialog alertDialog = (AlertDialog) dialog;

                neutral_btn = alertDialog.getButton(AlertDialog.BUTTON_NEUTRAL);
                neutral_btn.setTextColor(Color.parseColor("#84C1FF"));
                positive_btn = alertDialog.getButton(AlertDialog.BUTTON_POSITIVE);
                positive_btn.setTextColor(Color.parseColor("#84C1FF"));

                editText.setText("");
            }
        });

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT) {
            Objects.requireNonNull(saver.getWindow()).setType(Build.VERSION.SDK_INT >= 26 ? 2038 : 2002);
        }
    }

    private void initChecker(final Context context) {
        LinearLayout linearLayout = new LinearLayout(context);

        TextView message = new TextView(context);
        message.setPadding(30, 30, 0, 0);
        message.setText("繼續執行?");
        message.setTextSize(17);
        message.setTextColor(Color.parseColor("#66B3FF"));

        linearLayout.addView(message);

        checker = new AlertDialog.Builder(context)
                .setView(linearLayout)
                .setNeutralButton("取消", null)
                .setPositiveButton("確定", new DialogInterface.OnClickListener() {

                    private void switchCheckerStatus(CheckerStatus checkerStatus) {
                        switch (checkerStatus) {
                            case NEW_ACCOUNT:
                                if(newAccount()) {
                                    hide();
                                } else {
                                    Toast.makeText(context, "操作出現問題, 請稍後再試!", Toast.LENGTH_SHORT).show();
                                }
                                break;
                            case LOAD_ACCOUNT:
                                if(loadSelected(maccountTag)) {
                                    hide();
                                } else {
                                    Toast.makeText(context, "操作出現問題, 請稍後再試!", Toast.LENGTH_SHORT).show();
                                }
                                break;
                        }
                    }

                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        if(maccountTag == null || maccountTag.isEmpty()) {
                            switchCheckerStatus(CheckerStatus.NEW_ACCOUNT);
                        } else {
                            switchCheckerStatus(CheckerStatus.LOAD_ACCOUNT);
                        }
                    }
                })
                .create();

        GradientDrawable drawable = new GradientDrawable();
        drawable.setCornerRadius(10);
        drawable.setColor(Color.parseColor("#EEEDED"));
        checker.getWindow().setBackgroundDrawable(drawable);

        checker.setOnShowListener(new DialogInterface.OnShowListener() {
            private Button neutral_btn;
            private Button positive_btn;

            @Override
            public void onShow(DialogInterface dialog) {
                AlertDialog alertDialog = (AlertDialog) dialog;

                neutral_btn = alertDialog.getButton(AlertDialog.BUTTON_NEUTRAL);
                neutral_btn.setTextColor(Color.parseColor("#84C1FF"));
                positive_btn = alertDialog.getButton(AlertDialog.BUTTON_POSITIVE);
                positive_btn.setTextColor(Color.parseColor("#84C1FF"));
            }
        });

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT) {
            Objects.requireNonNull(checker.getWindow()).setType(Build.VERSION.SDK_INT >= 26 ? 2038 : 2002);
        }
    }

    public AccountManager(Context context) {
        initManager(context);
        initSaver(context);
        initChecker(context);
    }

    public ActionStatus addAccount(String tag) {
        AccountViewData acc = new AccountViewData(tag);

        if(!accounts.contains(acc)) {
            switch (saveCurrent(tag)) {
                case 0:
                    accounts.add(acc);
                    return ActionStatus.SUCCESS;
                case 1:
                    return ActionStatus.NATIVE_ADD_ERROR;
                case 2:
                    return ActionStatus.NATIVE_SAVE_ERROR;
                case 3:
                    return ActionStatus.NATIVE_ACCESS_ERROR;
                default:
                    return ActionStatus.NATIVE_UNKOWN_ERROR;
            }
        }

        return ActionStatus.ADD_ERROR;
    }

    public void show() {
        manager.show();
    }

    public void hide() {
        manager.cancel();
        saver.cancel();
        checker.cancel();
    }

    public void goTitle() {
        maccountTag = "";
        checker.show();
    }

    @Override
    public void invoke(String tag) {
        maccountTag = tag;
        checker.show();
    }

    public native boolean newAccount();

    public native int deleteSelected(String[] tags);

    public native int saveCurrent(String tag);

    public native boolean loadSelected(String tag);

    public native String[] loadAccountData();
}
