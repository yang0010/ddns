#include"alisdk.h"
//#include"class_client.h"
#include"json/json.h"
#include"curl/curl.h"
#include"regex"
#include"stdio.h"
#include"vector"
Ddns::Ddns():configuration( "cn-hangzhou" ){
configuration.setConnectTimeout(1500);
configuration.setReadTimeout(4000);
info_setting();
re();
}

Ddns::Ddns(string AccessKeyID,string AccessKeySecret,string domainName,string type,string subDomainName):configuration( "cn-hangzhou" ){
configuration.setConnectTimeout(1500);
configuration.setReadTimeout(4000);
this->AccessKeyID=AccessKeyID;
this->AccessKeySecret=AccessKeySecret;
this->domainName=domainName;
this->type=type;
this->subDomainName=subDomainName;
}

void Ddns::info_setting(void){
ifstream ddns_setting("./setting.dat");
if(!ddns_setting.is_open()){
cout<<"文件打开失败或未发现配置文件！"<<endl;
cout<<"正在重新生成配置文件......."<<endl;
string tmpinfo="AccessKeyID=\"xxx\"\nAccessKeySecret=\"xxx\"\ndomainName=\"你的顶级域名\"\ntype=\"AAAA或者A记录\"\nsubDomainName=\"你的子域名比如www\"";
ofstream setting("./setting.dat");

setting.write(tmpinfo.c_str(),tmpinfo.size());
setting.close();
cout<<"配置文件生成成功，请设置好配置文件后再运行此程序！"<<endl;
exit(0);
}
stringstream buff;
buff<<ddns_setting.rdbuf();
fileinfo=buff.str();
ddns_setting.close();
}
void Ddns::re(void){
regex r("\"([\\w\\.]+)\"");
sregex_iterator pos(fileinfo.cbegin(),fileinfo.cend(),r);
sregex_iterator end;
vector<string> info;
for (;pos!=end;++pos)
info.push_back(pos->str(1));
if (info.size()!=5){
cout<<"配置文件读取不完整，请仔细设置setting文件，程序即将退出！"<<endl;
exit(0);
}
AccessKeyID=info[0];
AccessKeySecret=info[1];
domainName=info[2];
type=info[3];
subDomainName=info[4];
}


void Ddns::re2(void){
ostringstream res;
res<<"\\{\"RR\":\"("<<subDomainName<<")\",\"Line\":\"default\",\"Status\":\"[\\w]+\",\"Locked\":false,\"Type\":\"("<<type<<")\",\"DomainName\":\"("<<domainName<<")\",\"Value\":\"([\\w:]+)\",\"RecordId\":\"([\\d]+)\",\"TTL\":600,\"Weight\":1\\}";
regex r(res.str());
//("\\{\"RR\":\"[\\w]+\",\"Line\":\"default\",\"Status\":\"[\\w]+\",\"Locked\":false,\"Type\":\"[\\w]{4}\",\"DomainName\":\"[\\w\\.]+\",\"Value\":\"[\\w:]+\",\"RecordId\":\"[\\d]+\",\"TTL\":600,\"Weight\":1\\}");
domainnameinfo=GetDomainRecords();
sregex_iterator pos(domainnameinfo.cbegin(),domainnameinfo.cend(),r),end;
/*for(int i=1;i<pos->size();++i)
cout<<pos->str(i)<<endl;*/
if(pos->size()){
value=pos->str(4);
recordId=pos->str(5);
}
else{
cout<<"未从域名解析记录中获取到你的域名信息记录，请检查配置文件域名是否正确或者AccessKeyID！"<<endl;
exit(0);
}


}

bool Ddns::info(void){
if(recordId.empty())
re2();
cout<<"主域名: "<<domainName<<endl;
cout<<"子域名: "<<subDomainName<<endl;
cout<<"记录类型: "<<type<<endl;
cout<<"记录id: "<<recordId<<endl;
if(type=="AAAA"){
localip=ipv6();
cout<<"本机ip: "<<localip<<endl;
}
else{
localip=ipv4();
cout<<"本机ip: "<<localip<<endl;
}
cout<<"解析域名ip: "<<value<<endl;
if(localip==value){
cout<<"本机ip和解析域名ip一致，无需更新！"<<endl;
return true;
}
else{
cout<<"本机ip和解析域名ip不一致，请更新！"<<endl;
return false;
}

}

bool Ddns::ipinfo(){
if(recordId.empty())
re2();
if(type=="AAAA")
localip=ipv6();
else
localip=ipv4();
if(localip==value)
return true;
else
return false;
}
size_t save_html(void *buffer, size_t size, size_t count, void *p){
string *tmp=(string *)p;
size_t sz=size*count;
tmp->append((char*)buffer,sz);
return size*count;
}
string Ddns::ipv6(void){
CURL *curl=curl_easy_init();
string html;
if(curl){
curl_easy_setopt(curl,CURLOPT_URL,"https://ipv6.jsonip.com");
curl_easy_setopt(curl,CURLOPT_NOSIGNAL,1L);
curl_easy_setopt(curl,CURLOPT_TIMEOUT,5L);
curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,save_html);
curl_easy_setopt(curl, CURLOPT_WRITEDATA, &html);
curl_easy_setopt(curl,CURLOPT_USERAGENT,R"(User-Agent":"Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/73.0.3683.103 Safari/537.36)");
curl_easy_perform(curl);
curl_easy_cleanup(curl);
string err;
Json::Value root;
Json::CharReaderBuilder reader;
unique_ptr<Json::CharReader> jsonreader(reader.newCharReader());
jsonreader->parse(html.c_str(),html.c_str()+html.size(),&root,NULL);
//cout<<html<<endl;
return root["ip"].asString();

}
return "\0";
}

