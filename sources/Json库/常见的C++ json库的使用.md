## 常见的C++ Json库
关于JSON库的描述，可以参见维基百科[https://en.wikipedia.org/wiki/JSON](https://en.wikipedia.org/wiki/JSON)以及[Introducing JSON](https://www.json.org/json-en.html)，感觉C/C++对于Json解析没有其他高级语言比如Java、C#、JavaScript的支持那么友好，尤其在序列化和反序列化方面。

目前常见的C++的Json解析库有：
- [RapidJSON](https://github.com/Tencent/rapidjson)
A fast JSON parser/generator for C++ with both SAX/DOM style API
- [nlohmann/json](https://github.com/nlohmann/json)
JSON for Modern C++
一个非常不错的Json解析库，支持序列化和反序列化，对自定义的结构体支持比较友好
- Qt的解析库
- Boost库中的json解析
在Github上面有一个仓库[miloyip/nativejson-benchmark](https://github.com/miloyip/nativejson-benchmark)，描述了常见的C/C++ Json解析库的性能对比。
另外，知乎上面有篇帖子讨论了[有哪些 C++ 的 JSON 库比较好呢？](https://www.zhihu.com/question/20211077)，可以借鉴一下。
我觉得有位兄弟对于虑[nlohmann/json](https://github.com/nlohmann/json)和[rapidjson](https://github.com/Tencent/rapidjson)这两个C++解析库的评论和对比比较到位，所以建议不是很追求效率的条件还是建议使用[nlohmann/json](https://github.com/nlohmann/json)，否则可以选择腾讯的[rapidjson](https://github.com/Tencent/rapidjson)。
非常推荐 [nlohmann/json](https://github.com/nlohmann/json)看性能测试应该是rapidjson效率最好，所以最开始我也用的rapidjson](https://github.com/Tencent/rapidjson)，但我发现[rapidjson](https://github.com/Tencent/rapidjson)的接口太难用了，太不友好，所以我就换成了[nlohmann/json](https://github.com/nlohmann/json0我觉得除非你的项目对效率要求极度严格，已经明确到需要考虑[nlohmann/json](https://github.com/nlohmann/json)和[rapidjson](https://github.com/Tencent/rapidjson)之间的这一点效率差异了，否则毫不犹豫用[nlohmann/json](https://github.com/nlohmann/json)吧，现代C++网格，用起来相当简单。为了那么一点可能根本不需要考虑的效率问题而去选择[rapidjson](https://github.com/Tencent/rapidjson)，而导致编码难度和心智负担大大提高，我觉得简直得不偿失。

作者：zhllxt
链接：https://www.zhihu.com/question/20211077/answer/1620192069
来源：知乎
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

下面是我在实际项目中使用nlohmann/json库以及Qt的Json库对自定义结构体进行序列化和反序列化的一些代码片段：
```cpp
#ifndef _ADVANCEPARAMDEFINE_H
#define _ADVANCEPARAMDEFINE_H

#include <QJsonArray>
#include <QJsonObject>
#include <QMap>

#include <string>
#include <nlohmann/json.hpp>

using namespace std;
using namespace nlohmann;

struct MotorInitParam
{
	int			nAcc;
	int			nLimitOffset;
	int			nReverseOffset;
	int			nSpeed;
	double		dbGear;	   // 转换系数
	int			eAxisType; //EHSK_AXIS_TYPE 0-4 x\y\z\刮片\墨栈
};

//马达初始化参数
struct MotorInitParamInfo
{
	bool                    bSupportMotorY;
	bool                    bSupportMotorZ;
	bool                    bSupportMotorScrap;

	std::vector<MotorInitParam>  motorParams;

	QJsonObject toJsonObject()
	{
		QJsonObject jo;
		jo.insert("bSupportMotorY", bSupportMotorY);
		jo.insert("bSupportMotorZ", bSupportMotorZ);
		jo.insert("bSupportMotorScrap", bSupportMotorScrap);

		QJsonArray jarr;
		for (auto& v : motorParams)
		{
			QJsonObject j;
			j.insert("eAxisType", v.eAxisType);
			j.insert("nAcc", v.nAcc);
			j.insert("nLimitOffset", v.nLimitOffset);
			j.insert("nReverseOffset", v.nReverseOffset);
			j.insert("nSpeed", v.nSpeed);
			j.insert("dbGear", v.dbGear);

			jarr.append(j);
		}
		jo.insert("motorInitParams", jarr);

		return jo;
	}

	MotorInitParamInfo& fromJsonObject(QJsonObject jo)
	{
		bSupportMotorY = jo["bSupportMotorY"].toBool();
		bSupportMotorZ = jo["bSupportMotorZ"].toBool();
		bSupportMotorScrap = jo["bSupportMotorScrap"].toBool();

		QJsonArray jarr = jo["motorInitParams"].toArray();
		for (auto& v : jarr)
		{
			MotorInitParam param;
			param.eAxisType = v.toObject()["eAxisType"].toInt();

			param.nAcc = v.toObject()["nAcc"].toInt();
			param.nLimitOffset = v.toObject()["nLimitOffset"].toInt();
			param.nReverseOffset = v.toObject()["nReverseOffset"].toInt();
			param.nSpeed = v.toObject()["nSpeed"].toInt();
			param.dbGear = v.toObject()["dbGear"].toDouble();

			motorParams.push_back(param);
		}

		return *this;
	}
};


struct MotorXParamInfo
{
	double	dbMotorParamAcc;//加减速距离
	double	dbSpeedMove;//常规速度
	double	dbSpeedReposition;//回零速度
	double	dbSpeedPrintEmpty;//空跑速度
	double	dbGear;//齿轮比
	double	dbXPrintLeftEdgeOffset;//左侧停止位补偿距离
	double	dbXPrintRightEdgeOffset;//右侧停止位补偿距离
	bool	bCheckRasterShake;//是否检测光栅抖动
	bool	bUsePrintSpeedAsBackCarSpeed; //复位速度是否使用打印速度
	double	uSpeedPrintSlow;//打印速度 慢
	double	uSpeedPrintMiddle;//打印速度 中
	double	uSpeedPrintFast;//打印速度 快
	double	dbLimitLenP; //正向运动限制最大距离
	double	dbLimitLenN;//负向运动限制最大距离
	int		nAxisPositionMode; //马达定位方式
	int		nMotoRasterPort; //接口绑定方式

	int		nRasterOfffset;
	double	dbGearRunLength;


	QJsonObject toJsonObject()
	{
		QJsonObject jo;
		jo.insert("dbMotorParamAcc", dbMotorParamAcc);
		jo.insert("dbSpeedMove", dbSpeedMove);
		jo.insert("dbSpeedReposition", dbSpeedReposition);

		jo.insert("dbSpeedPrintEmpty", dbSpeedPrintEmpty);
		jo.insert("dbGear", dbGear);
		jo.insert("dbXPrintLeftEdgeOffset", dbXPrintLeftEdgeOffset);
		jo.insert("dbXPrintRightEdgeOffset", dbXPrintRightEdgeOffset);

		jo.insert("bCheckRasterShake", bCheckRasterShake);
		jo.insert("bUsePrintSpeedAsBackCarSpeed", bUsePrintSpeedAsBackCarSpeed);

		jo.insert("uSpeedPrintSlow", uSpeedPrintSlow);
		jo.insert("uSpeedPrintMiddle", uSpeedPrintMiddle);
		jo.insert("uSpeedPrintFast", uSpeedPrintFast);

		jo.insert("dbLimitLenP", dbLimitLenP);
		jo.insert("dbLimitLenN", dbLimitLenN);

		jo.insert("nAxisPositionMode", nAxisPositionMode);
		jo.insert("nMotoRasterPort", nMotoRasterPort);

		return jo;
	}

	void fromJsonObject(QJsonObject jo)
	{
		dbMotorParamAcc = jo["dbMotorParamAcc"].toDouble();
		dbSpeedMove = jo["dbSpeedMove"].toDouble();
		dbSpeedReposition = jo["dbSpeedReposition"].toDouble();

		dbSpeedPrintEmpty = jo["dbSpeedPrintEmpty"].toDouble();
		dbGear = jo["dbGear"].toDouble();
		dbXPrintLeftEdgeOffset = jo["dbXPrintLeftEdgeOffset"].toDouble();
		dbXPrintRightEdgeOffset = jo["dbXPrintRightEdgeOffset"].toDouble();

		bCheckRasterShake = jo["bCheckRasterShake"].toBool();
		bUsePrintSpeedAsBackCarSpeed = jo["bUsePrintSpeedAsBackCarSpeed"].toBool();

		uSpeedPrintSlow = jo["uSpeedPrintSlow"].toDouble();
		uSpeedPrintMiddle = jo["uSpeedPrintMiddle"].toDouble();
		uSpeedPrintFast = jo["uSpeedPrintFast"].toDouble();

		dbLimitLenP = jo["dbLimitLenP"].toDouble();
		dbLimitLenN = jo["dbLimitLenN"].toDouble();

		nAxisPositionMode = jo["nAxisPositionMode"].toInt();
		nMotoRasterPort = jo["nMotoRasterPort"].toInt();
	}
};

struct MotoYParamInfo
{
	double	dbMotorParamAcc;//加减速距离
	double	dbSpeedMove;//常规速度
	double	dbSpeedReposition;//回零速度
	double	dbGear;//齿轮比
	double	dbSpeedPrintSlow;//打印速度 慢
	double	dbSpeedPrintMiddle;//打印速度 中
	double	dbSpeedPrintFast; //打印速度 快
	bool	bSupportPrintReverseCompensation;//是否支持反向补偿距离
	double	dbPrintReverseComp;//反向打印补偿
	double	dbLimitLenP; //正向运动限制最大距离
	double	dbLimitLenN;//负向运动限制最大距离

	int		nYPositionMode;//Y定位方式 
	bool	bEnableDoubleY;//是都支持双Y最大误差
	double	dbDoubleYError;//双Y最大误差

	int		nGearOffset;

	QJsonObject toJsonObject()
	{
		QJsonObject jo;
		jo.insert("dbMotorParamAcc", dbMotorParamAcc);
		jo.insert("dbSpeedMove", dbSpeedMove);
		jo.insert("dbSpeedReposition", dbSpeedReposition);

		jo.insert("dbGear", dbGear);
		jo.insert("dbSpeedPrintSlow", dbSpeedPrintSlow);
		jo.insert("dbSpeedPrintMiddle", dbSpeedPrintMiddle);
		jo.insert("dbSpeedPrintFast", dbSpeedPrintFast);

		jo.insert("bSupportPrintReverseCompensation", bSupportPrintReverseCompensation);
		jo.insert("dbPrintReverseComp", dbPrintReverseComp);

		jo.insert("dbLimitLenP", dbLimitLenP);
		jo.insert("dbLimitLenN", dbLimitLenN);
		jo.insert("nYPositionMode", nYPositionMode);

		jo.insert("bEnableDoubleY", bEnableDoubleY);
		jo.insert("dbDoubleYError", dbDoubleYError);

		jo.insert("nGearOffset", nGearOffset);

		return jo;
	}

	void fromJsonObject(QJsonObject jo)
	{
		dbMotorParamAcc = jo["dbMotorParamAcc"].toDouble();
		dbSpeedMove = jo["dbSpeedMove"].toDouble();
		dbSpeedReposition = jo["dbSpeedReposition"].toDouble();

		dbGear = jo["dbGear"].toDouble();
		dbSpeedPrintSlow = jo["dbSpeedPrintSlow"].toDouble();
		dbSpeedPrintMiddle = jo["dbSpeedPrintMiddle"].toDouble();
		dbSpeedPrintFast = jo["dbSpeedPrintFast"].toDouble();

		bSupportPrintReverseCompensation = jo["bSupportPrintReverseCompensation"].toBool();
		dbPrintReverseComp = jo["dbPrintReverseComp"].toBool();

		dbLimitLenP = jo["dbLimitLenP"].toDouble();
		dbLimitLenN = jo["dbLimitLenN"].toDouble();
		nYPositionMode = jo["nYPositionMode"].toDouble();

		bEnableDoubleY = jo["bEnableDoubleY"].toDouble();
		dbDoubleYError = jo["dbDoubleYError"].toDouble();

		nGearOffset = jo["nGearOffset"].toInt();
	}
};

struct MotoZParamInfo
{
	// 运动参数
	// 加速度巨鹿
	double	dbMotorParamAcc;
	// 速度
	double	dbSpeedMove;
	// 复位速度
	double	dbSpeedReposition;
	// 齿轮比
	double	dbGear;
	// 零点位置是否在上方
	bool	bZZeroOnTop;

	// 测高器
	// 测高补偿
	double	dAltimeterCompensate;
	// 运动参数 开始初始化高度
	double	dbInitHeight;
	// X偏移
	double	dAltimeterXOffset;
	// Y偏移
	double	dAltimeterYOffset;
	// 感应高度
	double	dAltimeterResponseHeight;
	// 弹出延时
	int		nAltimeterOutDelayTime;
	// 下限位与平台距离
	double	dbLowerLimitHeight;
	// 齿轮偏移
	int		nGearOffset;

	double	dbLimitLenP;
	double	dbLimitLenN;
	// 精度  分辨率
	double	dbMotoDpi;

	// 移动偏移量
	int		nPluseOffset;

	QJsonObject toJsonObject()
	{
		QJsonObject jo;
		jo.insert("dbMotorParamAcc", dbMotorParamAcc);
		jo.insert("dbSpeedMove", dbSpeedMove);
		jo.insert("dbSpeedReposition", dbSpeedReposition);

		jo.insert("dbGear", dbGear);
		jo.insert("bZZeroOnTop", bZZeroOnTop);
		jo.insert("dAltimeterCompensate", dAltimeterCompensate);
		jo.insert("dbInitHeight", dbInitHeight);

		jo.insert("dAltimeterXOffset", dAltimeterXOffset);
		jo.insert("dAltimeterYOffset", dAltimeterYOffset);

		jo.insert("dAltimeterResponseHeight", dAltimeterResponseHeight);
		jo.insert("nAltimeterOutDelayTime", nAltimeterOutDelayTime);
		jo.insert("dbLowerLimitHeight", dbLowerLimitHeight);

		jo.insert("nGearOffset", nGearOffset);
		jo.insert("dbLimitLenP", dbLimitLenP);
		jo.insert("dbLimitLenN", dbLimitLenN);
		jo.insert("dbMotoDpi", dbMotoDpi);
		jo.insert("nPluseOffset", nPluseOffset);

		return jo;
	}

	void fromJsonObject(QJsonObject jo)
	{
		dbMotorParamAcc = jo["dbMotorParamAcc"].toDouble();
		dbSpeedMove = jo["dbSpeedMove"].toDouble();
		dbSpeedReposition = jo["dbSpeedReposition"].toDouble();

		dbGear = jo["dbGear"].toDouble();
		bZZeroOnTop = jo["bZZeroOnTop"].toBool();
		dAltimeterCompensate = jo["dAltimeterCompensate"].toDouble();
		dbInitHeight = jo["dbInitHeight"].toDouble();

		dAltimeterXOffset = jo["dAltimeterXOffset"].toBool();
		dAltimeterYOffset = jo["dAltimeterYOffset"].toBool();

		dAltimeterResponseHeight = jo["dAltimeterResponseHeight"].toDouble();
		nAltimeterOutDelayTime = jo["nAltimeterOutDelayTime"].toInt();
		dbLowerLimitHeight = jo["dbLowerLimitHeight"].toDouble();

		nGearOffset = jo["nGearOffset"].toDouble();
		dbLimitLenP = jo["dbLimitLenP"].toDouble();

		dbLimitLenN = jo["dbLimitLenN"].toDouble();

		dbMotoDpi = jo["dbMotoDpi"].toDouble();
		nPluseOffset = jo["nPluseOffset"].toInt();
	}
};


struct Moto0ParamInfo
{
	int		nInkStackHeightFlash;
	int		nInkStackHeightKeepWet;
	int		nInkStackHeightScraping;
	int		nInkStackSpeed;
	int		nMotorParamAcc;
	int		nInkStackHeightPump;

	double	dbLimitLenP;
	double	dbLimitLenN;

	int		nInkstackType;

	QJsonObject toJsonObject()
	{
		QJsonObject jo;
		jo.insert("nInkStackHeightFlash", nInkStackHeightFlash);
		jo.insert("nInkStackHeightKeepWet", nInkStackHeightKeepWet);
		jo.insert("nInkStackHeightScraping", nInkStackHeightScraping);

		jo.insert("nInkStackSpeed", nInkStackSpeed);
		jo.insert("nMotorParamAcc", nMotorParamAcc);
		jo.insert("nInkStackHeightPump", nInkStackHeightPump);
		jo.insert("dbLimitLenP", dbLimitLenP);
		jo.insert("dbLimitLenN", dbLimitLenN);
		jo.insert("nInkstackType", nInkstackType);
		return jo;
	}

	void fromJsonObject(QJsonObject jo)
	{
		nInkStackHeightFlash = jo["nInkStackHeightFlash"].toDouble();
		nInkStackHeightKeepWet = jo["nInkStackHeightKeepWet"].toDouble();
		nInkStackHeightScraping = jo["nInkStackHeightScraping"].toDouble();

		nInkStackSpeed = jo["nInkStackSpeed"].toDouble();
		nMotorParamAcc = jo["nMotorParamAcc"].toBool();
		nInkStackHeightPump = jo["nInkStackHeightPump"].toDouble();

		dbLimitLenP = jo["dbLimitLenP"].toDouble();
		dbLimitLenN = jo["dbLimitLenN"].toBool();
		nInkstackType = jo["nInkstackType"].toBool();
	}
};

struct Moto1ParamInfo
{
	int         nScrapMotoSpeed;
	int         nScprapingPos;
	double      dbScprapingPosEnd;
	double      dbScprapingPosStart;
	int         nCarMoveSpeedToScraping;
	int		    nMotorParamAcc;

	double		dbLimitLenP;
	double		dbLimitLenN;

	int			nScraperType;
	int			nScraperIndex;

	QJsonObject toJsonObject()
	{
		QJsonObject jo;
		jo.insert("nScrapMotoSpeed", nScrapMotoSpeed);
		jo.insert("nScprapingPos", nScprapingPos);
		jo.insert("dbScprapingPosEnd", dbScprapingPosEnd);
		jo.insert("dbScprapingPosStart", dbScprapingPosStart);
		jo.insert("nCarMoveSpeedToScraping", nCarMoveSpeedToScraping);
		jo.insert("nMotorParamAcc", nMotorParamAcc);
		jo.insert("dbLimitLenP", dbLimitLenP);
		jo.insert("dbLimitLenN", dbLimitLenN);
		jo.insert("nScraperType", nScraperType);
		jo.insert("nScraperIndex", nScraperIndex);
		return jo;
	}
	void fromJsonObject(QJsonObject jo)
	{
		nScrapMotoSpeed = jo["nScrapMotoSpeed"].toDouble();
		nScprapingPos = jo["nScprapingPos"].toDouble();
		dbScprapingPosEnd = jo["dbScprapingPosEnd"].toDouble();
		dbScprapingPosStart = jo["dbScprapingPosStart"].toDouble();
		nCarMoveSpeedToScraping = jo["nCarMoveSpeedToScraping"].toBool();
		nMotorParamAcc = jo["nMotorParamAcc"].toDouble();

		dbLimitLenP = jo["dbLimitLenP"].toDouble();
		dbLimitLenN = jo["dbLimitLenN"].toBool();
		nScraperType = jo["nScraperType"].toBool();
		nScraperIndex = jo["nScraperIndex"].toBool();
	}
};



struct shskItemNode
{
	int nID;
	double dbValue;
	string szName;
public:
	friend inline void to_json(json &j, const shskItemNode &p)
	{
		j = json{
		{"nID",p.nID},
		{"dbValue",p.dbValue},
		{"szName",p.szName},

		};
	}
	friend inline void from_json(const json &j, shskItemNode &p)
	{
		j.at("nID").get_to(p.nID);
		j.at("dbValue").get_to(p.dbValue);
		j.at("szName").get_to(p.szName);

	}
};

//喷头组合参数
struct HeadPhysicalLayoutParam
{
	bool	bEnableHeadDecrypt;
	vector<float> dbHeadBaseDistanceMMs;
	int		nHeadGroupCnt;
	int		nCurPhyLayoutID;
	int		nHeadItemCount;
	vector<shskItemNode> itemNodeList;
public:
	friend inline void to_json(json &j, const HeadPhysicalLayoutParam &p)
	{
		j = json{
		{"bEnableHeadDecrypt",p.bEnableHeadDecrypt},
		{"dbHeadBaseDistanceMMs",p.dbHeadBaseDistanceMMs},
		{"nHeadGroupCnt",p.nHeadGroupCnt},
		{"nCurPhyLayoutID",p.nCurPhyLayoutID},
		{"nHeadItemCount",p.nHeadItemCount},
		{"itemNodeList",p.itemNodeList},
		};
	}
	friend inline void from_json(const json &j, HeadPhysicalLayoutParam &p)
	{
		j.at("bEnableHeadDecrypt").get_to(p.bEnableHeadDecrypt);
		j.at("dbHeadBaseDistanceMMs").get_to(p.dbHeadBaseDistanceMMs);
		j.at("nHeadGroupCnt").get_to(p.nHeadGroupCnt);
		j.at("nCurPhyLayoutID").get_to(p.nCurPhyLayoutID);
		j.at("nHeadItemCount").get_to(p.nHeadItemCount);
		j.at("itemNodeList").get_to(p.itemNodeList);
	}
};


//打印模式参数
struct prinModeParamItem
{
	int		uiID;							//打印模式ID
	string	szName;							//打印模式名字
	int		nPassMode;						//Pass模式
	int		nXDpiValue;						//横向DPI值

	int		nEclosionDotMode;				//羽化点模式
	int		nEclosionRangSuperStrong;		//羽化幅度-超强
	int		nEclosionRangStrong;			//羽化幅度-强
	int		nEclosionRangNormal;			//羽化幅度-正常
	int		nEclosionRangWeak;				//羽化幅度-弱
	int		bWaitForStepStop;				//是否等待步进停止
public:
	friend inline void to_json(json &j, const prinModeParamItem &p)
	{
		j = json{
		{"uiID",p.uiID},
		{"szName",p.szName},
		{"nPassMode",p.nPassMode},
		{"nXDpiValue",p.nXDpiValue},
		{"nEclosionDotMode",p.nEclosionDotMode},
		{"nEclosionRangSuperStrong",p.nEclosionRangSuperStrong},
		{"nEclosionRangStrong",p.nEclosionRangStrong},
		{"nEclosionRangNormal",p.nEclosionRangNormal},
		{"nEclosionRangWeak",p.nEclosionRangWeak},
		{"bWaitForStepStop",p.bWaitForStepStop},
		};
	}
	friend inline void from_json(const json &j, prinModeParamItem &p)
	{
		j.at("uiID").get_to(p.uiID);
		j.at("szName").get_to(p.szName);
		j.at("nPassMode").get_to(p.nPassMode);
		j.at("nXDpiValue").get_to(p.nXDpiValue);
		j.at("nEclosionDotMode").get_to(p.nEclosionDotMode);
		j.at("nEclosionRangSuperStrong").get_to(p.nEclosionRangSuperStrong);
		j.at("nEclosionRangStrong").get_to(p.nEclosionRangStrong);
		j.at("nEclosionRangNormal").get_to(p.nEclosionRangNormal);
		j.at("nEclosionRangWeak").get_to(p.nEclosionRangWeak);
		j.at("bWaitForStepStop").get_to(p.bWaitForStepStop);
	}
};

//打印模式配置参数项
struct printConfigParamItem
{
	int		nID;
	double	dbValue;
	string	szName;

	vector<prinModeParamItem> printModeParamItems;
public:
	friend inline void to_json(json &j, const printConfigParamItem &p)
	{
		j = json{
		{"nID",p.nID},
		{"dbValue",p.dbValue},
		{"szName",p.szName},
		{"printModeParamItems",p.printModeParamItems},
		};
	}
	friend inline void from_json(const json &j, printConfigParamItem &p)
	{
		j.at("nID").get_to(p.nID);
		j.at("dbValue").get_to(p.dbValue);
		j.at("szName").get_to(p.szName);
		j.at("printModeParamItems").get_to(p.printModeParamItems);
	}
};

//打印模式配置全部参数
struct printConfigParam
{
	vector<printConfigParamItem> printCfgParamItems;
public:
	friend inline void to_json(json &j, const printConfigParam &p)
	{
		j = json{
		{"printCfgParamItems",p.printCfgParamItems},

		};
	}
	friend inline void from_json(const json &j, printConfigParam &p)
	{
		j.at("printCfgParamItems").get_to(p.printCfgParamItems);
	}
};

//UV灯参数
struct UvLightParam
{
	bool		bUVEnable;			//uv灯使能
	bool		bLPrintLLight;		//向左运动-左灯
	bool		bLPrintRLight;		//向左运动-右灯
	bool		bRPrintLLight;		//向右运动-左灯
	bool		bRPrintRLight;		//向右运动-右灯

	int			nDelayTime;			//等待时间（毫秒）
	bool		bWaitUVToImageEdge;	//等待UV灯照射结束

	bool		bUVAfterPrint;		//UV灯打印完照射模式使能
	int			nUVAfterPrintModeAuto;//UV灯打印完照射模式自动匹配使能
	int			nUVCountCustom;		//自定义照射次数
	double		dUVHeightCustom;		//自定义照射距离(毫秒)

	bool		bCheckSecondUVLight;		//启动第二组UV灯
	double		dbFrontLightOffLengthMM;	//前灯关闭距离
	double		dbBackLightOnLengthMM;		//后灯关闭距离
	bool		bSecondLSwithEnable;		//第二组亮灭参数使能
	bool		bSecondLPositionEnable;		//第二组偏移参数使能
	bool		bSecondLPrintLLight;		//第二组向左运动-左灯
	bool		bSecondLPrintRLight;		//第二组向左运动-右灯
	bool		bSecondRPrintLLight;		//第二组向右运动-左灯
	bool		bSecondRPrintRLight;		//第二组向右运动-右灯

public:
	friend inline void to_json(json &j, const UvLightParam &p)
	{
		j = json{
		{"bUVEnable",p.bUVEnable},
		{"bLPrintLLight",p.bLPrintLLight},
		{"bLPrintRLight",p.bLPrintRLight},
		{"bRPrintLLight",p.bRPrintLLight},
		{"bRPrintRLight",p.bRPrintRLight},
		{"nDelayTime",p.nDelayTime},
		{"bWaitUVToImageEdge",p.bWaitUVToImageEdge},
		{"bUVAfterPrint",p.bUVAfterPrint},
		{"nUVAfterPrintModeAuto",p.nUVAfterPrintModeAuto},
		{"nUVCountCustom",p.nUVCountCustom},
		{"dUVHeightCustom",p.dUVHeightCustom},
		{"bCheckSecondUVLight",p.bCheckSecondUVLight},
		{"dbFrontLightOffLengthMM",p.dbFrontLightOffLengthMM},
		{"dbBackLightOnLengthMM",p.dbBackLightOnLengthMM},
		{"bSecondLSwithEnable",p.bSecondLSwithEnable},
		{"bSecondLPositionEnable",p.bSecondLPositionEnable},
		{"bSecondLPrintLLight",p.bSecondLPrintLLight},
		{"bSecondLPrintRLight",p.bSecondLPrintRLight},
		{"bSecondRPrintLLight",p.bSecondRPrintLLight},
		{"bSecondRPrintRLight",p.bSecondRPrintRLight},
		};
	}
	friend inline void from_json(const json &j, UvLightParam &p)
	{
		j.at("bUVEnable").get_to(p.bUVEnable);
		j.at("bLPrintLLight").get_to(p.bLPrintLLight);
		j.at("bLPrintRLight").get_to(p.bLPrintRLight);
		j.at("bRPrintLLight").get_to(p.bRPrintLLight);
		j.at("bRPrintRLight").get_to(p.bRPrintRLight);
		j.at("nDelayTime").get_to(p.nDelayTime);
		j.at("bWaitUVToImageEdge").get_to(p.bWaitUVToImageEdge);
		j.at("bUVAfterPrint").get_to(p.bUVAfterPrint);
		j.at("nUVAfterPrintModeAuto").get_to(p.nUVAfterPrintModeAuto);
		j.at("nUVCountCustom").get_to(p.nUVCountCustom);
		j.at("dUVHeightCustom").get_to(p.dUVHeightCustom);
		j.at("bCheckSecondUVLight").get_to(p.bCheckSecondUVLight);
		j.at("dbFrontLightOffLengthMM").get_to(p.dbFrontLightOffLengthMM);
		j.at("dbBackLightOnLengthMM").get_to(p.dbBackLightOnLengthMM);
		j.at("bSecondLSwithEnable").get_to(p.bSecondLSwithEnable);
		j.at("bSecondLPositionEnable").get_to(p.bSecondLPositionEnable);
		j.at("bSecondLPrintLLight").get_to(p.bSecondLPrintLLight);
		j.at("bSecondLPrintRLight").get_to(p.bSecondLPrintRLight);
		j.at("bSecondRPrintLLight").get_to(p.bSecondRPrintLLight);
		j.at("bSecondRPrintRLight").get_to(p.bSecondRPrintRLight);
	}
};


//自动清洗
struct HsAutoCleanParam
{
	int				nAutoCleanPass;				//清洗间隔

	int				nInkFlashCount_PrePrint;		//打印前闪喷次数
	int				nInkFlashOnceTime_PrePrint;	//打印前单次闪喷时间
	int				nInkFlashWaitTime_PrePrint;	//打印前闪喷间隔时间

	int				nInkFlashCount_Printing;		//打印中闪喷次数
	int				nInkFlashOnceTime_Printing;	//打印中单次闪喷时间
	int				nInkFlashWaitTime_Printing;	//打印中闪喷间隔时间

	int				nXMoveOffset;					//小车移动距离
	int				nXMoveSpeed;					//小车移动速度

	int             nKeepWetXPos;					//保湿X目标位
	int             nKeepWetZPos;					//保湿Z目标位

	int				nAutoCleantMode;				//自动清洗模式
	int				nAutoCleanHeadGruopId;		//自动清洗喷头组合Id
	int				nAutoCleanSpaceTypeIndex;		//自动清洗间隔类型

	int				nFlashFreq_Printing;			//打印中闪喷频率
	int				nFlashFreq_PrePrint;			//打印前闪喷频率


	vector<int>		vAutoCleanHeadGruopIds;		//自动清洗喷头组合Id列表
	vector<string>	vAutoCleanHeadGruopNames;		//自动清洗喷头组合名列表，id位置对应Name

	vector<string>	vAutoCleanModeNames;			//自动清洗模式名
	vector<string>	vAutoCleanSpaceTypeNames;		//自动清洗间隔类型名
public:
	friend inline void to_json(json &j, const HsAutoCleanParam &p)
	{
		j = json{
		{"nAutoCleanPass",p.nAutoCleanPass},
		{"nInkFlashCount_PrePrint",p.nInkFlashCount_PrePrint},
		{"nInkFlashOnceTime_PrePrint",p.nInkFlashOnceTime_PrePrint},
		{"nInkFlashWaitTime_PrePrint",p.nInkFlashWaitTime_PrePrint},
		{"nInkFlashCount_Printing",p.nInkFlashCount_Printing},
		{"nInkFlashOnceTime_Printing",p.nInkFlashOnceTime_Printing},
		{"nInkFlashWaitTime_Printing",p.nInkFlashWaitTime_Printing},
		{"nXMoveOffset",p.nXMoveOffset},
		{"nXMoveSpeed",p.nXMoveSpeed},
		{"nKeepWetXPos",p.nKeepWetXPos},
		{"nKeepWetZPos",p.nKeepWetZPos},
		{"nAutoCleantMode",p.nAutoCleantMode},
		{"nAutoCleanHeadGruopId",p.nAutoCleanHeadGruopId},
		{"nAutoCleanSpaceTypeIndex",p.nAutoCleanSpaceTypeIndex},
		{"nFlashFreq_Printing",p.nFlashFreq_Printing},
		{"nFlashFreq_PrePrint",p.nFlashFreq_PrePrint},
		{"vAutoCleanHeadGruopIds",p.vAutoCleanHeadGruopIds},
		{"vAutoCleanHeadGruopNames",p.vAutoCleanHeadGruopNames},
		{"vAutoCleanModeNames",p.vAutoCleanModeNames},
		{"vAutoCleanSpaceTypeNames",p.vAutoCleanSpaceTypeNames},
		};
	}
	friend inline void from_json(const json &j, HsAutoCleanParam &p)
	{
		j.at("nAutoCleanPass").get_to(p.nAutoCleanPass);
		j.at("nInkFlashCount_PrePrint").get_to(p.nInkFlashCount_PrePrint);
		j.at("nInkFlashOnceTime_PrePrint").get_to(p.nInkFlashOnceTime_PrePrint);
		j.at("nInkFlashWaitTime_PrePrint").get_to(p.nInkFlashWaitTime_PrePrint);
		j.at("nInkFlashCount_Printing").get_to(p.nInkFlashCount_Printing);
		j.at("nInkFlashOnceTime_Printing").get_to(p.nInkFlashOnceTime_Printing);
		j.at("nInkFlashWaitTime_Printing").get_to(p.nInkFlashWaitTime_Printing);
		j.at("nXMoveOffset").get_to(p.nXMoveOffset);
		j.at("nXMoveSpeed").get_to(p.nXMoveSpeed);
		j.at("nKeepWetXPos").get_to(p.nKeepWetXPos);
		j.at("nKeepWetZPos").get_to(p.nKeepWetZPos);
		j.at("nAutoCleantMode").get_to(p.nAutoCleantMode);
		j.at("nAutoCleanHeadGruopId").get_to(p.nAutoCleanHeadGruopId);
		j.at("nAutoCleanSpaceTypeIndex").get_to(p.nAutoCleanSpaceTypeIndex);
		j.at("nFlashFreq_Printing").get_to(p.nFlashFreq_Printing);
		j.at("nFlashFreq_PrePrint").get_to(p.nFlashFreq_PrePrint);
		j.at("vAutoCleanHeadGruopIds").get_to(p.vAutoCleanHeadGruopIds);
		j.at("vAutoCleanHeadGruopNames").get_to(p.vAutoCleanHeadGruopNames);
		j.at("vAutoCleanModeNames").get_to(p.vAutoCleanModeNames);
		j.at("vAutoCleanSpaceTypeNames").get_to(p.vAutoCleanSpaceTypeNames);
	}
};



//清洗模式对应抽墨参数
struct HsCleanScrapParam
{
	int		id;					//模式id
	string	cleanname;			//模式名
	int		nPumpInkCount;		//抽墨次数
	int		nPumpInkTime;		//抽墨时间
	int		nPumpInkWaitTime;	//抽墨间隔时间
	int		nPumpInkWasteTime;	//抽废墨时间
public:
	friend inline void to_json(json &j, const HsCleanScrapParam &p)
	{
		j = json{
		{"id",p.id},
		{"cleanname",p.cleanname},
		{"nPumpInkCount",p.nPumpInkCount},
		{"nPumpInkTime",p.nPumpInkTime},
		{"nPumpInkWaitTime",p.nPumpInkWaitTime},
		{"nPumpInkWasteTime",p.nPumpInkWasteTime},
		};
	}
	friend inline void from_json(const json &j, HsCleanScrapParam &p)
	{
		j.at("id").get_to(p.id);
		j.at("cleanname").get_to(p.cleanname);
		j.at("nPumpInkCount").get_to(p.nPumpInkCount);
		j.at("nPumpInkTime").get_to(p.nPumpInkTime);
		j.at("nPumpInkWaitTime").get_to(p.nPumpInkWaitTime);
		j.at("nPumpInkWasteTime").get_to(p.nPumpInkWasteTime);
	}
};


//手动清洗参数
struct HsCleanManualParam
{

	int				nCleanFlashCount;		//闪喷次数
	int				nFlashWaitTime;		//闪喷等待时间
	int				nOnceFlashTime;		//单次闪喷时间

	double			dbMotorMoveOutLen;	//小车移除距离
	double			dbMotorMoveOutSpeed;	//小车移除速度

	int				nFlashFreq;			//闪喷频率
	//int				nScrapIndex;			//抽墨参数索引

	vector<HsCleanScrapParam> scrapParams;	//抽墨参数
public:
	friend inline void to_json(json &j, const HsCleanManualParam &p)
	{
		j = json{
		{"nCleanFlashCount",p.nCleanFlashCount},
		{"nFlashWaitTime",p.nFlashWaitTime},
		{"nOnceFlashTime",p.nOnceFlashTime},
		{"dbMotorMoveOutLen",p.dbMotorMoveOutLen},
		{"dbMotorMoveOutSpeed",p.dbMotorMoveOutSpeed},
		{"nFlashFreq",p.nFlashFreq},
		//{"nScrapIndex",p.nScrapIndex},
		{"scrapParams",p.scrapParams},
		};
	}
	friend inline void from_json(const json &j, HsCleanManualParam &p)
	{
		j.at("nCleanFlashCount").get_to(p.nCleanFlashCount);
		j.at("nFlashWaitTime").get_to(p.nFlashWaitTime);
		j.at("nOnceFlashTime").get_to(p.nOnceFlashTime);
		j.at("dbMotorMoveOutLen").get_to(p.dbMotorMoveOutLen);
		j.at("dbMotorMoveOutSpeed").get_to(p.dbMotorMoveOutSpeed);
		j.at("nFlashFreq").get_to(p.nFlashFreq);
		//j.at("nScrapIndex").get_to(p.nScrapIndex);
		j.at("scrapParams").get_to(p.scrapParams);
	}
};

//清洗模式对应抽墨参数 扩展
struct HsCleanScrapExParam
{
	int		id;					//模式id
	string	cleanname;			//模式名
	int		nPumpInkCount;		//抽墨次数
	int		nPumpInkWasteTime;		//抽墨时间

	vector<int>		nOutputPowerPrecents;	//输出功能百分比
	vector<int>		nPumpInkWaitTimes;		//抽墨间隔时间
	vector<int>		nPumpInkTimes;		//抽废墨时间
public:
	friend inline void to_json(json &j, const HsCleanScrapExParam &p)
	{
		j = json{
		{"id",p.id},
		{"cleanname",p.cleanname},
		{"nPumpInkCount",p.nPumpInkCount},
		{"nPumpInkWasteTime",p.nPumpInkWasteTime},
		{"nOutputPowerPrecents",p.nOutputPowerPrecents},
		{"nPumpInkWaitTimes",p.nPumpInkWaitTimes},
		{"nPumpInkTimes",p.nPumpInkTimes},
		};
	}
	friend inline void from_json(const json &j, HsCleanScrapExParam &p)
	{
		j.at("id").get_to(p.id);
		j.at("cleanname").get_to(p.cleanname);
		j.at("nPumpInkCount").get_to(p.nPumpInkCount);
		j.at("nPumpInkWasteTime").get_to(p.nPumpInkWasteTime);
		j.at("nOutputPowerPrecents").get_to(p.nOutputPowerPrecents);
		j.at("nPumpInkWaitTimes").get_to(p.nPumpInkWaitTimes);
		j.at("nPumpInkTimes").get_to(p.nPumpInkTimes);
	}
};


//手动清洗参数 扩展
struct HsCleanManualExParam
{

	int				nCleanFlashCount;		//闪喷次数
	int				nFlashWaitTime;		//闪喷等待时间
	int				nOnceFlashTime;		//单次闪喷时间

	double			dbMotorMoveOutLen;	//小车移除距离
	double			dbMotorMoveOutSpeed;	//小车移除速度

	int				nFlashFreq;			//闪喷频率
	//int				nScrapIndex;			//抽墨参数索引

	vector<HsCleanScrapExParam> scrapParams;	//抽墨参数
public:
	friend inline void to_json(json &j, const HsCleanManualExParam &p)
	{
		j = json{
		{"nCleanFlashCount",p.nCleanFlashCount},
		{"nFlashWaitTime",p.nFlashWaitTime},
		{"nOnceFlashTime",p.nOnceFlashTime},
		{"dbMotorMoveOutLen",p.dbMotorMoveOutLen},
		{"dbMotorMoveOutSpeed",p.dbMotorMoveOutSpeed},
		{"nFlashFreq",p.nFlashFreq},
		//{"nScrapIndex",p.nScrapIndex},
		{"scrapParams",p.scrapParams},
		};
	}
	friend inline void from_json(const json &j, HsCleanManualExParam &p)
	{
		j.at("nCleanFlashCount").get_to(p.nCleanFlashCount);
		j.at("nFlashWaitTime").get_to(p.nFlashWaitTime);
		j.at("nOnceFlashTime").get_to(p.nOnceFlashTime);
		j.at("dbMotorMoveOutLen").get_to(p.dbMotorMoveOutLen);
		j.at("dbMotorMoveOutSpeed").get_to(p.dbMotorMoveOutSpeed);
		j.at("nFlashFreq").get_to(p.nFlashFreq);
		//j.at("nScrapIndex").get_to(p.nScrapIndex);
		j.at("scrapParams").get_to(p.scrapParams);
	}
};

//跑机参数
struct HsRunningMachineParam
{
	double	dbXBeginPos;		//小车开始位置
	double	dbXEndPos;		//小车结束位置
	double	dbXSpeed;			//小车速度
	double	dbZBeginPos;		//z轴开始位置
	double	dbZEndPos;		//z轴最大位置
	double	dbYOffsetOnce;	//单向走纸距离
	double	dbYOffsetAll;		//最大走纸距离
	int		nSPos;			//刮片移出位置
public:
	friend inline void to_json(json &j, const HsRunningMachineParam &p)
	{
		j = json{
		{"dbXBeginPos",p.dbXBeginPos},
		{"dbXEndPos",p.dbXEndPos},
		{"dbXSpeed",p.dbXSpeed},
		{"dbZBeginPos",p.dbZBeginPos},
		{"dbZEndPos",p.dbZEndPos},
		{"dbYOffsetAll",p.dbYOffsetAll},
		{"dbYOffsetOnce",p.dbYOffsetOnce},
		{"nSPos",p.nSPos},
		};
	}
	friend inline void from_json(const json &j, HsRunningMachineParam &p)
	{
		j.at("dbXBeginPos").get_to(p.dbXBeginPos);
		j.at("dbXEndPos").get_to(p.dbXEndPos);
		j.at("dbXSpeed").get_to(p.dbXSpeed);
		j.at("dbZBeginPos").get_to(p.dbZBeginPos);
		j.at("dbZEndPos").get_to(p.dbZEndPos);
		j.at("dbYOffsetAll").get_to(p.dbYOffsetAll);
		j.at("dbYOffsetOnce").get_to(p.dbYOffsetOnce);
		j.at("nSPos").get_to(p.nSPos);
	}
};


//闪喷参数
struct HsMicroFlashParam
{
	bool			bSupportAutoFlash;			//是否支持自动闪喷
	bool			bAutoFlash;					//开启自动闪喷
	int				nAutoFlashFreq;				//自动闪喷频率
	int				nAutoMicrowaveOnceTime;		//自动单次闪喷时间
	int				nAutoMicrowaveWatiTime;		//自动间隔时间
	bool			bAutoFlashPumpInkEnable;	//自动抽废墨使能
	int				nAutoCleanInkStackTimeInterval;//自动抽废墨周期
	int				nAutoCleanInkStackTime;		//自动抽废墨时间
	int				nManualFlashFreq;			//手动闪喷频率
	int				nCleanFlashCount;			//手动闪喷次数
	int				nOnceFlashTime;				//手动单次闪喷时间
	int				nFlashWaitTime;				//手动闪喷间隔时间
	bool			bFlashSlot;					//启用闪喷槽
	int             nFlashSlotLeftPos;			//闪喷槽左侧位置
	int				nFlashSlotRightPos;			//闪喷槽右侧位置
	int             nFlashSlotWidth;			//闪喷槽宽度
public:
	friend inline void to_json(json &j, const HsMicroFlashParam &p)
	{
		j = json{
		{"bSupportAutoFlash",p.bSupportAutoFlash},
		{"bAutoFlash",p.bAutoFlash},
		{"nAutoFlashFreq",p.nAutoFlashFreq},
		{"nAutoMicrowaveOnceTime",p.nAutoMicrowaveOnceTime},
		{"nAutoMicrowaveWatiTime",p.nAutoMicrowaveWatiTime},
		{"bAutoFlashPumpInkEnable",p.bAutoFlashPumpInkEnable},
		{"nAutoCleanInkStackTimeInterval",p.nAutoCleanInkStackTimeInterval},
		{"nAutoCleanInkStackTime",p.nAutoCleanInkStackTime},
		{"nManualFlashFreq",p.nManualFlashFreq},
		{"nCleanFlashCount",p.nCleanFlashCount},
		{"nOnceFlashTime",p.nOnceFlashTime},
		{"nFlashWaitTime",p.nFlashWaitTime},
		{"bFlashSlot",p.bFlashSlot},
		{"nFlashSlotLeftPos",p.nFlashSlotLeftPos},
		{"nFlashSlotRightPos",p.nFlashSlotRightPos},
		{"nFlashSlotWidth",p.nFlashSlotWidth},

		};
	}
	friend inline void from_json(const json &j, HsMicroFlashParam &p)
	{
		j.at("bSupportAutoFlash").get_to(p.bSupportAutoFlash);
		j.at("bAutoFlash").get_to(p.bAutoFlash);
		j.at("nAutoFlashFreq").get_to(p.nAutoFlashFreq);
		j.at("nAutoMicrowaveOnceTime").get_to(p.nAutoMicrowaveOnceTime);
		j.at("nAutoMicrowaveWatiTime").get_to(p.nAutoMicrowaveWatiTime);
		j.at("bAutoFlashPumpInkEnable").get_to(p.bAutoFlashPumpInkEnable);
		j.at("nAutoCleanInkStackTimeInterval").get_to(p.nAutoCleanInkStackTimeInterval);
		j.at("nAutoCleanInkStackTime").get_to(p.nAutoCleanInkStackTime);
		j.at("nManualFlashFreq").get_to(p.nManualFlashFreq);
		j.at("nCleanFlashCount").get_to(p.nCleanFlashCount);
		j.at("nOnceFlashTime").get_to(p.nOnceFlashTime);
		j.at("nFlashWaitTime").get_to(p.nFlashWaitTime);
		j.at("bFlashSlot").get_to(p.bFlashSlot);
		j.at("nFlashSlotLeftPos").get_to(p.nFlashSlotLeftPos);
		j.at("nFlashSlotRightPos").get_to(p.nFlashSlotRightPos);
		j.at("nFlashSlotWidth").get_to(p.nFlashSlotWidth);

	}
};

//走纸参数
struct HsRunPaperParam
{
	int					nYRunModeBeforePrint;		//打印前走纸模式
	int					nYRunModeAfterPrint;		//打印后走纸模式
	double				dbYRunAheadBeforePrint;//打印前进纸距离
	double				dbYRunBackBeforePrint;//打印前退纸距离
	double				dbYRunAheadAfterPrint;//打印后进纸距离
	double				dbYRunBackAfterPrint;//打印前进纸距离
	int					nPassModeYRunMode;	//前后pass数走纸模式
	int					nOffsetNozzle;			//前后PASS不走纸偏移孔	
	bool				bEnableDryingAfterPrint;//打印完烘干开关
	double				dbYAllLenAfterPrint;//总走纸距离
	int					nEachPassWaitTime;//每PASS等待时间
	bool				bMovebackAfterDrying;//烘干后是否回退
	bool				bCheckOutOfPaper;//缺纸检测
	int					nCheckPaperValidPassCnt;//检测有效PASS数
	bool				bSupportTestPaper;//是否支持测纸
	bool				bCheckTestPaper;//测纸开关
	int					nPaperLeftOffset;//纸张左边距微调
	int					nPaperRightOffset;//纸张右边距微调
	double				dPaperTestSensorOffset;//测纸传感器偏移
	double				dPaperTestInvalidSPace;//测纸非检测区域
	int					nTestPaperWidth;//测纸宽度
	bool				bEmptyXMargin;//清空X白边
	bool				bEnableYPrintDirection;	//启用Y定位方向
	bool				bMultiPrintTip;			//多份打印提示
public:
	friend inline void to_json(json &j, const HsRunPaperParam &p)
	{
		j = json{
		{"nYRunModeBeforePrint",p.nYRunModeBeforePrint},
		{"nYRunModeAfterPrint",p.nYRunModeAfterPrint},
		{"dbYRunAheadBeforePrint",p.dbYRunAheadBeforePrint},
		{"dbYRunBackBeforePrint",p.dbYRunBackBeforePrint},
		{"dbYRunAheadAfterPrint",p.dbYRunAheadAfterPrint},
		{"dbYRunBackAfterPrint",p.dbYRunBackAfterPrint},
		{"nPassModeYRunMode",p.nPassModeYRunMode},
		{"nOffsetNozzle",p.nOffsetNozzle},
		{"bEnableDryingAfterPrint",p.bEnableDryingAfterPrint},
		{"dbYAllLenAfterPrint",p.dbYAllLenAfterPrint},
		{"nEachPassWaitTime",p.nEachPassWaitTime},
		{"bMovebackAfterDrying",p.bMovebackAfterDrying},
		{"bCheckOutOfPaper",p.bCheckOutOfPaper},
		{"nCheckPaperValidPassCnt",p.nCheckPaperValidPassCnt},
		{"bSupportTestPaper",p.bSupportTestPaper},
		{"bCheckTestPaper",p.bCheckTestPaper},
		{"nPaperLeftOffset",p.nPaperLeftOffset},
		{"nPaperRightOffset",p.nPaperRightOffset},
		{"dPaperTestSensorOffset",p.dPaperTestSensorOffset},
		{"dPaperTestInvalidSPace",p.dPaperTestInvalidSPace},
		{"nTestPaperWidth",p.nTestPaperWidth},
		{"bEmptyXMargin",p.bEmptyXMargin},
		{"bEnableYPrintDirection",p.bEnableYPrintDirection},
		{"bMultiPrintTip",p.bMultiPrintTip},

		};
	}
	friend inline void from_json(const json &j, HsRunPaperParam &p)
	{
		j.at("nYRunModeBeforePrint").get_to(p.nYRunModeBeforePrint);
		j.at("nYRunModeAfterPrint").get_to(p.nYRunModeAfterPrint);
		j.at("dbYRunAheadBeforePrint").get_to(p.dbYRunAheadBeforePrint);
		j.at("dbYRunBackBeforePrint").get_to(p.dbYRunBackBeforePrint);
		j.at("dbYRunAheadAfterPrint").get_to(p.dbYRunAheadAfterPrint);
		j.at("dbYRunBackAfterPrint").get_to(p.dbYRunBackAfterPrint);
		j.at("nPassModeYRunMode").get_to(p.nPassModeYRunMode);
		j.at("nOffsetNozzle").get_to(p.nOffsetNozzle);
		j.at("bEnableDryingAfterPrint").get_to(p.bEnableDryingAfterPrint);
		j.at("dbYAllLenAfterPrint").get_to(p.dbYAllLenAfterPrint);
		j.at("nEachPassWaitTime").get_to(p.nEachPassWaitTime);
		j.at("bMovebackAfterDrying").get_to(p.bMovebackAfterDrying);
		j.at("bCheckOutOfPaper").get_to(p.bCheckOutOfPaper);
		j.at("nCheckPaperValidPassCnt").get_to(p.nCheckPaperValidPassCnt);
		j.at("bSupportTestPaper").get_to(p.bSupportTestPaper);
		j.at("bCheckTestPaper").get_to(p.bCheckTestPaper);
		j.at("nPaperLeftOffset").get_to(p.nPaperLeftOffset);
		j.at("nPaperRightOffset").get_to(p.nPaperRightOffset);
		j.at("dPaperTestSensorOffset").get_to(p.dPaperTestSensorOffset);
		j.at("dPaperTestInvalidSPace").get_to(p.dPaperTestInvalidSPace);
		j.at("nTestPaperWidth").get_to(p.nTestPaperWidth);
		j.at("bEmptyXMargin").get_to(p.bEmptyXMargin);
		j.at("bEnableYPrintDirection").get_to(p.bEnableYPrintDirection);
		j.at("bMultiPrintTip").get_to(p.bMultiPrintTip);

	}
};



////波形设置参数
//struct HsWaveSetParam
//{
//	string		szWaveName;			//当前波形文件名
//	vector<string> szWaveFileList;	//波形文件列表
//
//	int			nPrintWaveIndex;		//默认模型索引
//	vector<string> printWaveNameList;//波形名列表
//
//	float		fTempMin;		//最小温度
//	float		fTempMax;		//最大温度
//public:
//	friend inline void to_json(json &j, const HsWaveSetParam &p)
//	{
//		j = json{
//		{"szWaveName",p.szWaveName},
//		{"szWaveFileList",p.szWaveFileList},
//		{"nPrintWaveIndex",p.nPrintWaveIndex},
//		{"printWaveNameList",p.printWaveNameList},
//		{"fTempMin",p.fTempMin},
//		{"fTempMax",p.fTempMax},
//		};
//	}
//	friend inline void from_json(const json &j, HsWaveSetParam &p)
//	{
//		j.at("szWaveName").get_to(p.szWaveName);
//		j.at("szWaveFileList").get_to(p.szWaveFileList);
//		j.at("nPrintWaveIndex").get_to(p.nPrintWaveIndex);
//		j.at("printWaveNameList").get_to(p.printWaveNameList);
//		j.at("fTempMin").get_to(p.fTempMin);
//		j.at("fTempMax").get_to(p.fTempMax);
//	}
//};

//波形设置参数
struct HsWaveSetParam
{
	string		szWaveName;			//当前波形文件名
	vector<string> szWaveFileList;	//波形文件列表

	int			nPrintWaveIndex;		//默认模型索引
	vector<string> printWaveNameList;	//波形名列表

	double		fTempMin;		//最小温度
	double		fTempMax;		//最大温度
public:
	/*friend inline void to_json(json &j, const HsWaveSetParam &p)
	{
		j = json{
		{"szWaveName",p.szWaveName},
		{"szWaveFileList",p.szWaveFileList},
		{"nPrintWaveIndex",p.nPrintWaveIndex},
		{"printWaveNameList",p.printWaveNameList},
		{"fTempMin",p.fTempMin},
		{"fTempMax",p.fTempMax},
		};
	}
	friend inline void from_json(const json &j, HsWaveSetParam &p)
	{
		j.at("szWaveName").get_to(p.szWaveName);
		j.at("szWaveFileList").get_to(p.szWaveFileList);
		j.at("nPrintWaveIndex").get_to(p.nPrintWaveIndex);
		j.at("printWaveNameList").get_to(p.printWaveNameList);
		j.at("fTempMin").get_to(p.fTempMin);
		j.at("fTempMax").get_to(p.fTempMax);
	}*/
	QJsonObject toJsonObject()
	{
		QJsonObject jo;
		jo.insert("szWaveName", szWaveName.c_str());

		QJsonArray jarrWaveFileList;
		for (auto& v : szWaveFileList)
		{
			jarrWaveFileList.append(v.c_str());
		}
		jo.insert("szWaveFileList", jarrWaveFileList);

		jo.insert("nPrintWaveIndex", nPrintWaveIndex);

		QJsonArray jarrWaveNameList;
		for (auto& v : printWaveNameList)
		{
			jarrWaveNameList.append(v.c_str());
		}
		jo.insert("printWaveNameList", jarrWaveNameList);

		jo.insert("fTempMin", fTempMin);
		jo.insert("fTempMax", fTempMax);

		return jo;
	}

	HsWaveSetParam& fromJsonObject(QJsonObject jo)
	{
		szWaveName = jo["szWaveName"].toString().toStdString();

		QJsonArray jarrWaveFileList = jo["szWaveFileList"].toArray();
		for (auto& v : jarrWaveFileList)
		{
			szWaveFileList.push_back(v.toString().toStdString());
		}

		nPrintWaveIndex = jo["nPrintWaveIndex"].toInt();

		QJsonArray jarrWaveNameList = jo["printWaveNameList"].toArray();
		for (auto& v : jarrWaveNameList)
		{
			printWaveNameList.push_back(v.toString().toStdString());
		}

		fTempMin = jo["fTempMin"].toDouble();
		fTempMax = jo["fTempMax"].toDouble();

		return *this;
	}
};


struct HsAlarmSet
{
	int		iTypeId;				//报警类型Id
	string	sTypeName;				//报警类型名
	bool	bEnable;				//报警使能

	int	nRingIntetvalTime;			//响铃间隔时间
	int nRingTimesPerSecond;		//每秒响铃次数
public:
	friend inline void to_json(json &j, const HsAlarmSet &p)
	{
		j = json{
		{"iTypeId",p.iTypeId},
		{"sTypeName",p.sTypeName},
		{"bEnable",p.bEnable},
		{"nRingIntetvalTime",p.nRingIntetvalTime},
		{"nRingTimesPerSecond",p.nRingTimesPerSecond},

		};
	}
	friend inline void from_json(const json &j, HsAlarmSet &p)
	{
		j.at("iTypeId").get_to(p.iTypeId);
		j.at("sTypeName").get_to(p.sTypeName);
		j.at("bEnable").get_to(p.bEnable);
		j.at("nRingIntetvalTime").get_to(p.nRingIntetvalTime);
		j.at("nRingTimesPerSecond").get_to(p.nRingTimesPerSecond);

	}
};


struct HsGeneralParam
{
	double				dbXOriginToZero;				//X白边起始偏移
	double				dbYOriginToZero;				//Y白边起始偏移
	double				dbMarginSetOffset;			//X白边定位偏移
	bool				bSupportYMargin;				//是否支持Y轴白边
	int					nXPhysicalRasterDpi;			//X物理光栅DPI
	int					nXLogicRasterDpi;				//X逻辑光栅DPI
	int					nYPhysicalRasterDpi;			//Y物理光栅DPI
	int					nYLogicRasterDpi;				//Y逻辑光栅DPI
	bool                bSupportAdaptiveLogicRaster;  //是否支持逻辑光栅
	bool				bSupportBEEclosion;			//是否支持首尾羽化
	bool				bBEEclosionEnable;			//是否启用首尾羽化
	float				nEclosionHeight;				//首尾羽化高度
	int					nColorBarDeepnessWeak;		//彩条浓度弱
	int					nColorBarDeepnessMid;			//彩条浓度中
	int					nColorBarDeepnessStrong;		//彩条浓度强
	int					nPrintToOriginTime;			//下位机未收到包打印暂停时间
	int					nResumeWaitTime;				//打印暂停时间自动恢复时间
	int					nMotoSpeedScale;				//马达加速曲线比例
	//位置单位 0-脉冲 1-毫米
	int					nPositonUnitType;
	//计量单位 0-毫米 1-英寸
	int					nMeasurementUnit;
	//0-模板模式 1-开关模式 2-抽点模式 3-混合模式 4-校正模式 5-适配模式
	int					nHeadBetweenEcloMode;			//喷头间羽化模式	
	//0-不启用, 1-喷头间距, 2-套色, 3-喷头间距和套色
	int					nColorSplit;					//套色细分模式
	vector<HsAlarmSet>	vecAlarmSettings;		//报警器设置
public:
	friend inline void to_json(json &j, const HsGeneralParam &p)
	{
		j = json{
		{"dbXOriginToZero",p.dbXOriginToZero},
		{"dbYOriginToZero",p.dbYOriginToZero},
		{"dbMarginSetOffset",p.dbMarginSetOffset},
		{"bSupportYMargin",p.bSupportYMargin},
		{"nXPhysicalRasterDpi",p.nXPhysicalRasterDpi},
		{"nXLogicRasterDpi",p.nXLogicRasterDpi},
		{"nYPhysicalRasterDpi",p.nYPhysicalRasterDpi},
		{"nYLogicRasterDpi",p.nYLogicRasterDpi},
		{"bSupportAdaptiveLogicRaster",p.bSupportAdaptiveLogicRaster},
		{"bSupportBEEclosion",p.bSupportBEEclosion},
		{"bBEEclosionEnable",p.bBEEclosionEnable},
		{"nEclosionHeight",p.nEclosionHeight},
		{"nColorBarDeepnessWeak",p.nColorBarDeepnessWeak},
		{"nColorBarDeepnessMid",p.nColorBarDeepnessMid},
		{"nColorBarDeepnessStrong",p.nColorBarDeepnessStrong},
		{"nPrintToOriginTime",p.nPrintToOriginTime},
		{"nResumeWaitTime",p.nResumeWaitTime},
		{"nMotoSpeedScale",p.nMotoSpeedScale},
		{"nPositonUnitType",p.nPositonUnitType},
		{"nMeasurementUnit",p.nMeasurementUnit},
		{"nHeadBetweenEcloMode",p.nHeadBetweenEcloMode},
		{"nColorSplit",p.nColorSplit},
		{"vecAlarmSettings",p.vecAlarmSettings},

		};
	}
	friend inline void from_json(const json &j, HsGeneralParam &p)
	{
		j.at("dbXOriginToZero").get_to(p.dbXOriginToZero);
		j.at("dbYOriginToZero").get_to(p.dbYOriginToZero);
		j.at("dbMarginSetOffset").get_to(p.dbMarginSetOffset);
		j.at("bSupportYMargin").get_to(p.bSupportYMargin);
		j.at("nXPhysicalRasterDpi").get_to(p.nXPhysicalRasterDpi);
		j.at("nXLogicRasterDpi").get_to(p.nXLogicRasterDpi);
		j.at("nYPhysicalRasterDpi").get_to(p.nYPhysicalRasterDpi);
		j.at("nYLogicRasterDpi").get_to(p.nYLogicRasterDpi);
		j.at("bSupportAdaptiveLogicRaster").get_to(p.bSupportAdaptiveLogicRaster);
		j.at("bSupportBEEclosion").get_to(p.bSupportBEEclosion);
		j.at("bBEEclosionEnable").get_to(p.bBEEclosionEnable);
		j.at("nEclosionHeight").get_to(p.nEclosionHeight);
		j.at("nColorBarDeepnessWeak").get_to(p.nColorBarDeepnessWeak);
		j.at("nColorBarDeepnessMid").get_to(p.nColorBarDeepnessMid);
		j.at("nColorBarDeepnessStrong").get_to(p.nColorBarDeepnessStrong);
		j.at("nPrintToOriginTime").get_to(p.nPrintToOriginTime);
		j.at("nResumeWaitTime").get_to(p.nResumeWaitTime);
		j.at("nMotoSpeedScale").get_to(p.nMotoSpeedScale);
		j.at("nPositonUnitType").get_to(p.nPositonUnitType);
		j.at("nMeasurementUnit").get_to(p.nMeasurementUnit);
		j.at("nHeadBetweenEcloMode").get_to(p.nHeadBetweenEcloMode);
		j.at("nColorSplit").get_to(p.nColorSplit);
		j.at("vecAlarmSettings").get_to(p.vecAlarmSettings);
	}
};

// 基准套色参数
// 每一列的数据项
struct BaseAdjustColItem
{
	// 列头显示（close、C1~C8 M1~M8 Y1~Y8 K1~K8）
	string		szShow;
	// 向右偏移量
	int			nRightOffset;
	// 向左偏移量
	int			nLeftOffset;
	// 垂直偏移量
	int			nVerOffset;
};

// 每个喷头的数据项
struct HeadItem
{
	// 喷头的行数 4行
	//int nRow;
	// 喷头的通道数
	int nChannel;
	// 某一个喷头的数据
	vector<BaseAdjustColItem> headDataItem;
};

// 所有的喷头数据项
//struct AllHeadData
//{
//	// 喷头数
//	int nHeadCount;
//	// 所有的喷头数据
//	vector<HeadItem> HeadData;
//};

// 基准横向DPI
struct BaseXDpiItem
{
	int		nID;							//ID
	double	dbValue;						//I值
	string	szName;							//名字
};

// 喷头索引和值
struct PhyHeadIndexItem
{
	int		nIndex; // 0   1  5 7
	string  szText;	// H2 H4 H6 H8等
};

// 基准通道索引和值
struct BaseChannelIndexItem
{
	int		nIndex;
	string  szText;	// K1~K8 C1~C8 M1~M8 K1~K8
};

struct HsBaseAdjustParam
{
	int					nHeadGroupCnt;				//物理布局喷头个数
	int					nHeadChannelCnt;			//单喷头喷头通道数
	int					nPhyLayoutIndex;			//当前物理布局索引 
	int					nListRowCnt;				//列表行数
	int					nListColCnt;				//列表列数
	//char				byChannelInk[32];
	bool				bSupportHeadColumnOffset;	//是否支持喷头列偏移
	bool                bSupportColorSplit;			//是否支持套色细分
	vector<PhyHeadIndexItem>	szHeadGroupList;	//喷头选择列表
	vector<BaseChannelIndexItem>		szHeadChannelList;			//基准通道列表

	vector<HeadItem>  colParamList;		// 所有喷头通道的偏移数据（包括颜色信息、向右、向左、垂直偏移量）

	vector<BaseXDpiItem> baseXDpiList;				//基准横向DPI列表
	int					 nCurXDpi;					//当前基准横向DPI值

	QJsonObject toJsonObject()
	{
		QJsonObject jo;
		jo.insert("nHeadGroupCnt", nHeadGroupCnt);
		jo.insert("nHeadChannelCnt", nHeadChannelCnt);
		jo.insert("nPhyLayoutIndex", nPhyLayoutIndex);
		jo.insert("nListRowCnt", nListRowCnt);
		jo.insert("nListColCnt", nListColCnt);
		jo.insert("bSupportHeadColumnOffset", bSupportHeadColumnOffset);
		jo.insert("bSupportColorSplit", bSupportColorSplit);

		// 喷头选择列表
		QJsonArray jXHeadGroupArr;
		for (auto& v : szHeadGroupList)
		{
			QJsonObject j;
			j.insert("nIndex", v.nIndex);
			j.insert("szText", v.szText.c_str());

			jXHeadGroupArr.append(j);
		}
		jo.insert("szHeadGroupList", jXHeadGroupArr);

		// 基准通道列表
		QJsonArray jXHeadChannelArr;
		for (auto& v : szHeadChannelList)
		{
			QJsonObject j;
			j.insert("nIndex", v.nIndex);
			j.insert("szText", v.szText.c_str());
			jXHeadChannelArr.append(j);
		}
		jo.insert("szHeadChannelList", jXHeadChannelArr);

		// 喷头数据列表
		QJsonArray jColParamsArr;
		for (auto& v : colParamList)
		{
			// 每个喷头的数据项
			HeadItem headItem = v;
			int headChannelCount = v.nChannel;
			QJsonObject j2;
			j2.insert("nChannel", v.nChannel);

			QJsonArray arr2;
			for (auto&v2 : v.headDataItem)
			{
				QJsonObject j3;
				j3.insert("szShow", v2.szShow.c_str());
				j3.insert("nRightOffset", v2.nRightOffset);
				j3.insert("nLeftOffset", v2.nLeftOffset);
				j3.insert("nVerOffset", v2.nVerOffset);

				arr2.append(j3);
			}

			j2.insert("headDataItem", arr2);
		}
		jo.insert("colParams", jColParamsArr);

		//基准横向DPI列表
		QJsonArray jBaseXDpiArr;
		for (auto& v : baseXDpiList)
		{
			QJsonObject j;
			j.insert("nID", v.nID);
			j.insert("dbValue", v.dbValue);
			j.insert("szName", v.szName.c_str());

			jBaseXDpiArr.append(j);
		}
		jo.insert("baseXDpiList", jBaseXDpiArr);

		jo.insert("nCurXDpi", nCurXDpi);

		return jo;
	}

	HsBaseAdjustParam& fromJsonObject(QJsonObject jo)
	{
		nHeadGroupCnt = jo["nHeadGroupCnt"].toInt();
		nHeadChannelCnt = jo["nHeadChannelCnt"].toInt();
		nPhyLayoutIndex = jo["nPhyLayoutIndex"].toInt();
		nListRowCnt = jo["nListRowCnt"].toInt();
		nListColCnt = jo["nListColCnt"].toInt();
		bSupportColorSplit = jo["bSupportColorSplit"].toBool();
		bSupportHeadColumnOffset = jo["bSupportHeadColumnOffset"].toBool();

		//喷头选择列表
		QJsonArray jHeadGroupArr = jo["szHeadGroupList"].toArray();
		for (auto& v : jHeadGroupArr)
		{
			PhyHeadIndexItem item;
			item.nIndex = v.toObject()["nIndex"].toInt();
			item.szText = v.toObject()["szText"].toString().toStdString();
			szHeadGroupList.push_back(item);
		}

		//基准通道列表
		QJsonArray jHeadChannelArr = jo["szHeadChannelList"].toArray();
		for (auto& v : jHeadChannelArr)
		{
			BaseChannelIndexItem item;
			item.nIndex = v.toObject()["nIndex"].toInt();
			item.szText = v.toObject()["szText"].toString().toStdString();
			szHeadChannelList.push_back(item);
		}

		QJsonArray jColParamArr = jo["colParamList"].toArray();
		for (auto& v : jColParamArr)
		{
			HeadItem headItem;
			headItem.nChannel = v.toObject()["nChannel"].toInt();
			for (auto& v2 : v.toObject()["headDataItem"].toArray())
			{
				BaseAdjustColItem item;
				item.szShow = v2.toObject()["szShow"].toString().toStdString();

				item.nRightOffset = v2.toObject()["nRightOffset"].toInt();
				item.nLeftOffset = v2.toObject()["nLeftOffset"].toInt();
				item.nVerOffset = v2.toObject()["nVerOffset"].toInt();

				headItem.headDataItem.push_back(item);
			}

			colParamList.push_back(headItem);
		}

		QJsonArray jBaseXDpiArr = jo["baseXDpiList"].toArray();
		for (auto& v : jBaseXDpiArr)
		{
			BaseXDpiItem item;
			item.nID = v.toObject()["nID"].toInt();

			item.dbValue = v.toObject()["dbValue"].toDouble();
			item.szName = v.toObject()["szName"].toString().toStdString();

			baseXDpiList.push_back(item);
		}

		nCurXDpi = jo["nCurXDpi"].toInt();

		return *this;
	}
};
#endif
```
