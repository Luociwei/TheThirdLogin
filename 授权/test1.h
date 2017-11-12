一、准备工作
1、到微信开放平台注册成开发者，获取appid
2、导入WeChatConnection.framework
3、配置URL Schemes  输入appid  例如wx29ce0f21ea982cb8
二、配置AppDelegate.m
1、 注册微信
?
1
2
//微信登陆
[WXApi registerApp:WeiXin_AppId withDescription:@"weixin"];
2、设置函数
?
1
2
3
4
5
6
//把代理设置到登陆视图中
- (BOOL)application:(UIApplication *)application
handleOpenURL:(NSURL *)url
{
    return [WXApi handleOpenURL:url delegate:[LoginViewController shareLogin]];
}
?
1
2
3
4
5
6
7
- (BOOL)application:(UIApplication *)application
openURL:(NSURL *)url
sourceApplication:(NSString *)sourceApplication
annotation:(id)annotation
{
    return [WXApi handleOpenURL:url delegate:[LoginViewController shareLogin]];
}
三、登陆页代码
1、微信登录授权比较复杂，相比QQ，新浪多了几步，简单说就是需要三步，第一步，获取code，这个用来获取token，第二步，就是带上code获取token，第三步，根据第二步获取的token和openid来获取用户的相关信息
2、
第一步：获取code
?
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
17
18
19
20
21
22
23
24
-(void)weiXinLogin
{
    SendAuthReq* req =[[SendAuthReq alloc] init];
    req.scope = @"snsapi_userinfo,snsapi_base";
    req.state = @"0744" ;
    [WXApi sendReq:req];
}

-(void)onReq:(BaseReq *)req
{
    NSLog(@"呵呵");
    [self msgHint:@"登陆失败"];
}

-(void)onResp:(BaseResp *)resp
{
    SendAuthResp* sender = (SendAuthResp*)resp;
    NSString* code = sender.code;
    NSLog(@"啦啦 code = %@",code);
    
    MBProgressHUD * hud = [MBProgressHUD showHUDAddedTo:self.view animated:YES];
    hud.labelText = @"收取用户信息..";
    [self getAccess_tokenWithCode:code];
}
第二步 获取token
?
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
17
18
19
20
21
22
23
24
-(void)getAccess_tokenWithCode:(NSString*)myCode
{
    //https://api.weixin.qq.com/sns/oauth2/access_token?appid=APPID&secret=SECRET&code=CODE&grant_type=authorization_code
    
    NSString *url =[NSString stringWithFormat:@"https://api.weixin.qq.com/sns/oauth2/access_token?appid=%@&secret=%@&code=%@&grant_type=authorization_code",kWXAPP_ID,kWXAPP_SECRET,myCode];
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        NSURL *zoneUrl = [NSURL URLWithString:url];
        NSString *zoneStr = [NSString stringWithContentsOfURL:zoneUrl encoding:NSUTF8StringEncoding error:nil];
        NSData *data = [zoneStr dataUsingEncoding:NSUTF8StringEncoding];
        dispatch_async(dispatch_get_main_queue(), ^{
            if (data) {
                NSDictionary *dic = [NSJSONSerialization JSONObjectWithData:data options:NSJSONReadingMutableContainers error:nil];
                NSString* token = [dic objectForKey:@"access_token"];
                NSString* openid = [dic objectForKey:@"openid"];
                [self getUserInfoWithToken:token openId:openid];
                NSLog(@"token = %@",token);
                NSLog(@"openid = %@",openid);
                
                
            }
        });
    });
}
第三步：获取用户信息
?
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
17
18
19
20
21
22
23
24
25
26
27
28
29
30
31
32
33
34
-(void)getUserInfoWithToken:(NSString*)myToken openId:(NSString*)myOpenId
{
    // https://api.weixin.qq.com/sns/userinfo?access_token=ACCESS_TOKEN&openid=OPENID
    
    NSString *url =[NSString stringWithFormat:@"https://api.weixin.qq.com/sns/userinfo?access_token=%@&openid=%@",myToken,myOpenId];
    NSLog(@"infoUrl = %@",url);
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        NSURL *zoneUrl = [NSURL URLWithString:url];
        NSString *zoneStr = [NSString stringWithContentsOfURL:zoneUrl encoding:NSUTF8StringEncoding error:nil];
        NSData *data = [zoneStr dataUsingEncoding:NSUTF8StringEncoding];
        dispatch_async(dispatch_get_main_queue(), ^{
            if (data) {
                NSDictionary *dic = [NSJSONSerialization JSONObjectWithData:data options:NSJSONReadingMutableContainers error:nil];
                NSString* nickName = [dic objectForKey:@"nickname"];
                NSString* wxHeadImgUrl = [dic objectForKey:@"headimgurl"];
                
                NSLog(@"nickName = %@",nickName);
                NSLog(@"headImg = %@",wxHeadImgUrl);
                
                NSUserDefaults *userDefaults = [NSUserDefaults standardUserDefaults];
                [userDefaults setObject:ON forKey:LogState];
                [userDefaults setObject:ThirdFoudationLogin forKey:LogType];
                [userDefaults setObject:nickName forKey:LoginName];
                [userDefaults setObject:wxHeadImgUrl forKey:UserHeaderPath];
                [userDefaults synchronize]; 
                
                [MBProgressHUD hideAllHUDsForView:self.view animated:YES]; 
                [self msgHint:@"微信登陆成功"]; 
                [self popView]; 
            } 
        }); 
        
    }); 
} 