string Ddns::ipv4(void){
CURL *curl=curl_easy_init();
string html;
if(curl){
curl_easy_setopt(curl,CURLOPT_URL,"https://ipv4.jsonip.com");
curl_easy_setopt(curl,CURLOPT_NOSIGNAL,1L);
curl_easy_setopt(curl,CURLOPT_TIMEOUT,5L);
curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,save_html);
curl_easy_setopt(curl, CURLOPT_WRITEDATA, &html);
curl_easy_setopt(curl,CURLOPT_USERAGENT,R"(User-Agent":"Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/73.0.3683.103 Safari/537.36)");
curl_easy_perform(curl);
curl_easy_cleanup(curl);
string err;
Json::Value root;
Json::CharReaderBuilder reader;
unique_ptr<Json::CharReader> jsonreader(reader.newCharReader());
jsonreader->parse(html.c_str(),html.c_str()+html.size(),&root,NULL);
//cout<<html<<endl;
return root["ip"].asString();

}
return "\0";
}

/*string Ddns::ipv6addr(void){
string url="geoip.neu.edu.cn";
Client get(url,"80");
string requestHeader;
   requestHeader   = "GET / HTTP/1.1\r\n";
    requestHeader += "Host: " +url + "\r\n";*/
    //requestHeader += "Accept: */*\r\n";
    /*requestHeader += "User-Agent\":\"Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/73.0.3683.103 Safari/537.36";
     //requestHeader += "X-Requested-With\":\"mark.via";
    requestHeader += "connection:Keep-Alive\r\n";
    requestHeader += "\r\n";
    get.send(requestHeader);
    usleep(200000);
    string html=get.recv();
    //cout<<html<<endl;
    regex r("<p class=\"text-info lead\"> ([\\w\\.:]{15,39})");
	smatch m;
	regex_search(html,m,r);
	//cout<<m.str(1)<<endl;
	return m.str(1);
}

string Ddns::ipv4addr(void){
string url="ip.neu.edu.cn";
Client get(url,"80");
string requestHeader;
   requestHeader   = "GET / HTTP/1.1\r\n";
    requestHeader += "Host: " +url + "\r\n";*/
    //requestHeader += "Accept: */*\r\n";
    /*requestHeader += "User-Agent\":\"Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/73.0.3683.103 Safari/537.36";
    //requestHeader += "X-Requested-With\":\"mark.via";
    requestHeader += "connection:Keep-Alive\r\n";
    requestHeader += "\r\n";
    get.send(requestHeader);
    usleep(200000);
    string html=get.recv();
    //cout<<html<<endl;
    regex r("<p class=\"text-info lead\"> ([\\d\\.]+) </p>");
	smatch m;
	regex_search(html,m,r);
	return m.str(1);
}*/

bool Ddns::empty(void){
vector<int> tmp({AccessKeyID.empty(),AccessKeySecret.empty(),domainName.empty(),type.empty(),subDomainName.empty()});
int num=0;
for(int i: tmp)
num+=i;
return (num==5)?false:true;
}

string Ddns::GetDomainRecords(){
// specify timeout when create client.
//configuration.setConnectTimeout(1500);
//configuration.setReadTimeout(4000);
    AlibabaCloud::CommonClient client(AccessKeyID.c_str(), AccessKeySecret.c_str(), configuration );
    AlibabaCloud::CommonRequest request(AlibabaCloud::CommonRequest::RequestPattern::RpcPattern);
    request.setHttpMethod(AlibabaCloud::HttpRequest::Method::Post);
    request.setDomain("alidns.cn-hangzhou.aliyuncs.com");
    request.setVersion("2015-01-09");
    request.setQueryParameter("Action", "DescribeDomainRecords");
    request.setQueryParameter("Lang", "cn_hangzhou");
    request.setQueryParameter("DomainName",domainName.c_str());

    auto response = client.commonResponse(request);
    string info;
    if (response.isSuccess()) {
        //printf("request success.\n");
        //printf("result: %s\n", response.result().payload().c_str());
        info=response.result().payload().c_str();
    } else {
        printf("error: %s\n", response.error().errorMessage().c_str());
        printf("request id: %s\n", response.error().requestId().c_str());
        info="";
    }

    return info;
}

bool Ddns::update(void){
    AlibabaCloud::CommonClient client(AccessKeyID.c_str(),AccessKeySecret.c_str(),configuration);
    AlibabaCloud::CommonRequest request(AlibabaCloud::CommonRequest::RequestPattern::RpcPattern);
    request.setHttpMethod(AlibabaCloud::HttpRequest::Method::Post);
    request.setDomain("alidns.cn-hangzhou.aliyuncs.com");
    request.setVersion("2015-01-09");
    request.setQueryParameter("Action", "UpdateDomainRecord");
    request.setQueryParameter("Lang", "cn-hangzhou");
    request.setQueryParameter("RecordId", recordId.c_str());
    request.setQueryParameter("RR", subDomainName.c_str());
    request.setQueryParameter("Type", type.c_str());
    request.setQueryParameter("Value", localip.c_str());
    request.setQueryParameter("TTL", "600");

    auto response = client.commonResponse(request);
    if (response.isSuccess()) {
        /*printf("request success.\n");
        printf("result: %s\n", response.result().payload().c_str());*/
        value=localip;
        return true;
    } else {
        printf("error: %s\n", response.error().errorMessage().c_str());
        printf("request id: %s\n", response.error().requestId().c_str());
        return false;
    }

}

