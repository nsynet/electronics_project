package name.liushiyao.utils;

import com.google.gson.Gson;
import com.google.gson.reflect.TypeToken;

import java.util.Iterator;
import java.util.LinkedList;

import name.liushiyao.User;

/**
 * 定制版
 * JsonUtils
 *
 * @author 北岭山下
 *         <p/>
 *         2016年8月9日 下午10:01:49
 */
public class JsonUtils {

    public static User parseUserFromJson(String jsonData) {

        User U = new User();
        java.lang.reflect.Type listType = new TypeToken<LinkedList<User>>() {
        }.getType();
        Gson gson = new Gson();

        LinkedList<User> users = gson.fromJson(jsonData, listType);
        for (Iterator<User> iterator = users.iterator(); iterator.hasNext(); ) {
            User user = (User) iterator.next();

            U.setAccount(user.getAccount());
            U.setPassword(user.getPassword());
            U.setName(user.getName());
            U.setAge(user.getAge());
            U.setDevice(user.getDevice());
            U.setSex(user.getSex());
            U.setMedical_history(user.getMedical_history());
            U.setAttention(user.getAttention());
            U.setHeart(user.getHeart());
            U.setDrug(user.getDrug());
            U.setDrug_time(user.getDrug_time());
            U.setFall(user.getFall());
            U.setTemper(user.getTemper());
        }
        return U;
    }

}
