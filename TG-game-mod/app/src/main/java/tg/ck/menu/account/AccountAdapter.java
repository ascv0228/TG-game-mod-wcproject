package tg.ck.menu.account;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.res.ColorStateList;
import android.graphics.Color;
import android.graphics.PorterDuff;
import android.graphics.PorterDuffColorFilter;
import android.graphics.drawable.Drawable;
import android.os.Build;
import android.util.TypedValue;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;

import java.lang.reflect.Field;
import java.util.List;
import java.util.concurrent.CountDownLatch;

import tg.ck.Utils;

public class AccountAdapter extends BaseAdapter {
    public interface IAccountCallback {
        void invoke(String tag);
    }

    private class ViewHolder {
        public TextView textView;
        public CheckBox checkBox;
    }

    private boolean mcheckBoxStatus;
    private List<AccountViewData> mdatas;
    private IAccountCallback mcallback;

    public AccountAdapter(List<AccountViewData> datas, IAccountCallback action) {
        mdatas = datas;
        mcheckBoxStatus = false;
        mcallback = action;
    }

    @Override
    public int getCount() {
        return mdatas.size();
    }

    @Override
    public Object getItem(int position) {
        return mdatas.get(position);
    }

    @Override
    public long getItemId(int position) {
        return position;
    }

    @SuppressLint("ViewHolder")
    @Override
    public View getView(final int position, View convertView, final ViewGroup parent) {
        final AccountViewData acc = mdatas.get(position);

        final Context context = parent.getContext();
        final ViewHolder viewHolder;
        if(convertView == null) {
            final LinearLayout linearLayout = new LinearLayout(context);
            TypedValue outValue = new TypedValue();
            context.getTheme().resolveAttribute(
                    android.R.attr.selectableItemBackground, outValue, true);
            linearLayout.setBackgroundResource(outValue.resourceId);
            linearLayout.setOrientation(LinearLayout.HORIZONTAL);
            linearLayout.setLayoutParams(
                    new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, Utils.dpToPx(45, context)));

            final TextView textView = new TextView(context);
            textView.setPadding(30, 0, 0, 0);
            textView.setTextSize(27);
            textView.setLayoutParams(
                    new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.MATCH_PARENT, 1));
            textView.setGravity(Gravity.CENTER_VERTICAL);
            textView.setTextColor(Color.parseColor("#84C1FF"));

            final CheckBox checkbox = new CheckBox(context);
            checkbox.setFocusable(false);
            checkbox.setLayoutParams(
                    new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.MATCH_PARENT, 5));

            // 低版本使用反射設置CheckBox按紐顏色
            if(Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
                checkbox.setButtonTintList(ColorStateList.valueOf(Color.parseColor("#ACD6FF")));
            } else {
                try {
                    Field mButtonDrawable = CompoundButton.class.getDeclaredField("mButtonDrawable");
                    mButtonDrawable.setAccessible(true);
                    Drawable buttonDrawable = (Drawable)mButtonDrawable.get(checkbox);
                    buttonDrawable.setColorFilter(
                            new PorterDuffColorFilter(Color.parseColor("#ACD6FF"), PorterDuff.Mode.SRC_IN));
                } catch (Exception ex) {
                    ex.printStackTrace();
                }
            }

            linearLayout.addView(textView);
            linearLayout.addView(checkbox);

            viewHolder = new ViewHolder();
            viewHolder.textView = textView;
            viewHolder.checkBox = checkbox;

            convertView = linearLayout;
            convertView.setTag(viewHolder);
        } else {
            viewHolder = (ViewHolder) convertView.getTag();
        }

        convertView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                mcallback.invoke(acc.tag);
            }
        });

        convertView.setOnLongClickListener(new View.OnLongClickListener() {
            @Override
            public boolean onLongClick(View v) {

                Toast.makeText(context, "長按暫時沒東西啦 (ﾟ∀ﾟ)b", Toast.LENGTH_SHORT).show();

                return true;
            }
        });

        viewHolder.textView.setText(acc.tag);

        viewHolder.checkBox.setVisibility(mcheckBoxStatus ? View.VISIBLE : View.INVISIBLE);
        viewHolder.checkBox.setChecked(acc.isChecked);
        viewHolder.checkBox.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                acc.isChecked = viewHolder.checkBox.isChecked();
            }
        });

        return convertView;
    }

    public void setStatus(boolean isShow) {
        mcheckBoxStatus = isShow;
    }
}
