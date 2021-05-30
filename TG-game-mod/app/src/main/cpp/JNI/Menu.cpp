#include <jni.h>
#include <unistd.h>

#include <string>

#include <Utils.h>
#include <base64.hpp>
#include <obfuscate.hpp>

#include "data/Sounds.h"
#include "../Const/Const_General.h"

bool titleValid = false, headingValid = false, iconValid = false;

//Anti-leech: Check if text has been changed via java/smali
void *antiLeech(void *)
{
    sleep(15);
    if (!titleValid || !headingValid || !iconValid)
    {
        //Bad function to make it crash
        int32_t *p = 0;
        *p = 0;
    }
    return NULL;
}

void JNI_LoadSounds(JNIEnv *env, jclass clazz, jstring directory)
{
    std::string dir = env->GetStringUTFChars(directory, 0);

    writeToFile(dir + "On.ogg", base64::from_base64(Sounds::On));
    writeToFile(dir + "Off.ogg", base64::from_base64(Sounds::Off));
    writeToFile(dir + "Back.ogg", base64::from_base64(Sounds::Back));
    writeToFile(dir + "OpenMenu.ogg", base64::from_base64(Sounds::OpenMenu));
    writeToFile(dir + "Select.ogg", base64::from_base64(Sounds::Select));
    writeToFile(dir + "SliderDecrease.ogg", base64::from_base64(Sounds::SliderDecrease));
    writeToFile(dir + "SliderIncrease.ogg", base64::from_base64(Sounds::SliderIncrease));
}

jstring JNI_Title(JNIEnv *env, jobject thiz)
{
    titleValid = true;
    return env->NewStringUTF(OBFUSCATE("TechGG Mod"));
}

jstring JNI_Heading(JNIEnv *env, jobject thiz)
{
    headingValid = true;
    return env->NewStringUTF(OBFUSCATE(""));
}

jstring JNI_Icon(JNIEnv *env, jobject thiz)
{
    iconValid = true;

    //Use https://www.base64encode.org/ to encode your image to base64
    return env->NewStringUTF(Const::General::head_icon.c_str());
}

jstring JNI_IconWebViewData(JNIEnv *env, jobject thiz)
{
    iconValid = true;

    //Upload your image or gif on imgur.com or other sites
    //To disable it:
    //return NULL
    //It would load the base64 icon from above as usual

    //From internet (Requires android.permission.INTERNET)
    // return env->NewStringUTF("https://i.imgur.com/SujJ85j.gif");

    //From assets folder: (Requires android.permission.INTERNET)
    // return env->NewStringUTF("file:///android_asset/example.gif");

    //Base64 html:
    // return env->NewStringUTF("data:image/png;base64, <encoded base64 here>");

    //Nothing:
    // return NULL

    //return env->NewStringUTF("https://i.imgur.com/SujJ85j.gif");
    return NULL;
}

jobjectArray JNI_getFeatureList(JNIEnv *env, jobject activityObject)
{
    jobjectArray ret;

    // Note: Do not translate the first text

    // Usage:

    // Category_(text)
    // Toggle_(feature name)
    // SeekBar_(feature name)_(min value)_(max value)
    // Spinner_(feature name)_(Items e.g. item1,item2,item3)
    // Button_(feature name)
    // Button_OnOff_(feature name)
    // InputValue_(feature name)
    // RichTextView_(Text with limited HTML support)
    // RichWebView_(Full HTML support)

    // Learn more about HTML https://www.w3schools.com/

    const char *features[] = {
        OBFUSCATE("Toggle_總開關"),
        OBFUSCATE("Toggle_直接勝利"),
        OBFUSCATE("Toggle_跳過劇情"),
        OBFUSCATE("ID-11_Toggle_跳過教學"),
        OBFUSCATE("SeekBar_落物倍率_1_100"),
        OBFUSCATE("Toggle_怪自滅"),
        OBFUSCATE("Toggle_無傷"),
        OBFUSCATE("SeekBar_五圍倍率_1_100"),
        OBFUSCATE("SeekBar_跑速倍率_1_10"),
        OBFUSCATE("Toggle_上帝模式"),
        OBFUSCATE("Toggle_無限SP"),
        OBFUSCATE("Toggle_傷害隱藏"),
        OBFUSCATE("ID-20_SeekBar_注入類型_0_6"),
        OBFUSCATE("ID-21_InputValue_注入ID"),
        OBFUSCATE("ID-22_SeekBar_注入數量_0_100"),
        //OBFUSCATE("SeekBar_結算時間增加(s)_0_120"),
        //OBFUSCATE("Button_顯示"),
        //OBFUSCATE("SeekBar_Kittymemory slider example_0_5"),
        //OBFUSCATE("Spinner_The spinner_Items 1,Items 2,Items 3"),
        //OBFUSCATE("Button_The button"),
        //OBFUSCATE("Button_OnOff_The On/Off button"),
        //OBFUSCATE("InputValue_The input number"),
        //OBFUSCATE(
        //        "RichTextView_This is text view, not fully HTML. <b>Bold</b> <i>italic</i> <u>underline</u><br />New line <font color='red'>Support colors</font>"),
        /*OBFUSCATE(
                    "RichWebView_<html><head><style>body{color: white;}</style></head><body>"
                    "This is WebView, with REAL HTML support!"
                    "<div style=\"background-color: darkblue; text-align: center;\">Support CSS</div>"
                    "<marquee style=\"color: green; font-weight:bold;\" direction=\"left\" scrollamount=\"5\" behavior=\"scroll\">This is <u>scrollable</u> text</marquee>"
                    "</body></html>")*/
    };

    int32_t Total_Feature = (sizeof features /
                             sizeof features[0]); //Now you dont have to manually update the number everytime;
    ret = (jobjectArray)
              env->NewObjectArray(Total_Feature, env->FindClass(OBFUSCATE("java/lang/String")),
                                  env->NewStringUTF(""));
    int32_t i;
    for (i = 0; i < Total_Feature; i++)
        env->SetObjectArrayElement(ret, i, env->NewStringUTF(features[i]));

    //pthread_t ptid;
    //pthread_create(&ptid, NULL, antiLeech, NULL);

    return (ret);
}