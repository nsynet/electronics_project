package name.liushiyao;

/**
 * Created by 电子小孩 on 2016/8/9.
 */

import java.io.Serializable;

/**
 * 账号：1072915121
 名字：lsy
 年龄：21
 性别：男
 病史：content
 注意事项：content

 “account”:”1072915121”
 "password":"123456"
 “name”:”lsy”
 “age”:”21"
 “sexy”:”man”
 “medical_history”:”content”
 “attention”:”content”
 “xx2”：“null”/”xx2”
 */
public class User  implements Serializable {

    private String account;
    private String password;
    private String name;
    private String age;
    private String sex;
    private String medical_history;
    private String attention;
    private String device;

    private String temper;
    private String drug;
    private String drug_time;
    private String heart;
    private String fall;

    public String getTemper()
    {
        return temper;
    }
    public void setTemper(String temper)
    {
        this.temper = temper;
    }
    public String getDrug()
    {
        return drug;
    }
    public void setDrug(String drug)
    {
        this.drug = drug;
    }
    public String getDrug_time()
    {
        return drug_time;
    }
    public void setDrug_time(String drug_time)
    {
        this.drug_time = drug_time;
    }
    public String getHeart()
    {
        return heart;
    }
    public void setHeart(String heart)
    {
        this.heart = heart;
    }
    public String getFall()
    {
        return fall;
    }
    public void setFall(String fall)
    {
        this.fall = fall;
    }

    @Override
    public String toString() {
        return "User{" +
                "account='" + account + '\'' +
                ", password='" + password + '\'' +
                ", name='" + name + '\'' +
                ", age=" + age +
                ", sex='" + sex + '\'' +
                ", medical_history='" + medical_history + '\'' +
                ", attention='" + attention + '\'' +
                ", device='" + device + '\'' +
                ", temper='" + temper + '\'' +
                ", drug='" + drug + '\'' +
                ", drug_time='" + drug_time + '\'' +
                ", heart='" + heart + '\'' +
                ", fall='" + fall + '\'' +
                '}';
    }

    public User() {
    }

    public String getDevice() {
        return device;
    }

    public void setDevice(String device) {
        this.device = device;
    }

    public String getPassword() {
        return password;
    }

    public void setPassword(String password) {
        this.password = password;
    }

    public String getAge() {
        return age;
    }

    public void setAge(String age) {
        this.age = age;
    }

    public String getAccount() {
        return account;
    }

    public void setAccount(String account) {
        this.account = account;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getSex() {
        return sex;
    }

    public void setSex(String sex) {
        this.sex = sex;
    }

    public String getMedical_history() {
        return medical_history;
    }

    public void setMedical_history(String medical_history) {
        this.medical_history = medical_history;
    }

    public String getAttention() {
        return attention;
    }

    public void setAttention(String attention) {
        this.attention = attention;
    }


}
