#ifndef __ALISDK__
#define __ALISDK__
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <alibabacloud/core/AlibabaCloud.h>
#include <alibabacloud/core/CommonRequest.h>
#include <alibabacloud/core/CommonClient.h>
#include <alibabacloud/core/CommonResponse.h>

using namespace std;
using namespace AlibabaCloud;

class Ddns{
public:
Ddns();
Ddns(string,string,string,string,string);
~Ddns(){
AlibabaCloud::ShutdownSdk();
}
static string ipv4();
static string ipv6();
bool update(void);//更新域名解析
bool info(void);//输出域名信息并检查ip是否匹配
bool ipinfo(void);//检查本地ip和域名ip是否匹配
bool empty(void);//检查setting保存在内存的信息是否完整
private:
string AccessKeyID;
string AccessKeySecret;
string domainName;//主域名
string type;//记录类型，ipv6为AAAA
string subDomainName;//子域名
string fileinfo;//setting.dat配置文件存储
string domainnameinfo;//所有域名解析记录存储
string value;//ip地址
string localip;//本机ip
string recordId;//域名记录id
//string domaininfo;//主域名
AlibabaCloud::ClientConfiguration configuration;
void info_setting(void);
void re(void);
string GetDomainRecords(void);
void re2(void);

};

#endif