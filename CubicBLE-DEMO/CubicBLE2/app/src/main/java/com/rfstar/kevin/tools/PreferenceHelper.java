package com.rfstar.kevin.tools;

import android.content.Context;
import android.content.SharedPreferences;

public class PreferenceHelper {
    public static String APP_CONFIGURE_NAME = "app_configure";
//
//    public static String HAVE_CONNECTED_DEVICES = "have_connected_devices";
//    public static String LAST_CONNECTED_DEVICE = "last_connected_device";
//
//    public static String USER_ID = "userid"; // 用户id
//    public static String USER_NAME = "user_name";
//    public static String USER_PASSWORD = "user_password";
//    public static String USER_PHONE_NUMBER = "user_phone_number";
//    public static String USER_PICTURE_PATH = "user_picture_PATH";
//    public static String CART_FREE = "cart_free";
//    public static String MAIN_TAB_INDEX = "main_tab_index";
//    public static String ADD_TO_CART_BROAD_CAST =  "com.rumahcantik.action.addToCart";
//    public static String CLEAR_CART_BROAD_CAST = "com.rumahcantik.action.clearCart";
//    public static String CLEAR_CART = "cart_clear";
      public static String deviceConfigureName = "cubicBle";
    /**
     * 获取配置文件
     *
     * @param ctx
     * @param param
     * @return
     */
    public static String get(Context ctx, String param) {
        SharedPreferences settings = ctx.getSharedPreferences(deviceConfigureName, 0);
        String result = settings.getString(param, "");
        return result;
    }

    public static String get(Context ctx, String param, String configureName) {
        SharedPreferences settings = ctx.getSharedPreferences(configureName, 0);
        String result = settings.getString(param, "");
        return result;
    }

    /**
     * 删除配置文件
     *
     * @param ctx
     * @param param
     */
    public static void removePreference(Context ctx, String param) {
        SharedPreferences settings = ctx.getSharedPreferences(deviceConfigureName, Context.MODE_PRIVATE);
        SharedPreferences.Editor editor = settings.edit();
        editor.remove(param);
        editor.commit();
    }

    public static void removePreference(Context ctx, String param, String configureName) {
        SharedPreferences settings = ctx.getSharedPreferences(configureName, Context.MODE_PRIVATE);
        SharedPreferences.Editor editor = settings.edit();
        editor.remove(param);
        editor.commit();
    }

    /**
     * 添加配置文件
     *
     * @param ctx
     * @param param
     * @param value
     */
    public static void setParam(Context ctx, String param, String value) {
        SharedPreferences settings = ctx.getSharedPreferences(deviceConfigureName, Context.MODE_PRIVATE);
        SharedPreferences.Editor editor = settings.edit();
        editor.putString(param, value);
        editor.commit();
    }

    public static void setParam(Context ctx, String param, String value, String configureName) {
        SharedPreferences settings = ctx.getSharedPreferences(configureName, Context.MODE_PRIVATE);
        SharedPreferences.Editor editor = settings.edit();
        editor.putString(param, value);
        editor.commit();
    }

    /**
     * 添加配置文件(boolean)
     *
     * @param ctx
     * @param param
     * @param value
     */
    public static void setBooleanParam(Context ctx, String param, boolean value) {
        SharedPreferences settings = ctx.getSharedPreferences(deviceConfigureName, Context.MODE_PRIVATE);
        SharedPreferences.Editor editor = settings.edit();
        editor.putBoolean(param, value);
        editor.commit();
    }

    public static void setBooleanParam(Context ctx, String param, boolean value, String configureName) {
        SharedPreferences settings = ctx.getSharedPreferences(configureName, Context.MODE_PRIVATE);
        SharedPreferences.Editor editor = settings.edit();
        editor.putBoolean(param, value);
        editor.commit();
    }

    /**
     * 获取配置文件(boolean)
     *
     * @param ctx
     * @param param
     * @return
     */
    public static boolean getBoolean(Context ctx, String param) {
        SharedPreferences settings = ctx.getSharedPreferences(deviceConfigureName, 0);
        boolean result = settings.getBoolean(param, false);
        return result;
    }

    public static boolean getBoolean(Context ctx, String param, String configureName) {
        SharedPreferences settings = ctx.getSharedPreferences(configureName, 0);
        boolean result = settings.getBoolean(param, false);
        return result;
    }

    /**
     * 添加配置文件(int)
     *
     * @param ctx
     * @param param
     * @param value
     */
    public static void setIntParam(Context ctx, String param, int value) {
        SharedPreferences settings = ctx.getSharedPreferences(deviceConfigureName, Context.MODE_PRIVATE);
        SharedPreferences.Editor editor = settings.edit();
        editor.putInt(param, value);
        editor.commit();
    }

    public static void setIntParam(Context ctx, String param, int value, String configureName) {
        SharedPreferences settings = ctx.getSharedPreferences(configureName, Context.MODE_PRIVATE);
        SharedPreferences.Editor editor = settings.edit();
        editor.putInt(param, value);
        editor.commit();
    }

    /**
     * 获取配置文件(int)
     *
     * @param ctx
     * @param param
     * @return
     */
    public static int getInt(Context ctx, String param) {
        SharedPreferences settings = ctx.getSharedPreferences(deviceConfigureName, 0);
        int result = settings.getInt(param, 0);
        return result;
    }

    public static int getInt(Context ctx, String param, String configureName) {
        SharedPreferences settings = ctx.getSharedPreferences(configureName, 0);
        int result = settings.getInt(param, 0);
        return result;
    }

    /**
     * 添加配置文件(float)
     *
     * @param ctx
     * @param param
     * @param value
     */
    public static void setFloatParam(Context ctx, String param, float value) {
        SharedPreferences settings = ctx.getSharedPreferences(deviceConfigureName, Context.MODE_PRIVATE);
        SharedPreferences.Editor editor = settings.edit();
        editor.putFloat(param, value);
        editor.commit();
    }

    public static void setFloatParam(Context ctx, String param, float value, String configureName) {
        SharedPreferences settings = ctx.getSharedPreferences(configureName, Context.MODE_PRIVATE);
        SharedPreferences.Editor editor = settings.edit();
        editor.putFloat(param, value);
        editor.commit();
    }





}
