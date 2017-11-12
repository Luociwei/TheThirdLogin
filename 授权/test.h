7、向微信终端程序注册第三方应用，并在第三方应用实现从微信返回
在AppDelegate.m中引入"WXApi.h"头文件，然后写入如下:

1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
#import "AppDelegate.h"
#import "LoginViewController.h"
#import "WXApi.h"

#pragma mark - application delegate
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    
    [WXApi registerApp:@"wxd1931d4a0e46****" withDescription:@"Wechat"];
    return YES;
}
// 这个方法是用于从微信返回第三方App
- (BOOL)application:(UIApplication *)application handleOpenURL:(NSURL *)url {
    
    [WXApi handleOpenURL:url delegate:self];
    return YES;
}
8、请求CODE
开发者需要配合使用微信开放平台提供的SDK进行授权登录请求接入。正确接入SDK后并拥有相关授权域（scope，什么是授权域？）权限后，开发者移动应用会在终端本地拉起微信应用进行授权登录，微信用户确认后微信将拉起开发者移动应用，并带上授权临时票据（code）。

#import "LoginViewController.h"
#import "RegisterViewController.h"
#import "MBProgressHUD.h"
#import "AFNetworking.h"
#import "WXApi.h"

#pragma mark - 微信登录
/*
 目前移动应用上德微信登录只提供原生的登录方式，需要用户安装微信客户端才能配合使用。
 对于iOS应用,考虑到iOS应用商店审核指南中的相关规定，建议开发者接入微信登录时，先检测用户手机是否已经安装
 微信客户端(使用sdk中的isWXAppInstall函数),对于未安装的用户隐藏微信 登录按钮，只提供其他登录方式。
 */
- (IBAction)wechatLoginClick:(id)sender {
    
    if ([WXApi isWXAppInstalled]) {
        SendAuthReq *req = [[SendAuthReq alloc] init];
        req.scope = @"snsapi_userinfo";
        req.state = @"App";
        [WXApi sendReq:req];
    }
    else {
        [self setupAlertController];
    }
}

#pragma mark - 设置弹出提示语
- (void)setupAlertController {
    
    UIAlertController *alert = [UIAlertController alertControllerWithTitle:@"温馨提示" message:@"请先安装微信客户端" preferredStyle:UIAlertControllerStyleAlert];
    UIAlertAction *actionConfirm = [UIAlertAction actionWithTitle:@"确定" style:UIAlertActionStyleDefault handler:nil];
    [alert addAction:actionConfirm];
    [self presentViewController:alert animated:YES completion:nil];
}
