#include <iostream>
#include <random>
#include "/usr/include/curl/curl.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::random_device;
typedef random_device Random;
#define CREATURLGET "https://tapi.jingtum.com/v1/wallet/new"
#define ACTIVITURL "https://tapi.jingtum.com/v1/accounts/jsqRs9BDCjyTuRWEPZk3yHa4MFmRi9D834/payments?validated=true"
#define URL_DEFAULT  "https://tapi.jingtum.com/"
//#define TRUSTURL "https://tapi.jingtum.com/v1/accounts/jsqRs9BDCjyTuRWEPZk3yHa4MFmRi9D834/trustlines?validated=true"

void getprocess(const string strURL)
{
	CURL* curl;
	CURLcode res;
	curl = curl_easy_init();
	if(NULL != curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, strURL.c_str());
		struct curl_slist *plist = curl_slist_append(NULL,   
                "Content-Type:application/json;charset=utf-8");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, plist);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}
}

void postprocess(const string strURL, const string strJson)
{
	CURL* curl;
	CURLcode res;
	curl = curl_easy_init();
	if(NULL != curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, strURL.c_str());
		struct curl_slist *plist = curl_slist_append(NULL,   
                "Content-Type:application/json;charset=utf-8");

		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, plist);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strJson.c_str());
		curl_easy_setopt(curl,CURLOPT_POST,1);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}
}
//获取信任信息
void gettrustinfo()
{
	string strURL;
	string strPerfix("https://tapi.jingtum.com/v1/accounts/");
	string strSuffix("/trustlines");
	string strAddress;
	cout << "输入地址：";
	cin >> strAddress;
	strURL = strPerfix+strAddress+strSuffix;
	
	//Get
	getprocess( strURL );
}

//余额获取
void getbalances()
{
	string strURL;
	string strPerfix("https://tapi.jingtum.com/v1/accounts/");
	string strSuffix("/balances");
	string strAddress;
	cout << "输入地址：";
	cin >> strAddress;
	strURL = strPerfix+strAddress+strSuffix;
	
	//Get
	getprocess( strURL );
}

//信任
void posttrust( const string trustingAdd, 
				const string trustedAdd,
				string strSecret = "sn7wfavva6mV1wbjtHR1mwXjEkb8Y",
				string strLimit = "888",
				string strCurrency = "8000000001201504081454405641668733216531")
{
	cout<< endl << "trust" << endl;
	//设置URL
	string strURL;
	string strPerfix_trusting("https://tapi.jingtum.com/v1/accounts/");
	string strSuffix_trusting("/trustlines?validated=true");
	strURL = strPerfix_trusting+trustingAdd+strSuffix_trusting;
	//设置Json串
	//string strPerfix_trustedAdd("{\"secret\":  \"sn7wfavva6mV1wbjtHR1mwXjEkb8Y\",\"trustline\": {\"limit\": \"888\",\"currency\": \"8000000001201504081454405641668733216531\",\"counterparty\": \"");
	
	string strPresec("{\"secret\":  \"");
	string strPrelimit("\",\"trustline\": {\"limit\": \"");
	string strPrecurrency("\",\"currency\": \"");
	string strPrecounterparty("\",\"counterparty\": \"");
	string strSuffix_json("\"}}");
	
	string strJson;
	strJson = strPresec+strSecret+strPrelimit+strLimit+strPrecurrency+strCurrency+strPrecounterparty+trustedAdd+strSuffix_json;

	//Post
	postprocess(strURL, strJson);
}

//随机创建client_resource_id 需要支持c++11编译(-std=c++11)
//有很小概率获得重复的client_resource_id，导致激活失败
//可以修改数组中的值，改变随机内容
void creatorederno(string& orederno)
{
	char str[] = "123456789ABCabcDTYdty123456789";
	int len  = sizeof(str);
	Random rd;
	int i = 0;
	for(i=0; i<len-1; i++)
	{
		int index = rd()%(len-1);
		orederno += str[index];
	}
}

