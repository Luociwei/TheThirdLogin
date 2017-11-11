//
//  ViewController.m
//  授权
//
//  Created by 罗刺猬 on 17/10/21.
//  Copyright © 2017年 罗刺猬. All rights reserved.
//

#import "ViewController.h"

#import "testViewController.h"

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    UIButton *btn = [UIButton new];
    btn.frame = CGRectMake(60, 60, 60, 60);
    [btn setTitle:@"login" forState:UIControlStateNormal];
    [self.view addSubview:btn];
    btn.backgroundColor = [UIColor orangeColor];
    [btn addTarget:self action:@selector(login) forControlEvents:UIControlEventTouchDown];
}

-(void)login
{
    [self presentViewController:[testViewController new] animated:YES completion:nil];
}


@end
