/*	NSURLSession.h
	Copyright (c) 2013-2016, Apple Inc. All rights reserved.
 */

#import <Foundation/NSObject.h>
#import <Foundation/NSURLRequest.h>
#import <Foundation/NSHTTPCookieStorage.h>

#include <Security/SecureTransport.h>

@class NSString;
@class NSURL;
@class NSError;
@class NSArray<ObjectType>;
@class NSDictionary<KeyType, ObjectType>;
@class NSInputStream;
@class NSOutputStream;
@class NSData;
@class NSOperationQueue;

@class NSURLCache;
@class NSURLResponse;
@class NSHTTPURLResponse;
@class NSHTTPCookie;
@class NSCachedURLResponse;
@class NSURLAuthenticationChallenge;
@class NSURLProtectionSpace;
@class NSURLCredential;
@class NSURLCredentialStorage;
@class NSURLSessionDataTask;
@class NSURLSessionUploadTask;
@class NSURLSessionDownloadTask;
@class NSNetService;

@class NSURLSession;
@class NSURLSessionDataTask;                /* DataTask objects receive the payload through zero or more delegate messages */
@class NSURLSessionUploadTask;              /* UploadTask objects receive periodic progress updates but do not return a body */
@class NSURLSessionDownloadTask;            /* DownloadTask objects represent an active download to disk.  They can provide resume data when canceled. */
@class NSURLSessionStreamTask;              /* StreamTask objects may be used to create NSInput and NSOutputStreams, or used directly in reading and writing. */
@class NSURLSessionConfiguration;
@protocol NSURLSessionDelegate;

@class NSURLSessionTaskMetrics;
@class NSDateInterval;

NS_ASSUME_NONNULL_BEGIN

/*
 
 NSURLSession is not available for i386 targets before Mac OS X 10.10.
 
 */

#if __OBJC2__
#define NSURLSESSION_AVAILABLE	10_9
#else
#define NSURLSESSION_AVAILABLE	10_10
#endif


FOUNDATION_EXPORT const int64_t NSURLSessionTransferSizeUnknown NS_AVAILABLE(NSURLSESSION_AVAILABLE, 7_0);    /* -1LL */

NS_CLASS_AVAILABLE(NSURLSESSION_AVAILABLE, 7_0)


#pragma mark -
#pragma mark - NSURLSession
@interface NSURLSession : NSObject


#if FOUNDATION_SWIFT_SDK_EPOCH_AT_LEAST(8)
@property (class, readonly, strong) NSURLSession *sharedSession;        //会话管理者
#endif


+ (NSURLSession *)sessionWithConfiguration:(NSURLSessionConfiguration *)configuration;          //设置配置
+ (NSURLSession *)sessionWithConfiguration:(NSURLSessionConfiguration *)configuration           //设置配置
                                  delegate:(nullable id <NSURLSessionDelegate>)delegate         //设置代理
                             delegateQueue:(nullable NSOperationQueue *)queue;                  //设置操作队列

@property (readonly, retain) NSOperationQueue *delegateQueue;                   //代理操作队列
@property (nullable, readonly, retain) id <NSURLSessionDelegate> delegate;      //代理
@property (readonly, copy) NSURLSessionConfiguration *configuration;            //配置
@property (nullable, copy) NSString *sessionDescription;                        //描述

- (void)finishTasksAndInvalidate;       //完成任务并且销毁session
- (void)invalidateAndCancel;            //销毁session并且取消

- (void)resetWithCompletionHandler:(void (^)(void))completionHandler;   //清空所有的Cookie,缓存和证书存储,删除磁盘中的文件,移除正在进行的任务,并确保下一个请求是新的请求
- (void)flushWithCompletionHandler:(void (^)(void))completionHandler;   //清空磁盘中的Cookie和凭证,清除临时缓存,并确保下一个请求是新的TCP请求

- (void)getTasksWithCompletionHandler:(void (^)(NSArray<NSURLSessionDataTask *> *dataTasks, NSArray<NSURLSessionUploadTask *> *uploadTasks, NSArray<NSURLSessionDownloadTask *> *downloadTasks))completionHandler;

