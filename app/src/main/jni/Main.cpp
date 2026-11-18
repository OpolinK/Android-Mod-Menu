#include <list>
#include <vector>
#include <cstring>
#include <pthread.h>
#include <thread>
#include <cstring>
#include <string>
#include <jni.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <dlfcn.h>
#include "Includes/Logger.h"
#include "Includes/obfuscate.h"
#include "Includes/Utils.hpp"
#include "Menu/Menu.hpp"
#include "Menu/Jni.hpp"
#include "Includes/Macros.h"
#include "dobby.h"

int scoreMul = 1, coinsMul = 1;

// Do not change or translate the first text unless you know what you are doing
// Assigning feature numbers is optional. Without it, it will automatically count for you, starting from 0
// Assigned feature numbers can be like any numbers 1,3,200,10... instead in order 0,1,2,3,4,5...
// ButtonLink, Category, RichTextView and RichWebView is not counted. They can't have feature number assigned
// Toggle, ButtonOnOff and Checkbox can be switched on by default, if you add True_. Example: CheckBox_True_The Check Box
// To learn HTML, go to this page: https://www.w3schools.com/

jobjectArray GetFeatureList(JNIEnv *env, jobject context) {
    jobjectArray ret;

    const char *features[] = {
			OBFUSCATE("Category_Trench Warfare WW1 Mod"),
    		OBFUSCATE("1_Toggle_Безкоштовні покупки та 99М валюти"),
    		OBFUSCATE("2_Toggle_Нескінченні гроші в бою"),
    		OBFUSCATE("3_Toggle_Безкоштовний Донат (клік по паку в магазині)")
	
            OBFUSCATE("Toggle_No death"),
            OBFUSCATE("Button_Start Invcibility (30 sec duration)"),
            OBFUSCATE("SeekBar_Score multiplier_1_100"),
            OBFUSCATE("SeekBar_Coins multiplier_1_1000"),
            OBFUSCATE("Category_Examples"), //Not counted
            OBFUSCATE("Toggle_The toggle"),
            OBFUSCATE(
                    "100_Toggle_True_The toggle 2"), //This one have feature number assigned, and switched on by default
            OBFUSCATE("110_Toggle_The toggle 3"), //This one too
            OBFUSCATE("SeekBar_The slider_1_100"),
            OBFUSCATE("SeekBar_Kittymemory slider example_1_5"),
            OBFUSCATE("Spinner_The spinner_Items 1,Items 2,Items 3"),
            OBFUSCATE("Button_The button"),
            OBFUSCATE("ButtonLink_The button with link_https://www.youtube.com/"), //Not counted
            OBFUSCATE("ButtonOnOff_The On/Off button"),
            OBFUSCATE("CheckBox_The Check Box"),
            OBFUSCATE("InputValue_Input number"),
            OBFUSCATE("InputValue_1000_Input number 2"), //Max value
			OBFUSCATE("1111_InputLValue_Input long number"),
            OBFUSCATE("InputLValue_1000000000000_Input long number 2"), //Max value
            OBFUSCATE("InputText_Input text"),
            OBFUSCATE("RadioButton_Radio buttons_OFF,Mod 1,Mod 2,Mod 3"),

            //Create new collapse
            OBFUSCATE("Collapse_Collapse 1"),
            OBFUSCATE("CollapseAdd_Toggle_The toggle"),
            OBFUSCATE("CollapseAdd_Toggle_The toggle"),
            OBFUSCATE("123_CollapseAdd_Toggle_The toggle"),
            OBFUSCATE("122_CollapseAdd_CheckBox_Check box"),
            OBFUSCATE("CollapseAdd_Button_The button"),

            //Create new collapse again
            OBFUSCATE("Collapse_Collapse 2_True"),
            OBFUSCATE("CollapseAdd_SeekBar_The slider_1_100"),
            OBFUSCATE("CollapseAdd_InputValue_Input number"),

            OBFUSCATE("RichTextView_This is text view, not fully HTML."
                      "<b>Bold</b> <i>italic</i> <u>underline</u>"
                      "<br />New line <font color='red'>Support colors</font>"
                      "<br/><big>bigger Text</big>"),
            OBFUSCATE("RichWebView_<html><head><style>body{color: white;}</style></head><body>"
                      "This is WebView, with REAL HTML support!"
                      "<div style=\"background-color: darkblue; text-align: center;\">Support CSS</div>"
                      "<marquee style=\"color: green; font-weight:bold;\" direction=\"left\" scrollamount=\"5\" behavior=\"scroll\">This is <u>scrollable</u> text</marquee>"
                      "</body></html>")
    };

    int Total_Feature = (sizeof features / sizeof features[0]);
    ret = (jobjectArray)
            env->NewObjectArray(Total_Feature, env->FindClass(OBFUSCATE("java/lang/String")),
                                env->NewStringUTF(""));

    for (int i = 0; i < Total_Feature; i++)
        env->SetObjectArrayElement(ret, i, env->NewStringUTF(features[i]));

    return (ret);
}

