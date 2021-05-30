package tg.ck.menu.account;

public class AccountViewData {
    String tag;
    boolean isChecked;

    public AccountViewData(String tag) {
        this.tag = tag;
        this.isChecked = false;
    }

    @Override
    public boolean equals(Object obj) {
        if(obj instanceof AccountViewData) {
            AccountViewData acc = (AccountViewData) obj;
            return tag.equals(acc.tag);
        }
        return false;
    }
}