- (void)getAllTasksWithCompletionHandler:(void (^)(NSArray<__kindof NSURLSessionTask *> *tasks))completionHandler NS_AVAILABLE(10_11, 9_0);





/*----------------------------------------------------------------------------
 
 在接下在的所有网络数据请求的方法中都会调用代理实现一下的代理方法.不同的方法中调用的不同的代理协议
 例如: 
 dataTaskWithRequest  --  代理协议 :NSURLSessionTaskDelegate
 uploadTaskWithRequest  --  代理协议 :NSURLSessionTaskDelegate
 downloadTaskWithRequest  --  代理协议 :NSURLSessionDownloadDelegate
 

----------------------------------------------------------------------------*/


//请求数据
- (NSURLSessionDataTask *)dataTaskWithRequest:(NSURLRequest *)request;      //网络数据请求 : request
- (NSURLSessionDataTask *)dataTaskWithURL:(NSURL *)url;                     //网络数据请求 : url

//上传
- (NSURLSessionUploadTask *)uploadTaskWithRequest:(NSURLRequest *)request fromFile:(NSURL *)fileURL;
- (NSURLSessionUploadTask *)uploadTaskWithRequest:(NSURLRequest *)request fromData:(NSData *)bodyData;
- (NSURLSessionUploadTask *)uploadTaskWithStreamedRequest:(NSURLRequest *)request;


//下载
- (NSURLSessionDownloadTask *)downloadTaskWithRequest:(NSURLRequest *)request;
- (NSURLSessionDownloadTask *)downloadTaskWithURL:(NSURL *)url;
- (NSURLSessionDownloadTask *)downloadTaskWithResumeData:(NSData *)resumeData;

//任务流
- (NSURLSessionStreamTask *)streamTaskWithHostName:(NSString *)hostname port:(NSInteger)port NS_AVAILABLE(10_11, 9_0) __WATCHOS_PROHIBITED;
- (NSURLSessionStreamTask *)streamTaskWithNetService:(NSNetService *)service NS_AVAILABLE(10_11, 9_0) __WATCHOS_PROHIBITED;

@end

#pragma mark -
#pragma mark - NSURLSession(NSURLSessionAsynchronousConvenience)
@interface NSURLSession (NSURLSessionAsynchronousConvenience)

//网络请求
- (NSURLSessionDataTask *)dataTaskWithRequest:(NSURLRequest *)request
                            completionHandler:(void (^)(NSData * _Nullable data, NSURLResponse * _Nullable response, NSError * _Nullable error))completionHandler;

- (NSURLSessionDataTask *)dataTaskWithURL:(NSURL *)url
                        completionHandler:(void (^)(NSData * _Nullable data, NSURLResponse * _Nullable response, NSError * _Nullable error))completionHandler;

//上传
- (NSURLSessionUploadTask *)uploadTaskWithRequest:(NSURLRequest *)request
                                         fromFile:(NSURL *)fileURL
                                completionHandler:(void (^)(NSData * _Nullable data, NSURLResponse * _Nullable response, NSError * _Nullable error))completionHandler;

- (NSURLSessionUploadTask *)uploadTaskWithRequest:(NSURLRequest *)request
                                         fromData:(nullable NSData *)bodyData
                                completionHandler:(void (^)(NSData * _Nullable data, NSURLResponse * _Nullable response, NSError * _Nullable error))completionHandler;

//下载
- (NSURLSessionDownloadTask *)downloadTaskWithRequest:(NSURLRequest *)request
                                    completionHandler:(void (^)(NSURL * _Nullable location, NSURLResponse * _Nullable response, NSError * _Nullable error))completionHandler;

- (NSURLSessionDownloadTask *)downloadTaskWithURL:(NSURL *)url
                                completionHandler:(void (^)(NSURL * _Nullable location, NSURLResponse * _Nullable response, NSError * _Nullable error))completionHandler;

