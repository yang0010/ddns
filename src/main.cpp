#include"alisdk.h"
#include"unistd.h"
#include"cstring"
int main(int argc,char *argv[]){

if(argc==2 && strcmp(*(argv+1),"ipv4")==0){
cout<<Ddns::ipv4()<<endl;
return 0;
}
else if(argc==2 && strcmp(*(argv+1),"ipv6")==0){
cout<<Ddns::ipv6()<<endl;
return 0;
}
else if(argc==7 && strcmp(*(argv+1),"d")==0){
Ddns ddns(*(argv+2),*(argv+3),*(argv+4),*(argv+5),*(argv+6));
if(!ddns.ipinfo()){
if(ddns.update())
cout<<"更新成功！"<<endl;
return 0;
}
else{
cout<<"本机和域名解析ip一致！"<<endl;
return 0;
}
}
else{
if(argc>1){
cout<<"指令输入错误，请输入参数 help 获取帮助！"<<endl;
return 0;
}
}
cout<<"使用说明 :"<<endl;
cout<<"程序参数:(如何使用:比如 ./ddns ipv4 即可返回本机ipv4信息)"<<endl;
cout<<"ipv4 返回ipv4地址"<<endl;
cout<<"ipv6 返回ipv6地址"<<endl;
cout<<"d AccessKeyID AccessKeySecret 你的顶级域名 解析记录类型AAAA或者A 你的子域名 此参数为临时单次解析，可不需要设置配置文件，可配合crontab之内的配合使用！"<<endl;
cout<<"比如: ./ddns d xxx xxx baidu.com AAAA www"<<endl;

/*Ddns dns;
while(1){
if(!dns.ipinfo()){
cout<<"本机ip和域名ip不一致，正在更新......."<<endl;
if(dns.update())
cout<<"更新成功，一分钟后将再次检查！"<<endl;
else{
cout<<"ip更新失败，正在尝试重试！"<<endl;
continue;
}
}
else
cout<<"ip一致，一分钟后将再次检查！"<<endl;

sleep(60);
}
return 0;*/
}