bool btnPressed = false;

//Target main lib here
#define targetLibName OBFUSCATE("libil2cpp.so")

void Changes(JNIEnv *env, jclass clazz, jobject obj, jint featNum, jstring featName, jint value, jlong Lvalue, jboolean boolean, jstring text) {

    switch (featNum) {
        case 0:
            // offset, hex
            PATCH_SWITCH(targetLibName, "0x1079728", "C0 03 5F D6", boolean);
            // The patch switch has been returned and reworked:
            // - (active) Dobby-Kitty implementation
            // - reworked KittyMemory implementation
            //
            // if you encounter any problems:
            // - switch to Kitty implementation (uncomment code in Macros.h)
            // - uncommiting logging for detailed debug
            // - special attention to the preferences -> this is the only source of this problem in the past that I have noticed:
            // -- try rename the preferences file;
            // -- for a maximum stable and flexible save settings, recommend using own system with XML/JSON files.

            // alt possibles usage variants:
            // symbol, hex
            PATCH_SWITCH(targetLibName, "_example__sym", "C0 03 5F D6", boolean);
            // offset, asm
            PATCH_SWITCH(targetLibName, "0x1079728", "ret", boolean);
            // symbol, asm
            PATCH_SWITCH(targetLibName, "_example__sym", "ret", boolean);

            // asm allows you to avoid using hex code, as it is generated automatically from the instructions.
            // - this is the awesome option if you know what you're doing
            // recommended insert ';' to separate statements, example: "mov x0, #1; ret"
            // recommended to test your instructions on https://armconverter.com or
            // https://shell-storm.org/online/Online-Assembler-and-Disassembler/

            // - this is probably especially useful with creating dynamic deep patches
            dPATCH_SWITCH(true, targetLibName, "0x1079728", "mov w%d, #%d", 0, 222);
            dPATCH_SWITCH(true, targetLibName, "_example__sym", "mov w%d, #%d", 0, 222);
            // standard formatting specifiers are supported (%d, %i, %x, %s, etc.)


            // Relative patches allow you to speed up patch creation if you are sure that the offsets within methods rarely change
            // So, you only need to update the offset instruction for the function
            // https://www.rapidtables.com/calc/math/hex-calculator.html <- use hex calculator to calculate the offset relative to the method
            // ! This is an extremely unstable due to the hard offsets... don't forget to check the logs to identify outdated offsets
            // offset, offset, hex
            rPATCH_SWITCH(targetLibName, "0x1079728", "0x204", "C0 03 5F D6", boolean);
            // sym, offset, hex
            rPATCH_SWITCH(targetLibName, "_example__sym", "0xAC", "C0 03 5F D6", boolean);
            // offset, offset, asm
            rPATCH_SWITCH(targetLibName, "0x1079728", "0x204", "mov x0, #0xffffff; ret", boolean);
            // sym, offset, asm
            rPATCH_SWITCH(targetLibName, "_example__sym", "0xAC", "mov x0, #0xffffff; ret", boolean);
            break;
        case 4:
            if(boolean) {
                // offset, hex
                PATCH(targetLibName, "0x10709AC", "E05F40B2 C0035FD6");
                rPATCH(targetLibName, "0x107094D", "0x5F", "E05F40B2 C0035FD6");

                // alt possibles usage variants:
                // symbol, hex
                PATCH(targetLibName, "_example__sym", "E0 5F 40 B2 C0 03 5F D6");
                rPATCH(targetLibName, "_example__sym", "0x5F", "E0 5F 40 B2 C0 03 5F D6");
                // offset, asm
                PATCH(targetLibName, "0x10709AC", "mov x0, #0xffffff; ret");
                rPATCH(targetLibName, "0x107094D", "0x5F", "mov x0, #0xffffff; ret");
                // symbol, asm
                PATCH(targetLibName, "_example__sym", "mov x0, #0xffffff; ret");
                rPATCH(targetLibName, "_example__sym", "0x5F", "mov x0, #0xffffff; ret");
            } else {
                RESTORE(targetLibName, "0x10709AC");
                rRESTORE(targetLibName, "0x10709AC", "0x5F");
                // or
                RESTORE(targetLibName, "_example__sym");
                rRESTORE(targetLibName, "_example__sym", "0x5F");
            }
            break;
        case 1:
        	isFreeShopAndCurrency = boolean;
        	break;
    	case 2:
        	isInfiniteBattleMoney = boolean;
        	break;
    	case 3:
        	isFreeDonatePacks = boolean;
        	break;
        case 5:
            // you can use this for things as detect log, counting function calls, executing side code before the function is executed
            // now instrument wrapper implemented for detecting execution in logcat
            INST(targetLibName, "0x235630", "AnyNameForDetect2", boolean);

            if(boolean) {
                INST(targetLibName, "_example__sym", "AnyNameForDetect3", true);
            } else {
                INST(targetLibName, "_example__sym", "AnyNameForDetect3", false);
            }
            break;
        default:
            break;
    }
}