- (NSURLSessionDownloadTask *)downloadTaskWithResumeData:(NSData *)resumeData
                                       completionHandler:(void (^)(NSURL * _Nullable location, NSURLResponse * _Nullable response, NSError * _Nullable error))completionHandler;

@end

typedef NS_ENUM(NSInteger, NSURLSessionTaskState) {
    NSURLSessionTaskStateRunning = 0,   /* 任务执行中 */
    NSURLSessionTaskStateSuspended = 1, /* 任务暂停 */
    NSURLSessionTaskStateCanceling = 2, /* 任务取消.  调用方法 URLSession:task:didCompleteWithError: message. */
    NSURLSessionTaskStateCompleted = 3, /* 任务完成,不调用任何代理方法 */
} NS_ENUM_AVAILABLE(NSURLSESSION_AVAILABLE, 7_0);


NS_CLASS_AVAILABLE(NSURLSESSION_AVAILABLE, 7_0)


#pragma mark -
#pragma mark - NSURLSessionTask
@interface NSURLSessionTask : NSObject <NSCopying>

@property (readonly)                 NSUInteger    taskIdentifier;    /* an identifier for this task, assigned by and unique to the owning session */
@property (nullable, readonly, copy) NSURLRequest  *originalRequest;  /* may be nil if this is a stream task */
@property (nullable, readonly, copy) NSURLRequest  *currentRequest;   /* may differ from originalRequest due to http server redirection */
@property (nullable, readonly, copy) NSURLResponse *response;         /* may be nil if no response has been received */



@property (readonly) int64_t countOfBytesReceived;          /* 已经接收的字节数 */
@property (readonly) int64_t countOfBytesSent;              /* 已经发送的字节数 */
@property (readonly) int64_t countOfBytesExpectedToSend;    /* number of body bytes we expect to send, derived from the Content-Length of the HTTP request */
@property (readonly) int64_t countOfBytesExpectedToReceive; /* number of byte bytes we expect to receive, usually derived from the Content-Length header of an HTTP response. */



@property (nullable, copy) NSString *taskDescription;


- (void)cancel;

@property (readonly) NSURLSessionTaskState state;
@property (nullable, readonly, copy) NSError *error;


- (void)suspend;
- (void)resume;


@property float priority NS_AVAILABLE(10_10, 8_0);

@end

FOUNDATION_EXPORT const float NSURLSessionTaskPriorityDefault NS_AVAILABLE(10_10, 8_0);
FOUNDATION_EXPORT const float NSURLSessionTaskPriorityLow NS_AVAILABLE(10_10, 8_0);
FOUNDATION_EXPORT const float NSURLSessionTaskPriorityHigh NS_AVAILABLE(10_10, 8_0);


#pragma mark -
#pragma mark - NSURLSessionDataTask
@interface NSURLSessionDataTask : NSURLSessionTask
@end

#pragma mark -
#pragma mark - NSURLSessionUploadTask
@interface NSURLSessionUploadTask : NSURLSessionDataTask
@end

#pragma mark -
#pragma mark - NSURLSessionDownloadTask
@interface NSURLSessionDownloadTask : NSURLSessionTask


- (void)cancelByProducingResumeData:(void (^)(NSData * _Nullable resumeData))completionHandler;

@end

NS_CLASS_AVAILABLE(10_11, 9_0)
@interface NSURLSessionStreamTask : NSURLSessionTask

- (void)readDataOfMinLength:(NSUInteger)minBytes
                  maxLength:(NSUInteger)maxBytes
                    timeout:(NSTimeInterval)timeout
          completionHandler:(void (^) (NSData * _Nullable data, BOOL atEOF, NSError * _Nullable error))completionHandler;

- (void)writeData:(NSData *)data
          timeout:(NSTimeInterval)timeout
completionHandler:(void (^) (NSError * _Nullable error))completionHandler;

- (void)captureStreams;
- (void)closeWrite;
- (void)closeRead;


- (void)startSecureConnection;
- (void)stopSecureConnection;

@end


NS_CLASS_AVAILABLE(NSURLSESSION_AVAILABLE, 7_0)



