//
//  ViewController.m
//  ResearchAFN
//
//  Created by 李礼光 on 2017/3/24.
//  Copyright © 2017年 李礼光. All rights reserved.
//

#import "ViewController.h"


//3M大小
static NSString *bookFile = @"https://1geauomt3gezdrctsfaauyce.ourdvsss.com/d1.baidupcs.com/file/e448a77e55cf76741c177edcbcc65d0d?bkt=p3-1400e448a77e55cf76741c177edcbcc65d0d09aecf9d0000002df86c&xcode=7512531e5092aae4168567bd675ead80caa71725af8628dafc7e6775e72092ed&fid=2500937479-250528-316399661488313&time=1489996823&sign=FDTAXGERLBHS-DCb740ccc5511e5e8fedcff06b081203-INq%2BCLDS7si%2BTo0Vt7TTX33sHjs%3D&to=d1&size=3012716&sta_dx=3012716&sta_cs=1762&sta_ft=pdf&sta_ct=7&sta_mt=0&fm2=MH,Ningbo,Netizen-anywhere,,guangdongct&newver=1&newfm=1&secfm=1&flow_ver=3&pkey=1400e448a77e55cf76741c177edcbcc65d0d09aecf9d0000002df86c&sl=72286287&expires=8h&rt=pr&r=920434610&mlogid=1824508507973040117&vuk=2500937479&vbdid=3748018546&fin=Objective-Ccode.pdf&fn=Objective-Ccode.pdf&rtype=1&iv=0&dp-logid=1824508507973040117&dp-callid=0.1.1&hps=1&csl=400&csign=N1qazWsdLpZlNxMfqkyGGRJrnek%3D&by=themis&wshc_tag=0&wsts_tag=58cf8c18&wsid_tag=da11a146&wsiphost=ipdbm";

//图片
static NSString *imgFile = @"https://timgsa.baidu.com/timg?image&quality=80&size=b9999_10000&sec=1490009867280&di=85eb9d2f6a728ca2a6eb8558f2b5aafe&imgtype=0&src=http%3A%2F%2Fimg.pconline.com.cn%2Fimages%2Fupload%2Fupc%2Ftx%2Fitbbs%2F1501%2F08%2Fc20%2F1646585_1420732490684.jpg";


//                             dataTask                     下载                          上传
@interface ViewController ()<NSURLSessionTaskDelegate,NSURLSessionDownloadDelegate,NSURLSessionDataDelegate>
@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    [self dataTask];
}

#pragma mark - 系统网络请求步骤

//实现文件下载步骤
//1.确定请求路径                              url
//2.根据URL创建一个请求对象                     request
//3.设置请求对象 HTTP头部详细信息,GET/POST
//4.创建会话对象                              session
//5.创建会话对象任务
//6.发送请求
//7.解析服务器返回的数据


//网络请求类
//NSURLSessionTask : NSObject
//NSURLSessionDataTask : NSURLSessionTask           (单纯的声明的类)
//NSURLSessionUploadTask : NSURLSessionDataTask     (单纯的声明的类)
//NSURLSessionDownloadTask : NSURLSessionTask       (只有一个方法)
//NSURLSessionStreamTask : NSURLSessionTask


//网络请求代理
//@protocol NSURLSessionDelegate <NSObject>
//@protocol NSURLSessionTaskDelegate <NSURLSessionDelegate> //基本的网路请求协议

//@protocol NSURLSessionDataDelegate <NSURLSessionTaskDelegate>     //关于上传
//@protocol NSURLSessionDownloadDelegate <NSURLSessionTaskDelegate> //关于下载

//@protocol NSURLSessionStreamDelegate <NSURLSessionTaskDelegate>



//关于NSURLSessionDelegate
//NSURLSessionDelegate协议描述了NSURLSession对象调起他们的代理对象去处理一些会话级别相关的事件方法。
//除了这个协议中定义的方法,大多数代理对象也应该实现部分或全部在NSURLSessionTaskDelegate,NSURLSessionDataDelegate,NSURLSessionDownloadDelegate协议中定义的处理任务级别的事件的方法。