//CharacterPlayer
void (*StartInvcibility)(void *instance, float duration);

void (*old_Update)(void *instance);
void Update(void *instance) {
    if (instance != nullptr) {
        if (btnPressed) {
            StartInvcibility(instance, 30);
            btnPressed = false;
        }
    }
    return old_Update(instance);
}

/*
 void (*old_AddScore)(void *instance, int score);
 void AddScore(void *instance, int score) {
    //default any actions
    return old_AddScore(instance, score * scoreMul);
 }
*/
// === This function was completely replaced with `install_hook_name` from dobby.h ===
// (base name, return type, ... args)
install_hook_name(AddScore, void *, void *instance, int score) {
    // default any actions

    // use orig_ for call original function
    return orig_AddScore(instance, score + scoreMul);
}

void (*old_AddCoins)(void *instance, int count);
void AddCoins(void *instance, int count) {
    return old_AddCoins(instance, count * coinsMul);
}


// we will run our hacks in a new thread so our while loop doesn't block process main thread

// Стани наших перемикачів у Mod Menu
bool isFreeShopAndCurrency = false;
bool isInfiniteBattleMoney = false;
bool isFreeDonatePacks = false;

// --- 1. НЕОБМЕЖЕНА ВАЛЮТА В МЕНЮ ТА БЕЗКОШТОВНІ ПОКУПКИ ---
int (*old_get_SpecialCurrencyAmount)(void *instance);
int get_SpecialCurrencyAmount(void *instance) {
    if (isFreeShopAndCurrency && instance != NULL) {
        return 99999999; // 99 мільйонів валюти в меню
    }
    return old_get_SpecialCurrencyAmount(instance);
}

void (*old_set_SpecialCurrencyAmount)(void *instance, int value);
void set_SpecialCurrencyAmount(void *instance, int value) {
    if (isFreeShopAndCurrency && instance != NULL) {
        old_set_SpecialCurrencyAmount(instance, 99999999); // Зберігає 99 млн у сейв
        return;
    }
    old_set_SpecialCurrencyAmount(instance, value);
}

int (*old_Skin_get_Price)(void *instance);
int Skin_get_Price(void *instance) {
    if (isFreeShopAndCurrency && instance != NULL) {
        return 0; // Усі скіни коштують 0
    }
    return old_Skin_get_Price(instance);
}

int (*old_SpecialAttackPack_get_Price)(void *instance);
int SpecialAttackPack_get_Price(void *instance) {
    if (isFreeShopAndCurrency && instance != NULL) {
        return 0; // Усі набори спец-атак коштують 0
    }
    return old_SpecialAttackPack_get_Price(instance);
}

int (*old_SpecialAttack_get_Amount)(void *instance);
int SpecialAttack_get_Amount(void *instance) {
    if (isFreeShopAndCurrency && instance != NULL) {
        return 999; // Завжди 999 зарядів спец-атак
    }
    return old_SpecialAttack_get_Amount(instance);
}

// --- 2. НЕСКІНЧЕННІ ГРОШІ В БОЮ (LEVEL MONEY) ---
int (*old_get_CurrentMoney)(void *instance);
int get_CurrentMoney(void *instance) {
    if (isInfiniteBattleMoney && instance != NULL) {
        return 999999; // Нескінченні гроші для виклику юнітів у бою
    }
    return old_get_CurrentMoney(instance);
}

void (*old_set_CurrentMoney)(void *instance, int value);
void set_CurrentMoney(void *instance, int value) {
    if (isInfiniteBattleMoney && instance != NULL) {
        old_set_CurrentMoney(instance, 999999);
        return;
    }
    old_set_CurrentMoney(instance, value);
}

// --- 3. БЕЗКОШТОВНИЙ ДОНАТ (PREMIUM, STARTER PACK, 2X SPEED, LEVEL BOOST) ---
// Вказівники на внутрішні функції розблокування з IAPManager
void (*UnlockPremium)(void *instance);
void (*UnlockStarterPack)(void *instance);
void (*UnlockDoubleSpeedPack)(void *instance);
void (*UnlockLevelBoostPack)(void *instance);

void (*old_OnPurchaseClicked)(void *instance, void *product, int cartType);
void OnPurchaseClicked(void *instance, void *product, int cartType) {
    if (isFreeDonatePacks && instance != NULL) {
        // При натисканні на покупку будь-якого донат-паку миттєво відкриваємо всі 4 набори без Google Play!
        UnlockPremium(instance);
        UnlockStarterPack(instance);
        UnlockDoubleSpeedPack(instance);
        UnlockLevelBoostPack(instance);
        return;
    }
    old_OnPurchaseClicked(instance, product, cartType);
}