#pragma mark -
#pragma mark - NSURLSessionConfiguration
@interface NSURLSessionConfiguration : NSObject <NSCopying>

#if FOUNDATION_SWIFT_SDK_EPOCH_AT_LEAST(8)
@property (class, readonly, strong) NSURLSessionConfiguration *defaultSessionConfiguration;         //默认配置
@property (class, readonly, strong) NSURLSessionConfiguration *ephemeralSessionConfiguration;       //临时配置
#endif

+ (NSURLSessionConfiguration *)backgroundSessionConfigurationWithIdentifier:(NSString *)identifier NS_AVAILABLE(10_10, 8_0);    //后台会话配置

@property (nullable, readonly, copy) NSString *identifier;                                  //标识符
@property NSURLRequestCachePolicy requestCachePolicy;                                       //
@property NSTimeInterval timeoutIntervalForRequest;                                         //请求超时时间
@property NSTimeInterval timeoutIntervalForResource;                                        //资源请求时间
@property NSURLRequestNetworkServiceType networkServiceType;                                //网络服务类型
@property BOOL allowsCellularAccess;                                                        //是否允许移动蜂窝数据访问
@property (getter=isDiscretionary) BOOL discretionary NS_AVAILABLE(10_10, 7_0);             //是否可以安排在后台任务的系统默认的最优性能方式处理
@property (nullable, copy) NSString *sharedContainerIdentifier NS_AVAILABLE(10_10, 8_0);    //
@property BOOL sessionSendsLaunchEvents NS_AVAILABLE(NA, 7_0);                              //
@property (nullable, copy) NSDictionary *connectionProxyDictionary;
@property SSLProtocol TLSMinimumSupportedProtocol;
@property SSLProtocol TLSMaximumSupportedProtocol;
@property BOOL HTTPShouldUsePipelining;
@property BOOL HTTPShouldSetCookies;
@property NSHTTPCookieAcceptPolicy HTTPCookieAcceptPolicy;
@property (nullable, copy) NSDictionary *HTTPAdditionalHeaders;
@property NSInteger HTTPMaximumConnectionsPerHost;                                          //每个链接最大请求数
@property (nullable, retain) NSHTTPCookieStorage *HTTPCookieStorage;                        //网络请求缓存数据
@property (nullable, retain) NSURLCredentialStorage *URLCredentialStorage;
@property (nullable, retain) NSURLCache *URLCache;
@property BOOL shouldUseExtendedBackgroundIdleMode NS_AVAILABLE(10_11, 9_0);
@property (nullable, copy) NSArray<Class> *protocolClasses;                                 //协议类

@end

//为各种委托信息配置选项
typedef NS_ENUM(NSInteger, NSURLSessionAuthChallengeDisposition) {
    NSURLSessionAuthChallengeUseCredential = 0,             /* 指定凭证 */
    NSURLSessionAuthChallengePerformDefaultHandling = 1,    /* Default handling for the challenge - as if this delegate were not implemented; the credential parameter is ignored. */
    NSURLSessionAuthChallengeCancelAuthenticationChallenge = 2, /* The entire request will be canceled; the credential parameter is ignored. */
    NSURLSessionAuthChallengeRejectProtectionSpace = 3,         /* This challenge is rejected and the next authentication protection space should be tried; the credential parameter is ignored. */
} NS_ENUM_AVAILABLE(NSURLSESSION_AVAILABLE, 7_0);


typedef NS_ENUM(NSInteger, NSURLSessionResponseDisposition) {
    NSURLSessionResponseCancel = 0,                                      /* Cancel the load, this is the same as -[task cancel] */
    NSURLSessionResponseAllow = 1,                                       /* Allow the load to continue */
    NSURLSessionResponseBecomeDownload = 2,                              /* Turn this request into a download */
    NSURLSessionResponseBecomeStream NS_ENUM_AVAILABLE(10_11, 9_0) = 3,  /* Turn this task into a stream task */
} NS_ENUM_AVAILABLE(NSURLSESSION_AVAILABLE, 7_0);