//激活
void activiting( const string str )
{
	cout << endl << "activiting" << endl;
	CURL* curl;
	CURLcode res;
	
	string strPrefix("{\"secret\":  \"sn7wfavva6mV1wbjtHR1mwXjEkb8Y\",\"client_resource_id\": \"");
	string orederNo;
	//创建client_resource_id
	creatorederno(orederNo);
	string strMidfix("\",\"payment\": {\"source_account\": \"jsqRs9BDCjyTuRWEPZk3yHa4MFmRi9D834\",\"destination_account\": \"");
	string strSuffix("\",\"destination_amount\": {\"value\": \"200.1\",\"currency\": \"SWT\",\"issuer\": \"\"}}}");
	string strJson; 
	strJson = strPrefix+orederNo+strMidfix+str+strSuffix;

	curl = curl_easy_init();
	if(NULL != curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, ACTIVITURL);
		struct curl_slist *plist = curl_slist_append(NULL,   
                "Content-Type:application/json;charset=utf-8");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, plist);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strJson.c_str());
		curl_easy_setopt(curl,CURLOPT_POST,1);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}
}

//curl回调函数
size_t write_data(void* buffer,size_t size,size_t nmemb,void *stream)  
{  
	FILE *fptr = (FILE*)stream;  
	fwrite(buffer,size,nmemb,fptr);  
	string str = (char*)buffer;
	cout << str.c_str() << endl;
	int lenth_add = sizeof("\"address\": \"");
	int lenth = str.find("\"address\": \"");
	string strAccount = str.substr(lenth+lenth_add-1, 34);
	int lenth_sec = sizeof("\"secret\": \"");
	lenth = str.find("\"secret\": \"");
	string strSecret = str.substr(lenth+lenth_sec-1, 29);
	string trustingAdd("jsqRs9BDCjyTuRWEPZk3yHa4MFmRi9D834");
	//激活刚创建的地址,所有刚创建的地址均被jsqRs9BDCjyTuRWEPZk3yHa4MFmRi9D834激活
	activiting(strAccount);
	//信任刚创建的地址,所有刚创建的地址均被jsqRs9BDCjyTuRWEPZk3yHa4MFmRi9D834信任
	posttrust(trustingAdd, strAccount);
	
	return size*nmemb;
}

//创建地址
void creataccount()
{
	int count = 0;
	cout << "请输入创建地址的数量：";
	cin >> count;
	int i = 0;
	FILE *fptr = NULL;
	for (i=0; i<count; i++)
	{
		CURL* curl;
		CURLcode res;

		if ( NULL == (fptr = fopen("accountid.txt","a+")) )  
		{  
			cout << "fopen file error:%s\n";  
			return;  
		}  
	
		cout << endl << "creat address" <<endl;
		curl = curl_easy_init();
		if(NULL != curl)
		{
			curl_easy_setopt(curl, CURLOPT_URL, CREATURLGET);
			struct curl_slist *plist = curl_slist_append(NULL,   
                "Content-Type:application/json;charset=utf-8");
			curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,write_data);
			curl_easy_setopt(curl,CURLOPT_WRITEDATA,fptr);
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, plist);
			res = curl_easy_perform(curl);
			curl_easy_cleanup(curl);
		}
		fclose(fptr);
	}

	return;
}

int main()
{
	creataccount();
	
	while(true)
	{
		cout << endl;
		cout << "获取余额信息：1" << endl;
		cout << "获取信任信息：2" << endl;
		cout << "设置信任    ：3" << endl;
		cout << "退出        ：0" <<endl;
		int a;
		cin >> a;
		if(0 == a)break;
		
		switch(a)
		{
			case 1 :
				getbalances();
				break;
			case 2 :
				gettrustinfo();
				break;
			case 3 :
			{

				cout << "输入信任方地址:" << endl;	
				string trustingAdd;
				cin >> trustingAdd;
	
				cout << "输入信任方密码:" << endl;	
				string strSecret;
				cin >> strSecret;
	
				cout << "输入被信任地址:" << endl;	
				string trustedAdd;
				cin >> trustedAdd;
	
				cout << "输入信任金额:" <<endl;
				string strMonety;
				cin >> strMonety;
	
				cout << "输入货币种类:" << endl;
				string strCurrency;
				cin >> strCurrency;
	
				posttrust(trustingAdd, trustedAdd, strSecret, strMonety, strCurrency);
				break;
			}
			default:
				break;
		}
	}
	
	return 0;
}

