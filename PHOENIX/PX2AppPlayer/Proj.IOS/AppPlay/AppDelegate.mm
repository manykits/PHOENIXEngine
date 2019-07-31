// AppDelegate.mm

#import "AppDelegate.h"
#include "AppPlay.hpp"
#include <vector>
#import "ViewController.h"

@implementation AppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    // Override point for customization after application launch.
    
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
    
    appplay::NativeCall::DidEnterBackground();
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
    
    CGSize size = [UIScreen mainScreen].applicationFrame.size;
    CGFloat scale = [UIScreen mainScreen].scale;
    appplay::NativeCall::Initlize(size.width * scale, size.height * scale);
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
    
    appplay::NativeCall::Ternamate();
}


- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    std::vector<int> ids;
    std::vector<float> xs;
    std::vector<float> ys;
    
    int index = 0;
    
    for (UITouch *touch in touches)
    {
        UIView *view = [touch view];
        
        if (view == pxMainViewController.view)
        {
            CGFloat x = [touch locationInView: view].x;
            CGFloat y = [touch locationInView: view].y;
            CGFloat scale = [[UIScreen mainScreen] scale];
                
            ids.push_back(index++);
            xs.push_back(x * scale);
            ys.push_back(y * scale);
        }
    }
    
    if (ids.size() > 0)
    {
        appplay::NativeCall::OnTouchPressed((int)ids.size(), &ids[0], &xs[0], &ys[0]);
    }
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    std::vector<int> ids;
    std::vector<float> xs;
    std::vector<float> ys;
    
    int index = 0;
    
    for (UITouch *touch in touches)
    {
        UIView *view = [touch view];
       
        if (view == pxMainViewController.view)
        {
            CGFloat x = [touch locationInView: view].x;
            CGFloat y = [touch locationInView: view].y;
            CGFloat scale = [[UIScreen mainScreen] scale];
        
            ids.push_back(index++);
            xs.push_back(x * scale);
            ys.push_back(y * scale);
        }
    }
    
    if (ids.size() > 0)
    {
        appplay::NativeCall::OnTouchReleased((int)ids.size(), &ids[0], &xs[0], &ys[0]);
    }
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    std::vector<int> ids;
    std::vector<float> xs;
    std::vector<float> ys;
    
    int index = 0;
    
    for (UITouch *touch in touches)
    {
        UIView *view = [touch view];
        
       if (view == pxMainViewController.view)
        {
            CGFloat x = [touch locationInView: view].x;
            CGFloat y = [touch locationInView: view].y;
            CGFloat scale = [[UIScreen mainScreen] scale];
            
            ids.push_back(index++);
            xs.push_back(x * scale);
            ys.push_back(y * scale);
        }
    }
    
    if (ids.size() > 0)
    {
        appplay::NativeCall::OnTouchMoved((int)ids.size(), &ids[0], &xs[0], &ys[0]);
    }
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    std::vector<int> ids;
    std::vector<float> xs;
    std::vector<float> ys;
    
    int index = 0;
    
    for (UITouch *touch in touches)
    {
        UIView *view = [touch view];
        
        if (view == pxMainViewController.view)
        {
            CGFloat x = [touch locationInView: view].x;
            CGFloat y = [touch locationInView: view].y;
            CGFloat scale = [[UIScreen mainScreen] scale];
            
            ids.push_back(index++);
            xs.push_back(x * scale);
            ys.push_back(y * scale);
        }
    }
    
    if (ids.size() > 0)
    {
        appplay::NativeCall::OnTouchCancelled((int)ids.size(), &ids[0], &xs[0], &ys[0]);
    }
}

@end
