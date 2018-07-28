#include "com_okcoin_vault_jni_bitcoindiamond_BitcoinDiamondj.h"
#include "wallet.h"
#include "string.h"


static char* jstring2char(JNIEnv* env, jstring jstr)
{
    jclass clsstring = env->FindClass("java/lang/String");
    jstring strencode = env->NewStringUTF("utf-8");
    jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
    jbyteArray barr= (jbyteArray)env->CallObjectMethod(jstr, mid, strencode);
    jsize alen = env->GetArrayLength(barr);
    jbyte* ba = env->GetByteArrayElements(barr, JNI_FALSE);

    char* rtn = NULL;
    if (alen > 0)
    {
        rtn = (char*)malloc(alen + 1);
        memcpy(rtn, ba, alen);
        rtn[alen] = 0;
    }
    env->ReleaseByteArrayElements(barr, ba, 0);


    return rtn;
}

static jstring char2jstring(JNIEnv* env, const char* pat)
{
    jclass strClass = env->FindClass("Ljava/lang/String;");
    jmethodID ctorID = env->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");

    jbyteArray bytes = env->NewByteArray(strlen(pat));
    env->SetByteArrayRegion(bytes, 0, strlen(pat), (jbyte*)pat);
    jstring encoding = env->NewStringUTF("utf-8");
    return (jstring)env->NewObject(strClass, ctorID, bytes, encoding);
}

static jobjectArray stringList2jobjectArray(JNIEnv* env, const std::list<std::string>& stringList)
{
    size_t size = stringList.size();
    jclass objClass = env->FindClass("java/lang/String");
    jobjectArray resArray = env->NewObjectArray(size, objClass, 0);
    jsize i = 0;
    for (auto &s: stringList) {
        env->SetObjectArrayElement(resArray, i++, char2jstring(env, s.c_str()));
    }
    return resArray;
}

extern "C"
JNIEXPORT jobjectArray JNICALL
Java_com_okcoin_vault_jni_bitcoindiamond_BitcoinDiamondj_execute
  (JNIEnv *env, jclass, jstring networkType, jstring command)
{
  std::list<std::string> resultList;
  execute(jstring2char(env, networkType), jstring2char(env, command), resultList);
  return stringList2jobjectArray(env, resultList);
}