//关于NSURLSessionTaskDelegate
//NSURLSessionTaskDelegate协议定义了制定的任务代理方法,那些是在你执行网络请求任务(任何的NSURLSession task)
//比如: 你在执行下载任务,应该实现NSURLSessionDownloadDelegate协议
//比如: 你在执行上传任务,应该实现NSURLSessionDataDelegate协议        (注意是DataDelegate,没有UploadDelegate)


//关于NSURLSessionDataDelegate
//上传相关代理方法

//关于NSURLSessionDownloadDelegate
//下载相关代理方法

/*--------------------------------------------------
 
 NSURLSessionTask
        |
        |-- NSURLSessionDataTask
                    |
                    |-- NSURLSessionUploadTask
        |
        |-- NSURLSessionDownloadTask
        |
        |-- NSURLSessionStreamTask
 
 
 
 
 --------------------------------------------------*/


//关于配置
//NSURLSessionConfiguration : NSObject <NSCopying>

#pragma mark - task任务
- (void)dataTask {
    NSURL *imgUrl = [NSURL URLWithString:imgFile];
    NSURL *bookUrl = [NSURL URLWithString:bookFile];
    
    NSURLRequest *imgRequest = [NSURLRequest requestWithURL:imgUrl];
    NSURLRequest *bookRequest = [NSURLRequest requestWithURL:bookUrl];
    
    NSURLSession *session = [NSURLSession sharedSession];
    NSURLSession *sessionWithDelegate = [NSURLSession sessionWithConfiguration:[NSURLSessionConfiguration defaultSessionConfiguration]
                                                                      delegate:self
                                                                 delegateQueue:[NSOperationQueue currentQueue]];
    
    NSURLSessionDataTask *dataTask = [sessionWithDelegate dataTaskWithRequest:imgRequest
                                                            completionHandler:^(NSData * _Nullable data, NSURLResponse * _Nullable response, NSError * _Nullable error) {
                                                                NSLog(@"currentThread : %@",[NSThread currentThread]);
                                                                //接收都的数据
                                                                if (!error) {
                                                                    NSLog(@"response : %@",response);
                                                                    [self showImage:data];
                                                                }else {
                                                                    NSLog(@"error : %@",error);
                                                                }
    }];
    
    [dataTask resume];
    
    /*
     
     
     
     */
    
    
    
}
#pragma mark - 下载
- (void)downloadTask {
    
}
#pragma mark - 上传
- (void)uploadTask {
    
}
#pragma mark - dataTask代理


//任务下载失败
- (void)URLSession:(NSURLSession *)session
              task:(NSURLSessionTask *)task
didCompleteWithError:(nullable NSError *)error {
    NSLog(@"%s",__func__);
    NSLog(@"session : %@",session);
    NSLog(@"task %@",task);
    NSLog(@"error %@",error);
    
}





#pragma mark - 下载代理
- (void)URLSession:(NSURLSession *)session downloadTask:(NSURLSessionDownloadTask *)downloadTask didFinishDownloadingToURL:(NSURL *)location {
    NSLog(@"downloadTask : %@",downloadTask);
    NSLog(@"location : %@",location);
}

-(void)URLSession:(NSURLSession *)session dataTask:(NSURLSessionDataTask *)dataTask didReceiveResponse:(NSURLResponse *)response completionHandler:(void (^)(NSURLSessionResponseDisposition))completionHandler {
    NSLog(@"%s",__func__);
}

- (void)URLSession:(NSURLSession *)session dataTask:(NSURLSessionDataTask *)dataTask didReceiveData:(NSData *)data {
    NSLog(@"%s",__func__);

}


#pragma mark - 上传代理









#pragma mark - 其他方法
- (void)showImage:(NSData *)data {
    UIImageView *imageView = [[UIImageView alloc]init];
    imageView.frame = CGRectMake(0, 0, 300, 300);
    imageView.center = self.view.center;
    imageView.backgroundColor = [UIColor lightGrayColor];
    [self.view addSubview:imageView];
    
    imageView.image = [UIImage imageWithData:data];
}











@end