void hack_thread() {
    // This loop should be always enabled in unity game
    // because libil2cpp.so is not loaded into memory immediately.
    while (!isLibraryLoaded(targetLibName)) {
        sleep(1); // Wait for target lib be loaded.
    }

    // In Android Studio, to switch between arm64-v8a and armeabi-v7a syntax highlighting,
    // You can modify the "Active ABI" in "Build Variants" to switch to another architecture for parsing.
#if defined(__aarch64__)
	// 1. Валюта в меню, ціни скінів та спец-атаки
    HOOK_LIB("libil2cpp.so", "0x242E63C", get_SpecialCurrencyAmount, old_get_SpecialCurrencyAmount);
    HOOK_LIB("libil2cpp.so", "0x242E644", set_SpecialCurrencyAmount, old_set_SpecialCurrencyAmount);
    HOOK_LIB("libil2cpp.so", "0x24434F4", Skin_get_Price, old_Skin_get_Price);
    HOOK_LIB("libil2cpp.so", "0x24220BC", SpecialAttackPack_get_Price, old_SpecialAttackPack_get_Price);
    HOOK_LIB("libil2cpp.so", "0x242203C", SpecialAttack_get_Amount, old_SpecialAttack_get_Amount);

    // 2. Гроші в бою
    HOOK_LIB("libil2cpp.so", "0x2438EF4", get_CurrentMoney, old_get_CurrentMoney);
    HOOK_LIB("libil2cpp.so", "0x2438EFC", set_CurrentMoney, old_set_CurrentMoney);

    // 3. Ініціалізація методів безкоштовного донату
    UnlockPremium = (void (*)(void *))getAbsoluteAddress("libil2cpp.so", 0x24339EC);
    UnlockStarterPack = (void (*)(void *))getAbsoluteAddress("libil2cpp.so", 0x2433AD0);
    UnlockDoubleSpeedPack = (void (*)(void *))getAbsoluteAddress("libil2cpp.so", 0x2433BB4);
    UnlockLevelBoostPack = (void (*)(void *))getAbsoluteAddress("libil2cpp.so", 0x2433C98);
    HOOK_LIB("libil2cpp.so", "0x2431FB0", OnPurchaseClicked, old_OnPurchaseClicked);
    //Il2Cpp: Use RVA offset
    StartInvcibility = (void (*)(void *, float)) getAbsoluteAddress(targetLibName, OBFUSCATE("0x107A3BC"));
    StartInvcibility = (void (*)(void *, float)) getAbsoluteAddress(targetLibName, OBFUSCATE("_characterPlayer_Update"));

    HOOK(targetLibName, "0x107A2FC", AddCoins, old_AddCoins);

    // HOOK(targetLibName, "0x107A2E0", AddScore, old_AddScore);
    // === This function was completely replaced with super-macro `install_hook_name` from dobby.h ===
    // don't forget set address for install_hook:
    // ! getAbsoluteAddress not have OBFUSCATE, so don't forget use his here
    install_hook_AddScore(getAbsoluteAddress(targetLibName,OBFUSCATE("0x107A2E0")));

    HOOK(targetLibName, "0x1078C44", Update, old_Update);
    //HOOK(targetLibName, "0x1079728", Kill, old_Kill);
    //HOOK(targetLibName, "_example__sym", Kill, old_Kill);
    //HOOK_NO_ORIG("libFileC.so", "0x123456", FunctionExample);
    //HOOK_NO_ORIG("libFileC.so", "_example__sym", FunctionExample);

    //PATCH(targetLibName, "0x10709AC", "E05F40B2C0035FD6");

    INST(targetLibName, "0x23558C", "AnyNameForDetect", true);

    // LOGI(OBFUSCATE("Test SYM: 0x%llx"), (uintptr_t)getAbsoluteAddress(OBFUSCATE("libil2cpp.so"), OBFUSCATE("il2cpp_init")));
#elif defined(__arm__)
    //Put your code here if you want the code to be compiled for armv7 only
#endif

    LOGI(OBFUSCATE("Done"));
}

// Functions with `__attribute__((constructor))` are executed immediately when System.loadLibrary("lib_name") is called.
// If there are multiple such functions at the same time, `constructor(priority)` (the priority is an integer)
// will determine the execution priority, otherwise the execution order is undefined behavior.
__attribute__((constructor))
void lib_main() {
    // Create a new thread so it does not block the main thread, means the game would not freeze
    // In modern C++, you should use std::thread(yourFunction).detach() instead of pthread_create
    std::thread(hack_thread).detach();
}
