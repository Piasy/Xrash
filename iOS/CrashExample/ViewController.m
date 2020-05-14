//
//  ViewController.m
//  CrashExample
//
//  Created by Piasy on 2020/5/14.
//  Copyright © 2020 Piasy. All rights reserved.
//

#import "ViewController.h"

#import <crash/crash_test.h>

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.

    CrashTest* crash = [[CrashTest alloc] init];
    [crash crashTest];
}


@end
