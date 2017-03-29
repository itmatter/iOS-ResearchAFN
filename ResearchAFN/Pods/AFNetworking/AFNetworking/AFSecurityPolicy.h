// AFSecurityPolicy.h
// Copyright (c) 2011–2016 Alamofire Software Foundation ( http://alamofire.org/ )
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#import <Foundation/Foundation.h>
#import <Security/Security.h>

typedef NS_ENUM(NSUInteger, AFSSLPinningMode) {
    AFSSLPinningModeNone,           //不做SSL pinning 只信任证书颁发机构证书，自己生成证书不通过
    AFSSLPinningModePublicKey,      //客户端保存证书拷贝 第一步验证证书的域名/有效期等信息，第二步是对比服务端返回的证书跟客户端返回的是否一致
    AFSSLPinningModeCertificate,    //客户端保存证书拷贝 只是验证时只验证证书里的公钥，不验证证书的有效期等信息
};

NS_ASSUME_NONNULL_BEGIN

@interface AFSecurityPolicy : NSObject <NSSecureCoding, NSCopying>


//SSL模式
@property (readonly, nonatomic, assign) AFSSLPinningMode SSLPinningMode;
//获取的证书集
@property (nonatomic, strong, nullable) NSSet <NSData *> *pinnedCertificates;

//是否信任服务器无效或过期的SSL证书. Defaults to `NO`.
@property (nonatomic, assign) BOOL allowInvalidCertificates;

//是否验证域名证书的CN字段. Defaults to `YES`.
@property (nonatomic, assign) BOOL validatesDomainName;

///-----------------------------------------
/// @name 从Bundle中获取证书
///-----------------------------------------

//返回任何在bundle中的证书,如果你使用AFN作为嵌入式框架,你必须调用该方法到你的app bundle中查找证书.并使用它们在创建你的安全策略通过调用“policyWithPinningMode:withPinnedCertificates”。
//返回bundle中的证书
+ (NSSet <NSData *> *)certificatesInBundle:(NSBundle *)bundle;

///-----------------------------------------
/// @name 获取指定的安全策略
///-----------------------------------------

//返回一个默认安全策略的单例对象,不允许无效证书,验证域名,不验证证书或公共密钥。
+ (instancetype)defaultPolicy;

///---------------------
/// @name 初始化
///---------------------


//用指定的模式创建一个安全策略
+ (instancetype)policyWithPinningMode:(AFSSLPinningMode)pinningMode;

//用指定的模式创建一个安全策略
+ (instancetype)policyWithPinningMode:(AFSSLPinningMode)pinningMode withPinnedCertificates:(NSSet <NSData *> *)pinnedCertificates;



#pragma mark - 核心证书校验方法
///------------------------------
/// @name 评估服务器信任度
///------------------------------

//基于安全策略判断,指定的服务器是否应该被接受信任。
//这个方法应该在服务器身份验证发生改变的时候调用。
- (BOOL)evaluateServerTrust:(SecTrustRef)serverTrust
                  forDomain:(nullable NSString *)domain;

@end

NS_ASSUME_NONNULL_END


/*
 4.SSL Pinning
 
 SSL Pinning
 SSL Pinning 即证书绑定，客户端直接保存服务端证书，建立HTTPS连接时会校验服务端返回证书和客户端证书是否一致，一致则不再去信任证书机构里验证。
 
 
 
 
 */