#pragma mark -
#pragma mark - 代理方法
@protocol NSURLSessionDelegate <NSObject>
@optional
/*
 * NSURLSessionDelegate specifies the methods that a session delegate
 * may respond to.  There are both session specific messages (for
 * example, connection based auth) as well as task based messages.
 */

/*
 * Messages related to the URL session as a whole
 */
- (void)URLSession:(NSURLSession *)session didBecomeInvalidWithError:(nullable NSError *)error;
- (void)URLSession:(NSURLSession *)session didReceiveChallenge:(NSURLAuthenticationChallenge *)challenge
 completionHandler:(void (^)(NSURLSessionAuthChallengeDisposition disposition, NSURLCredential * _Nullable credential))completionHandler;
- (void)URLSessionDidFinishEventsForBackgroundURLSession:(NSURLSession *)session NS_AVAILABLE_IOS(7_0);
@end


@protocol NSURLSessionTaskDelegate <NSURLSessionDelegate>
@optional


- (void)URLSession:(NSURLSession *)session task:(NSURLSessionTask *)task
willPerformHTTPRedirection:(NSHTTPURLResponse *)response
        newRequest:(NSURLRequest *)request
 completionHandler:(void (^)(NSURLRequest * _Nullable))completionHandler;


- (void)URLSession:(NSURLSession *)session task:(NSURLSessionTask *)task
didReceiveChallenge:(NSURLAuthenticationChallenge *)challenge
 completionHandler:(void (^)(NSURLSessionAuthChallengeDisposition disposition, NSURLCredential * _Nullable credential))completionHandler;


- (void)URLSession:(NSURLSession *)session task:(NSURLSessionTask *)task
 needNewBodyStream:(void (^)(NSInputStream * _Nullable bodyStream))completionHandler;


- (void)URLSession:(NSURLSession *)session task:(NSURLSessionTask *)task
   didSendBodyData:(int64_t)bytesSent
    totalBytesSent:(int64_t)totalBytesSent
totalBytesExpectedToSend:(int64_t)totalBytesExpectedToSend;


- (void)URLSession:(NSURLSession *)session task:(NSURLSessionTask *)task didFinishCollectingMetrics:(NSURLSessionTaskMetrics *)metrics API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0));


- (void)URLSession:(NSURLSession *)session task:(NSURLSessionTask *)task
didCompleteWithError:(nullable NSError *)error;

@end


@protocol NSURLSessionDataDelegate <NSURLSessionTaskDelegate>
@optional

- (void)URLSession:(NSURLSession *)session dataTask:(NSURLSessionDataTask *)dataTask
didReceiveResponse:(NSURLResponse *)response
 completionHandler:(void (^)(NSURLSessionResponseDisposition disposition))completionHandler;


- (void)URLSession:(NSURLSession *)session dataTask:(NSURLSessionDataTask *)dataTask
didBecomeDownloadTask:(NSURLSessionDownloadTask *)downloadTask;


- (void)URLSession:(NSURLSession *)session dataTask:(NSURLSessionDataTask *)dataTask
didBecomeStreamTask:(NSURLSessionStreamTask *)streamTask;


- (void)URLSession:(NSURLSession *)session dataTask:(NSURLSessionDataTask *)dataTask
    didReceiveData:(NSData *)data;


- (void)URLSession:(NSURLSession *)session dataTask:(NSURLSessionDataTask *)dataTask
 willCacheResponse:(NSCachedURLResponse *)proposedResponse
 completionHandler:(void (^)(NSCachedURLResponse * _Nullable cachedResponse))completionHandler;

@end


@protocol NSURLSessionDownloadDelegate <NSURLSessionTaskDelegate>


- (void)URLSession:(NSURLSession *)session downloadTask:(NSURLSessionDownloadTask *)downloadTask
didFinishDownloadingToURL:(NSURL *)location;

@optional

- (void)URLSession:(NSURLSession *)session downloadTask:(NSURLSessionDownloadTask *)downloadTask
      didWriteData:(int64_t)bytesWritten
 totalBytesWritten:(int64_t)totalBytesWritten
