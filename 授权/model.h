//
//  model.h
//  授权
//
//  Created by 罗刺猬 on 17/10/21.
//  Copyright © 2017年 罗刺猬. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface model : NSObject

/**　string	用于调用access_token，接口获取授权后的access token。*/
@property (nonatomic, copy) NSString *access_token;

/**　string	access_token的生命周期，单位是秒数。*/
@property (nonatomic, copy) NSNumber *expires_in;

/**　string	当前授权用户的UID。*/
@property (nonatomic, copy) NSString *uid;

/**	access token的创建时间 */
//@property (nonatomic, strong) NSDate *created_time;

/** 用户的昵称 */
@property (nonatomic, copy) NSString *remind_in;

@property (nonatomic,assign) BOOL isRealName;

+(instancetype)modelWithDict:(NSDictionary *)dict;

@end