totalBytesExpectedToWrite:(int64_t)totalBytesExpectedToWrite;


- (void)URLSession:(NSURLSession *)session downloadTask:(NSURLSessionDownloadTask *)downloadTask
 didResumeAtOffset:(int64_t)fileOffset
expectedTotalBytes:(int64_t)expectedTotalBytes;

@end

@protocol NSURLSessionStreamDelegate <NSURLSessionTaskDelegate>
@optional


- (void)URLSession:(NSURLSession *)session readClosedForStreamTask:(NSURLSessionStreamTask *)streamTask;
- (void)URLSession:(NSURLSession *)session writeClosedForStreamTask:(NSURLSessionStreamTask *)streamTask;
- (void)URLSession:(NSURLSession *)session betterRouteDiscoveredForStreamTask:(NSURLSessionStreamTask *)streamTask;
- (void)URLSession:(NSURLSession *)session streamTask:(NSURLSessionStreamTask *)streamTask
didBecomeInputStream:(NSInputStream *)inputStream
      outputStream:(NSOutputStream *)outputStream;

@end

/* Key in the userInfo dictionary of an NSError received during a failed download. */
FOUNDATION_EXPORT NSString * const NSURLSessionDownloadTaskResumeData NS_AVAILABLE(NSURLSESSION_AVAILABLE, 7_0);

@interface NSURLSessionConfiguration (NSURLSessionDeprecated)
+ (NSURLSessionConfiguration *)backgroundSessionConfiguration:(NSString *)identifier NS_DEPRECATED(NSURLSESSION_AVAILABLE, 10_10, 7_0, 8_0, "Please use backgroundSessionConfigurationWithIdentifier: instead");
@end


typedef NS_ENUM(NSInteger, NSURLSessionTaskMetricsResourceFetchType) {
    NSURLSessionTaskMetricsResourceFetchTypeUnknown,
    NSURLSessionTaskMetricsResourceFetchTypeNetworkLoad,   /* The resource was loaded over the network. */
    NSURLSessionTaskMetricsResourceFetchTypeServerPush,    /* The resource was pushed by the server to the client. */
    NSURLSessionTaskMetricsResourceFetchTypeLocalCache,    /* The resource was retrieved from the local storage. */
} API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0));


API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0))

@interface NSURLSessionTaskTransactionMetrics : NSObject

@property (copy, readonly) NSURLRequest *request;
@property (nullable, copy, readonly) NSURLResponse *response;
@property (nullable, copy, readonly) NSDate *fetchStartDate;
@property (nullable, copy, readonly) NSDate *domainLookupStartDate;
@property (nullable, copy, readonly) NSDate *domainLookupEndDate;
@property (nullable, copy, readonly) NSDate *connectStartDate;
@property (nullable, copy, readonly) NSDate *secureConnectionStartDate;
@property (nullable, copy, readonly) NSDate *secureConnectionEndDate;
@property (nullable, copy, readonly) NSDate *connectEndDate;
@property (nullable, copy, readonly) NSDate *requestStartDate;
@property (nullable, copy, readonly) NSDate *requestEndDate;
@property (nullable, copy, readonly) NSDate *responseStartDate;
@property (nullable, copy, readonly) NSDate *responseEndDate;
@property (nullable, copy, readonly) NSString *networkProtocolName;
@property (assign, readonly, getter=isProxyConnection) BOOL proxyConnection;
@property (assign, readonly, getter=isReusedConnection) BOOL reusedConnection;
@property (assign, readonly) NSURLSessionTaskMetricsResourceFetchType resourceFetchType;


-(instancetype)init;


@end


API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0))
@interface NSURLSessionTaskMetrics : NSObject
@property (copy, readonly) NSArray<NSURLSessionTaskTransactionMetrics *> *transactionMetrics;
@property (copy, readonly) NSDateInterval *taskInterval;
@property (assign, readonly) NSUInteger redirectCount;

-(instancetype)init;

@end

NS_ASSUME_NONNULL_END
